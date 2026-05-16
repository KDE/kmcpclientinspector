/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorserversettingswidgettest.h"
#include "kmcpclientinspectorserversettingswidget.h"
#include <QTest>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
QTEST_MAIN(KMcpClientInspectorServerSettingsWidgetTest)
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorServerSettingsWidgetTest::KMcpClientInspectorServerSettingsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorServerSettingsWidgetTest::shouldHaveDefaultValues()
{
    const KMcpClientInspectorServerSettingsWidget w(TextAutoGenerateTextMcpProtocolCore::McpServer{});

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins{});

    // TODO
}

#include "moc_kmcpclientinspectorserversettingswidgettest.cpp"
