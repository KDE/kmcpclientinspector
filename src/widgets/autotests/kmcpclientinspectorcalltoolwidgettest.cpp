/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorcalltoolwidgettest.h"
#include "actions/kmcpclientinspectorcalltoolwidget.h"
#include "actions/kmcpclientinspectortoolargumentswidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <KListWidgetSearchLine>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QTest>
#include <QTextBrowser>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolCallToolResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
QTEST_MAIN(KMcpClientInspectorCallToolWidgetTest)
using namespace Qt::Literals::StringLiterals;

namespace
{
QJsonObject jsonFromText(const QByteArray &json)
{
    return QJsonDocument::fromJson(json).object();
}

const QByteArray toolsListResponse = R"({
    "jsonrpc": "2.0",
    "id": 2,
    "result": {
        "tools": [
            {"name": "echo", "description": "Echo text", "inputSchema": {"type": "object", "properties": {"text": {"type": "string"}}, "required": ["text"]}},
            {"name": "add", "title": "Addition", "inputSchema": {"type": "object", "properties": {"a": {"type": "number"}, "b": {"type": "number"}}}}
        ]
    }
})";
}

KMcpClientInspectorCallToolWidgetTest::KMcpClientInspectorCallToolWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorCallToolWidgetTest::shouldHaveDefaultValues()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorCallToolWidget w(&manager);
    QVERIFY(w.tools().isEmpty());

    auto mListToolsButton = w.findChild<QPushButton *>(u"mListToolsButton"_s);
    QVERIFY(mListToolsButton);
    QVERIFY(!mListToolsButton->text().isEmpty());

    auto mToolsListWidget = w.findChild<QListWidget *>(u"mToolsListWidget"_s);
    QVERIFY(mToolsListWidget);
    QCOMPARE(mToolsListWidget->count(), 0);

    auto searchLine = w.findChild<KListWidgetSearchLine *>(u"searchLine"_s);
    QVERIFY(searchLine);
    QVERIFY(!searchLine->placeholderText().isEmpty());

    auto mToolNameLabel = w.findChild<QLabel *>(u"mToolNameLabel"_s);
    QVERIFY(mToolNameLabel);
    QVERIFY(!mToolNameLabel->text().isEmpty());

    auto mToolDescriptionLabel = w.findChild<QLabel *>(u"mToolDescriptionLabel"_s);
    QVERIFY(mToolDescriptionLabel);
    QVERIFY(mToolDescriptionLabel->text().isEmpty());

    auto mArgumentsWidget = w.findChild<KMcpClientInspectorToolArgumentsWidget *>(u"mArgumentsWidget"_s);
    QVERIFY(mArgumentsWidget);
    QVERIFY(!mArgumentsWidget->isEnabled());

    auto mCallToolButton = w.findChild<QPushButton *>(u"mCallToolButton"_s);
    QVERIFY(mCallToolButton);
    QVERIFY(!mCallToolButton->isEnabled());

    auto mStatusLabel = w.findChild<QLabel *>(u"mStatusLabel"_s);
    QVERIFY(mStatusLabel);
    QVERIFY(mStatusLabel->text().isEmpty());

    auto mResultTabWidget = w.findChild<QTabWidget *>(u"mResultTabWidget"_s);
    QVERIFY(mResultTabWidget);
    QCOMPARE(mResultTabWidget->count(), 2);

    auto mContentBrowser = w.findChild<QTextBrowser *>(u"mContentBrowser"_s);
    QVERIFY(mContentBrowser);
    QVERIFY(!mContentBrowser->openLinks());

    QVERIFY(w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s));
}

void KMcpClientInspectorCallToolWidgetTest::shouldUpdateToolsList()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorCallToolWidget w(&manager);
    w.setToolsList(jsonFromText(toolsListResponse));
    QCOMPARE(w.tools().count(), 2);

    auto mToolsListWidget = w.findChild<QListWidget *>(u"mToolsListWidget"_s);
    QCOMPARE(mToolsListWidget->count(), 2);
    QCOMPARE(mToolsListWidget->item(1)->text(), u"echo"_s);
    QCOMPARE(mToolsListWidget->item(1)->toolTip(), u"Echo text"_s);
    QCOMPARE(mToolsListWidget->item(0)->text(), u"Addition (add)"_s);
    QCOMPARE(mToolsListWidget->currentRow(), -1);
    // Tools are sorted like list: list row is index in tools()
    QCOMPARE(w.tools().at(0).name(), u"add"_s);
    QCOMPARE(w.tools().at(1).name(), u"echo"_s);

    mToolsListWidget->setCurrentRow(1);
    QCOMPARE(w.findChild<QLabel *>(u"mToolNameLabel"_s)->text(), u"echo"_s);
    QCOMPARE(w.findChild<QLabel *>(u"mToolDescriptionLabel"_s)->text(), u"Echo text"_s);
    auto mArgumentsWidget = w.findChild<KMcpClientInspectorToolArgumentsWidget *>(u"mArgumentsWidget"_s);
    QVERIFY(mArgumentsWidget->isEnabled());
    QCOMPARE(mArgumentsWidget->arguments(), QJsonObject({{u"text"_s, QString()}}));
    // Client is not initialized
    QVERIFY(!w.findChild<QPushButton *>(u"mCallToolButton"_s)->isEnabled());

    // Selected tool is kept after update
    mToolsListWidget->setCurrentRow(0);
    w.setToolsList(jsonFromText(toolsListResponse));
    QCOMPARE(mToolsListWidget->currentRow(), 0);
    QCOMPARE(w.findChild<QLabel *>(u"mToolNameLabel"_s)->text(), u"add"_s);

    // Error response keeps list
    w.setToolsList(jsonFromText(R"({"jsonrpc": "2.0", "id": 3, "error": {"code": -32601, "message": "Method not found"}})"));
    QCOMPARE(w.tools().count(), 2);
}

void KMcpClientInspectorCallToolWidgetTest::shouldShowResult()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorCallToolWidget w(&manager);
    w.setResult(jsonFromText(R"({"jsonrpc": "2.0", "id": 4, "result": {"content": [{"type": "text", "text": "Hello <world>"}]}})"));
    QVERIFY(!w.findChild<QLabel *>(u"mStatusLabel"_s)->text().isEmpty());
    auto mContentBrowser = w.findChild<QTextBrowser *>(u"mContentBrowser"_s);
    QVERIFY(mContentBrowser->toPlainText().contains(u"Hello <world>"_s));
    QCOMPARE(w.findChild<QTabWidget *>(u"mResultTabWidget"_s)->currentWidget(), mContentBrowser);
}

void KMcpClientInspectorCallToolWidgetTest::shouldShowError()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorCallToolWidget w(&manager);
    w.setResult(jsonFromText(R"({"jsonrpc": "2.0", "id": 5, "error": {"code": -32602, "message": "Unknown tool"}})"));
    const QString status = w.findChild<QLabel *>(u"mStatusLabel"_s)->text();
    QVERIFY(status.contains(u"-32602"_s));
    QVERIFY(status.contains(u"Unknown tool"_s));
    QVERIFY(w.findChild<QTextBrowser *>(u"mContentBrowser"_s)->toPlainText().isEmpty());
    QCOMPARE(w.findChild<QTabWidget *>(u"mResultTabWidget"_s)->currentWidget(), w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s));
}

void KMcpClientInspectorCallToolWidgetTest::shouldConvertResultToHtml()
{
    using TextAutoGenerateTextMcpProtocolCore::McpProtocolCallToolResult;
    // Empty result
    QVERIFY(!KMcpClientInspectorCallToolWidget::resultToHtml(McpProtocolCallToolResult()).isEmpty());

    const McpProtocolCallToolResult result = McpProtocolCallToolResult::fromJson(jsonFromText(R"({
        "content": [
            {"type": "text", "text": "a < b"},
            {"type": "resource_link", "uri": "file:///tmp/foo.txt", "name": "foo.txt", "description": "Foo file"},
            {"type": "resource", "resource": {"uri": "file:///tmp/bar.txt", "text": "Bar content"}}
        ],
        "structuredContent": {"temperature": 22}
    })"));
    const QString html = KMcpClientInspectorCallToolWidget::resultToHtml(result);
    QVERIFY(html.contains(u"a &lt; b"_s));
    QVERIFY(html.contains(u"file:///tmp/foo.txt"_s));
    QVERIFY(html.contains(u"Foo file"_s));
    QVERIFY(html.contains(u"Bar content"_s));
    QVERIFY(html.contains(u"temperature"_s));
}

#include "moc_kmcpclientinspectorcalltoolwidgettest.cpp"
