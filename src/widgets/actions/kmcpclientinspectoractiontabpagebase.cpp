/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectoractiontabpagebase.h"

KMcpClientInspectorActionTabPageBase::KMcpClientInspectorActionTabPageBase(QWidget *parent)
    : QWidget{parent}
{
}

KMcpClientInspectorActionTabPageBase::~KMcpClientInspectorActionTabPageBase() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolClient *KMcpClientInspectorActionTabPageBase::client() const
{
    return mClient;
}

void KMcpClientInspectorActionTabPageBase::setClient(TextAutoGenerateTextMcpProtocolCore::McpProtocolClient *newClient)
{
    mClient = newClient;
}

#include "moc_kmcpclientinspectoractiontabpagebase.cpp"
