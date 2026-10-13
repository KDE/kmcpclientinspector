/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorpingwidget.h"
#include <KLocalizedString>
KMcpClientInspectorPingWidget::KMcpClientInspectorPingWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                             QWidget *parent)
    : KMcpClientInspectorResultWidget{protocolManager,
                                      i18nc("@action:button", "Ping"),
                                      TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Ping,
                                      parent}
{
}

KMcpClientInspectorPingWidget::~KMcpClientInspectorPingWidget() = default;

#include "moc_kmcpclientinspectorpingwidget.cpp"
