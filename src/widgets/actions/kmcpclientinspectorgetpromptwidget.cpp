/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorgetpromptwidget.h"
#include "actions/kmcpclientinspectorcalltoolwidget.h"
#include "actions/kmcpclientinspectorpromptargumentswidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <KListWidgetSearchLine>
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QSplitter>
#include <QTabWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolError>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolGetPromptResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolListPromptsResult>
using namespace Qt::Literals::StringLiterals;
using namespace TextAutoGenerateTextMcpProtocolCore;

namespace
{
QString promptDisplayName(const McpProtocolPrompt &prompt)
{
    const QString title = prompt.title().value_or(QString());
    return (title.isEmpty() || title == prompt.name()) ? prompt.name() : u"%1 (%2)"_s.arg(title, prompt.name());
}

QString roleToString(McpProtocolUtils::Role role)
{
    switch (role) {
    case McpProtocolUtils::Role::User:
        return i18nc("@title message role", "User");
    case McpProtocolUtils::Role::Assistant:
        return i18nc("@title message role", "Assistant");
    case McpProtocolUtils::Role::Unknown:
        break;
    }
    return i18nc("@title message role", "Unknown role");
}
}

KMcpClientInspectorGetPromptWidget::KMcpClientInspectorGetPromptWidget(McpProtocolClientProtocolManager *protocolManager, QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
    , mListPromptsButton(new QPushButton(i18nc("@action:button", "List Prompts"), this))
    , mPromptsListWidget(new QListWidget(this))
    , mPromptNameLabel(new QLabel(this))
    , mPromptDescriptionLabel(new QLabel(this))
    , mArgumentsWidget(new KMcpClientInspectorPromptArgumentsWidget(this))
    , mGetPromptButton(new QPushButton(i18nc("@action:button", "Get Prompt"), this))
    , mStatusLabel(new QLabel(this))
    , mResultTabWidget(new QTabWidget(this))
    , mMessagesBrowser(new QTextBrowser(this))
    , mJsonPlainTextEditWidget(new JsonPlainTextEditWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setObjectName(u"splitter"_s);
    splitter->setChildrenCollapsible(false);
    mainLayout->addWidget(splitter);

    // Prompts list
    auto promptsWidget = new QWidget(splitter);
    auto promptsLayout = new QVBoxLayout(promptsWidget);
    promptsLayout->setObjectName(u"promptsLayout"_s);
    promptsLayout->setContentsMargins({});

    mListPromptsButton->setObjectName(u"mListPromptsButton"_s);
    promptsLayout->addWidget(mListPromptsButton);
    connect(mListPromptsButton, &QPushButton::clicked, this, [this]() {
        mProtocolManager->executeAction(McpProtocolClientProtocolManager::MethodType::ListPrompts);
    });

    auto searchLine = new KListWidgetSearchLine(this, mPromptsListWidget);
    searchLine->setPlaceholderText(i18nc("@info:placeholder", "Search"));
    searchLine->setObjectName(u"searchLine"_s);
    promptsLayout->addWidget(searchLine);

    mPromptsListWidget->setObjectName(u"mPromptsListWidget"_s);
    promptsLayout->addWidget(mPromptsListWidget);
    connect(mPromptsListWidget, &QListWidget::currentRowChanged, this, &KMcpClientInspectorGetPromptWidget::slotCurrentPromptChanged);
    splitter->addWidget(promptsWidget);

    // Selected prompt
    auto promptSplitter = new QSplitter(Qt::Vertical, splitter);
    promptSplitter->setObjectName(u"promptSplitter"_s);
    promptSplitter->setChildrenCollapsible(false);

    auto argumentsPage = new QWidget(promptSplitter);
    auto argumentsLayout = new QVBoxLayout(argumentsPage);
    argumentsLayout->setObjectName(u"argumentsLayout"_s);
    argumentsLayout->setContentsMargins({});

    mPromptNameLabel->setObjectName(u"mPromptNameLabel"_s);
    mPromptNameLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    QFont font = mPromptNameLabel->font();
    font.setBold(true);
    mPromptNameLabel->setFont(font);
    argumentsLayout->addWidget(mPromptNameLabel);

    mPromptDescriptionLabel->setObjectName(u"mPromptDescriptionLabel"_s);
    mPromptDescriptionLabel->setWordWrap(true);
    mPromptDescriptionLabel->setTextFormat(Qt::PlainText);
    mPromptDescriptionLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    argumentsLayout->addWidget(mPromptDescriptionLabel);

    mArgumentsWidget->setObjectName(u"mArgumentsWidget"_s);
    argumentsLayout->addWidget(mArgumentsWidget);

    auto getPromptLayout = new QHBoxLayout;
    getPromptLayout->setObjectName(u"getPromptLayout"_s);
    mGetPromptButton->setObjectName(u"mGetPromptButton"_s);
    getPromptLayout->addWidget(mGetPromptButton);
    mStatusLabel->setObjectName(u"mStatusLabel"_s);
    mStatusLabel->setWordWrap(true);
    mStatusLabel->setTextFormat(Qt::PlainText);
    getPromptLayout->addWidget(mStatusLabel, 1);
    argumentsLayout->addLayout(getPromptLayout);
    connect(mGetPromptButton, &QPushButton::clicked, this, &KMcpClientInspectorGetPromptWidget::slotGetPrompt);
    promptSplitter->addWidget(argumentsPage);

    // Result
    mResultTabWidget->setObjectName(u"mResultTabWidget"_s);
    mMessagesBrowser->setObjectName(u"mMessagesBrowser"_s);
    // Links come from server: don't open them
    mMessagesBrowser->setOpenLinks(false);
    mJsonPlainTextEditWidget->setObjectName(u"mJsonPlainTextEditWidget"_s);
    mResultTabWidget->addTab(mMessagesBrowser, i18nc("@title:tab", "Messages"));
    mResultTabWidget->addTab(mJsonPlainTextEditWidget, i18nc("@title:tab", "JSON"));
    promptSplitter->addWidget(mResultTabWidget);

    splitter->addWidget(promptSplitter);
    splitter->setStretchFactor(1, 1);

    connect(mProtocolManager, &McpProtocolClientProtocolManager::initialized, this, [this]() {
        if (mProtocolManager->serverSupports(McpProtocolClientProtocolManager::MethodType::ListPrompts)) {
            mProtocolManager->executeAction(McpProtocolClientProtocolManager::MethodType::ListPrompts);
        }
        updateGetPromptButton();
    });
    connect(mProtocolManager, &McpProtocolClientProtocolManager::promptsListChanged, this, [this]() {
        if (mProtocolManager->isInitialized()) {
            mProtocolManager->executeAction(McpProtocolClientProtocolManager::MethodType::ListPrompts);
        }
    });
    connect(mProtocolManager, &McpProtocolClientProtocolManager::finished, this, [this]() {
        // Reset pending request first, otherwise result is not cleared
        setRequestInProgress(false);
        clearPrompts();
    });
    connect(mProtocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::initialized, this, [this]() {
        mListPromptsButton->setEnabled(true);
    });
    connect(mProtocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::finished, this, [this]() {
        mListPromptsButton->setEnabled(false);
    });

    clearPrompts();
}

KMcpClientInspectorGetPromptWidget::~KMcpClientInspectorGetPromptWidget() = default;

QList<McpProtocolPrompt> KMcpClientInspectorGetPromptWidget::prompts() const
{
    return mPrompts;
}

void KMcpClientInspectorGetPromptWidget::clearPrompts()
{
    mPrompts.clear();
    mPromptsListWidget->clear();
    slotCurrentPromptChanged(-1);
}

void KMcpClientInspectorGetPromptWidget::clearResult()
{
    mStatusLabel->clear();
    mMessagesBrowser->clear();
    mJsonPlainTextEditWidget->setPlainText({});
}

void KMcpClientInspectorGetPromptWidget::setPromptsList(const QJsonObject &obj)
{
    if (!obj.value("result"_L1).isObject()) {
        // Error response: keep current list
        return;
    }
    const McpProtocolListPromptsResult result = McpProtocolListPromptsResult::fromJson(obj.value("result"_L1).toObject());
    const int currentRow = mPromptsListWidget->currentRow();
    const std::optional<McpProtocolPrompt> currentPrompt = (currentRow >= 0) ? std::optional<McpProtocolPrompt>(mPrompts.at(currentRow)) : std::nullopt;

    const QSignalBlocker blocker(mPromptsListWidget);
    mPromptsListWidget->clear();
    mPrompts.clear();
    const QList<McpProtocolPrompt> prompts = result.prompts();
    for (const McpProtocolPrompt &prompt : prompts) {
        if (!prompt.name().isEmpty()) {
            mPrompts.append(prompt);
        }
    }
    // Sort mPrompts itself: list widget rows are used as index in mPrompts
    std::stable_sort(mPrompts.begin(), mPrompts.end(), [](const McpProtocolPrompt &left, const McpProtocolPrompt &right) {
        return promptDisplayName(left).localeAwareCompare(promptDisplayName(right)) < 0;
    });
    for (const McpProtocolPrompt &prompt : std::as_const(mPrompts)) {
        auto item = new QListWidgetItem(promptDisplayName(prompt), mPromptsListWidget);
        item->setToolTip(prompt.description().value_or(QString()));
    }

    // Keep selected prompt when list is updated
    int row = -1;
    for (int i = 0; i < mPrompts.count(); ++i) {
        if (currentPrompt.has_value() && mPrompts.at(i).name() == currentPrompt->name()) {
            row = i;
            break;
        }
    }
    mPromptsListWidget->setCurrentRow(row);
    // Don't reset arguments when selected prompt didn't change
    if (row < 0 || mPrompts.at(row) != *currentPrompt) {
        slotCurrentPromptChanged(row);
    }
}

void KMcpClientInspectorGetPromptWidget::slotCurrentPromptChanged(int row)
{
    if (row < 0 || row >= mPrompts.count()) {
        mPromptNameLabel->setText(i18n("No prompt selected"));
        mPromptDescriptionLabel->clear();
        mPromptDescriptionLabel->setVisible(false);
        mArgumentsWidget->clear();
        mArgumentsWidget->setEnabled(false);
    } else {
        const McpProtocolPrompt &prompt = mPrompts.at(row);
        mPromptNameLabel->setText(prompt.name());
        const QString description = prompt.description().value_or(QString());
        mPromptDescriptionLabel->setText(description);
        mPromptDescriptionLabel->setVisible(!description.isEmpty());
        mArgumentsWidget->setPromptArguments(prompt.arguments().value_or(QList<McpProtocolPromptArgument>{}));
        mArgumentsWidget->setEnabled(true);
    }
    if (mPendingRequestId < 0) {
        clearResult();
    }
    updateGetPromptButton();
}

void KMcpClientInspectorGetPromptWidget::updateGetPromptButton()
{
    mGetPromptButton->setEnabled(mPendingRequestId < 0 && mPromptsListWidget->currentRow() >= 0 && mProtocolManager->isInitialized());
}

void KMcpClientInspectorGetPromptWidget::setRequestInProgress(bool inProgress)
{
    if (!inProgress) {
        mPendingRequestId = -1;
    }
    updateGetPromptButton();
}

void KMcpClientInspectorGetPromptWidget::showMessage(const QString &message, bool isError)
{
    mStatusLabel->setText(message);
    QPalette palette = mStatusLabel->palette();
    palette.setColor(QPalette::WindowText, isError ? QColor(Qt::red) : this->palette().color(QPalette::WindowText));
    mStatusLabel->setPalette(palette);
}

void KMcpClientInspectorGetPromptWidget::slotGetPrompt()
{
    const int row = mPromptsListWidget->currentRow();
    if (row < 0) {
        return;
    }
    clearResult();
    if (const QStringList missing = mArgumentsWidget->missingRequiredArguments(); !missing.isEmpty()) {
        showMessage(i18np("Argument \"%2\" is required.", "Arguments %2 are required.", missing.count(), missing.join(u", "_s)), true);
        return;
    }
    const qint64 requestId = mProtocolManager->getPrompt(mPrompts.at(row).name(), mArgumentsWidget->arguments());
    if (requestId < 0) {
        showMessage(i18n("Prompt cannot be retrieved. Is the server connected and does it support prompts?"), true);
        return;
    }
    mPendingRequestId = requestId;
    showMessage(i18n("Getting prompt…"), false);
    updateGetPromptButton();
}

void KMcpClientInspectorGetPromptWidget::setResult(const QJsonObject &obj)
{
    setRequestInProgress(false);
    mJsonPlainTextEditWidget->setPlainText(QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented)));
    if (const QJsonValue errorValue = obj.value("error"_L1); errorValue.isObject()) {
        const McpProtocolError error = McpProtocolError::fromJson(errorValue.toObject());
        showMessage(i18n("Error %1: %2", QString::number(error.code()), error.message()), true);
        mMessagesBrowser->clear();
        mResultTabWidget->setCurrentWidget(mJsonPlainTextEditWidget);
        return;
    }
    const McpProtocolGetPromptResult result = McpProtocolGetPromptResult::fromJson(obj.value("result"_L1).toObject());
    showMessage(i18np("Prompt retrieved: %1 message.", "Prompt retrieved: %1 messages.", result.messages().count()), false);
    mMessagesBrowser->clear();
    mMessagesBrowser->setHtml(resultToHtml(result, mMessagesBrowser->document()));
    mResultTabWidget->setCurrentWidget(mMessagesBrowser);
}

QString KMcpClientInspectorGetPromptWidget::resultToHtml(const McpProtocolGetPromptResult &result, QTextDocument *document)
{
    QString html;
    if (const QString description = result.description().value_or(QString()); !description.isEmpty()) {
        html += u"<p><i>%1</i></p>"_s.arg(description.toHtmlEscaped());
    }
    const QList<McpProtocolPromptMessage> messages = result.messages();
    int imageIndex = 0;
    for (const McpProtocolPromptMessage &message : messages) {
        html += u"<h4>%1</h4>"_s.arg(roleToString(message.role()).toHtmlEscaped());
        html += KMcpClientInspectorCallToolWidget::contentBlockToHtml(message.content(), document, imageIndex);
    }
    if (messages.isEmpty()) {
        html += u"<p><i>%1</i></p>"_s.arg(i18n("No messages.").toHtmlEscaped());
    }
    return html;
}

#include "moc_kmcpclientinspectorgetpromptwidget.cpp"
