/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectortabwidget.h"
#include "kmcpclientinspectormanager.h"
#include "kmcpclientinspectortabpage.h"
#include <TextAutoGenerateTextMcpProtocolCore/McpServerModel>

KMcpClientInspectorTabWidget::KMcpClientInspectorTabWidget(KMcpClientInspectorManager *manager, QWidget *parent)
    : QTabWidget(parent)
    , mManager(manager)
{
    loadServers();
}

KMcpClientInspectorTabWidget::~KMcpClientInspectorTabWidget() = default;

void KMcpClientInspectorTabWidget::loadServers()
{
    const auto servers = mManager->mcpServerModel()->mcpServers();
    for (const auto &s : servers) {
        auto page = new KMcpClientInspectorTabPage(this);
        addTab(page, s.name());
    }
    // TODO
}

#include "moc_kmcpclientinspectortabwidget.cpp"
