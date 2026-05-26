# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
include(ECMAddAppIcon)

add_executable(
    kmcpclientinspector
    main.cpp
    kmcpclientinspector.qrc
)

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
    KF6::Crash
    KF6::IconThemes
    KF6::I18n
    KF6::CoreAddons
    Qt::Widgets
    libkmcpclientinspectorwidgets
)

file(GLOB_RECURSE KMCPCLIENTINSPECTOR_ICONS "${PROJECT_SOURCE_DIR}/src/widgets/icons/*-apps-kmcpclientinspector.png")
ecm_add_app_icon(appIcons ICONS "${KMCPCLIENTINSPECTOR_ICONS}")
target_sources(kmcpclientinspector PRIVATE ${appIcons})

install(
    TARGETS
        kmcpclientinspector
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()
