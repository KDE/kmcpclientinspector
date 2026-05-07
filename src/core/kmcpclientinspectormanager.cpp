/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectormanager.h"
#include <TextAutoGenerateTextMcpProtocolCore/McpServerModel>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorManager::KMcpClientInspectorManager(QObject *parent)
    : TextAutoGenerateTextMcpProtocolCore::McpServerManager{parent}
{
}

KMcpClientInspectorManager::~KMcpClientInspectorManager() = default;

QString KMcpClientInspectorManager::serverConfigFileName() const
{
    return u"kmcpclientinspectorrc"_s;
}
#include "moc_kmcpclientinspectormanager.cpp"
