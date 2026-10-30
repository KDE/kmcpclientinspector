/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorreadresourcewidget.h"
#include "actions/kmcpclientinspectorcalltoolwidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <KListWidgetSearchLine>
#include <KLocalizedString>
#include <QIcon>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSplitter>
#include <QTabWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolError>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolListResourcesResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolReadResourceResult>
using namespace Qt::Literals::StringLiterals;
using namespace TextAutoGenerateTextMcpProtocolCore;

namespace
{
QString resourceDisplayName(const McpProtocolResource &resource)
{
    const QString title = resource.title().value_or(QString());
    return (title.isEmpty() || title == resource.name()) ? resource.name() : u"%1 (%2)"_s.arg(title, resource.name());
}

QString errorMessage(const QJsonObject &obj)
{
    const McpProtocolError error = McpProtocolError::fromJson(obj.value("error"_L1).toObject());
    return i18n("Error %1: %2", QString::number(error.code()), error.message());
}
}

KMcpClientInspectorReadResourceWidget::KMcpClientInspectorReadResourceWidget(McpProtocolClientProtocolManager *protocolManager, QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
    , mListResourcesButton(new QPushButton(i18nc("@action:button", "List Resources"), this))
    , mResourcesListWidget(new QListWidget(this))
    , mResourceNameLabel(new QLabel(this))
    , mResourceDescriptionLabel(new QLabel(this))
    , mUriLineEdit(new QLineEdit(this))
    , mReadButton(new QPushButton(i18nc("@action:button", "Read Resource"), this))
    , mSubscribeButton(new QPushButton(i18nc("@action:button", "Subscribe"), this))
    , mUnsubscribeButton(new QPushButton(i18nc("@action:button", "Unsubscribe"), this))
    , mStatusLabel(new QLabel(this))
    , mResultTabWidget(new QTabWidget(this))
    , mContentsBrowser(new QTextBrowser(this))
    , mJsonPlainTextEditWidget(new JsonPlainTextEditWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setObjectName(u"splitter"_s);
    splitter->setChildrenCollapsible(false);
    mainLayout->addWidget(splitter);

    // Resources list
    auto resourcesWidget = new QWidget(splitter);
    auto resourcesLayout = new QVBoxLayout(resourcesWidget);
    resourcesLayout->setObjectName(u"resourcesLayout"_s);
    resourcesLayout->setContentsMargins({});

    mListResourcesButton->setObjectName(u"mListResourcesButton"_s);
    resourcesLayout->addWidget(mListResourcesButton);
    connect(mListResourcesButton, &QPushButton::clicked, this, [this]() {
        mProtocolManager->executeAction(McpProtocolClientProtocolManager::MethodType::ListResources);
    });

    auto searchLine = new KListWidgetSearchLine(this, mResourcesListWidget);
    searchLine->setPlaceholderText(i18nc("@info:placeholder", "Search"));
    searchLine->setObjectName(u"searchLine"_s);
    resourcesLayout->addWidget(searchLine);

    mResourcesListWidget->setObjectName(u"mResourcesListWidget"_s);
    resourcesLayout->addWidget(mResourcesListWidget);
    connect(mResourcesListWidget, &QListWidget::currentRowChanged, this, &KMcpClientInspectorReadResourceWidget::slotCurrentResourceChanged);
    splitter->addWidget(resourcesWidget);

    // Selected resource
    auto resourceSplitter = new QSplitter(Qt::Vertical, splitter);
    resourceSplitter->setObjectName(u"resourceSplitter"_s);
    resourceSplitter->setChildrenCollapsible(false);

    auto resourcePage = new QWidget(resourceSplitter);
    auto resourceLayout = new QVBoxLayout(resourcePage);
    resourceLayout->setObjectName(u"resourceLayout"_s);
    resourceLayout->setContentsMargins({});

    mResourceNameLabel->setObjectName(u"mResourceNameLabel"_s);
    mResourceNameLabel->setTextFormat(Qt::PlainText);
    mResourceNameLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    QFont font = mResourceNameLabel->font();
    font.setBold(true);
    mResourceNameLabel->setFont(font);
    resourceLayout->addWidget(mResourceNameLabel);

    mResourceDescriptionLabel->setObjectName(u"mResourceDescriptionLabel"_s);
    mResourceDescriptionLabel->setWordWrap(true);
    mResourceDescriptionLabel->setTextFormat(Qt::PlainText);
    mResourceDescriptionLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    resourceLayout->addWidget(mResourceDescriptionLabel);

    // Uri can be edited: allows to read a resource created from a resource template
    auto uriLayout = new QHBoxLayout;
    uriLayout->setObjectName(u"uriLayout"_s);
    auto uriLabel = new QLabel(i18nc("@label:textbox", "URI:"), resourcePage);
    uriLabel->setBuddy(mUriLineEdit);
    uriLayout->addWidget(uriLabel);
    mUriLineEdit->setObjectName(u"mUriLineEdit"_s);
    mUriLineEdit->setClearButtonEnabled(true);
    mUriLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Select a resource or enter its URI"));
    uriLayout->addWidget(mUriLineEdit, 1);
    resourceLayout->addLayout(uriLayout);
    connect(mUriLineEdit, &QLineEdit::textChanged, this, &KMcpClientInspectorReadResourceWidget::updateButtons);
    connect(mUriLineEdit, &QLineEdit::returnPressed, this, &KMcpClientInspectorReadResourceWidget::slotReadResource);

    auto buttonsLayout = new QHBoxLayout;
    buttonsLayout->setObjectName(u"buttonsLayout"_s);
    mReadButton->setObjectName(u"mReadButton"_s);
    buttonsLayout->addWidget(mReadButton);
    mSubscribeButton->setObjectName(u"mSubscribeButton"_s);
    buttonsLayout->addWidget(mSubscribeButton);
    mUnsubscribeButton->setObjectName(u"mUnsubscribeButton"_s);
    buttonsLayout->addWidget(mUnsubscribeButton);
    mStatusLabel->setObjectName(u"mStatusLabel"_s);
    mStatusLabel->setWordWrap(true);
    mStatusLabel->setTextFormat(Qt::PlainText);
    buttonsLayout->addWidget(mStatusLabel, 1);
    resourceLayout->addLayout(buttonsLayout);
    resourceLayout->addStretch(1);
    connect(mReadButton, &QPushButton::clicked, this, &KMcpClientInspectorReadResourceWidget::slotReadResource);
    connect(mSubscribeButton, &QPushButton::clicked, this, &KMcpClientInspectorReadResourceWidget::slotSubscribe);
    connect(mUnsubscribeButton, &QPushButton::clicked, this, &KMcpClientInspectorReadResourceWidget::slotUnsubscribe);
    resourceSplitter->addWidget(resourcePage);

    // Result
    mResultTabWidget->setObjectName(u"mResultTabWidget"_s);
    mContentsBrowser->setObjectName(u"mContentsBrowser"_s);
    // Links come from server: don't open them
    mContentsBrowser->setOpenLinks(false);
    mJsonPlainTextEditWidget->setObjectName(u"mJsonPlainTextEditWidget"_s);
    mResultTabWidget->addTab(mContentsBrowser, i18nc("@title:tab", "Contents"));
    mResultTabWidget->addTab(mJsonPlainTextEditWidget, i18nc("@title:tab", "JSON"));
    resourceSplitter->addWidget(mResultTabWidget);

    splitter->addWidget(resourceSplitter);
    splitter->setStretchFactor(1, 1);

    // Resources list is requested by list resources widget when client is initialized or list changed
    connect(mProtocolManager, &McpProtocolClientProtocolManager::initialized, this, &KMcpClientInspectorReadResourceWidget::updateButtons);
    connect(mProtocolManager, &McpProtocolClientProtocolManager::resourceUpdated, this, &KMcpClientInspectorReadResourceWidget::slotResourceUpdated);
    connect(mProtocolManager, &McpProtocolClientProtocolManager::finished, this, [this]() {
        // Subscriptions are lost with connection
        mPendingRequestId = -1;
        mPendingSubscriptionRequests.clear();
        mSubscribedResources.clear();
        clearResources();
    });
    connect(mProtocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::initialized, this, [this]() {
        mReadButton->setEnabled(true);
    });
    connect(mProtocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::finished, this, [this]() {
        mReadButton->setEnabled(false);
    });

    clearResources();
}

KMcpClientInspectorReadResourceWidget::~KMcpClientInspectorReadResourceWidget() = default;

QList<McpProtocolResource> KMcpClientInspectorReadResourceWidget::resources() const
{
    return mResources;
}

QSet<QString> KMcpClientInspectorReadResourceWidget::subscribedResources() const
{
    return mSubscribedResources;
}

void KMcpClientInspectorReadResourceWidget::clearResources()
{
    mResources.clear();
    mResourcesListWidget->clear();
    slotCurrentResourceChanged(-1);
}

void KMcpClientInspectorReadResourceWidget::clearResult()
{
    mStatusLabel->clear();
    mContentsBrowser->clear();
    mJsonPlainTextEditWidget->setPlainText({});
}

void KMcpClientInspectorReadResourceWidget::setResourcesList(const QJsonObject &obj)
{
    if (!obj.value("result"_L1).isObject()) {
        // Error response: keep current list
        return;
    }
    const McpProtocolListResourcesResult result = McpProtocolListResourcesResult::fromJson(obj.value("result"_L1).toObject());
    const int currentRow = mResourcesListWidget->currentRow();
    const QString currentUri = (currentRow >= 0) ? mResources.at(currentRow).uri() : QString();

    const QSignalBlocker blocker(mResourcesListWidget);
    mResourcesListWidget->clear();
    mResources.clear();
    const QList<McpProtocolResource> resources = result.resources();
    for (const McpProtocolResource &resource : resources) {
        if (!resource.uri().isEmpty()) {
            mResources.append(resource);
        }
    }
    // Sort mResources itself: list widget rows are used as index in mResources
    std::stable_sort(mResources.begin(), mResources.end(), [](const McpProtocolResource &left, const McpProtocolResource &right) {
        return resourceDisplayName(left).localeAwareCompare(resourceDisplayName(right)) < 0;
    });
    for (const McpProtocolResource &resource : std::as_const(mResources)) {
        new QListWidgetItem(resourceDisplayName(resource), mResourcesListWidget);
    }
    updateResourceItems();

    // Keep selected resource when list is updated
    int row = -1;
    for (int i = 0; i < mResources.count(); ++i) {
        if (!currentUri.isEmpty() && mResources.at(i).uri() == currentUri) {
            row = i;
            break;
        }
    }
    mResourcesListWidget->setCurrentRow(row);
    // Don't reset uri (perhaps edited by user) when selected resource didn't change
    if (row < 0) {
        slotCurrentResourceChanged(row);
    }
}

void KMcpClientInspectorReadResourceWidget::updateResourceItems()
{
    for (int i = 0; i < mResources.count(); ++i) {
        const McpProtocolResource &resource = mResources.at(i);
        QListWidgetItem *item = mResourcesListWidget->item(i);
        const bool subscribed = mSubscribedResources.contains(resource.uri());
        item->setIcon(subscribed ? QIcon::fromTheme(u"notifications"_s) : QIcon());
        QStringList toolTip{resource.uri()};
        if (const QString description = resource.description().value_or(QString()); !description.isEmpty()) {
            toolTip.append(description);
        }
        if (subscribed) {
            toolTip.append(i18n("Subscribed to updates"));
        }
        item->setToolTip(toolTip.join(u'\n'));
    }
}

void KMcpClientInspectorReadResourceWidget::slotCurrentResourceChanged(int row)
{
    if (row < 0 || row >= mResources.count()) {
        mResourceNameLabel->setText(i18n("No resource selected"));
        mResourceDescriptionLabel->clear();
        mResourceDescriptionLabel->setVisible(false);
        mUriLineEdit->clear();
    } else {
        const McpProtocolResource &resource = mResources.at(row);
        QString name = resourceDisplayName(resource);
        if (const QString mimeType = resource.mimeType().value_or(QString()); !mimeType.isEmpty()) {
            name = u"%1 [%2]"_s.arg(name, mimeType);
        }
        mResourceNameLabel->setText(name);
        const QString description = resource.description().value_or(QString());
        mResourceDescriptionLabel->setText(description);
        mResourceDescriptionLabel->setVisible(!description.isEmpty());
        mUriLineEdit->setText(resource.uri());
    }
    if (mPendingRequestId < 0) {
        clearResult();
    }
    updateButtons();
}

void KMcpClientInspectorReadResourceWidget::updateButtons()
{
    const QString uri = mUriLineEdit->text().trimmed();
    const bool initialized = mProtocolManager->isInitialized();
    const bool canSubscribe = initialized && !uri.isEmpty() && mProtocolManager->serverSupports(McpProtocolClientProtocolManager::MethodType::SubscribeResource)
        && mPendingSubscriptionRequests.key(uri, -1) < 0;
    const bool subscribed = mSubscribedResources.contains(uri);
    mListResourcesButton->setEnabled(initialized);
    mReadButton->setEnabled(initialized && mPendingRequestId < 0 && !uri.isEmpty());
    mSubscribeButton->setEnabled(canSubscribe && !subscribed);
    mUnsubscribeButton->setEnabled(canSubscribe && subscribed);
}

void KMcpClientInspectorReadResourceWidget::showMessage(const QString &message, bool isError)
{
    mStatusLabel->setText(message);
    QPalette palette = mStatusLabel->palette();
    palette.setColor(QPalette::WindowText, isError ? QColor(Qt::red) : this->palette().color(QPalette::WindowText));
    mStatusLabel->setPalette(palette);
}

void KMcpClientInspectorReadResourceWidget::slotReadResource()
{
    const QString uri = mUriLineEdit->text().trimmed();
    if (uri.isEmpty() || mPendingRequestId >= 0) {
        return;
    }
    clearResult();
    const qint64 requestId = mProtocolManager->readResource(uri);
    if (requestId < 0) {
        showMessage(i18n("Resource cannot be read. Is the server connected and does it support resources?"), true);
        return;
    }
    mPendingRequestId = requestId;
    showMessage(i18n("Reading resource…"), false);
    updateButtons();
}

void KMcpClientInspectorReadResourceWidget::slotSubscribe()
{
    const QString uri = mUriLineEdit->text().trimmed();
    const qint64 requestId = mProtocolManager->subscribeResource(uri);
    if (requestId < 0) {
        showMessage(i18n("Cannot subscribe to resource. Does the server support resource subscriptions?"), true);
        return;
    }
    mPendingSubscriptionRequests.insert(requestId, uri);
    showMessage(i18n("Subscribing to resource…"), false);
    updateButtons();
}

void KMcpClientInspectorReadResourceWidget::slotUnsubscribe()
{
    const QString uri = mUriLineEdit->text().trimmed();
    const qint64 requestId = mProtocolManager->unsubscribeResource(uri);
    if (requestId < 0) {
        showMessage(i18n("Cannot unsubscribe from resource."), true);
        return;
    }
    mPendingSubscriptionRequests.insert(requestId, uri);
    showMessage(i18n("Unsubscribing from resource…"), false);
    updateButtons();
}

void KMcpClientInspectorReadResourceWidget::setSubscriptionResult(const QJsonObject &obj, bool subscribe)
{
    const QString uri = mPendingSubscriptionRequests.take(obj.value("id"_L1).toInteger(-1));
    if (obj.value("error"_L1).isObject()) {
        showMessage(errorMessage(obj), true);
    } else if (!uri.isEmpty()) {
        if (subscribe) {
            mSubscribedResources.insert(uri);
            showMessage(i18n("Subscribed to \"%1\".", uri), false);
        } else {
            mSubscribedResources.remove(uri);
            showMessage(i18n("Unsubscribed from \"%1\".", uri), false);
        }
        updateResourceItems();
    }
    updateButtons();
}

void KMcpClientInspectorReadResourceWidget::slotResourceUpdated(const QString &uri)
{
    // Displayed resource changed: read it again
    if (uri == mUriLineEdit->text().trimmed() && mPendingRequestId < 0) {
        slotReadResource();
        if (mPendingRequestId >= 0) {
            showMessage(i18n("Resource \"%1\" was updated, reading it again…", uri), false);
        }
    } else {
        showMessage(i18n("Resource \"%1\" was updated.", uri), false);
    }
}

void KMcpClientInspectorReadResourceWidget::setResult(const QJsonObject &obj)
{
    mPendingRequestId = -1;
    updateButtons();
    mJsonPlainTextEditWidget->setPlainText(QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented)));
    if (obj.value("error"_L1).isObject()) {
        showMessage(errorMessage(obj), true);
        mContentsBrowser->clear();
        mResultTabWidget->setCurrentWidget(mJsonPlainTextEditWidget);
        return;
    }
    const McpProtocolReadResourceResult result = McpProtocolReadResourceResult::fromJson(obj.value("result"_L1).toObject());
    showMessage(i18np("Resource read: %1 content.", "Resource read: %1 contents.", result.contents().count()), false);
    mContentsBrowser->setHtml(resultToHtml(result));
    mResultTabWidget->setCurrentWidget(mContentsBrowser);
}

QString KMcpClientInspectorReadResourceWidget::resultToHtml(const McpProtocolReadResourceResult &result)
{
    QString html;
    const QList<McpProtocolUtils::EmbeddedResourceResource> contents = result.contents();
    for (const McpProtocolUtils::EmbeddedResourceResource &resource : contents) {
        html += KMcpClientInspectorCallToolWidget::resourceContentsToHtml(resource);
    }
    if (contents.isEmpty()) {
        html += u"<p><i>%1</i></p>"_s.arg(i18n("No contents.").toHtmlEscaped());
    }
    return html;
}

#include "moc_kmcpclientinspectorreadresourcewidget.cpp"
