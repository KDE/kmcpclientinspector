/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorservernotificationwidgettest.h"
#include "actions/kmcpclientinspectorservernotificationwidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <QComboBox>
#include <QPushButton>
#include <QSplitter>
#include <QTest>
#include <QTreeView>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolLoggingMessageNotificationParams>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
QTEST_MAIN(KMcpClientInspectorServerNotificationWidgetTest)
using namespace Qt::Literals::StringLiterals;
using LoggingLevel = TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel;

static void emitLoggingMessage(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager &manager, LoggingLevel level, const QString &message)
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolLoggingMessageNotificationParams params;
    params.setLevel(level);
    params.setData(message);
    Q_EMIT manager.loggingMessageReceived(params);
}

KMcpClientInspectorServerNotificationWidgetTest::KMcpClientInspectorServerNotificationWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorServerNotificationWidgetTest::shouldHaveDefaultValues()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorServerNotificationWidget w(&manager);

    auto mLevelComboBox = w.findChild<QComboBox *>(u"mLevelComboBox"_s);
    QVERIFY(mLevelComboBox);
    QCOMPARE(mLevelComboBox->count(), 8);
    QCOMPARE(mLevelComboBox->currentIndex(), 0);

    auto mLogSearchWidget = w.findChild<QWidget *>(u"mLogSearchWidget"_s);
    QVERIFY(mLogSearchWidget);

    auto mClearButton = w.findChild<QPushButton *>(u"mClearButton"_s);
    QVERIFY(mClearButton);
    QVERIFY(!mClearButton->text().isEmpty());

    auto mSplitter = w.findChild<QSplitter *>(u"mSplitter"_s);
    QVERIFY(mSplitter);
    QCOMPARE(mSplitter->orientation(), Qt::Vertical);

    auto mTreeView = w.findChild<QTreeView *>(u"mTreeView"_s);
    QVERIFY(mTreeView);
    QVERIFY(mTreeView->model());
    QCOMPARE(mTreeView->model()->rowCount(), 0);

    auto mJsonPlainTextEditWidget = w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s);
    QVERIFY(mJsonPlainTextEditWidget);
    QVERIFY(mJsonPlainTextEditWidget->toPlainText().isEmpty());
}

void KMcpClientInspectorServerNotificationWidgetTest::shouldAddLoggingMessage()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorServerNotificationWidget w(&manager);
    auto mTreeView = w.findChild<QTreeView *>(u"mTreeView"_s);

    emitLoggingMessage(manager, LoggingLevel::Info, u"hello"_s);
    QCOMPARE(mTreeView->model()->rowCount(), 1);

    mTreeView->setCurrentIndex(mTreeView->model()->index(0, 0));
    auto mJsonPlainTextEditWidget = w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s);
    QVERIFY(mJsonPlainTextEditWidget->toPlainText().contains(u"hello"_s));
}

void KMcpClientInspectorServerNotificationWidgetTest::shouldFilterByLevel()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorServerNotificationWidget w(&manager);
    auto mTreeView = w.findChild<QTreeView *>(u"mTreeView"_s);
    auto mLevelComboBox = w.findChild<QComboBox *>(u"mLevelComboBox"_s);

    emitLoggingMessage(manager, LoggingLevel::Debug, u"a"_s);
    emitLoggingMessage(manager, LoggingLevel::Error, u"b"_s);
    QCOMPARE(mTreeView->model()->rowCount(), 2);

    mLevelComboBox->setCurrentIndex(mLevelComboBox->findData(static_cast<int>(LoggingLevel::Warning)));
    QCOMPARE(mTreeView->model()->rowCount(), 1);
}

void KMcpClientInspectorServerNotificationWidgetTest::shouldClearMessages()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorServerNotificationWidget w(&manager);
    auto mTreeView = w.findChild<QTreeView *>(u"mTreeView"_s);

    emitLoggingMessage(manager, LoggingLevel::Info, u"a"_s);
    emitLoggingMessage(manager, LoggingLevel::Info, u"b"_s);
    QCOMPARE(mTreeView->model()->rowCount(), 2);

    auto mClearButton = w.findChild<QPushButton *>(u"mClearButton"_s);
    QTest::mouseClick(mClearButton, Qt::LeftButton);
    QCOMPARE(mTreeView->model()->rowCount(), 0);
}

#include "moc_kmcpclientinspectorservernotificationwidgettest.cpp"
