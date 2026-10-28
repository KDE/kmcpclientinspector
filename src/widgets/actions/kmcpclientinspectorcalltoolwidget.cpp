/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorcalltoolwidget.h"
#include "actions/kmcpclientinspectortoolargumentswidget.h"
#include "dialogs/kmcpclientinspectorurlelicitationdialog.h"
#include "misc/jsonplaintexteditwidget.h"
#include <KListWidgetSearchLine>
#include <KLocalizedString>
#include <QDesktopServices>
#include <QIcon>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QListWidget>
#include <QLocale>
#include <QPushButton>
#include <QSplitter>
#include <QTabWidget>
#include <QTextBrowser>
#include <QTextDocument>
#include <QUrl>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolCallToolResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolElicitRequestURLParams>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolError>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolListToolsResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolURLElicitationRequiredError>
using namespace Qt::Literals::StringLiterals;
using namespace TextAutoGenerateTextMcpProtocolCore;

namespace
{
QString toolDisplayName(const McpProtocolTool &tool)
{
    // Spec: tool title has precedence over annotations title
    QString title = tool.title().value_or(QString());
    if (title.isEmpty() && tool.annotations().has_value()) {
        title = tool.annotations()->title().value_or(QString());
    }
    return (title.isEmpty() || title == tool.name()) ? tool.name() : u"%1 (%2)"_s.arg(title, tool.name());
}

QString preformatted(const QString &text)
{
    return u"<pre style=\"white-space: pre-wrap\">%1</pre>"_s.arg(text.toHtmlEscaped());
}

QString dataSize(const QString &base64)
{
    return QLocale().formattedDataSize(QByteArray::fromBase64(base64.toLatin1()).size());
}

QString badge(const QString &text, const QColor &color, bool isDefault)
{
    if (isDefault) {
        return u"<span style=\"color: #7f8c8d\"><i>&nbsp;%1&nbsp;</i></span>"_s.arg(text.toHtmlEscaped());
    }
    return u"<span style=\"background-color: %1; color: white\">&nbsp;%2&nbsp;</span>"_s.arg(color.name(), text.toHtmlEscaped());
}

QString hintToolTip(const QString &name, std::optional<bool> hint, bool defaultValue)
{
    const auto boolToString = [](bool value) {
        return value ? u"true"_s : u"false"_s;
    };
    return hint.has_value() ? u"%1: %2"_s.arg(name, boolToString(*hint)) : i18n("%1: not set (default: %2)", name, boolToString(defaultValue));
}

bool isDestructiveTool(const McpProtocolTool &tool)
{
    const auto annotations = tool.annotations();
    // Only flag tools which declare it explicitly, otherwise all tools without annotations would be flagged
    return annotations.has_value() && !annotations->readOnlyHint().value_or(false) && annotations->destructiveHint().value_or(false);
}
}

KMcpClientInspectorCallToolWidget::KMcpClientInspectorCallToolWidget(McpProtocolClientProtocolManager *protocolManager, QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
    , mListToolsButton(new QPushButton(i18nc("@action:button", "List Tools"), this))
    , mToolsListWidget(new QListWidget(this))
    , mToolNameLabel(new QLabel(this))
    , mToolDescriptionLabel(new QLabel(this))
    , mToolAnnotationsLabel(new QLabel(this))
    , mArgumentsWidget(new KMcpClientInspectorToolArgumentsWidget(this))
    , mCallToolButton(new QPushButton(i18nc("@action:button", "Call Tool"), this))
    , mStatusLabel(new QLabel(this))
    , mResultTabWidget(new QTabWidget(this))
    , mContentBrowser(new QTextBrowser(this))
    , mJsonPlainTextEditWidget(new JsonPlainTextEditWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setObjectName(u"splitter"_s);
    splitter->setChildrenCollapsible(false);
    mainLayout->addWidget(splitter);

    // Tools list
    auto toolsWidget = new QWidget(splitter);
    auto toolsLayout = new QVBoxLayout(toolsWidget);
    toolsLayout->setObjectName(u"toolsLayout"_s);
    toolsLayout->setContentsMargins({});

    mListToolsButton->setObjectName(u"mListToolsButton"_s);
    toolsLayout->addWidget(mListToolsButton);
    connect(mListToolsButton, &QPushButton::clicked, this, [this]() {
        mProtocolManager->executeAction(McpProtocolClientProtocolManager::MethodType::ListTools);
    });

    auto searchLine = new KListWidgetSearchLine(this, mToolsListWidget);
    searchLine->setPlaceholderText(i18nc("@info:placeholder", "Search"));
    searchLine->setObjectName(u"searchLine"_s);
    toolsLayout->addWidget(searchLine);

    mToolsListWidget->setObjectName(u"mToolsListWidget"_s);
    toolsLayout->addWidget(mToolsListWidget);
    connect(mToolsListWidget, &QListWidget::currentRowChanged, this, &KMcpClientInspectorCallToolWidget::slotCurrentToolChanged);
    splitter->addWidget(toolsWidget);

    // Selected tool
    auto toolSplitter = new QSplitter(Qt::Vertical, splitter);
    toolSplitter->setObjectName(u"toolSplitter"_s);
    toolSplitter->setChildrenCollapsible(false);

    auto argumentsPage = new QWidget(toolSplitter);
    auto argumentsLayout = new QVBoxLayout(argumentsPage);
    argumentsLayout->setObjectName(u"argumentsLayout"_s);
    argumentsLayout->setContentsMargins({});

    mToolNameLabel->setObjectName(u"mToolNameLabel"_s);
    mToolNameLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    QFont font = mToolNameLabel->font();
    font.setBold(true);
    mToolNameLabel->setFont(font);
    argumentsLayout->addWidget(mToolNameLabel);

    mToolDescriptionLabel->setObjectName(u"mToolDescriptionLabel"_s);
    mToolDescriptionLabel->setWordWrap(true);
    mToolDescriptionLabel->setTextFormat(Qt::PlainText);
    mToolDescriptionLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    argumentsLayout->addWidget(mToolDescriptionLabel);

    mToolAnnotationsLabel->setObjectName(u"mToolAnnotationsLabel"_s);
    mToolAnnotationsLabel->setTextFormat(Qt::RichText);
    mToolAnnotationsLabel->setWordWrap(true);
    argumentsLayout->addWidget(mToolAnnotationsLabel);

    mArgumentsWidget->setObjectName(u"mArgumentsWidget"_s);
    argumentsLayout->addWidget(mArgumentsWidget);

    auto callLayout = new QHBoxLayout;
    callLayout->setObjectName(u"callLayout"_s);
    mCallToolButton->setObjectName(u"mCallToolButton"_s);
    callLayout->addWidget(mCallToolButton);
    mStatusLabel->setObjectName(u"mStatusLabel"_s);
    mStatusLabel->setWordWrap(true);
    mStatusLabel->setTextFormat(Qt::PlainText);
    callLayout->addWidget(mStatusLabel, 1);
    argumentsLayout->addLayout(callLayout);
    connect(mCallToolButton, &QPushButton::clicked, this, &KMcpClientInspectorCallToolWidget::slotCallTool);
    toolSplitter->addWidget(argumentsPage);

    // Result
    mResultTabWidget->setObjectName(u"mResultTabWidget"_s);
    mContentBrowser->setObjectName(u"mContentBrowser"_s);
    // Links come from server: don't open them
    mContentBrowser->setOpenLinks(false);
    mJsonPlainTextEditWidget->setObjectName(u"mJsonPlainTextEditWidget"_s);
    mResultTabWidget->addTab(mContentBrowser, i18nc("@title:tab", "Content"));
    mResultTabWidget->addTab(mJsonPlainTextEditWidget, i18nc("@title:tab", "JSON"));
    toolSplitter->addWidget(mResultTabWidget);

    splitter->addWidget(toolSplitter);
    splitter->setStretchFactor(1, 1);

    connect(mProtocolManager, &McpProtocolClientProtocolManager::initialized, this, [this]() {
        if (mProtocolManager->serverSupports(McpProtocolClientProtocolManager::MethodType::ListTools)) {
            mProtocolManager->executeAction(McpProtocolClientProtocolManager::MethodType::ListTools);
        }
        updateCallButton();
    });
    connect(mProtocolManager, &McpProtocolClientProtocolManager::toolsListChanged, this, [this]() {
        if (mProtocolManager->isInitialized()) {
            mProtocolManager->executeAction(McpProtocolClientProtocolManager::MethodType::ListTools);
        }
    });
    connect(mProtocolManager, &McpProtocolClientProtocolManager::finished, this, [this]() {
        // Reset pending request first, otherwise result is not cleared
        setCallInProgress(false);
        clearTools();
    });
    clearTools();
}

KMcpClientInspectorCallToolWidget::~KMcpClientInspectorCallToolWidget() = default;

QList<McpProtocolTool> KMcpClientInspectorCallToolWidget::tools() const
{
    return mTools;
}

void KMcpClientInspectorCallToolWidget::clearTools()
{
    mTools.clear();
    mToolsListWidget->clear();
    slotCurrentToolChanged(-1);
}

void KMcpClientInspectorCallToolWidget::clearResult()
{
    mStatusLabel->clear();
    mContentBrowser->clear();
    mJsonPlainTextEditWidget->setPlainText({});
}

void KMcpClientInspectorCallToolWidget::setToolsList(const QJsonObject &obj)
{
    if (!obj.value("result"_L1).isObject()) {
        // Error response: keep current list
        return;
    }
    const McpProtocolListToolsResult result = McpProtocolListToolsResult::fromJson(obj.value("result"_L1).toObject());
    const int currentRow = mToolsListWidget->currentRow();
    const std::optional<McpProtocolTool> currentTool = (currentRow >= 0) ? std::optional<McpProtocolTool>(mTools.at(currentRow)) : std::nullopt;

    const QSignalBlocker blocker(mToolsListWidget);
    mToolsListWidget->clear();
    mTools.clear();
    const QList<McpProtocolTool> tools = result.tools();
    for (const McpProtocolTool &tool : tools) {
        if (tool.isValid()) {
            mTools.append(tool);
        }
    }
    // Sort mTools itself: list widget rows are used as index in mTools
    std::stable_sort(mTools.begin(), mTools.end(), [](const McpProtocolTool &left, const McpProtocolTool &right) {
        return toolDisplayName(left).localeAwareCompare(toolDisplayName(right)) < 0;
    });
    for (const McpProtocolTool &tool : std::as_const(mTools)) {
        auto item = new QListWidgetItem(toolDisplayName(tool), mToolsListWidget);
        item->setToolTip(tool.description().value_or(QString()));
        if (isDestructiveTool(tool)) {
            item->setIcon(QIcon::fromTheme(u"dialog-warning"_s));
        }
    }

    // Keep selected tool when list is updated
    int row = -1;
    for (int i = 0; i < mTools.count(); ++i) {
        if (currentTool.has_value() && mTools.at(i).name() == currentTool->name()) {
            row = i;
            break;
        }
    }
    mToolsListWidget->setCurrentRow(row);
    // Don't reset arguments when selected tool didn't change
    if (row < 0 || mTools.at(row) != *currentTool) {
        slotCurrentToolChanged(row);
    }
}

void KMcpClientInspectorCallToolWidget::slotCurrentToolChanged(int row)
{
    if (row < 0 || row >= mTools.count()) {
        mToolNameLabel->setText(i18n("No tool selected"));
        mToolDescriptionLabel->clear();
        mToolDescriptionLabel->setVisible(false);
        mToolAnnotationsLabel->clear();
        mToolAnnotationsLabel->setToolTip({});
        mToolAnnotationsLabel->setVisible(false);
        mArgumentsWidget->clear();
        mArgumentsWidget->setEnabled(false);
    } else {
        const McpProtocolTool &tool = mTools.at(row);
        mToolNameLabel->setText(tool.name());
        const QString description = tool.description().value_or(QString());
        mToolDescriptionLabel->setText(description);
        mToolDescriptionLabel->setVisible(!description.isEmpty());
        mToolAnnotationsLabel->setText(annotationsToHtml(tool.annotations()));
        mToolAnnotationsLabel->setToolTip(annotationsToolTip(tool.annotations()));
        mToolAnnotationsLabel->setVisible(true);
        mArgumentsWidget->setInputSchema(tool.inputSchema());
        mArgumentsWidget->setEnabled(true);
    }
    if (mPendingRequestId < 0) {
        clearResult();
    }
    updateCallButton();
}

void KMcpClientInspectorCallToolWidget::updateCallButton()
{
    mCallToolButton->setEnabled(mPendingRequestId < 0 && mToolsListWidget->currentRow() >= 0 && mProtocolManager->isInitialized());
}

void KMcpClientInspectorCallToolWidget::setCallInProgress(bool inProgress)
{
    if (!inProgress) {
        mPendingRequestId = -1;
    }
    updateCallButton();
}

void KMcpClientInspectorCallToolWidget::showMessage(const QString &message, bool isError)
{
    mStatusLabel->setText(message);
    QPalette palette = mStatusLabel->palette();
    palette.setColor(QPalette::WindowText, isError ? QColor(Qt::red) : this->palette().color(QPalette::WindowText));
    mStatusLabel->setPalette(palette);
}

void KMcpClientInspectorCallToolWidget::slotCallTool()
{
    const int row = mToolsListWidget->currentRow();
    if (row < 0) {
        return;
    }
    clearResult();
    QString errorMessage;
    const std::optional<QJsonObject> arguments = mArgumentsWidget->arguments(&errorMessage);
    if (!arguments.has_value()) {
        showMessage(errorMessage, true);
        return;
    }
    const qint64 requestId = mProtocolManager->callTool(mTools.at(row).name(), *arguments);
    if (requestId < 0) {
        showMessage(i18n("Tool cannot be called. Is the server connected and does it support tools?"), true);
        return;
    }
    mPendingRequestId = requestId;
    showMessage(i18n("Calling tool…"), false);
    updateCallButton();
}

void KMcpClientInspectorCallToolWidget::setResult(const QJsonObject &obj)
{
    setCallInProgress(false);
    mJsonPlainTextEditWidget->setPlainText(QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented)));
    if (const QJsonValue errorValue = obj.value("error"_L1); errorValue.isObject()) {
        const McpProtocolError error = McpProtocolError::fromJson(errorValue.toObject());
        mContentBrowser->clear();
        mResultTabWidget->setCurrentWidget(mJsonPlainTextEditWidget);
        if (error.code() == McpProtocolURLElicitationRequiredError::errorCode) {
            showMessage(i18n("The server requires an interaction in the web browser before calling this tool: %1", error.message()), true);
            showUrlElicitations(obj);
        } else {
            showMessage(i18n("Error %1: %2", QString::number(error.code()), error.message()), true);
        }
        return;
    }
    const McpProtocolCallToolResult result = McpProtocolCallToolResult::fromJson(obj.value("result"_L1).toObject());
    if (result.isError().value_or(false)) {
        showMessage(i18n("Tool returned an error."), true);
    } else {
        showMessage(i18n("Tool executed successfully."), false);
    }
    mContentBrowser->clear();
    mContentBrowser->setHtml(resultToHtml(result, mContentBrowser->document()));
    mResultTabWidget->setCurrentWidget(mContentBrowser);
}

void KMcpClientInspectorCallToolWidget::showUrlElicitations(const QJsonObject &obj)
{
    const QList<McpProtocolElicitRequestURLParams> elicitations = McpProtocolURLElicitationRequiredError::fromJson(obj).elicitations();
    for (const McpProtocolElicitRequestURLParams &params : elicitations) {
        auto dialog = new KMcpClientInspectorUrlElicitationDialog(params, this);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        // No answer to send: server is informed by web page, user retries tool afterwards
        connect(dialog, &QDialog::accepted, this, [dialog]() {
            QDesktopServices::openUrl(dialog->url());
        });
        dialog->show();
    }
}

QString KMcpClientInspectorCallToolWidget::annotationsToHtml(const std::optional<McpProtocolToolAnnotations> &annotations)
{
    const McpProtocolToolAnnotations hints = annotations.value_or(McpProtocolToolAnnotations());
    QStringList badges;
    // Default values come from MCP specification
    const std::optional<bool> readOnly = hints.readOnlyHint();
    if (readOnly.value_or(false)) {
        badges.append(badge(i18nc("@label tool annotation", "Read-only"), QColor(0x27, 0xae, 0x60), !readOnly.has_value()));
    } else {
        // Destructive and idempotent hints are meaningful only when tool is not read-only
        const std::optional<bool> destructive = hints.destructiveHint();
        if (destructive.value_or(true)) {
            badges.append(badge(i18nc("@label tool annotation", "Destructive"), QColor(0xda, 0x44, 0x53), !destructive.has_value()));
        } else {
            badges.append(badge(i18nc("@label tool annotation", "Additive"), QColor(0x3d, 0xae, 0xe9), false));
        }
        const std::optional<bool> idempotent = hints.idempotentHint();
        if (idempotent.value_or(false)) {
            badges.append(badge(i18nc("@label tool annotation", "Idempotent"), QColor(0x3d, 0xae, 0xe9), false));
        }
    }
    const std::optional<bool> openWorld = hints.openWorldHint();
    if (openWorld.value_or(true)) {
        badges.append(badge(i18nc("@label tool annotation", "Open world"), QColor(0x3d, 0xae, 0xe9), !openWorld.has_value()));
    } else {
        badges.append(badge(i18nc("@label tool annotation", "Closed world"), QColor(0x3d, 0xae, 0xe9), false));
    }
    return badges.join(u"&nbsp;&nbsp;"_s);
}

QString KMcpClientInspectorCallToolWidget::annotationsToolTip(const std::optional<McpProtocolToolAnnotations> &annotations)
{
    if (!annotations.has_value()) {
        return i18n("The server did not send annotations: default values are used.");
    }
    const QStringList lines{
        hintToolTip(u"readOnlyHint"_s, annotations->readOnlyHint(), false),
        hintToolTip(u"destructiveHint"_s, annotations->destructiveHint(), true),
        hintToolTip(u"idempotentHint"_s, annotations->idempotentHint(), false),
        hintToolTip(u"openWorldHint"_s, annotations->openWorldHint(), true),
    };
    return lines.join(u'\n');
}

QString KMcpClientInspectorCallToolWidget::resultToHtml(const McpProtocolCallToolResult &result, QTextDocument *document)
{
    QString html;
    const QList<McpProtocolUtils::ContentBlock> content = result.content();
    int imageIndex = 0;
    for (const McpProtocolUtils::ContentBlock &block : content) {
        html += contentBlockToHtml(block, document, imageIndex);
    }
    if (const auto structuredContent = result.structuredContent(); structuredContent.has_value()) {
        QJsonObject obj;
        for (auto it = structuredContent->constBegin(); it != structuredContent->constEnd(); ++it) {
            obj.insert(it.key(), it.value());
        }
        html += u"<h4>%1</h4>"_s.arg(i18n("Structured Content").toHtmlEscaped());
        html += preformatted(QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented)));
    }
    if (html.isEmpty()) {
        html = u"<p><i>%1</i></p>"_s.arg(i18n("No content.").toHtmlEscaped());
    }
    return html;
}

QString KMcpClientInspectorCallToolWidget::contentBlockToHtml(const McpProtocolUtils::ContentBlock &block, QTextDocument *document, int &imageIndex)
{
    QString html;
    if (const auto text = std::get_if<McpProtocolTextContent>(&block)) {
        html += preformatted(text->text());
    } else if (const auto image = std::get_if<McpProtocolImageContent>(&block)) {
        QImage img;
        if (document && img.loadFromData(QByteArray::fromBase64(image->data().toLatin1()))) {
            const QUrl url(u"mcpimage:%1"_s.arg(imageIndex++));
            document->addResource(QTextDocument::ImageResource, url, img);
            html += u"<p><img src=\"%1\"/></p>"_s.arg(url.toString());
        } else {
            html += u"<p><i>%1</i></p>"_s.arg(i18n("Image (%1, %2)", image->mimeType(), dataSize(image->data())).toHtmlEscaped());
        }
    } else if (const auto audio = std::get_if<McpProtocolAudioContent>(&block)) {
        html += u"<p><i>%1</i></p>"_s.arg(i18n("Audio (%1, %2)", audio->mimeType(), dataSize(audio->data())).toHtmlEscaped());
    } else if (const auto link = std::get_if<McpProtocolResourceLink>(&block)) {
        html += u"<p>%1 <a href=\"%2\">%3</a></p>"_s.arg(i18n("Resource link:").toHtmlEscaped(),
                                                         link->uri().toHtmlEscaped(),
                                                         link->title().value_or(link->name()).toHtmlEscaped());
        if (const QString description = link->description().value_or(QString()); !description.isEmpty()) {
            html += u"<p>%1</p>"_s.arg(description.toHtmlEscaped());
        }
    } else if (const auto embedded = std::get_if<McpProtocolEmbeddedResource>(&block)) {
        const McpProtocolUtils::EmbeddedResourceResource resource = embedded->resource();
        if (const auto textResource = std::get_if<McpProtocolTextResourceContents>(&resource)) {
            html += u"<p><b>%1</b></p>"_s.arg(textResource->uri().toHtmlEscaped());
            html += preformatted(textResource->text());
        } else if (const auto blobResource = std::get_if<McpProtocolBlobResourceContents>(&resource)) {
            html += u"<p><b>%1</b></p>"_s.arg(blobResource->uri().toHtmlEscaped());
            html += u"<p><i>%1</i></p>"_s.arg(
                i18n("Binary resource (%1, %2)", blobResource->mimeType().value_or(i18n("unknown type")), dataSize(blobResource->blob())).toHtmlEscaped());
        }
    }
    return html;
}

#include "moc_kmcpclientinspectorcalltoolwidget.cpp"
