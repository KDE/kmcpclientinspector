/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlistpromptswidget.h"
#include <KLocalizedString>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorListPromptsWidget::KMcpClientInspectorListPromptsWidget(
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
    QWidget *parent)
    : KMcpClientInspectorResultWidget{protocolManager,
                                      i18nc("@action:button", "List Prompts"),
                                      TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListPrompts,
                                      parent}
{
}

KMcpClientInspectorListPromptsWidget::~KMcpClientInspectorListPromptsWidget() = default;

void KMcpClientInspectorListPromptsWidget::setResult(const QJsonObject &obj)
{
    showJson(obj);
}

#include "moc_kmcpclientinspectorlistpromptswidget.cpp"
