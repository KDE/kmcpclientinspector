# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

add_test(
    NAME kmcpclientinspector-self-test
    COMMAND
        kmcpclientinspector --self-test
)
