/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorresourcetemplateswidgettest.h"
#include "actions/kmcpclientinspectorresourcetemplateswidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
QTEST_MAIN(KMcpClientInspectorResourceTemplatesWidgetTest)
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorResourceTemplatesWidgetTest::KMcpClientInspectorResourceTemplatesWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorResourceTemplatesWidgetTest::shouldHaveDefaultValues()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorResourceTemplatesWidget w(&manager);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto actionButton = w.findChild<QPushButton *>(u"actionButton"_s);
    QVERIFY(actionButton);
    QVERIFY(!actionButton->text().isEmpty());

    auto mJsonPlainTextEditWidget = w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s);
    QVERIFY(mJsonPlainTextEditWidget);
    QVERIFY(mJsonPlainTextEditWidget->toPlainText().isEmpty());
}

void KMcpClientInspectorResourceTemplatesWidgetTest::shouldShowResult()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorResourceTemplatesWidget w(&manager);
    const QJsonObject obj = QJsonDocument::fromJson(R"({"jsonrpc": "2.0", "id": 1, "result": {"foo": "bar", "list": [1, 2]}})").object();
    w.setResult(obj);
    QCOMPARE(QJsonDocument::fromJson(w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s)->toPlainText().toUtf8()).object(),
             obj[u"result"_s].toObject());
}

#include "moc_kmcpclientinspectorresourcetemplateswidgettest.cpp"
