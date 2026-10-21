/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorservernotificationwidget.h"

KMcpClientInspectorServerNotificationWidget::KMcpClientInspectorServerNotificationWidget(
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
    QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
{
}

KMcpClientInspectorServerNotificationWidget::~KMcpClientInspectorServerNotificationWidget() = default;
#include "moc_kmcpclientinspectorservernotificationwidget.cpp"
