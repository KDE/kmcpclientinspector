/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorservernotificationwidget.h"
#include "actions/kmcpclientinspectorlogsearchwidget.h"
#include "kmcpclientinspectorserverlogmodel.h"
#include "kmcpclientinspectorserverlogsortfilterproxymodel.h"
#include "misc/jsonplaintexteditwidget.h"
#include <KLocalizedString>
#include <QComboBox>
#include <QDateTime>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QTreeView>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolLoggingMessageNotificationParams>

using namespace Qt::Literals::StringLiterals;
using LoggingLevel = TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel;

KMcpClientInspectorServerNotificationWidget::KMcpClientInspectorServerNotificationWidget(
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
    QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
    , mServerLevelComboBox(new QComboBox(this))
    , mLevelComboBox(new QComboBox(this))
    , mLogSearchWidget(new KMcpClientInspectorLogSearchWidget(this))
    , mSplitter(new QSplitter(this))
    , mTreeView(new QTreeView(this))
    , mJsonPlainTextEditWidget(new JsonPlainTextEditWidget(this))
    , mServerLogModel(new KMcpClientInspectorServerLogModel(this))
    , mServerLogSortFilterProxyModel(new KMcpClientInspectorServerLogSortFilterProxyModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto filterLayout = new QHBoxLayout;
    filterLayout->setObjectName(u"filterLayout"_s);
    filterLayout->setContentsMargins({});
    mainLayout->addLayout(filterLayout);

    auto serverLevelLabel = new QLabel(i18nc("@label:listbox", "Server level:"), this);
    serverLevelLabel->setObjectName(u"serverLevelLabel"_s);
    serverLevelLabel->setBuddy(mServerLevelComboBox);
    filterLayout->addWidget(serverLevelLabel);

    mServerLevelComboBox->setObjectName(u"mServerLevelComboBox"_s);
    mServerLevelComboBox->addItem(i18nc("@item:inlistbox logging level", "Server Default"), static_cast<int>(LoggingLevel::Unknown));
    fillLevelComboBox(mServerLevelComboBox);
    mServerLevelComboBox->setEnabled(false);
    mServerLevelComboBox->setToolTip(i18nc("@info:tooltip", "Minimum level of the log messages sent by the server (logging/setLevel)."));
    filterLayout->addWidget(mServerLevelComboBox);

    auto levelLabel = new QLabel(i18nc("@label:listbox", "Show from:"), this);
    levelLabel->setObjectName(u"levelLabel"_s);
    levelLabel->setBuddy(mLevelComboBox);
    filterLayout->addWidget(levelLabel);

    mLevelComboBox->setObjectName(u"mLevelComboBox"_s);
    fillLevelComboBox(mLevelComboBox);
    mLevelComboBox->setToolTip(i18nc("@info:tooltip", "Hide received log messages below this level."));
    filterLayout->addWidget(mLevelComboBox);

    mLogSearchWidget->setObjectName(u"mLogSearchWidget"_s);
    filterLayout->addWidget(mLogSearchWidget, 1);

    mSplitter->setObjectName(u"mSplitter"_s);
    mSplitter->setOrientation(Qt::Vertical);
    mSplitter->setChildrenCollapsible(false);
    mainLayout->addWidget(mSplitter);

    mServerLogSortFilterProxyModel->setSourceModel(mServerLogModel);

    mTreeView->setObjectName(u"mTreeView"_s);
    mTreeView->setRootIsDecorated(false);
    mTreeView->setModel(mServerLogSortFilterProxyModel);
    mTreeView->setColumnHidden(KMcpClientInspectorServerLogModel::ServerLogColumns::Json, true);
    mTreeView->header()->setStretchLastSection(false);
    mTreeView->header()->setSectionResizeMode(KMcpClientInspectorServerLogModel::ServerLogColumns::Message, QHeaderView::Stretch);
    mSplitter->addWidget(mTreeView);

    mJsonPlainTextEditWidget->setObjectName(u"mJsonPlainTextEditWidget"_s);
    mJsonPlainTextEditWidget->setReadOnly(true);
    mSplitter->addWidget(mJsonPlainTextEditWidget);

    connect(mTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &KMcpClientInspectorServerNotificationWidget::slotCurrentChanged);
    connect(mServerLevelComboBox, &QComboBox::activated, this, &KMcpClientInspectorServerNotificationWidget::slotServerLevelActivated);
    connect(mProtocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::initialized, this, [this]() {
        mServerLevelComboBox->setEnabled(
            mProtocolManager->serverSupports(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::SetLoggingLevel));
    });
    connect(mProtocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::finished, this, [this]() {
        mServerLevelComboBox->setEnabled(false);
        mServerLevelComboBox->setCurrentIndex(0);
    });
    connect(mLevelComboBox, &QComboBox::currentIndexChanged, this, &KMcpClientInspectorServerNotificationWidget::slotMinimumLevelChanged);
    connect(mLogSearchWidget,
            &KMcpClientInspectorLogSearchWidget::searchTextChanged,
            mServerLogSortFilterProxyModel,
            &KMcpClientInspectorServerLogSortFilterProxyModel::setFilterString);
    connect(mLogSearchWidget, &KMcpClientInspectorLogSearchWidget::clearRequested, mServerLogModel, &KMcpClientInspectorServerLogModel::clear);
    connect(mProtocolManager,
            &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::loggingMessageReceived,
            this,
            &KMcpClientInspectorServerNotificationWidget::slotLoggingMessageReceived);

    connect(mLogSearchWidget, &KMcpClientInspectorLogSearchWidget::clearRequested, mJsonPlainTextEditWidget, &JsonPlainTextEditWidget::clear);
}

KMcpClientInspectorServerNotificationWidget::~KMcpClientInspectorServerNotificationWidget() = default;

void KMcpClientInspectorServerNotificationWidget::fillLevelComboBox(QComboBox *comboBox)
{
    comboBox->addItem(i18nc("@item:inlistbox logging level", "Debug"), static_cast<int>(LoggingLevel::Debug));
    comboBox->addItem(i18nc("@item:inlistbox logging level", "Info"), static_cast<int>(LoggingLevel::Info));
    comboBox->addItem(i18nc("@item:inlistbox logging level", "Notice"), static_cast<int>(LoggingLevel::Notice));
    comboBox->addItem(i18nc("@item:inlistbox logging level", "Warning"), static_cast<int>(LoggingLevel::Warning));
    comboBox->addItem(i18nc("@item:inlistbox logging level", "Error"), static_cast<int>(LoggingLevel::Error));
    comboBox->addItem(i18nc("@item:inlistbox logging level", "Critical"), static_cast<int>(LoggingLevel::Critical));
    comboBox->addItem(i18nc("@item:inlistbox logging level", "Alert"), static_cast<int>(LoggingLevel::Alert));
    comboBox->addItem(i18nc("@item:inlistbox logging level", "Emergency"), static_cast<int>(LoggingLevel::Emergency));
}

void KMcpClientInspectorServerNotificationWidget::slotServerLevelActivated()
{
    const auto level = static_cast<LoggingLevel>(mServerLevelComboBox->currentData().toInt());
    if (level == LoggingLevel::Unknown) {
        // MCP has no request to restore the server default level.
        return;
    }
    (void)mProtocolManager->setLoggingLevel(level);
}

void KMcpClientInspectorServerNotificationWidget::setLoggingLevelResult(const QJsonObject &obj)
{
    mTreeView->setCurrentIndex({});
    mJsonPlainTextEditWidget->setPlainText(QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented)));
}

void KMcpClientInspectorServerNotificationWidget::slotLoggingMessageReceived(
    const TextAutoGenerateTextMcpProtocolCore::McpProtocolLoggingMessageNotificationParams &params)
{
    mServerLogModel->addLoggingMessage(QDateTime::currentDateTime(), params);
}

void KMcpClientInspectorServerNotificationWidget::slotMinimumLevelChanged()
{
    mServerLogSortFilterProxyModel->setMinimumLevel(static_cast<LoggingLevel>(mLevelComboBox->currentData().toInt()));
}

void KMcpClientInspectorServerNotificationWidget::slotCurrentChanged(const QModelIndex &index)
{
    if (!index.isValid()) {
        mJsonPlainTextEditWidget->setPlainText({});
        return;
    }
    const QJsonObject obj = index.siblingAtColumn(KMcpClientInspectorServerLogModel::ServerLogColumns::Json).data().toJsonObject();
    mJsonPlainTextEditWidget->setPlainText(QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented)));
}

#include "moc_kmcpclientinspectorservernotificationwidget.cpp"
