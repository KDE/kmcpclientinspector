/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorcapabilitywidgettest.h"
#include "actions/kmcpclientinspectorcapabilitywidget.h"
#include <QCheckBox>
#include <QGroupBox>
#include <QTest>
#include <QVBoxLayout>
QTEST_MAIN(KMcpClientInspectorCapabilityWidgetTest)
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorCapabilityWidgetTest::KMcpClientInspectorCapabilityWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorCapabilityWidgetTest::shouldHaveDefaultValues()
{
    const KMcpClientInspectorCapabilityWidget w;
    QVERIFY(w.elicitationSupport());
    QVERIFY(w.rootsSupport());
    QVERIFY(w.samplingSupport());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto groupBox = w.findChild<QGroupBox *>(u"groupBox"_s);
    QVERIFY(groupBox);
    QVERIFY(!groupBox->title().isEmpty());

    auto mElicitationSupported = w.findChild<QCheckBox *>(u"mElicitationSupported"_s);
    QVERIFY(mElicitationSupported);
    QVERIFY(mElicitationSupported->isChecked());
    QVERIFY(!mElicitationSupported->text().isEmpty());

    auto mRootsSupported = w.findChild<QCheckBox *>(u"mRootsSupported"_s);
    QVERIFY(mRootsSupported);
    QVERIFY(mRootsSupported->isChecked());
    QVERIFY(!mRootsSupported->text().isEmpty());

    auto mSamplingSupported = w.findChild<QCheckBox *>(u"mSamplingSupported"_s);
    QVERIFY(mSamplingSupported);
    QVERIFY(mSamplingSupported->isChecked());
    QVERIFY(!mSamplingSupported->text().isEmpty());
}

#include "moc_kmcpclientinspectorcapabilitywidgettest.cpp"
