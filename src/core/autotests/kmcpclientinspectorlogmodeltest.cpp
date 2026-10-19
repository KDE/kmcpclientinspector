/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlogmodeltest.h"
#include "kmcpclientinspectorlogmodel.h"
#include <QAbstractItemModelTester>
#include <QSignalSpy>
#include <QTest>
QTEST_GUILESS_MAIN(KMcpClientInspectorLogModelTest)

static LogInfo createLogInfo(quint64 identifier, LogInfo::Direction direction)
{
    LogInfo info;
    info.setIdentifier(identifier);
    info.setDirection(direction);
    info.setDateTime(QDateTime(QDate(2026, 1, 1), QTime(10, 0)));
    return info;
}

KMcpClientInspectorLogModelTest::KMcpClientInspectorLogModelTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorLogModelTest::shouldHaveDefaultValues()
{
    const KMcpClientInspectorLogModel model;
    QCOMPARE(model.rowCount(), 0);
}

void KMcpClientInspectorLogModelTest::shouldAddLogInfo()
{
    KMcpClientInspectorLogModel model;
    QSignalSpy rowsInsertedSpy(&model, &QAbstractItemModel::rowsInserted);

    model.addLogInfo(createLogInfo(1, LogInfo::Direction::Input));
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(rowsInsertedSpy.at(0).at(1).toInt(), 0);
    QCOMPARE(rowsInsertedSpy.at(0).at(2).toInt(), 0);

    model.addLogInfo(createLogInfo(2, LogInfo::Direction::Output));
    QCOMPARE(model.rowCount(), 2);
    QCOMPARE(rowsInsertedSpy.count(), 2);
    QCOMPARE(rowsInsertedSpy.at(1).at(1).toInt(), 1);
    QCOMPARE(rowsInsertedSpy.at(1).at(2).toInt(), 1);

    // Flat model: children of a valid index have no rows
    QCOMPARE(model.rowCount(model.index(0, 0)), 0);
}

void KMcpClientInspectorLogModelTest::shouldClearModel()
{
    KMcpClientInspectorLogModel model;
    model.addLogInfo(createLogInfo(1, LogInfo::Direction::Input));
    model.addLogInfo(createLogInfo(2, LogInfo::Direction::Output));
    QCOMPARE(model.rowCount(), 2);

    QSignalSpy modelResetSpy(&model, &QAbstractItemModel::modelReset);
    model.clear();
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(modelResetSpy.count(), 1);
}

void KMcpClientInspectorLogModelTest::shouldReturnInvalidDataForInvalidIndex()
{
    KMcpClientInspectorLogModel model;
    QVERIFY(!model.data(QModelIndex(), Qt::DisplayRole).isValid());

    model.addLogInfo(createLogInfo(1, LogInfo::Direction::Input));
    QVERIFY(!model.data(QModelIndex(), Qt::DisplayRole).isValid());
    QVERIFY(!model.index(1, 0).isValid());
}

void KMcpClientInspectorLogModelTest::shouldPassModelTester()
{
    KMcpClientInspectorLogModel model;
    QAbstractItemModelTester tester(&model, QAbstractItemModelTester::FailureReportingMode::QtTest);
    model.addLogInfo(createLogInfo(1, LogInfo::Direction::Input));
    model.addLogInfo(createLogInfo(2, LogInfo::Direction::Output));
    model.clear();
    model.addLogInfo(createLogInfo(3, LogInfo::Direction::Unknown));
}

#include "moc_kmcpclientinspectorlogmodeltest.cpp"
