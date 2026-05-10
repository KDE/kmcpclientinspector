# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause
set(icons
    16-apps-kmcpclientinspector.png
    22-apps-kmcpclientinspector.png
    24-apps-kmcpclientinspector.png
    32-apps-kmcpclientinspector.png
    48-apps-kmcpclientinspector.png
    sc-apps-kmcpclientinspector.svg
)

set(all_icons ${icons})

ecm_install_icons(ICONS ${all_icons}
    DESTINATION ${KDE_INSTALL_ICONDIR}
    THEME hicolor
)
