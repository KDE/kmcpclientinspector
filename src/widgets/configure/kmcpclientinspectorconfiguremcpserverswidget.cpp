/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kmcpclientinspectorconfiguremcpserverswidget.h"
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpServerManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpServerModel>
#include <TextAutoGenerateTextMcpProtocolWidgets/McpServerWidget>

using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorConfigureMcpServersWidget::KMcpClientInspectorConfigureMcpServersWidget(TextAutoGenerateTextMcpProtocolCore::McpServerManager *manager,
                                                                                           QWidget *parent)
    : QWidget{parent}
    , mMcpServerWidget(new TextAutoGenerateTextMcpProtocolWidgets::McpServerWidget(manager->mcpServerModel(), this))
    , mManager(manager)
    , mOriginalServers(manager->mcpServerModel()->mcpServers())
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mMcpServerWidget->setObjectName(u"mMcpServerWidget"_s);
    mainLayout->addWidget(mMcpServerWidget);
}

KMcpClientInspectorConfigureMcpServersWidget::~KMcpClientInspectorConfigureMcpServersWidget() = default;

void KMcpClientInspectorConfigureMcpServersWidget::save()
{
    mManager->saveServers();
    mOriginalServers = mManager->mcpServerModel()->mcpServers();
}

void KMcpClientInspectorConfigureMcpServersWidget::revert()
{
    // McpServerWidget edits model directly: restore servers as they were when dialog was opened
    if (mManager->mcpServerModel()->mcpServers() != mOriginalServers) {
        mManager->mcpServerModel()->setMcpServers(mOriginalServers);
    }
}

void KMcpClientInspectorConfigureMcpServersWidget::restoreToDefaults()
{
    // nothing
}

#include "moc_kmcpclientinspectorconfiguremcpserverswidget.cpp"
