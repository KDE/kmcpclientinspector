#! /usr/bin/env bash
# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org> 

$EXTRACTRC `find . -name "*.rc"` >> rc.cpp
$XGETTEXT `find . -name "*.cpp" -o -name "*.h"` -o $podir/kmcpclientinspector.pot
rm -f rc.cpp
