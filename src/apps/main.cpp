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
#include <QPointer>
#include <QTimer>

using namespace Qt::Literals::StringLiterals;
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(u":/kmcpclientinspector/kmcpclientinspector.svg"_s));
    app.setDesktopFileName(u"org.kde.kmcpclientinspector"_s);

    KLocalizedString::setApplicationDomain("kmcpclientinspector"_ba);

    KAboutData aboutData = KAboutData::fromAppStreamForApplication();
    aboutData.setCopyrightStatement(i18n("Copyright © 2026 Laurent Montel"));
    aboutData.setVersion(KMCPCLIENTINSPECTOR_VERSION);
    aboutData.setComponentName(u"kmcpclientinspector"_s);

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
    QPointer<KMcpClientInspectorMainWindow> mw{new KMcpClientInspectorMainWindow(aboutData.releases())};
    mw->show();
    if (parser.isSet(KMcpClientInspectorCommandLineParser::optionParserFromEnum(KMcpClientInspectorCommandLineParser::OptionParser::SelfTest))) {
        QTimer::singleShot(std::chrono::milliseconds(250), &app, [mw, &app]() {
            delete mw;
            app.quit();
        });
    }
    const int val = app.exec();
    delete mw;
    return val;
}
