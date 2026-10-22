/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorsamplingdialogtest.h"
#include "dialogs/kmcpclientinspectorsamplingdialog.h"
#include "misc/jsonplaintexteditwidget.h"
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolCreateMessageRequestParams>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolCreateMessageResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTextContent>
QTEST_MAIN(KMcpClientInspectorSamplingDialogTest)
using namespace Qt::Literals::StringLiterals;
using namespace TextAutoGenerateTextMcpProtocolCore;
KMcpClientInspectorSamplingDialogTest::KMcpClientInspectorSamplingDialogTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorSamplingDialogTest::shouldHaveDefaultValues()
{
    const KMcpClientInspectorSamplingDialog d(McpProtocolCreateMessageRequestParams{});
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mRequestWidget = d.findChild<JsonPlainTextEditWidget *>(u"mRequestWidget"_s);
    QVERIFY(mRequestWidget);
    QVERIFY(!mRequestWidget->toPlainText().isEmpty());

    auto mModelLineEdit = d.findChild<QLineEdit *>(u"mModelLineEdit"_s);
    QVERIFY(mModelLineEdit);
    QCOMPARE(mModelLineEdit->text(), u"kmcpclientinspector"_s);

    auto mStopReasonLineEdit = d.findChild<QLineEdit *>(u"mStopReasonLineEdit"_s);
    QVERIFY(mStopReasonLineEdit);
    QCOMPARE(mStopReasonLineEdit->text(), u"endTurn"_s);

    auto mResponseTextEdit = d.findChild<QPlainTextEdit *>(u"mResponseTextEdit"_s);
    QVERIFY(mResponseTextEdit);
    QVERIFY(mResponseTextEdit->toPlainText().isEmpty());
    QVERIFY(!mResponseTextEdit->placeholderText().isEmpty());

    auto buttonBox = d.findChild<QDialogButtonBox *>(u"buttonBox"_s);
    QVERIFY(buttonBox);
    QVERIFY(!buttonBox->button(QDialogButtonBox::Ok)->isEnabled());
}

void KMcpClientInspectorSamplingDialogTest::shouldReturnResult()
{
    KMcpClientInspectorSamplingDialog d(McpProtocolCreateMessageRequestParams{});
    auto okButton = d.findChild<QDialogButtonBox *>(u"buttonBox"_s)->button(QDialogButtonBox::Ok);
    auto mResponseTextEdit = d.findChild<QPlainTextEdit *>(u"mResponseTextEdit"_s);
    mResponseTextEdit->setPlainText(u"   "_s);
    QVERIFY(!okButton->isEnabled());
    mResponseTextEdit->setPlainText(u"Hello"_s);
    QVERIFY(okButton->isEnabled());
    d.findChild<QLineEdit *>(u"mStopReasonLineEdit"_s)->clear();

    const McpProtocolCreateMessageResult result = d.createMessageResult();
    QCOMPARE(result.role(), McpProtocolUtils::Role::Assistant);
    QCOMPARE(result.model(), u"kmcpclientinspector"_s);
    QVERIFY(!result.stopReason().has_value());
    QCOMPARE(std::get<McpProtocolTextContent>(result.content()).text(), u"Hello"_s);
}

#include "moc_kmcpclientinspectorsamplingdialogtest.cpp"
