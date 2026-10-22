/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorresultwidgettest.h"
#include "actions/kmcpclientinspectorresultwidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
QTEST_MAIN(KMcpClientInspectorResultWidgetTest)
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorResultWidgetTest::KMcpClientInspectorResultWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorResultWidgetTest::shouldHaveDefaultValues()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorResultWidget w(&manager, u"Foo"_s, TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Ping);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto actionButton = w.findChild<QPushButton *>(u"actionButton"_s);
    QVERIFY(actionButton);
    QCOMPARE(actionButton->text(), u"Foo"_s);

    auto mJsonPlainTextEditWidget = w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s);
    QVERIFY(mJsonPlainTextEditWidget);
    QVERIFY(mJsonPlainTextEditWidget->toPlainText().isEmpty());
}

void KMcpClientInspectorResultWidgetTest::shouldShowResult()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorResultWidget w(&manager, u"Foo"_s, TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Ping);
    const QJsonObject obj = QJsonDocument::fromJson(R"({"jsonrpc": "2.0", "id": 1, "result": {"foo": "bar", "list": [1, 2]}})").object();
    w.setResult(obj);
    QCOMPARE(QJsonDocument::fromJson(w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s)->toPlainText().toUtf8()).object(), obj);
}

#include "moc_kmcpclientinspectorresultwidgettest.cpp"
