/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorserverinfowidgettest.h"
#include "kmcpclientinspectorserverinfowidget.h"
#include <QFormLayout>
#include <QLabel>
#include <QTest>
QTEST_MAIN(KMcpClientInspectorServerInfoWidgetTest)
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorServerInfoWidgetTest::KMcpClientInspectorServerInfoWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorServerInfoWidgetTest::shouldHaveDefaultValues()
{
    const KMcpClientInspectorServerInfoWidget w;

    auto mainLayout = w.findChild<QFormLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    for (const auto &name : {u"mName"_s, u"mTransportType"_s, u"mCommand"_s, u"mArguments"_s, u"mEnvironments"_s, u"mServerUrl"_s, u"mRoots"_s}) {
        auto label = w.findChild<QLabel *>(name);
        QVERIFY(label);
        QVERIFY(label->text().isEmpty());
        QCOMPARE(label->textInteractionFlags(), Qt::TextSelectableByMouse);
        QCOMPARE(label->textFormat(), Qt::PlainText);
        QVERIFY(!label->wordWrap());
    }
}

void KMcpClientInspectorServerInfoWidgetTest::shouldShowServerUrl()
{
    KMcpClientInspectorServerInfoWidget w;
    TextAutoGenerateTextMcpProtocolCore::McpServer server;
    server.setName(u"foo"_s);
    server.setTransportType(TextAutoGenerateTextMcpProtocolCore::McpProtocolPlugin::TransportType::StreamableHttp);
    TextAutoGenerateTextMcpProtocolCore::McpProtocolSettings settings;
    settings.setServerUrl(QUrl(u"http://127.0.0.1:8765/mcp"_s));
    server.setSettings(settings);
    w.setServerInfo(server);
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));

    auto mServerUrl = w.findChild<QLabel *>(u"mServerUrl"_s);
    QCOMPARE(mServerUrl->text(), u"http://127.0.0.1:8765/mcp"_s);
    QVERIFY(mServerUrl->isVisible());
    QVERIFY(!w.findChild<QLabel *>(u"mCommand"_s)->isVisible());
}

#include "moc_kmcpclientinspectorserverinfowidgettest.cpp"
