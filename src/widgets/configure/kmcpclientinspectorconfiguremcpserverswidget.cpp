/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "kmcpclientinspectorconfiguremcpserverswidget.h"
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpServerManager>
#include <TextAutoGenerateTextMcpProtocolWidgets/McpServerWidget>

using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorConfigureMcpServersWidget::KMcpClientInspectorConfigureMcpServersWidget(TextAutoGenerateTextMcpProtocolCore::McpServerManager *manager,
                                                                                           QWidget *parent)
    : QWidget{parent}
    , mMcpServerWidget(new TextAutoGenerateTextMcpProtocolWidgets::McpServerWidget(manager->mcpServerModel(), this))
    , mManager(manager)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mMcpServerWidget->setObjectName(u"mMcpServerWidget"_s);
    mainLayout->addWidget(mMcpServerWidget);
    connect(mMcpServerWidget,
            &TextAutoGenerateTextMcpProtocolWidgets::McpServerWidget::settingsChanged,
            this,
            &KMcpClientInspectorConfigureMcpServersWidget::save);
    load();
}

KMcpClientInspectorConfigureMcpServersWidget::~KMcpClientInspectorConfigureMcpServersWidget() = default;

void KMcpClientInspectorConfigureMcpServersWidget::save()
{
    mManager->saveServers();
}

void KMcpClientInspectorConfigureMcpServersWidget::load()
{
    mManager->loadServers();
}

void KMcpClientInspectorConfigureMcpServersWidget::restoreToDefaults()
{
    // nothing
}

#include "moc_kmcpclientinspectorconfiguremcpserverswidget.cpp"
