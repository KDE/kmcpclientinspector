/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorconnectbuttonswidgettest.h"
#include "kmcpclientinspectorconnectbuttonswidget.h"
#include <QHBoxLayout>
#include <QTest>
QTEST_MAIN(KMcpClientInspectorConnectButtonsWidgetTest)
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorConnectButtonsWidgetTest::KMcpClientInspectorConnectButtonsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorConnectButtonsWidgetTest::shouldHaveDefaultValues()
{
    const KMcpClientInspectorConnectButtonsWidget w;
    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});
    // TODO
}

#include "moc_kmcpclientinspectorconnectbuttonswidgettest.cpp"
