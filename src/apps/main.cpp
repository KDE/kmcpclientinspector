/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectormainwindow.h"
#include <KLocalizedString>
#include <QApplication>
using namespace Qt::Literals::StringLiterals;
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("kmcpclientinspector"_ba);

    auto mw = new KMcpClientInspectorMainWindow;
    mw->show();
    const int val = app.exec();
    return val;
}
