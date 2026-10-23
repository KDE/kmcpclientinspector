/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectortabwidget.h"
#include "kmcpclientinspectormanager.h"
#include "kmcpclientinspectortabpage.h"
#include <TextAutoGenerateTextMcpProtocolCore/McpServerModel>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorTabWidget::KMcpClientInspectorTabWidget(KMcpClientInspectorManager *manager, QWidget *parent)
    : QTabWidget(parent)
    , mManager(manager)
{
    connect(mManager, &KMcpClientInspectorManager::serverLoaded, this, &KMcpClientInspectorTabWidget::loadServers);
}

KMcpClientInspectorTabWidget::~KMcpClientInspectorTabWidget() = default;

void KMcpClientInspectorTabWidget::loadServers()
{
    while (count() > 0) {
        delete widget(0);
    }
    const auto servers = mManager->mcpServerModel()->mcpServers();
    for (const auto &s : servers) {
        if (s.enabled()) {
            auto page = new KMcpClientInspectorTabPage(s, this);
            const int index = addTab(page, s.name());
            setTabIcon(index, QIcon::fromTheme(u"network-disconnect"_s));
            connect(page, &KMcpClientInspectorTabPage::finished, this, [page, this]() {
                setTabIcon(indexOf(page), QIcon::fromTheme(u"network-disconnect"_s));
            });
            connect(page, &KMcpClientInspectorTabPage::started, this, [page, this]() {
                setTabIcon(indexOf(page), QIcon::fromTheme(u"network-connect"_s));
            });
            connect(page, &KMcpClientInspectorTabPage::serverModified, this, [page, this](const TextAutoGenerateTextMcpProtocolCore::McpServer &server) {
                mManager->mcpServerModel()->editMcpServer(server);
                mManager->saveServers();
                setTabText(indexOf(page), server.name());
            });
        }
    }
}

#include "moc_kmcpclientinspectortabwidget.cpp"
