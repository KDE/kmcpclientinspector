# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
include(ECMAddAppIcon)

add_executable(kmcpclientinspector main.cpp)

if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(
        kmcpclientinspector
        PROPERTIES
            UNITY_BUILD
                ON
    )
endif()

target_link_libraries(
    kmcpclientinspector
    # KF6::Crash
    # KF6::XmlGui
    # KF6::IconThemes
    KF6::I18n
    KF6::CoreAddons
    Qt::Widgets
    libkmcpclientinspectorwidgets
)

#file(GLOB_RECURSE KAICHAT_ICONS "${PROJECT_SOURCE_DIR}/src/widgets/icons/*-apps-kaichat.png")
#ecm_add_app_icon(appIcons ICONS "${KAICHAT_ICONS}")
#target_sources(kaichat PRIVATE ${appIcons})

install(
    TARGETS
        kmcpclientinspector
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)
