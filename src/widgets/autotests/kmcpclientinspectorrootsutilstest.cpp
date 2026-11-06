/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorrootsutilstest.h"
#include "common/kmcpclientinspectorrootsutils.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(KMcpClientInspectorRootsUtilsTest)

KMcpClientInspectorRootsUtilsTest::KMcpClientInspectorRootsUtilsTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorRootsUtilsTest::shouldReturnEffectiveRoots_data()
{
    QTest::addColumn<QStringList>("serverRoots");
    QTest::addColumn<bool>("useGlobalRoots");
    QTest::addColumn<QStringList>("globalRoots");
    QTest::addColumn<QStringList>("expected");

    const QStringList global{u"/global"_s};
    const QStringList own{u"/own"_s};
    QTest::newRow("empty-use-global") << QStringList() << true << global << global;
    QTest::newRow("empty-no-global") << QStringList() << false << global << QStringList();
    QTest::newRow("own-use-global") << own << true << global << own;
    QTest::newRow("own-no-global") << own << false << global << own;
    QTest::newRow("all-empty") << QStringList() << true << QStringList() << QStringList();
}

void KMcpClientInspectorRootsUtilsTest::shouldReturnEffectiveRoots()
{
    QFETCH(QStringList, serverRoots);
    QFETCH(bool, useGlobalRoots);
    QFETCH(QStringList, globalRoots);
    QFETCH(QStringList, expected);

    TextAutoGenerateTextMcpProtocolCore::McpProtocolSettings settings;
    settings.setRoots(serverRoots);
    settings.setUseGlobalRoots(useGlobalRoots);
    QCOMPARE(KMcpClientInspectorRootsUtils::effectiveRoots(settings, globalRoots), expected);
}

#include "moc_kmcpclientinspectorrootsutilstest.cpp"
