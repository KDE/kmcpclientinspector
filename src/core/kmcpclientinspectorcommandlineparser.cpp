/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorcommandlineparser.h"
#include <KLocalizedString>
#include <QCommandLineParser>

using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorCommandLineParser::KMcpClientInspectorCommandLineParser(QCommandLineParser *parser)
{
    initializeCommandLine(parser);
}

KMcpClientInspectorCommandLineParser::~KMcpClientInspectorCommandLineParser() = default;

void KMcpClientInspectorCommandLineParser::initializeCommandLine(QCommandLineParser *parser)
{
    QCommandLineOption selfTestOption(optionParserFromEnum(OptionParser::SelfTest), QStringLiteral("internal, for automated testing"));
    selfTestOption.setFlags(QCommandLineOption::HiddenFromHelp);
    parser->addOption(selfTestOption);
}

QString KMcpClientInspectorCommandLineParser::optionParserFromEnum(OptionParser e)
{
    switch (e) {
    case OptionParser::SelfTest:
        return u"self-test"_s;
    }
    return {};
}
