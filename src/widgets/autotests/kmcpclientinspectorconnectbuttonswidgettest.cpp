/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorconnectbuttonswidgettest.h"
#include "kmcpclientinspectorconnectbuttonswidget.h"
#include <QTest>
QTEST_GUILESS_MAIN(KMcpClientInspectorConnectButtonsWidgetTest)

KMcpClientInspectorConnectButtonsWidgetTest::KMcpClientInspectorConnectButtonsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorConnectButtonsWidgetTest::shouldHaveDefaultValues()
{
    const KMcpClientInspectorConnectButtonsWidget w;
    // TODO
}

#include "moc_kmcpclientinspectorconnectbuttonswidgettest.cpp"
