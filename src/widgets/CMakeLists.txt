# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

#add_subdirectory(icons)

add_library(libkmcpclientinspectorwidgets)
ecm_qt_declare_logging_category(libkmcpclientinspectorwidgets HEADER kmcpclientinspector_widget_debug.h IDENTIFIER KMCPCLIENTINSPECTOR_WIDGET_LOG
    CATEGORY_NAME org.kde.kmcpclientinspector_widget DESCRIPTION "kmcpclientinspector (widget lib)" EXPORT KMCPCLIENTINSPECTOR
)

target_sources(
    libkmcpclientinspectorwidgets
    PRIVATE
        kmcpclientinspectormainwindow.h
        kmcpclientinspectormainwindow.cpp
        kmcpclientinspectorwidgets.qrc
        kmcpclientinspectorcentralwidget.h
        kmcpclientinspectorcentralwidget.cpp
        kmcpclientinspectortabwidget.h
        kmcpclientinspectortabwidget.cpp
        kmcpclientinspectortabpage.h
        kmcpclientinspectortabpage.cpp
        libkmcpclientinspectorwidgets_private_export.h
        kmcpclientinspectorserversettingswidget.h
        kmcpclientinspectorserversettingswidget.cpp
        configure/kmcpclientinspectorconfiguresettingsdialog.cpp
        configure/kmcpclientinspectorconfiguresettingsdialog.h
        configure/kmcpclientinspectorconfiguremcpserverswidget.cpp
        configure/kmcpclientinspectorconfiguremcpserverswidget.h
        actions/kmcpclientinspectorpingwidget.h
        actions/kmcpclientinspectorpingwidget.cpp
)
if(COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(
        libkmcpclientinspectorwidgets
        PROPERTIES
            UNITY_BUILD
                ON
    )
endif()
generate_export_header(libkmcpclientinspectorwidgets BASE_NAME libkmcpclientinspectorwidgets)

target_link_libraries(
    libkmcpclientinspectorwidgets
    Qt::Widgets
    KF6::XmlGui
    KF6::ConfigGui
    KF6::ConfigWidgets
    KF6::NotifyConfig
    KF6::TextAutoGenerateTextMcpProtocolWidgets
    libkmcpclientinspectorcore
)

set_target_properties(
    libkmcpclientinspectorwidgets
    PROPERTIES
        OUTPUT_NAME
            kmcpclientinspectorwidgets
        VERSION
            ${KMCPCLIENTINSPECTOR_LIB_VERSION}
        SOVERSION
            ${KMCPCLIENTINSPECTOR_LIB_SOVERSION}
)

install(
    TARGETS
        libkmcpclientinspectorwidgets
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
    LIBRARY
        NAMELINK_SKIP
)

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()
