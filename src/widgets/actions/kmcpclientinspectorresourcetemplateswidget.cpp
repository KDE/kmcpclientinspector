/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorresourcetemplateswidget.h"
#include <KLocalizedString>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorResourceTemplatesWidget::KMcpClientInspectorResourceTemplatesWidget(
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
    QWidget *parent)
    : KMcpClientInspectorResultWidget{protocolManager,
                                      i18nc("@action:button", "Resource Templates"),
                                      TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ResourceTemplates,
                                      parent}
{
    connect(mProtocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::initialized, this, [this]() {
        if (mProtocolManager->serverSupports(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ResourceTemplates)) {
            mProtocolManager->executeAction(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ResourceTemplates);
        }
    });
    connect(mProtocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::resourcesListChanged, this, [this]() {
        if (mProtocolManager->isInitialized()
            && mProtocolManager->serverSupports(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ResourceTemplates)) {
            mProtocolManager->executeAction(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ResourceTemplates);
        }
    });
}

KMcpClientInspectorResourceTemplatesWidget::~KMcpClientInspectorResourceTemplatesWidget() = default;

void KMcpClientInspectorResourceTemplatesWidget::setResult(const QJsonObject &obj)
{
    showJson(obj["result"_L1].toObject());
}

#include "moc_kmcpclientinspectorresourcetemplateswidget.cpp"
