# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

#add_subdirectory(icons)

add_library(libkmcpclientinspectorwidgets)
ecm_qt_declare_logging_category(libkmcpclientinspectorwidgets HEADER kaichat_widget_debug.h IDENTIFIER KAICHAT_WIDGET_LOG
    CATEGORY_NAME org.kde.kaichat_widget DESCRIPTION "kaichat (widget lib)" EXPORT KAICHAT
)

target_sources(libkmcpclientinspectorwidgets PRIVATE)
if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(
        libkmcpclientinspectorwidgets
        PROPERTIES
            UNITY_BUILD
                ON
    )
endif()
generate_export_header(libkmcpclientinspectorwidgets BASE_NAME libkmcpclientinspectorwidgets)

target_link_libraries(libkmcpclientinspectorwidgets Qt::Widgets)

set_target_properties(
    libkmcpclientinspectorwidgets
    PROPERTIES
        OUTPUT_NAME
            kmcpclientinspectorwidgets
        VERSION
            ${KAICHAT_LIB_VERSION}
        SOVERSION
            ${KAICHAT_LIB_SOVERSION}
)

install(
    TARGETS
        libkmcpclientinspectorwidgets
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
    LIBRARY
        NAMELINK_SKIP
)
