/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "config-kmcpclientinspector.h"
#include "kmcpclientinspectorcommandlineparser.h"
#include "kmcpclientinspectormainwindow.h"
#include <KAboutData>
#include <KCrash>
#include <KLocalizedString>
#include <QApplication>
#include <QCommandLineParser>
#include <QTimer>

using namespace Qt::Literals::StringLiterals;
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("kmcpclientinspector"_ba);

    KAboutData aboutData(u"kmcpclientinspector"_s,
                         i18n("KMcpClientInspector"),
                         QStringLiteral(KMCPCLIENTINSPECTOR_VERSION),
                         i18n("MCP client inspector"),
                         KAboutLicense::GPL_V2,
                         i18n("Copyright © 2026 Laurent Montel"));
    aboutData.addAuthor(i18nc("@info:credit", "Laurent Montel"), i18n("Maintainer"), u"montel@kde.org"_s);
    aboutData.setOrganizationDomain("kde.org"_ba);
    aboutData.setProductName("kmcpclientinspector"_ba);
    aboutData.setProgramLogo(QIcon(u":/kmcpclientinspector/kmcpclientinspector.svg"_s));

    KAboutData::setApplicationData(aboutData);
    KCrash::initialize();
    QCommandLineParser parser;

    KMcpClientInspectorCommandLineParser commandLineParser(&parser);

    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);
    auto mw = new KMcpClientInspectorMainWindow;
    mw->show();
    if (parser.isSet(KMcpClientInspectorCommandLineParser::optionParserFromEnum(KMcpClientInspectorCommandLineParser::OptionParser::SelfTest))) {
        QTimer::singleShot(std::chrono::milliseconds(250), &app, &QCoreApplication::quit);
    }
    const int val = app.exec();
    // TODO delete mw;
    return val;
}
