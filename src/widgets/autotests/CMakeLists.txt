# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

macro(add_kmcpclientinspector_widgets_unittest _source)
    get_filename_component(_name ${_source} NAME_WE)
    ecm_add_test(${_source} ${_name}.h TEST_NAME ${_name}
        NAME_PREFIX "kmcpclientinspector-widgets-"
        LINK_LIBRARIES Qt::Test libkmcpclientinspectorwidgets
    )
endmacro()

add_kmcpclientinspector_widgets_unittest(kmcpclientinspectorserversettingswidgettest.cpp)
add_kmcpclientinspector_widgets_unittest(kmcpclientinspectorconnectbuttonswidgettest.cpp)
