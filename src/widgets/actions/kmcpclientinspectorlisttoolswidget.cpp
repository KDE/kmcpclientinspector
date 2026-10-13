/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlisttoolswidget.h"
#include <KLocalizedString>
KMcpClientInspectorListToolsWidget::KMcpClientInspectorListToolsWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                                       QWidget *parent)
    : KMcpClientInspectorResultWidget{protocolManager,
                                      i18nc("@action:button", "List Tools"),
                                      TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListTools,
                                      parent}
{
}

KMcpClientInspectorListToolsWidget::~KMcpClientInspectorListToolsWidget() = default;

#include "moc_kmcpclientinspectorlisttoolswidget.cpp"
