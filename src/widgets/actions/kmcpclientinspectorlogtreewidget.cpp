/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorlogtreewidget.h"
#include "kmcpclientinspectorlogmodel.h"
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
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mTreeView->setObjectName(u"mTreeView"_s);
    mainLayout->addWidget(mTreeView);
    mTreeView->setRootIsDecorated(false);
    mTreeView->setModel(mLogModel);
    connect(mProtocolManager,
            &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::messageLogged,
            this,
            &KMcpClientInspectorLogTreeWidget::slotMessageLogged);
}

KMcpClientInspectorLogTreeWidget::~KMcpClientInspectorLogTreeWidget() = default;

void KMcpClientInspectorLogTreeWidget::slotMessageLogged(const TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageInfo &info)
{
    mLogModel->addLogInfo(info);
}
#include "moc_kmcpclientinspectorlogtreewidget.cpp"
