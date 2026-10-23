/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlogsearchwidgettest.h"
#include "actions/kmcpclientinspectorlogsearchwidget.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(KMcpClientInspectorLogSearchWidgetTest)
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorLogSearchWidgetTest::KMcpClientInspectorLogSearchWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorLogSearchWidgetTest::shouldHaveDefaultValues()
{
    const KMcpClientInspectorLogSearchWidget w;

    auto mainLayout = w.findChild<QHBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto mSearchLineEdit = w.findChild<QLineEdit *>(u"mSearchLineEdit"_s);
    QVERIFY(mSearchLineEdit);
    QVERIFY(mSearchLineEdit->text().isEmpty());
    QVERIFY(!mSearchLineEdit->placeholderText().isEmpty());
    QVERIFY(mSearchLineEdit->isClearButtonEnabled());

    auto mClearButton = w.findChild<QPushButton *>(u"mClearButton"_s);
    QVERIFY(mClearButton);
    QVERIFY(!mClearButton->text().isEmpty());
}

void KMcpClientInspectorLogSearchWidgetTest::shouldEmitSearchTextChanged()
{
    KMcpClientInspectorLogSearchWidget w;
    QSignalSpy searchTextChangedSpy(&w, &KMcpClientInspectorLogSearchWidget::searchTextChanged);
    auto mSearchLineEdit = w.findChild<QLineEdit *>(u"mSearchLineEdit"_s);
    mSearchLineEdit->setText(u"foo"_s);
    QCOMPARE(searchTextChangedSpy.count(), 1);
    QCOMPARE(searchTextChangedSpy.at(0).at(0).toString(), u"foo"_s);

    mSearchLineEdit->clear();
    QCOMPARE(searchTextChangedSpy.count(), 2);
    QVERIFY(searchTextChangedSpy.at(1).at(0).toString().isEmpty());
}

#include "moc_kmcpclientinspectorlogsearchwidgettest.cpp"
