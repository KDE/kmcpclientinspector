# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_library(libkmcpclientinspectorcore)
ecm_qt_declare_logging_category(libkmcpclientinspectorcore HEADER kmcpclientinspector_core_debug.h IDENTIFIER KMCPCLIENTINSPECTOR_CORE_LOG
    CATEGORY_NAME org.kde.kmcpclientinspector_core DESCRIPTION "kmcpclientinspector (core lib)" EXPORT KMCPCLIENTINSPECTOR
)

target_sources(
    libkmcpclientinspectorcore
    PRIVATE
        kmcpclientinspectormanager.h
        kmcpclientinspectormanager.cpp
        kmcpclientinspectorcommandlineparser.cpp
        kmcpclientinspectorcommandlineparser.h
)

if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(
        libkmcpclientinspectorcore
        PROPERTIES
            UNITY_BUILD
                ON
    )
endif()
generate_export_header(libkmcpclientinspectorcore BASE_NAME libkmcpclientinspectorcore)

target_link_libraries(libkmcpclientinspectorcore KF6::TextAutoGenerateTextMcpProtocolCore)

set_target_properties(
    libkmcpclientinspectorcore
    PROPERTIES
        OUTPUT_NAME
            kmcpclientinspectorcore
        VERSION
            ${KMCPCLIENTINSPECTOR_LIB_VERSION}
        SOVERSION
            ${KMCPCLIENTINSPECTOR_LIB_SOVERSION}
)

install(
    TARGETS
        libkmcpclientinspectorcore
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
    LIBRARY
        NAMELINK_SKIP
)

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()
