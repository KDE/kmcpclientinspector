/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectoractiontabwidgettest.h"
#include "actions/kmcpclientinspectoractiontabwidget.h"
#include "actions/kmcpclientinspectorcalltoolwidget.h"
#include "actions/kmcpclientinspectorgetpromptwidget.h"
#include "actions/kmcpclientinspectorinitializewidget.h"
#include "actions/kmcpclientinspectorlistpromptswidget.h"
#include "actions/kmcpclientinspectorlistresourceswidget.h"
#include "actions/kmcpclientinspectorlisttoolswidget.h"
#include "actions/kmcpclientinspectormessagewidget.h"
#include "actions/kmcpclientinspectorpingwidget.h"
#include "actions/kmcpclientinspectorresourcetemplateswidget.h"
#include "actions/kmcpclientinspectorservernotificationwidget.h"
#include <QTabWidget>
#include <QTest>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
QTEST_MAIN(KMcpClientInspectorActionTabWidgetTest)
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorActionTabWidgetTest::KMcpClientInspectorActionTabWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorActionTabWidgetTest::shouldHaveDefaultValues()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    const KMcpClientInspectorActionTabWidget w(&manager);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());
    QCOMPARE(mainLayout->spacing(), 0);

    auto mMessageWidget = w.findChild<KMcpClientInspectorMessageWidget *>(u"mMessageWidget"_s);
    QVERIFY(mMessageWidget);

    auto mTabWidget = w.findChild<QTabWidget *>(u"mTabWidget"_s);
    QVERIFY(mTabWidget);
    QCOMPARE(mTabWidget->count(), 10);

    auto mInitializeWidget = w.findChild<KMcpClientInspectorInitializeWidget *>(u"mInitializeWidget"_s);
    QVERIFY(mInitializeWidget);
    QVERIFY(mTabWidget->isTabVisible(mTabWidget->indexOf(mInitializeWidget)));

    auto mPingWidget = w.findChild<KMcpClientInspectorPingWidget *>(u"mPingWidget"_s);
    QVERIFY(mPingWidget);
    QVERIFY(mTabWidget->isTabVisible(mTabWidget->indexOf(mPingWidget)));

    auto mListToolsWidget = w.findChild<KMcpClientInspectorListToolsWidget *>(u"mListToolsWidget"_s);
    QVERIFY(mListToolsWidget);
    QVERIFY(mTabWidget->isTabVisible(mTabWidget->indexOf(mListToolsWidget)));

    auto mServerNotificationWidget = w.findChild<KMcpClientInspectorServerNotificationWidget *>(u"mServerNotificationWidget"_s);
    QVERIFY(mServerNotificationWidget);
    QVERIFY(mTabWidget->isTabVisible(mTabWidget->indexOf(mServerNotificationWidget)));

    // Hidden until server is initialized
    auto mCallToolWidget = w.findChild<KMcpClientInspectorCallToolWidget *>(u"mCallToolWidget"_s);
    QVERIFY(mCallToolWidget);
    QVERIFY(!mTabWidget->isTabVisible(mTabWidget->indexOf(mCallToolWidget)));

    auto mListPromptsWidget = w.findChild<KMcpClientInspectorListPromptsWidget *>(u"mListPromptsWidget"_s);
    QVERIFY(mListPromptsWidget);
    QVERIFY(!mTabWidget->isTabVisible(mTabWidget->indexOf(mListPromptsWidget)));

    auto mGetPromptWidget = w.findChild<KMcpClientInspectorGetPromptWidget *>(u"mGetPromptWidget"_s);
    QVERIFY(mGetPromptWidget);
    QVERIFY(!mTabWidget->isTabVisible(mTabWidget->indexOf(mGetPromptWidget)));

    auto mListResourcesWidget = w.findChild<KMcpClientInspectorListResourcesWidget *>(u"mListResourcesWidget"_s);
    QVERIFY(mListResourcesWidget);
    QVERIFY(!mTabWidget->isTabVisible(mTabWidget->indexOf(mListResourcesWidget)));

    auto mResourceTemplatesWidget = w.findChild<KMcpClientInspectorResourceTemplatesWidget *>(u"mResourceTemplatesWidget"_s);
    QVERIFY(mResourceTemplatesWidget);
    QVERIFY(!mTabWidget->isTabVisible(mTabWidget->indexOf(mResourceTemplatesWidget)));
}

#include "moc_kmcpclientinspectoractiontabwidgettest.cpp"
