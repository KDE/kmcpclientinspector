/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorrootsutils.h"

QStringList KMcpClientInspectorRootsUtils::effectiveRoots(const TextAutoGenerateTextMcpProtocolCore::McpProtocolSettings &settings,
                                                          const QStringList &globalRoots)
{
    if (const QStringList roots = settings.roots(); !roots.isEmpty() || !settings.useGlobalRoots()) {
        return roots;
    }
    return globalRoots;
}
