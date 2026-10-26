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

    auto levelLabel = new QLabel(i18nc("@label:listbox", "Minimum level:"), this);
    levelLabel->setObjectName(u"levelLabel"_s);
    levelLabel->setBuddy(mLevelComboBox);
    filterLayout->addWidget(levelLabel);

    mLevelComboBox->setObjectName(u"mLevelComboBox"_s);
    mLevelComboBox->addItem(i18nc("@item:inlistbox logging level", "Debug"), static_cast<int>(LoggingLevel::Debug));
    mLevelComboBox->addItem(i18nc("@item:inlistbox logging level", "Info"), static_cast<int>(LoggingLevel::Info));
    mLevelComboBox->addItem(i18nc("@item:inlistbox logging level", "Notice"), static_cast<int>(LoggingLevel::Notice));
    mLevelComboBox->addItem(i18nc("@item:inlistbox logging level", "Warning"), static_cast<int>(LoggingLevel::Warning));
    mLevelComboBox->addItem(i18nc("@item:inlistbox logging level", "Error"), static_cast<int>(LoggingLevel::Error));
    mLevelComboBox->addItem(i18nc("@item:inlistbox logging level", "Critical"), static_cast<int>(LoggingLevel::Critical));
    mLevelComboBox->addItem(i18nc("@item:inlistbox logging level", "Alert"), static_cast<int>(LoggingLevel::Alert));
    mLevelComboBox->addItem(i18nc("@item:inlistbox logging level", "Emergency"), static_cast<int>(LoggingLevel::Emergency));
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
