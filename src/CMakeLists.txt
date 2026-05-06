# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(core)
add_subdirectory(widgets)
add_subdirectory(apps)

install(PROGRAMS data/org.kde.kmcpclientinspector.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES data/org.kde.kmcpclientinspector.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
