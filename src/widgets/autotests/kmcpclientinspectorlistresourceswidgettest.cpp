/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlistresourceswidgettest.h"
#include "actions/kmcpclientinspectorlistresourceswidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
QTEST_MAIN(KMcpClientInspectorListResourcesWidgetTest)
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorListResourcesWidgetTest::KMcpClientInspectorListResourcesWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorListResourcesWidgetTest::shouldHaveDefaultValues()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorListResourcesWidget w(&manager);

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

void KMcpClientInspectorListResourcesWidgetTest::shouldShowResult()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorListResourcesWidget w(&manager);
    const QJsonObject obj = QJsonDocument::fromJson(R"({"jsonrpc": "2.0", "id": 1, "result": {"foo": "bar", "list": [1, 2]}})").object();
    w.setResult(obj);
    QCOMPARE(QJsonDocument::fromJson(w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s)->toPlainText().toUtf8()).object(), obj);
}

#include "moc_kmcpclientinspectorlistresourceswidgettest.cpp"
