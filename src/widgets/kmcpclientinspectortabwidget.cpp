/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectortabwidget.h"
#include "kmcpclientinspectormanager.h"
#include "kmcpclientinspectortabpage.h"
#include <QTabBar>
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
    // Keep existing pages (and their connections, logs…): only add, update, move or remove tabs
    QHash<QByteArray, KMcpClientInspectorTabPage *> pages;
    for (int i = 0; i < count(); ++i) {
        if (auto page = qobject_cast<KMcpClientInspectorTabPage *>(widget(i))) {
            pages.insert(page->server().identifier(), page);
        }
    }
    int index = 0;
    const auto servers = mManager->mcpServerModel()->mcpServers();
    for (const auto &s : servers) {
        if (!s.enabled()) {
            continue;
        }
        if (auto page = pages.take(s.identifier())) {
            if (page->server() != s) {
                page->setServer(s);
                setTabText(indexOf(page), s.name());
            }
            tabBar()->moveTab(indexOf(page), index);
        } else {
            insertTab(index, createPage(s), QIcon::fromTheme(u"network-disconnect"_s), s.name());
        }
        ++index;
    }
    // Servers removed or disabled
    qDeleteAll(pages);
}

KMcpClientInspectorTabPage *KMcpClientInspectorTabWidget::createPage(const TextAutoGenerateTextMcpProtocolCore::McpServer &server)
{
    auto page = new KMcpClientInspectorTabPage(server, this);
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
    return page;
}

#include "moc_kmcpclientinspectortabwidget.cpp"
