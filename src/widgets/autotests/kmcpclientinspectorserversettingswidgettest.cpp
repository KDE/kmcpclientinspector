/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorserversettingswidgettest.h"
#include "actions/kmcpclientinspectorcapabilitywidget.h"
#include "kmcpclientinspectorconnectbuttonswidget.h"
#include "kmcpclientinspectorserverinfowidget.h"
#include "kmcpclientinspectorserversettingswidget.h"
#include <QPushButton>
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

    auto mServerInfoWidget = w.findChild<KMcpClientInspectorServerInfoWidget *>(u"mServerInfoWidget"_s);
    QVERIFY(mServerInfoWidget);

    auto mEditButton = w.findChild<QPushButton *>(u"mEditButton"_s);
    QVERIFY(mEditButton);
    QVERIFY(!mEditButton->text().isEmpty());
    QVERIFY(mEditButton->isEnabled());

    auto mConnectButtonsWidget = w.findChild<KMcpClientInspectorConnectButtonsWidget *>(u"mConnectButtonsWidget"_s);
    QVERIFY(mConnectButtonsWidget);

    auto mCapabilityWidget = w.findChild<KMcpClientInspectorCapabilityWidget *>(u"mCapabilityWidget"_s);
    QVERIFY(mCapabilityWidget);
}

#include "moc_kmcpclientinspectorserversettingswidgettest.cpp"
