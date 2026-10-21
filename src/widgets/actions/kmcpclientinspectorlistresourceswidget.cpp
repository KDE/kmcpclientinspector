/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlistresourceswidget.h"
#include <KLocalizedString>
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorListResourcesWidget::KMcpClientInspectorListResourcesWidget(
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
    QWidget *parent)
    : KMcpClientInspectorResultWidget{protocolManager,
                                      i18nc("@action:button", "List Resources"),
                                      TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListResources,
                                      parent}
{
    connect(mProtocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::initialized, this, [this]() {
        if (mProtocolManager->serverSupports(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListResources)) {
            mProtocolManager->executeAction(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListResources);
        }
    });
    connect(mProtocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::resourcesListChanged, this, [this]() {
        if (mProtocolManager->isInitialized()
            && mProtocolManager->serverSupports(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListResources)) {
            mProtocolManager->executeAction(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListResources);
        }
    });
}

KMcpClientInspectorListResourcesWidget::~KMcpClientInspectorListResourcesWidget() = default;

void KMcpClientInspectorListResourcesWidget::setResult(const QJsonObject &obj)
{
    showJson(obj["result"_L1].toObject());
}

#include "moc_kmcpclientinspectorlistresourceswidget.cpp"
