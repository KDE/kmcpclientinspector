/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QStringList>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolSettings>

namespace KMcpClientInspectorRootsUtils
{
/*!
 * Roots really sent to the server: its own roots when defined, otherwise
 * \a globalRoots unless the server explicitly opted out of them.
 */
[[nodiscard]] LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT QStringList effectiveRoots(const TextAutoGenerateTextMcpProtocolCore::McpProtocolSettings &settings,
                                                                                    const QStringList &globalRoots);
}
