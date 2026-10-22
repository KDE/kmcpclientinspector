/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorurlelicitationdialogtest.h"
#include "dialogs/kmcpclientinspectorurlelicitationdialog.h"
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QTest>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolElicitRequestURLParams>
QTEST_MAIN(KMcpClientInspectorUrlElicitationDialogTest)
using namespace Qt::Literals::StringLiterals;
using namespace TextAutoGenerateTextMcpProtocolCore;
namespace
{
McpProtocolElicitRequestURLParams createParams(const QString &url)
{
    McpProtocolElicitRequestURLParams params;
    params.setElicitationId(u"id-1"_s);
    params.setMessage(u"Please authorize access"_s);
    params.setUrl(url);
    return params;
}
}

KMcpClientInspectorUrlElicitationDialogTest::KMcpClientInspectorUrlElicitationDialogTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorUrlElicitationDialogTest::shouldHaveDefaultValues()
{
    const KMcpClientInspectorUrlElicitationDialog d(createParams(u"https://example.com/auth?state=1"_s));
    QVERIFY(!d.windowTitle().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mMessageLabel = d.findChild<QLabel *>(u"mMessageLabel"_s);
    QVERIFY(mMessageLabel);
    QCOMPARE(mMessageLabel->text(), u"Please authorize access"_s);

    auto mDomainLabel = d.findChild<QLabel *>(u"mDomainLabel"_s);
    QVERIFY(mDomainLabel);
    QCOMPARE(mDomainLabel->text(), u"example.com"_s);
    QVERIFY(mDomainLabel->font().bold());

    auto mUrlLabel = d.findChild<QLabel *>(u"mUrlLabel"_s);
    QVERIFY(mUrlLabel);
    QCOMPARE(mUrlLabel->text(), u"https://example.com/auth?state=1"_s);
    QCOMPARE(mUrlLabel->textFormat(), Qt::PlainText);

    auto infoLabel = d.findChild<QLabel *>(u"infoLabel"_s);
    QVERIFY(infoLabel);
    QVERIFY(!infoLabel->text().isEmpty());

    auto buttonBox = d.findChild<QDialogButtonBox *>(u"buttonBox"_s);
    QVERIFY(buttonBox);
    auto openButton = buttonBox->button(QDialogButtonBox::Ok);
    QVERIFY(openButton->isEnabled());
    QVERIFY(!openButton->isDefault());
    QVERIFY(d.findChild<QPushButton *>(u"declineButton"_s));

    QCOMPARE(d.url(), QUrl(u"https://example.com/auth?state=1"_s));
    QCOMPARE(d.elicitResult().action(), McpProtocolElicitResult::Action::Cancel);
}

void KMcpClientInspectorUrlElicitationDialogTest::shouldReturnResult()
{
    {
        KMcpClientInspectorUrlElicitationDialog d(createParams(u"https://example.com"_s));
        d.findChild<QDialogButtonBox *>(u"buttonBox"_s)->button(QDialogButtonBox::Ok)->click();
        QCOMPARE(d.result(), QDialog::Accepted);
        QCOMPARE(d.elicitResult().action(), McpProtocolElicitResult::Action::Accept);
        QVERIFY(!d.elicitResult().content().has_value());
    }
    {
        KMcpClientInspectorUrlElicitationDialog d(createParams(u"https://example.com"_s));
        d.findChild<QPushButton *>(u"declineButton"_s)->click();
        QCOMPARE(d.result(), QDialog::Rejected);
        QCOMPARE(d.elicitResult().action(), McpProtocolElicitResult::Action::Decline);
    }
    {
        KMcpClientInspectorUrlElicitationDialog d(createParams(u"https://example.com"_s));
        d.findChild<QDialogButtonBox *>(u"buttonBox"_s)->button(QDialogButtonBox::Cancel)->click();
        QCOMPARE(d.result(), QDialog::Rejected);
        QCOMPARE(d.elicitResult().action(), McpProtocolElicitResult::Action::Cancel);
    }
}

void KMcpClientInspectorUrlElicitationDialogTest::shouldNotOpenInvalidUrl()
{
    const KMcpClientInspectorUrlElicitationDialog d(createParams(u"file:///etc/passwd"_s));
    QVERIFY(!d.findChild<QDialogButtonBox *>(u"buttonBox"_s)->button(QDialogButtonBox::Ok)->isEnabled());
}

void KMcpClientInspectorUrlElicitationDialogTest::shouldCheckUrl_data()
{
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<bool>("canOpen");
    QTest::newRow("https") << QUrl(u"https://example.com/path"_s) << true;
    QTest::newRow("http") << QUrl(u"http://localhost:8080"_s) << true;
    QTest::newRow("empty") << QUrl() << false;
    QTest::newRow("file") << QUrl(u"file:///tmp/foo"_s) << false;
    QTest::newRow("javascript") << QUrl(u"javascript:alert(1)"_s) << false;
    QTest::newRow("no-host") << QUrl(u"https:///path"_s) << false;
}

void KMcpClientInspectorUrlElicitationDialogTest::shouldCheckUrl()
{
    QFETCH(QUrl, url);
    QFETCH(bool, canOpen);
    QCOMPARE(KMcpClientInspectorUrlElicitationDialog::canOpenUrl(url), canOpen);
}

#include "moc_kmcpclientinspectorurlelicitationdialogtest.cpp"
