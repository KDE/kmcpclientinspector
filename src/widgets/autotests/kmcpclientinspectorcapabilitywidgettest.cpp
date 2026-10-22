/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorcapabilitywidgettest.h"
#include "actions/kmcpclientinspectorcapabilitywidget.h"
#include <QCheckBox>
#include <QGroupBox>
#include <QSignalSpy>
#include <QTest>
#include <QToolButton>
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
    QVERIFY(w.urlElicitationSupport());
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

    auto mUrlElicitationSupported = w.findChild<QCheckBox *>(u"mUrlElicitationSupported"_s);
    QVERIFY(mUrlElicitationSupported);
    QVERIFY(mUrlElicitationSupported->isChecked());
    QVERIFY(!mUrlElicitationSupported->text().isEmpty());

    auto mRootsSupported = w.findChild<QCheckBox *>(u"mRootsSupported"_s);
    QVERIFY(mRootsSupported);
    QVERIFY(mRootsSupported->isChecked());
    QVERIFY(!mRootsSupported->text().isEmpty());

    auto mConfigureRootsButton = w.findChild<QToolButton *>(u"mConfigureRootsButton"_s);
    QVERIFY(mConfigureRootsButton);
    QVERIFY(mConfigureRootsButton->isEnabled());
    QVERIFY(!mConfigureRootsButton->toolTip().isEmpty());

    auto mSamplingSupported = w.findChild<QCheckBox *>(u"mSamplingSupported"_s);
    QVERIFY(mSamplingSupported);
    QVERIFY(mSamplingSupported->isChecked());
    QVERIFY(!mSamplingSupported->text().isEmpty());
}

void KMcpClientInspectorCapabilityWidgetTest::shouldEnableConfigureRootsButton()
{
    KMcpClientInspectorCapabilityWidget w;
    auto mRootsSupported = w.findChild<QCheckBox *>(u"mRootsSupported"_s);
    auto mConfigureRootsButton = w.findChild<QToolButton *>(u"mConfigureRootsButton"_s);
    mRootsSupported->setChecked(false);
    QVERIFY(!mConfigureRootsButton->isEnabled());
    mRootsSupported->setChecked(true);
    QVERIFY(mConfigureRootsButton->isEnabled());

    QSignalSpy spy(&w, &KMcpClientInspectorCapabilityWidget::configureRootsRequested);
    mConfigureRootsButton->click();
    QCOMPARE(spy.count(), 1);
}

void KMcpClientInspectorCapabilityWidgetTest::shouldDisableCapabilitiesWhenConnected()
{
    KMcpClientInspectorCapabilityWidget w;
    w.setConnected(true);
    for (const auto &name : {u"mElicitationSupported"_s, u"mUrlElicitationSupported"_s, u"mRootsSupported"_s, u"mSamplingSupported"_s}) {
        QVERIFY(!w.findChild<QCheckBox *>(name)->isEnabled());
    }
    // Roots can be modified when client is connected
    QVERIFY(w.findChild<QToolButton *>(u"mConfigureRootsButton"_s)->isEnabled());

    w.setConnected(false);
    QVERIFY(w.findChild<QCheckBox *>(u"mRootsSupported"_s)->isEnabled());
}

#include "moc_kmcpclientinspectorcapabilitywidgettest.cpp"
