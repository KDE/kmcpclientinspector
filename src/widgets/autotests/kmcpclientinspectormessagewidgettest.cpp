/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectormessagewidgettest.h"
#include "actions/kmcpclientinspectormessagewidget.h"
#include <QTest>

QTEST_MAIN(KMcpClientInspectorMessageWidgetTest)

KMcpClientInspectorMessageWidgetTest::KMcpClientInspectorMessageWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorMessageWidgetTest::shouldHaveDefaultValues()
{
    const KMcpClientInspectorMessageWidget w;
    QVERIFY(w.isCloseButtonVisible());
    QCOMPARE(w.messageType(), KMessageWidget::Error);
}
#include "moc_kmcpclientinspectormessagewidgettest.cpp"
