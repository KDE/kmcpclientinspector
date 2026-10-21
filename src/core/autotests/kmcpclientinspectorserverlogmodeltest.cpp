/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorserverlogmodeltest.h"
#include "kmcpclientinspectorserverlogmodel.h"
#include <QAbstractItemModelTester>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>
QTEST_GUILESS_MAIN(KMcpClientInspectorServerLogModelTest)
using namespace Qt::Literals::StringLiterals;
using LoggingLevel = TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel;

static TextAutoGenerateTextMcpProtocolCore::McpProtocolLoggingMessageNotificationParams
createParams(LoggingLevel level, const QJsonValue &data, const std::optional<QString> &logger = std::nullopt)
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolLoggingMessageNotificationParams params;
    params.setLevel(level);
    params.setData(data);
    params.setLogger(logger);
    return params;
}

static const QDateTime dateTime(QDate(2026, 1, 1), QTime(10, 0));

KMcpClientInspectorServerLogModelTest::KMcpClientInspectorServerLogModelTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorServerLogModelTest::shouldHaveDefaultValues()
{
    const KMcpClientInspectorServerLogModel model;
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(model.columnCount(), 5);
}

void KMcpClientInspectorServerLogModelTest::shouldAddLoggingMessage()
{
    KMcpClientInspectorServerLogModel model;
    QSignalSpy rowsInsertedSpy(&model, &QAbstractItemModel::rowsInserted);

    model.addLoggingMessage(dateTime, createParams(LoggingLevel::Error, u"boom"_s, u"database"_s));
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(rowsInsertedSpy.count(), 1);

    QCOMPARE(model.index(0, KMcpClientInspectorServerLogModel::Level).data().toString(), u"error"_s);
    QCOMPARE(model.index(0, KMcpClientInspectorServerLogModel::Level).data(KMcpClientInspectorServerLogModel::LoggingLevelRole).toInt(),
             static_cast<int>(LoggingLevel::Error));
    QCOMPARE(model.index(0, KMcpClientInspectorServerLogModel::Logger).data().toString(), u"database"_s);
    QCOMPARE(model.index(0, KMcpClientInspectorServerLogModel::Message).data().toString(), u"boom"_s);
    QVERIFY(!model.index(0, KMcpClientInspectorServerLogModel::Json).data().toJsonObject().isEmpty());

    model.addLoggingMessage(dateTime, createParams(LoggingLevel::Info, u"hello"_s));
    QCOMPARE(model.rowCount(), 2);
    QVERIFY(model.index(1, KMcpClientInspectorServerLogModel::Logger).data().toString().isEmpty());

    // Flat model: children of a valid index have no rows
    QCOMPARE(model.rowCount(model.index(0, 0)), 0);
}

void KMcpClientInspectorServerLogModelTest::shouldDisplayMessage_data()
{
    QTest::addColumn<QJsonValue>("data");
    QTest::addColumn<QString>("message");
    QTest::newRow("string") << QJsonValue(u"text"_s) << u"text"_s;
    QTest::newRow("object") << QJsonValue(QJsonObject{{u"error"_s, u"failed"_s}}) << u"{\"error\":\"failed\"}"_s;
    QTest::newRow("number") << QJsonValue(42) << u"42"_s;
}

void KMcpClientInspectorServerLogModelTest::shouldDisplayMessage()
{
    QFETCH(QJsonValue, data);
    QFETCH(QString, message);
    KMcpClientInspectorServerLogModel model;
    model.addLoggingMessage(dateTime, createParams(LoggingLevel::Info, data));
    QCOMPARE(model.index(0, KMcpClientInspectorServerLogModel::Message).data().toString(), message);
}

void KMcpClientInspectorServerLogModelTest::shouldClearModel()
{
    KMcpClientInspectorServerLogModel model;
    model.addLoggingMessage(dateTime, createParams(LoggingLevel::Info, u"a"_s));
    model.addLoggingMessage(dateTime, createParams(LoggingLevel::Warning, u"b"_s));
    QCOMPARE(model.rowCount(), 2);

    QSignalSpy modelResetSpy(&model, &QAbstractItemModel::modelReset);
    model.clear();
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(modelResetSpy.count(), 1);
}

void KMcpClientInspectorServerLogModelTest::shouldPassModelTester()
{
    KMcpClientInspectorServerLogModel model;
    QAbstractItemModelTester tester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    model.addLoggingMessage(dateTime, createParams(LoggingLevel::Info, u"a"_s));
    model.addLoggingMessage(dateTime, createParams(LoggingLevel::Unknown, u"b"_s));
    model.clear();
    model.addLoggingMessage(dateTime, createParams(LoggingLevel::Emergency, u"c"_s));
}

#include "moc_kmcpclientinspectorserverlogmodeltest.cpp"
