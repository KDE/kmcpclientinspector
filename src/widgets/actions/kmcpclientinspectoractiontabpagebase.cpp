/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectoractiontabpagebase.h"

KMcpClientInspectorActionTabPageBase::KMcpClientInspectorActionTabPageBase(KMcpClientInspectorClientProtocolManager *protocolManager, QWidget *parent)
    : QWidget{parent}
    , mProtocolManager(protocolManager)
{
}

KMcpClientInspectorActionTabPageBase::~KMcpClientInspectorActionTabPageBase() = default;

#include "moc_kmcpclientinspectoractiontabpagebase.cpp"
