/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectortreeviewtest.h"
#include "kmcpclientinspectortreeview.h"
#include <QTest>
QTEST_MAIN(KMcpClientInspectorTreeViewTest)

KMcpClientInspectorTreeViewTest::KMcpClientInspectorTreeViewTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorTreeViewTest::shouldHaveDefaultValues()
{
    const KMcpClientInspectorTreeView w(nullptr);
    // TODO
}
#include "moc_kmcpclientinspectortreeviewtest.cpp"
