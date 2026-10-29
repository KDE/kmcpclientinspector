/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorinitializewidgettest.h"
#include "actions/kmcpclientinspectorinitializewidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QTabWidget>
#include <QTest>
#include <QTextEdit>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolInitializeResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
QTEST_MAIN(KMcpClientInspectorInitializeWidgetTest)
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorInitializeWidgetTest::KMcpClientInspectorInitializeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorInitializeWidgetTest::shouldHaveDefaultValues()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    const KMcpClientInspectorInitializeWidget w(&manager);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto mJsonPlainTextEditWidget = w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s);
    QVERIFY(mJsonPlainTextEditWidget);
    QVERIFY(mJsonPlainTextEditWidget->toPlainText().isEmpty());

    auto tabWidget = w.findChild<QTabWidget *>(u"tabWidget"_s);
    QVERIFY(tabWidget);
    QCOMPARE(tabWidget->count(), 2);

    for (const auto &name : {u"mServerName"_s, u"mServerTitle"_s, u"mServerVersion"_s, u"mServerDescription"_s, u"mServerWebsite"_s, u"mProtocolVersion"_s}) {
        auto label = w.findChild<QLabel *>(name);
        QVERIFY(label);
        QVERIFY(label->text().isEmpty() || name == u"mServerWebsite"_s);
    }
    auto mCapabilities = w.findChild<QLabel *>(u"mCapabilities"_s);
    QVERIFY(mCapabilities);
    QCOMPARE(mCapabilities->text(), u"None"_s);

    auto mInstructions = w.findChild<QTextEdit *>(u"mInstructions"_s);
    QVERIFY(mInstructions);
    QVERIFY(mInstructions->isReadOnly());
    QVERIFY(mInstructions->toPlainText().isEmpty());
}

void KMcpClientInspectorInitializeWidgetTest::shouldShowResult()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorInitializeWidget w(&manager);
    const QJsonObject obj =
        QJsonDocument::fromJson(R"({"jsonrpc": "2.0", "id": 0, "result": {"protocolVersion": "2025-06-18", "serverInfo": {"name": "test"}}})").object();
    w.setResult(obj);
    QCOMPARE(QJsonDocument::fromJson(w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s)->toPlainText().toUtf8()).object(), obj);
}

void KMcpClientInspectorInitializeWidgetTest::shouldShowInitializeResult()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorInitializeWidget w(&manager);
    const QJsonObject obj = QJsonDocument::fromJson(R"({"protocolVersion": "2025-06-18",
        "serverInfo": {"name": "test", "title": "Test Server", "version": "1.2.3"},
        "capabilities": {"tools": {"listChanged": true}, "logging": {}},
        "instructions": "Use the tools wisely."})")
                                .object();
    w.setInitializeResult(TextAutoGenerateTextMcpProtocolCore::McpProtocolInitializeResult::fromJson(obj));

    QCOMPARE(w.findChild<QLabel *>(u"mServerName"_s)->text(), u"test"_s);
    QCOMPARE(w.findChild<QLabel *>(u"mServerTitle"_s)->text(), u"Test Server"_s);
    QCOMPARE(w.findChild<QLabel *>(u"mServerVersion"_s)->text(), u"1.2.3"_s);
    QCOMPARE(w.findChild<QLabel *>(u"mProtocolVersion"_s)->text(), u"2025-06-18"_s);
    QCOMPARE(w.findChild<QLabel *>(u"mCapabilities"_s)->text(), u"Tools (list changed notifications)\nLogging"_s);
    QCOMPARE(w.findChild<QTextEdit *>(u"mInstructions"_s)->toPlainText(), u"Use the tools wisely."_s);
}

void KMcpClientInspectorInitializeWidgetTest::shouldConvertCapabilitiesToText()
{
    const QJsonObject obj = QJsonDocument::fromJson(R"({"prompts": {}, "resources": {"subscribe": true, "listChanged": true}, "completions": {},
        "experimental": {"foo": {}}})")
                                .object();
    QCOMPARE(KMcpClientInspectorInitializeWidget::capabilitiesToText(TextAutoGenerateTextMcpProtocolCore::McpProtocolServerCapabilities::fromJson(obj)),
             u"Prompts\nResources (subscribe, list changed notifications)\nCompletions\nExperimental (foo)"_s);
}

#include "moc_kmcpclientinspectorinitializewidgettest.cpp"
