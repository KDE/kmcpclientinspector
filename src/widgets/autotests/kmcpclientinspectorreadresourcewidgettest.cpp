/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorreadresourcewidgettest.h"
#include "actions/kmcpclientinspectorreadresourcewidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <KListWidgetSearchLine>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QTest>
#include <QTextBrowser>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolReadResourceResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
QTEST_MAIN(KMcpClientInspectorReadResourceWidgetTest)
using namespace Qt::Literals::StringLiterals;

namespace
{
QJsonObject jsonFromText(const QByteArray &json)
{
    return QJsonDocument::fromJson(json).object();
}

const QByteArray resourcesListResponse = R"({
    "jsonrpc": "2.0",
    "id": 2,
    "result": {
        "resources": [
            {"uri": "file:///project/readme.md", "name": "readme", "description": "Project readme", "mimeType": "text/markdown"},
            {"uri": "file:///project/logo.png", "name": "logo", "title": "Logo"},
            {"name": "invalid"}
        ]
    }
})";
}

KMcpClientInspectorReadResourceWidgetTest::KMcpClientInspectorReadResourceWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorReadResourceWidgetTest::shouldHaveDefaultValues()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorReadResourceWidget w(&manager);
    QVERIFY(w.resources().isEmpty());
    QVERIFY(w.subscribedResources().isEmpty());

    auto mListResourcesButton = w.findChild<QPushButton *>(u"mListResourcesButton"_s);
    QVERIFY(mListResourcesButton);
    QVERIFY(!mListResourcesButton->text().isEmpty());
    QVERIFY(!mListResourcesButton->isEnabled());

    auto mResourcesListWidget = w.findChild<QListWidget *>(u"mResourcesListWidget"_s);
    QVERIFY(mResourcesListWidget);
    QCOMPARE(mResourcesListWidget->count(), 0);

    auto searchLine = w.findChild<KListWidgetSearchLine *>(u"searchLine"_s);
    QVERIFY(searchLine);
    QVERIFY(!searchLine->placeholderText().isEmpty());

    auto mResourceNameLabel = w.findChild<QLabel *>(u"mResourceNameLabel"_s);
    QVERIFY(mResourceNameLabel);
    QVERIFY(!mResourceNameLabel->text().isEmpty());

    auto mResourceDescriptionLabel = w.findChild<QLabel *>(u"mResourceDescriptionLabel"_s);
    QVERIFY(mResourceDescriptionLabel);
    QVERIFY(mResourceDescriptionLabel->text().isEmpty());

    auto mUriLineEdit = w.findChild<QLineEdit *>(u"mUriLineEdit"_s);
    QVERIFY(mUriLineEdit);
    QVERIFY(mUriLineEdit->text().isEmpty());
    QVERIFY(mUriLineEdit->isClearButtonEnabled());
    QVERIFY(!mUriLineEdit->placeholderText().isEmpty());

    for (const auto &name : {u"mReadButton"_s, u"mSubscribeButton"_s, u"mUnsubscribeButton"_s}) {
        auto button = w.findChild<QPushButton *>(name);
        QVERIFY(button);
        QVERIFY(!button->text().isEmpty());
        QVERIFY(!button->isEnabled());
    }

    auto mStatusLabel = w.findChild<QLabel *>(u"mStatusLabel"_s);
    QVERIFY(mStatusLabel);
    QVERIFY(mStatusLabel->text().isEmpty());

    auto mResultTabWidget = w.findChild<QTabWidget *>(u"mResultTabWidget"_s);
    QVERIFY(mResultTabWidget);
    QCOMPARE(mResultTabWidget->count(), 2);

    auto mContentsBrowser = w.findChild<QTextBrowser *>(u"mContentsBrowser"_s);
    QVERIFY(mContentsBrowser);
    QVERIFY(!mContentsBrowser->openLinks());

    QVERIFY(w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s));
}

void KMcpClientInspectorReadResourceWidgetTest::shouldUpdateResourcesList()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorReadResourceWidget w(&manager);
    w.setResourcesList(jsonFromText(resourcesListResponse));
    // Resource without uri is ignored
    QCOMPARE(w.resources().count(), 2);

    auto mResourcesListWidget = w.findChild<QListWidget *>(u"mResourcesListWidget"_s);
    QCOMPARE(mResourcesListWidget->count(), 2);
    QCOMPARE(mResourcesListWidget->item(0)->text(), u"Logo (logo)"_s);
    QCOMPARE(mResourcesListWidget->item(1)->text(), u"readme"_s);
    QCOMPARE(mResourcesListWidget->item(1)->toolTip(), u"file:///project/readme.md\nProject readme"_s);
    QCOMPARE(mResourcesListWidget->currentRow(), -1);

    mResourcesListWidget->setCurrentRow(1);
    QCOMPARE(w.findChild<QLabel *>(u"mResourceNameLabel"_s)->text(), u"readme [text/markdown]"_s);
    QCOMPARE(w.findChild<QLabel *>(u"mResourceDescriptionLabel"_s)->text(), u"Project readme"_s);
    auto mUriLineEdit = w.findChild<QLineEdit *>(u"mUriLineEdit"_s);
    QCOMPARE(mUriLineEdit->text(), u"file:///project/readme.md"_s);
    // Client is not initialized
    QVERIFY(!w.findChild<QPushButton *>(u"mReadButton"_s)->isEnabled());

    // Selected resource and edited uri are kept after update
    mUriLineEdit->setText(u"file:///project/readme.md?v=2"_s);
    w.setResourcesList(jsonFromText(resourcesListResponse));
    QCOMPARE(mResourcesListWidget->currentRow(), 1);
    QCOMPARE(mUriLineEdit->text(), u"file:///project/readme.md?v=2"_s);

    // Error response keeps list
    w.setResourcesList(jsonFromText(R"({"jsonrpc": "2.0", "id": 3, "error": {"code": -32601, "message": "Method not found"}})"));
    QCOMPARE(w.resources().count(), 2);
}

void KMcpClientInspectorReadResourceWidgetTest::shouldShowResult()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorReadResourceWidget w(&manager);
    w.setResult(jsonFromText(R"({"jsonrpc": "2.0", "id": 4, "result": {"contents": [{"uri": "file:///project/readme.md", "text": "# Read <me>"}]}})"));
    QVERIFY(!w.findChild<QLabel *>(u"mStatusLabel"_s)->text().isEmpty());
    auto mContentsBrowser = w.findChild<QTextBrowser *>(u"mContentsBrowser"_s);
    QVERIFY(mContentsBrowser->toPlainText().contains(u"# Read <me>"_s));
    QCOMPARE(w.findChild<QTabWidget *>(u"mResultTabWidget"_s)->currentWidget(), mContentsBrowser);
}

void KMcpClientInspectorReadResourceWidgetTest::shouldShowError()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorReadResourceWidget w(&manager);
    w.setResult(jsonFromText(R"({"jsonrpc": "2.0", "id": 5, "error": {"code": -32002, "message": "Resource not found"}})"));
    const QString status = w.findChild<QLabel *>(u"mStatusLabel"_s)->text();
    QVERIFY(status.contains(u"-32002"_s));
    QVERIFY(status.contains(u"Resource not found"_s));
    QVERIFY(w.findChild<QTextBrowser *>(u"mContentsBrowser"_s)->toPlainText().isEmpty());
    QCOMPARE(w.findChild<QTabWidget *>(u"mResultTabWidget"_s)->currentWidget(), w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s));
}

void KMcpClientInspectorReadResourceWidgetTest::shouldIgnoreUnknownSubscriptionResult()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorReadResourceWidget w(&manager);
    // Response to a request not sent by this widget
    w.setSubscriptionResult(jsonFromText(R"({"jsonrpc": "2.0", "id": 6, "result": {}})"), true);
    QVERIFY(w.subscribedResources().isEmpty());

    w.setSubscriptionResult(jsonFromText(R"({"jsonrpc": "2.0", "id": 7, "error": {"code": -32601, "message": "Method not found"}})"), true);
    QVERIFY(w.subscribedResources().isEmpty());
    QVERIFY(w.findChild<QLabel *>(u"mStatusLabel"_s)->text().contains(u"Method not found"_s));
}

void KMcpClientInspectorReadResourceWidgetTest::shouldConvertResultToHtml()
{
    using TextAutoGenerateTextMcpProtocolCore::McpProtocolReadResourceResult;
    // Empty result
    QVERIFY(!KMcpClientInspectorReadResourceWidget::resultToHtml(McpProtocolReadResourceResult()).isEmpty());

    const McpProtocolReadResourceResult result = McpProtocolReadResourceResult::fromJson(jsonFromText(R"({
        "contents": [
            {"uri": "file:///tmp/foo.txt", "mimeType": "text/plain", "text": "a < b"},
            {"uri": "file:///tmp/bar.png", "mimeType": "image/png", "blob": "iVBORw0KGgo="}
        ]
    })"));
    const QString html = KMcpClientInspectorReadResourceWidget::resultToHtml(result);
    QVERIFY(html.contains(u"file:///tmp/foo.txt"_s));
    QVERIFY(html.contains(u"a &lt; b"_s));
    QVERIFY(html.contains(u"file:///tmp/bar.png"_s));
    QVERIFY(html.contains(u"image/png"_s));
}

#include "moc_kmcpclientinspectorreadresourcewidgettest.cpp"
