/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorrootsdialogtest.h"
#include "dialogs/kmcpclientinspectorrootsdialog.h"
#include <QDialogButtonBox>
#include <QTest>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolWidgets/AddMcpServerRootsWidget>
QTEST_MAIN(KMcpClientInspectorRootsDialogTest)
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorRootsDialogTest::KMcpClientInspectorRootsDialogTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorRootsDialogTest::shouldHaveDefaultValues()
{
    const KMcpClientInspectorRootsDialog d;
    QVERIFY(!d.windowTitle().isEmpty());
    QVERIFY(d.roots().isEmpty());

    auto mainLayout = d.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);

    auto mRootsWidget = d.findChild<TextAutoGenerateTextMcpProtocolWidgets::AddMcpServerRootsWidget *>(u"mRootsWidget"_s);
    QVERIFY(mRootsWidget);

    auto buttonBox = d.findChild<QDialogButtonBox *>(u"buttonBox"_s);
    QVERIFY(buttonBox);
}

void KMcpClientInspectorRootsDialogTest::shouldSetRoots()
{
    KMcpClientInspectorRootsDialog d;
    const QStringList roots{u"/tmp"_s, u"/home"_s};
    d.setRoots(roots);
    QCOMPARE(d.roots(), roots);
}

#include "moc_kmcpclientinspectorrootsdialogtest.cpp"
