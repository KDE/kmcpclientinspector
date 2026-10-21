/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorlogtreewidget.h"
#include "actions/kmcpclientinspectorlogsearchwidget.h"
#include "kmcpclientinspectorlogmodel.h"
#include "kmcpclientinspectorlogsortfilterproxymodel.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTreeView>
#include <TextAutoGenerateTextMcpProtocolCore/McpServerModel>

#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;

KMcpClientInspectorLogTreeWidget::KMcpClientInspectorLogTreeWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                                   QWidget *parent)
    : QWidget{parent}
    , mTreeView(new QTreeView(this))
    , mProtocolManager(protocolManager)
    , mLogModel(new KMcpClientInspectorLogModel(this))
    , mLogSortFilterProxyModel(new KMcpClientInspectorLogSortFilterProxyModel(this))
    , mLogSearchWidget(new KMcpClientInspectorLogSearchWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mLogSearchWidget->setObjectName(u"mLogSearchWidget"_s);
    mainLayout->addWidget(mLogSearchWidget);

    mTreeView->setObjectName(u"mTreeView"_s);
    mainLayout->addWidget(mTreeView);
    mTreeView->setRootIsDecorated(false);

    mLogSortFilterProxyModel->setSourceModel(mLogModel);

    mTreeView->setModel(mLogSortFilterProxyModel);
    mTreeView->setSortingEnabled(true);
    mTreeView->sortByColumn(KMcpClientInspectorLogModel::LogRoles::Identifier, Qt::AscendingOrder);
    mTreeView->setColumnHidden(KMcpClientInspectorLogModel::LogRoles::Json, true);
    connect(mTreeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &KMcpClientInspectorLogTreeWidget::slotCurrentChanged);
    connect(mProtocolManager,
            &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::messageLogged,
            this,
            &KMcpClientInspectorLogTreeWidget::slotMessageLogged);
    connect(mLogSearchWidget,
            &KMcpClientInspectorLogSearchWidget::searchTextChanged,
            mLogSortFilterProxyModel,
            &KMcpClientInspectorLogSortFilterProxyModel::setFilterString);
}

KMcpClientInspectorLogTreeWidget::~KMcpClientInspectorLogTreeWidget() = default;

void KMcpClientInspectorLogTreeWidget::slotMessageLogged(const TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageInfo &info)
{
    mLogModel->addLogInfo(info);
}
void KMcpClientInspectorLogTreeWidget::slotCurrentChanged(const QModelIndex &index)
{
    if (!index.isValid()) {
        Q_EMIT jsonSelected({});
        return;
    }
    const QJsonObject obj = index.siblingAtColumn(KMcpClientInspectorLogModel::LogRoles::Json).data().toJsonObject();
    Q_EMIT jsonSelected(QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented)));
}

#include "moc_kmcpclientinspectorlogtreewidget.cpp"
