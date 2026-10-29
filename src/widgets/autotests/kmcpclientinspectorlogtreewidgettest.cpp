/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlogtreewidgettest.h"
#include "actions/kmcpclientinspectorlogsearchwidget.h"
#include "actions/kmcpclientinspectorlogtreewidget.h"
#include "kmcpclientinspectorlogmodel.h"
#include "misc/directioncombobox.h"
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QSignalSpy>
#include <QTest>
#include <QTreeView>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
QTEST_MAIN(KMcpClientInspectorLogTreeWidgetTest)
using namespace Qt::Literals::StringLiterals;

namespace
{
TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageInfo
createMessageInfo(qint64 identifier,
                  TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType type,
                  TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageDirection direction =
                      TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageDirection::Sent)
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageInfo info;
    info.identifier = identifier;
    info.type = type;
    info.direction = direction;
    info.dateTime = QDateTime::currentDateTime();
    info.message = QJsonObject({{u"jsonrpc"_s, u"2.0"_s}, {u"id"_s, identifier}});
    return info;
}
}

KMcpClientInspectorLogTreeWidgetTest::KMcpClientInspectorLogTreeWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorLogTreeWidgetTest::shouldHaveDefaultValues()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    const KMcpClientInspectorLogTreeWidget w(&manager);

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto hboxLayout = w.findChild<QHBoxLayout *>(u"hboxLayout"_s);
    QVERIFY(hboxLayout);
    QCOMPARE(hboxLayout->contentsMargins(), QMargins());

    auto mDirectionComboBox = w.findChild<DirectionComboBox *>(u"mDirectionComboBox"_s);
    QVERIFY(mDirectionComboBox);
    QCOMPARE(mDirectionComboBox->count(), 3);
    QCOMPARE(mDirectionComboBox->type(), KMcpClientInspectorLogModel::DirectionType::All);

    auto mLogSearchWidget = w.findChild<KMcpClientInspectorLogSearchWidget *>(u"mLogSearchWidget"_s);
    QVERIFY(mLogSearchWidget);

    auto mTreeView = w.findChild<QTreeView *>(u"mTreeView"_s);
    QVERIFY(mTreeView);
    QVERIFY(!mTreeView->rootIsDecorated());
    QVERIFY(mTreeView->isSortingEnabled());
    QVERIFY(mTreeView->model());
    QCOMPARE(mTreeView->model()->rowCount(), 0);
    QVERIFY(mTreeView->isColumnHidden(KMcpClientInspectorLogModel::LogColumns::Json));
    QVERIFY(mTreeView->isColumnHidden(KMcpClientInspectorLogModel::LogColumns::DurationValue));
    QVERIFY(mTreeView->isColumnHidden(KMcpClientInspectorLogModel::LogColumns::DirectionValue));
}

void KMcpClientInspectorLogTreeWidgetTest::shouldAddLogMessage()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorLogTreeWidget w(&manager);
    auto mTreeView = w.findChild<QTreeView *>(u"mTreeView"_s);

    Q_EMIT manager.messageLogged(createMessageInfo(1, TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Initialize));
    Q_EMIT manager.messageLogged(createMessageInfo(2, TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Ping));
    QCOMPARE(mTreeView->model()->rowCount(), 2);

    QSignalSpy jsonSelectedSpy(&w, &KMcpClientInspectorLogTreeWidget::jsonSelected);
    mTreeView->setCurrentIndex(mTreeView->model()->index(1, 0));
    QCOMPARE(jsonSelectedSpy.count(), 1);
    const QJsonObject obj = QJsonDocument::fromJson(jsonSelectedSpy.at(0).at(0).toString().toUtf8()).object();
    QCOMPARE(obj[u"id"_s].toInteger(), 2);
}

void KMcpClientInspectorLogTreeWidgetTest::shouldFilterLog()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorLogTreeWidget w(&manager);
    auto mTreeView = w.findChild<QTreeView *>(u"mTreeView"_s);

    Q_EMIT manager.messageLogged(createMessageInfo(1, TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Initialize));
    Q_EMIT manager.messageLogged(createMessageInfo(2, TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Ping));
    Q_EMIT manager.messageLogged(createMessageInfo(3, TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Ping));

    auto mSearchLineEdit = w.findChild<QLineEdit *>(u"mSearchLineEdit"_s);
    mSearchLineEdit->setText(u"Ping"_s);
    QCOMPARE(mTreeView->model()->rowCount(), 2);

    mSearchLineEdit->setText(u"Initialize"_s);
    QCOMPARE(mTreeView->model()->rowCount(), 1);

    mSearchLineEdit->clear();
    QCOMPARE(mTreeView->model()->rowCount(), 3);
}

void KMcpClientInspectorLogTreeWidgetTest::shouldFilterDirection()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager manager(TextAutoGenerateTextMcpProtocolCore::McpServer{});
    KMcpClientInspectorLogTreeWidget w(&manager);
    auto mTreeView = w.findChild<QTreeView *>(u"mTreeView"_s);
    auto mDirectionComboBox = w.findChild<DirectionComboBox *>(u"mDirectionComboBox"_s);

    Q_EMIT manager.messageLogged(createMessageInfo(1,
                                                   TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Ping,
                                                   TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageDirection::Sent));
    Q_EMIT manager.messageLogged(createMessageInfo(1,
                                                   TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Ping,
                                                   TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageDirection::Received));
    Q_EMIT manager.messageLogged(createMessageInfo(2,
                                                   TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Initialize,
                                                   TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageDirection::Sent));
    QCOMPARE(mTreeView->model()->rowCount(), 3);

    mDirectionComboBox->setCurrentIndex(mDirectionComboBox->findData(QVariant::fromValue(KMcpClientInspectorLogModel::DirectionType::Sent)));
    QCOMPARE(mTreeView->model()->rowCount(), 2);

    mDirectionComboBox->setCurrentIndex(mDirectionComboBox->findData(QVariant::fromValue(KMcpClientInspectorLogModel::DirectionType::Received)));
    QCOMPARE(mTreeView->model()->rowCount(), 1);

    // Direction and search text filters are combined
    mDirectionComboBox->setCurrentIndex(mDirectionComboBox->findData(QVariant::fromValue(KMcpClientInspectorLogModel::DirectionType::Sent)));
    auto mSearchLineEdit = w.findChild<QLineEdit *>(u"mSearchLineEdit"_s);
    mSearchLineEdit->setText(u"Initialize"_s);
    QCOMPARE(mTreeView->model()->rowCount(), 1);

    mSearchLineEdit->clear();
    mDirectionComboBox->setCurrentIndex(mDirectionComboBox->findData(QVariant::fromValue(KMcpClientInspectorLogModel::DirectionType::All)));
    QCOMPARE(mTreeView->model()->rowCount(), 3);
}

#include "moc_kmcpclientinspectorlogtreewidgettest.cpp"
