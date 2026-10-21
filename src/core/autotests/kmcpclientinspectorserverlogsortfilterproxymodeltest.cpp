/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorserverlogsortfilterproxymodeltest.h"
#include "kmcpclientinspectorserverlogmodel.h"
#include "kmcpclientinspectorserverlogsortfilterproxymodel.h"
#include <QTest>
QTEST_GUILESS_MAIN(KMcpClientInspectorServerLogSortFilterProxyModelTest)
using namespace Qt::Literals::StringLiterals;
using LoggingLevel = TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel;

static void addMessage(KMcpClientInspectorServerLogModel &model, LoggingLevel level, const QString &message, const QString &logger = {})
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolLoggingMessageNotificationParams params;
    params.setLevel(level);
    params.setData(message);
    if (!logger.isEmpty()) {
        params.setLogger(logger);
    }
    model.addLoggingMessage(QDateTime::currentDateTime(), params);
}

KMcpClientInspectorServerLogSortFilterProxyModelTest::KMcpClientInspectorServerLogSortFilterProxyModelTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorServerLogSortFilterProxyModelTest::shouldHaveDefaultValues()
{
    const KMcpClientInspectorServerLogSortFilterProxyModel proxy;
    QCOMPARE(proxy.minimumLevel(), LoggingLevel::Debug);
}

void KMcpClientInspectorServerLogSortFilterProxyModelTest::shouldFilterByMinimumLevel()
{
    KMcpClientInspectorServerLogModel model;
    addMessage(model, LoggingLevel::Debug, u"debug"_s);
    addMessage(model, LoggingLevel::Info, u"info"_s);
    addMessage(model, LoggingLevel::Warning, u"warning"_s);
    addMessage(model, LoggingLevel::Emergency, u"emergency"_s);
    addMessage(model, LoggingLevel::Unknown, u"unknown"_s);

    KMcpClientInspectorServerLogSortFilterProxyModel proxy;
    proxy.setSourceModel(&model);
    QCOMPARE(proxy.rowCount(), 5);

    proxy.setMinimumLevel(LoggingLevel::Warning);
    QCOMPARE(proxy.rowCount(), 3);

    proxy.setMinimumLevel(LoggingLevel::Emergency);
    // Unknown level is always shown
    QCOMPARE(proxy.rowCount(), 2);

    proxy.setMinimumLevel(LoggingLevel::Debug);
    QCOMPARE(proxy.rowCount(), 5);
}

void KMcpClientInspectorServerLogSortFilterProxyModelTest::shouldFilterByText()
{
    KMcpClientInspectorServerLogModel model;
    addMessage(model, LoggingLevel::Info, u"connection opened"_s, u"network"_s);
    addMessage(model, LoggingLevel::Error, u"query failed"_s, u"database"_s);
    addMessage(model, LoggingLevel::Warning, u"slow network"_s);

    KMcpClientInspectorServerLogSortFilterProxyModel proxy;
    proxy.setSourceModel(&model);

    proxy.setFilterString(u"network"_s);
    QCOMPARE(proxy.rowCount(), 2);

    proxy.setFilterString(u"DATABASE"_s);
    QCOMPARE(proxy.rowCount(), 1);

    proxy.setMinimumLevel(LoggingLevel::Critical);
    QCOMPARE(proxy.rowCount(), 0);

    proxy.setMinimumLevel(LoggingLevel::Debug);
    proxy.setFilterString({});
    QCOMPARE(proxy.rowCount(), 3);
}

#include "moc_kmcpclientinspectorserverlogsortfilterproxymodeltest.cpp"
