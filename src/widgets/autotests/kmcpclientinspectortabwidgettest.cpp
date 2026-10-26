/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectortabwidgettest.h"
#include "kmcpclientinspectormanager.h"
#include "kmcpclientinspectortabpage.h"
#include "kmcpclientinspectortabwidget.h"
#include <QPointer>
#include <QTest>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
#include <TextAutoGenerateTextMcpProtocolCore/McpServerModel>
QTEST_MAIN(KMcpClientInspectorTabWidgetTest)
using namespace Qt::Literals::StringLiterals;

namespace
{
TextAutoGenerateTextMcpProtocolCore::McpServer createServer(const QByteArray &identifier, const QString &name, bool enabled = true)
{
    TextAutoGenerateTextMcpProtocolCore::McpServer server;
    server.setIdentifier(identifier);
    server.setName(name);
    server.setEnabled(enabled);
    server.setTransportType(TextAutoGenerateTextMcpProtocolCore::McpProtocolPlugin::TransportType::StreamableHttp);
    TextAutoGenerateTextMcpProtocolCore::McpProtocolSettings settings;
    settings.setServerUrl(QUrl(u"http://127.0.0.1:8765/mcp"_s));
    server.setSettings(settings);
    return server;
}

KMcpClientInspectorTabPage *page(const KMcpClientInspectorTabWidget &w, int index)
{
    return qobject_cast<KMcpClientInspectorTabPage *>(w.widget(index));
}
}

KMcpClientInspectorTabWidgetTest::KMcpClientInspectorTabWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorTabWidgetTest::shouldHaveDefaultValues()
{
    KMcpClientInspectorManager manager;
    const KMcpClientInspectorTabWidget w(&manager);
    QCOMPARE(w.count(), 0);
}

void KMcpClientInspectorTabWidgetTest::shouldCreateTabsForEnabledServers()
{
    KMcpClientInspectorManager manager;
    KMcpClientInspectorTabWidget w(&manager);
    manager.mcpServerModel()->setMcpServers({createServer("1", u"foo"_s), createServer("2", u"bla"_s, false), createServer("3", u"bli"_s)});
    w.loadServers();

    QCOMPARE(w.count(), 2);
    QCOMPARE(w.tabText(0), u"foo"_s);
    QCOMPARE(w.tabText(1), u"bli"_s);
    QCOMPARE(page(w, 0)->server().identifier(), "1");
    QCOMPARE(page(w, 1)->server().identifier(), "3");
}

void KMcpClientInspectorTabWidgetTest::shouldKeepExistingPagesWhenServersChanged()
{
    KMcpClientInspectorManager manager;
    KMcpClientInspectorTabWidget w(&manager);
    manager.mcpServerModel()->setMcpServers({createServer("1", u"foo"_s), createServer("2", u"bla"_s), createServer("3", u"bli"_s)});
    w.loadServers();
    QCOMPARE(w.count(), 3);
    const QPointer<KMcpClientInspectorTabPage> page1 = page(w, 0);
    const QPointer<KMcpClientInspectorTabPage> page2 = page(w, 1);
    const QPointer<KMcpClientInspectorTabPage> page3 = page(w, 2);

    // Rename "1", disable "2", add "4" before "3"
    manager.mcpServerModel()->setMcpServers(
        {createServer("1", u"foo2"_s), createServer("2", u"bla"_s, false), createServer("4", u"new"_s), createServer("3", u"bli"_s)});
    w.loadServers();

    QCOMPARE(w.count(), 3);
    // Existing pages are not recreated
    QCOMPARE(page(w, 0), page1.data());
    QCOMPARE(w.tabText(0), u"foo2"_s);
    QCOMPARE(page1->server().name(), u"foo2"_s);
    // Disabled server: page is deleted
    QVERIFY(page2.isNull());
    QCOMPARE(page(w, 1)->server().identifier(), "4");
    QCOMPARE(w.tabText(1), u"new"_s);
    QCOMPARE(page(w, 2), page3.data());
    QCOMPARE(w.tabText(2), u"bli"_s);
}

#include "moc_kmcpclientinspectortabwidgettest.cpp"
