/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorclientprotocolmanager.h"
#include "kmcpclientinspector_widget_debug.h"
#include <QDebug>
#include <QJsonObject>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClient>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolInitializeRequest>
#include <textautogeneratetextmcpprotocolcore/mcpprotocolpingrequest.h>
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
            qCDebug(KMCPCLIENTINSPECTOR_WIDGET_LOG) << " ERROR " << error;
        });
        connect(mClient, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClient::received, this, [](const QJsonObject &obj) {
            qCDebug(KMCPCLIENTINSPECTOR_WIDGET_LOG) << " receive " << obj;
        });
        connect(mClient, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClient::started, this, []() {
            qCDebug(KMCPCLIENTINSPECTOR_WIDGET_LOG) << " Started ! ";
        });
        // TODO mActionTabWidget->setClient(mClient);
    }
    mClient->setSettings(mServer.settings());
    mClient->start();

    // TODO initialize
    TextAutoGenerateTextMcpProtocolCore::McpProtocolInitializeRequest initRequest;
    TextAutoGenerateTextMcpProtocolCore::McpProtocolInitializeRequestParams params;
    params.setProtocolVersion(TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::convertProtocolVersionToString(
        TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::ProtocolVersion::V2025_03_26));

    // TODO fix me.
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
    TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::RequestId id = requestId();
    initRequest.setId(id);
    qCDebug(KMCPCLIENTINSPECTOR_WIDGET_LOG) << " initRequest " << initRequest;
    mClient->request(TextAutoGenerateTextMcpProtocolCore::McpProtocolInitializeRequest::toJson(initRequest));
}

void KMcpClientInspectorClientProtocolManager::ping()
{
    TextAutoGenerateTextMcpProtocolCore::McpProtocolPingRequest pingRequest;
    pingRequest.setId(requestId());
    mClient->request(TextAutoGenerateTextMcpProtocolCore::McpProtocolPingRequest::toJson(pingRequest));
}

#include "moc_kmcpclientinspectorclientprotocolmanager.cpp"
