/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorclientprotocolmanager.h"
#include "kmcpclientinspector_core_debug.h"
#include <QDebug>
#include <QJsonObject>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClient>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolInitializeRequest>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolListPromptsRequest>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolListToolsRequest>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolPingRequest>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorClientProtocolManager::KMcpClientInspectorClientProtocolManager(const TextAutoGenerateTextMcpProtocolCore::McpServer &server,
                                                                                   QObject *parent)
    : QObject{parent}
    , mServer(server)
{
}

KMcpClientInspectorClientProtocolManager::~KMcpClientInspectorClientProtocolManager() = default;

int KMcpClientInspectorClientProtocolManager::requestId()
{
    mRequestIdentifier++;
    return mRequestIdentifier;
}

void KMcpClientInspectorClientProtocolManager::initializeClient(bool started)
{
    Q_UNUSED(started)
    if (!mClient) {
        mClient = new TextAutoGenerateTextMcpProtocolCore::McpProtocolClient(mServer.transportType(), this);
        connect(mClient, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClient::error, this, [](const QString &error) {
            qCDebug(KMCPCLIENTINSPECTOR_CORE_LOG) << " ERROR " << error;
        });
        connect(mClient, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClient::received, this, [this](const QJsonObject &obj) {
            qCDebug(KMCPCLIENTINSPECTOR_CORE_LOG) << " receive " << obj;
            Q_EMIT received(obj);
        });
        connect(mClient, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClient::started, this, []() {
            qCDebug(KMCPCLIENTINSPECTOR_CORE_LOG) << " Started ! ";
        });
    }
    mClient->setSettings(mServer.settings());
    mClient->start();

    // TODO initialize
    TextAutoGenerateTextMcpProtocolCore::McpProtocolInitializeRequest initRequest;
    TextAutoGenerateTextMcpProtocolCore::McpProtocolInitializeRequestParams params;
    params.setProtocolVersion(TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::convertProtocolVersionToString(
        TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::ProtocolVersion::V2025_03_26));

    auto clientInfo = params.clientInfo();
    clientInfo.setName(u"kmcpinspector"_s);
    clientInfo.setVersion(u"1"_s);
    params.setClientInfo(clientInfo);

    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientCapabilities capabilities = params.capabilities();
    auto roots = capabilities.roots();
    roots.emplace(true);
    capabilities.setRoots(roots);

    params.setCapabilities(capabilities);
    initRequest.setParams(params);
    initRequest.setId(requestId());
    qCDebug(KMCPCLIENTINSPECTOR_CORE_LOG) << " initRequest " << initRequest;
    mClient->request(TextAutoGenerateTextMcpProtocolCore::McpProtocolInitializeRequest::toJson(initRequest));
}

void KMcpClientInspectorClientProtocolManager::ping()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolPingRequest pingRequest;
    pingRequest.setId(requestId());
    mClient->request(TextAutoGenerateTextMcpProtocolCore::McpProtocolPingRequest::toJson(pingRequest));
}

void KMcpClientInspectorClientProtocolManager::listTools()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolListToolsRequest listToolsRequest;
    listToolsRequest.setId(requestId());
    mClient->request(TextAutoGenerateTextMcpProtocolCore::McpProtocolListToolsRequest::toJson(listToolsRequest));
}

void KMcpClientInspectorClientProtocolManager::listPrompts()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolListPromptsRequest listPromptsRequest;
    listPromptsRequest.setId(requestId());
    mClient->request(TextAutoGenerateTextMcpProtocolCore::McpProtocolListPromptsRequest::toJson(listPromptsRequest));
}

void KMcpClientInspectorClientProtocolManager::resouceTemplates()
{
    // TODO
}

#include "moc_kmcpclientinspectorclientprotocolmanager.cpp"
