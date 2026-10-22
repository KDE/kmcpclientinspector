/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorelicitationdialogtest.h"
#include "actions/kmcpclientinspectortoolargumentswidget.h"
#include "dialogs/kmcpclientinspectorelicitationdialog.h"
#include <QDialogButtonBox>
#include <QJsonArray>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolElicitRequestFormParams>
QTEST_MAIN(KMcpClientInspectorElicitationDialogTest)
using namespace Qt::Literals::StringLiterals;
using namespace TextAutoGenerateTextMcpProtocolCore;
namespace
{
McpProtocolElicitRequestFormParams createParams()
{
    const QJsonObject obj{
        {u"mode"_s, u"form"_s},
        {u"message"_s, u"Please provide your name"_s},
        {u"requestedSchema"_s,
         QJsonObject{
             {u"type"_s, u"object"_s},
             {u"properties"_s, QJsonObject{{u"name"_s, QJsonObject{{u"type"_s, u"string"_s}}}}},
             {u"required"_s, QJsonArray{u"name"_s}},
         }},
    };
    return McpProtocolElicitRequestFormParams::fromJson(obj);
}
}

KMcpClientInspectorElicitationDialogTest::KMcpClientInspectorElicitationDialogTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorElicitationDialogTest::shouldHaveDefaultValues()
{
    const KMcpClientInspectorElicitationDialog d(createParams());
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mMessageLabel = d.findChild<QLabel *>(u"mMessageLabel"_s);
    QVERIFY(mMessageLabel);
    QCOMPARE(mMessageLabel->text(), u"Please provide your name"_s);
    QVERIFY(mMessageLabel->wordWrap());

    auto mArgumentsWidget = d.findChild<KMcpClientInspectorToolArgumentsWidget *>(u"mArgumentsWidget"_s);
    QVERIFY(mArgumentsWidget);
    QVERIFY(mArgumentsWidget->findChild<QLineEdit *>(u"name"_s));

    auto buttonBox = d.findChild<QDialogButtonBox *>(u"buttonBox"_s);
    QVERIFY(buttonBox);
    auto declineButton = d.findChild<QPushButton *>(u"declineButton"_s);
    QVERIFY(declineButton);
    QVERIFY(!declineButton->text().isEmpty());

    QCOMPARE(d.elicitResult().action(), McpProtocolElicitResult::Action::Cancel);
    QVERIFY(!d.elicitResult().content().has_value());
}

void KMcpClientInspectorElicitationDialogTest::shouldReturnResult()
{
    {
        KMcpClientInspectorElicitationDialog d(createParams());
        auto lineEdit = d.findChild<QLineEdit *>(u"name"_s);
        QVERIFY(lineEdit);
        lineEdit->setText(u"Konqi"_s);
        auto buttonBox = d.findChild<QDialogButtonBox *>(u"buttonBox"_s);
        buttonBox->button(QDialogButtonBox::Ok)->click();
        QCOMPARE(d.result(), QDialog::Accepted);
        const McpProtocolElicitResult result = d.elicitResult();
        QCOMPARE(result.action(), McpProtocolElicitResult::Action::Accept);
        QVERIFY(result.content().has_value());
        QCOMPARE(std::get<QString>(result.content()->value(u"name"_s)), u"Konqi"_s);
    }
    {
        KMcpClientInspectorElicitationDialog d(createParams());
        d.findChild<QPushButton *>(u"declineButton"_s)->click();
        QCOMPARE(d.result(), QDialog::Rejected);
        QCOMPARE(d.elicitResult().action(), McpProtocolElicitResult::Action::Decline);
        QVERIFY(!d.elicitResult().content().has_value());
    }
    {
        KMcpClientInspectorElicitationDialog d(createParams());
        d.findChild<QDialogButtonBox *>(u"buttonBox"_s)->button(QDialogButtonBox::Cancel)->click();
        QCOMPARE(d.elicitResult().action(), McpProtocolElicitResult::Action::Cancel);
    }
}

#include "moc_kmcpclientinspectorelicitationdialogtest.cpp"
