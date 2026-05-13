# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(core)
add_subdirectory(widgets)
add_subdirectory(apps)

if(APPLE)
    set_target_properties(
        kmcpclientinspector
        PROPERTIES
            MACOSX_BUNDLE_DISPLAY_NAME
                "KMcpClientInspector"
            MACOSX_BUNDLE_BUNDLE_NAME
                "KMcpClientInspector"
            MACOSX_BUNDLE_LONG_VERSION_STRING
                "KMcpClientInspector ${KMCPCLIENTINSPECTOR_VERSION}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING
                "${KMCPCLIENTINSPECTOR_VERSION}"
            MACOSX_BUNDLE_BUNDLE_VERSION
                "${KMCPCLIENTINSPECTOR_VERSION}"
            MACOSX_BUNDLE_GUI_IDENTIFIER
                "org.kde.kmcpclientinspector"
            MACOSX_BUNDLE_COPYRIGHT
                "2026 The KMcpClientInspector Developers"
    )
endif()

install(PROGRAMS data/org.kde.kmcpclientinspector.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES data/org.kde.kmcpclientinspector.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
