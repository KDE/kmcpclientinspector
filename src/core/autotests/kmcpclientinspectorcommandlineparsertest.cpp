/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorcommandlineparsertest.h"
#include "kmcpclientinspectorcommandlineparser.h"
#include <QTest>
QTEST_GUILESS_MAIN(KMcpClientInspectorCommandLineParserTest)
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorCommandLineParserTest::KMcpClientInspectorCommandLineParserTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorCommandLineParserTest::shouldVerifyOptionParserFromEnumValues()
{
    QCOMPARE(KMcpClientInspectorCommandLineParser::optionParserFromEnum(KMcpClientInspectorCommandLineParser::OptionParser::SelfTest), u"self-test"_s);
}

#include "moc_kmcpclientinspectorcommandlineparsertest.cpp"
