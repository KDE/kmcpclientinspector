/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorclientprotocolmanager.h"

KMcpClientInspectorClientProtocolManager::KMcpClientInspectorClientProtocolManager(QObject *parent)
    : QObject{parent}
{
}

KMcpClientInspectorClientProtocolManager::~KMcpClientInspectorClientProtocolManager() = default;
#include "moc_kmcpclientinspectorclientprotocolmanager.cpp"
