/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorgetpromptwidgettest.h"
#include "actions/kmcpclientinspectorgetpromptwidget.h"
#include "actions/kmcpclientinspectorpromptargumentswidget.h"
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
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolGetPromptResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
QTEST_MAIN(KMcpClientInspectorGetPromptWidgetTest)
using namespace Qt::Literals::StringLiterals;

namespace
{
QJsonObject jsonFromText(const QByteArray &json)
{
    return QJsonDocument::fromJson(json).object();
}

const QByteArray promptsListResponse = R"({
    "jsonrpc": "2.0",
    "id": 2,
    "result": {
        "prompts": [
            {"name": "code_review", "description": "Review code", "arguments": [{"name": "code", "required": true}, {"name": "language"}]},
            {"name": "greeting", "title": "Greeting"}
        ]
    }
})";
}

KMcpClientInspectorGetPromptWidgetTest::KMcpClientInspectorGetPromptWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorGetPromptWidgetTest::shouldHaveDefaultValues()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorGetPromptWidget w(&manager);
    QVERIFY(w.prompts().isEmpty());

    auto mListPromptsButton = w.findChild<QPushButton *>(u"mListPromptsButton"_s);
    QVERIFY(mListPromptsButton);
    QVERIFY(!mListPromptsButton->text().isEmpty());

    auto mPromptsListWidget = w.findChild<QListWidget *>(u"mPromptsListWidget"_s);
    QVERIFY(mPromptsListWidget);
    QCOMPARE(mPromptsListWidget->count(), 0);

    auto searchLine = w.findChild<KListWidgetSearchLine *>(u"searchLine"_s);
    QVERIFY(searchLine);
    QVERIFY(!searchLine->placeholderText().isEmpty());

    auto mPromptNameLabel = w.findChild<QLabel *>(u"mPromptNameLabel"_s);
    QVERIFY(mPromptNameLabel);
    QVERIFY(!mPromptNameLabel->text().isEmpty());

    auto mPromptDescriptionLabel = w.findChild<QLabel *>(u"mPromptDescriptionLabel"_s);
    QVERIFY(mPromptDescriptionLabel);
    QVERIFY(mPromptDescriptionLabel->text().isEmpty());

    auto mArgumentsWidget = w.findChild<KMcpClientInspectorPromptArgumentsWidget *>(u"mArgumentsWidget"_s);
    QVERIFY(mArgumentsWidget);
    QVERIFY(!mArgumentsWidget->isEnabled());

    auto mGetPromptButton = w.findChild<QPushButton *>(u"mGetPromptButton"_s);
    QVERIFY(mGetPromptButton);
    QVERIFY(!mGetPromptButton->isEnabled());

    auto mStatusLabel = w.findChild<QLabel *>(u"mStatusLabel"_s);
    QVERIFY(mStatusLabel);
    QVERIFY(mStatusLabel->text().isEmpty());

    auto mResultTabWidget = w.findChild<QTabWidget *>(u"mResultTabWidget"_s);
    QVERIFY(mResultTabWidget);
    QCOMPARE(mResultTabWidget->count(), 2);

    auto mMessagesBrowser = w.findChild<QTextBrowser *>(u"mMessagesBrowser"_s);
    QVERIFY(mMessagesBrowser);
    QVERIFY(!mMessagesBrowser->openLinks());

    QVERIFY(w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s));
}

void KMcpClientInspectorGetPromptWidgetTest::shouldUpdatePromptsList()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorGetPromptWidget w(&manager);
    w.setPromptsList(jsonFromText(promptsListResponse));
    QCOMPARE(w.prompts().count(), 2);

    auto mPromptsListWidget = w.findChild<QListWidget *>(u"mPromptsListWidget"_s);
    QCOMPARE(mPromptsListWidget->count(), 2);
    QCOMPARE(mPromptsListWidget->item(0)->text(), u"code_review"_s);
    QCOMPARE(mPromptsListWidget->item(0)->toolTip(), u"Review code"_s);
    QCOMPARE(mPromptsListWidget->item(1)->text(), u"Greeting (greeting)"_s);
    QCOMPARE(mPromptsListWidget->currentRow(), -1);

    mPromptsListWidget->setCurrentRow(0);
    QCOMPARE(w.findChild<QLabel *>(u"mPromptNameLabel"_s)->text(), u"code_review"_s);
    QCOMPARE(w.findChild<QLabel *>(u"mPromptDescriptionLabel"_s)->text(), u"Review code"_s);
    auto mArgumentsWidget = w.findChild<KMcpClientInspectorPromptArgumentsWidget *>(u"mArgumentsWidget"_s);
    QVERIFY(mArgumentsWidget->isEnabled());
    QCOMPARE(mArgumentsWidget->missingRequiredArguments(), QStringList{u"code"_s});
    // Client is not initialized
    QVERIFY(!w.findChild<QPushButton *>(u"mGetPromptButton"_s)->isEnabled());

    // Selected prompt and its arguments are kept after update
    mArgumentsWidget->setArguments({{u"code"_s, u"foo"_s}});
    w.setPromptsList(jsonFromText(promptsListResponse));
    QCOMPARE(mPromptsListWidget->currentRow(), 0);
    QCOMPARE(mArgumentsWidget->arguments(), (QMap<QString, QString>{{u"code"_s, u"foo"_s}}));

    // Error response keeps list
    w.setPromptsList(jsonFromText(R"({"jsonrpc": "2.0", "id": 3, "error": {"code": -32601, "message": "Method not found"}})"));
    QCOMPARE(w.prompts().count(), 2);
}

void KMcpClientInspectorGetPromptWidgetTest::shouldShowResult()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorGetPromptWidget w(&manager);
    w.setResult(
        jsonFromText(R"({"jsonrpc": "2.0", "id": 4, "result": {"messages": [{"role": "user", "content": {"type": "text", "text": "Review <code>"}}]}})"));
    QVERIFY(!w.findChild<QLabel *>(u"mStatusLabel"_s)->text().isEmpty());
    auto mMessagesBrowser = w.findChild<QTextBrowser *>(u"mMessagesBrowser"_s);
    QVERIFY(mMessagesBrowser->toPlainText().contains(u"Review <code>"_s));
    QCOMPARE(w.findChild<QTabWidget *>(u"mResultTabWidget"_s)->currentWidget(), mMessagesBrowser);
}

void KMcpClientInspectorGetPromptWidgetTest::shouldShowError()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorGetPromptWidget w(&manager);
    w.setResult(jsonFromText(R"({"jsonrpc": "2.0", "id": 5, "error": {"code": -32602, "message": "Missing required argument"}})"));
    const QString status = w.findChild<QLabel *>(u"mStatusLabel"_s)->text();
    QVERIFY(status.contains(u"-32602"_s));
    QVERIFY(status.contains(u"Missing required argument"_s));
    QVERIFY(w.findChild<QTextBrowser *>(u"mMessagesBrowser"_s)->toPlainText().isEmpty());
    QCOMPARE(w.findChild<QTabWidget *>(u"mResultTabWidget"_s)->currentWidget(), w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s));
}

void KMcpClientInspectorGetPromptWidgetTest::shouldConvertResultToHtml()
{
    using TextAutoGenerateTextMcpProtocolCore::McpProtocolGetPromptResult;
    // Empty result
    QVERIFY(!KMcpClientInspectorGetPromptWidget::resultToHtml(McpProtocolGetPromptResult()).isEmpty());

    const McpProtocolGetPromptResult result = McpProtocolGetPromptResult::fromJson(jsonFromText(R"({
        "description": "Code review prompt",
        "messages": [
            {"role": "user", "content": {"type": "text", "text": "a < b"}},
            {"role": "assistant", "content": {"type": "resource", "resource": {"uri": "file:///tmp/bar.txt", "text": "Bar content"}}}
        ]
    })"));
    const QString html = KMcpClientInspectorGetPromptWidget::resultToHtml(result);
    QVERIFY(html.contains(u"Code review prompt"_s));
    QVERIFY(html.contains(u"a &lt; b"_s));
    QVERIFY(html.contains(u"file:///tmp/bar.txt"_s));
    QVERIFY(html.contains(u"Bar content"_s));
}

#include "moc_kmcpclientinspectorgetpromptwidgettest.cpp"
