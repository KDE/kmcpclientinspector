/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorclientprotocolmanager.h"
#include <QDebug>
#include <QJsonObject>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClient>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolInitializeRequest>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorClientProtocolManager::KMcpClientInspectorClientProtocolManager(const TextAutoGenerateTextMcpProtocolCore::McpServer &server,
                                                                                   QObject *parent)
    : QObject{parent}
    , mServer(server)
{
}

void KMcpClientInspectorClientProtocolManager::initializeClient(bool started)
{
    Q_UNUSED(started)
    if (!mClient) {
        mClient = new TextAutoGenerateTextMcpProtocolCore::McpProtocolClient(mServer.transportType(), this);
        connect(mClient, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClient::error, this, [](const QString &error) {
            qDebug() << " ERROR " << error;
        });
        connect(mClient, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClient::received, this, [](const QJsonObject &obj) {
            qDebug() << " receive " << obj;
        });
        connect(mClient, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClient::started, this, []() {
            qDebug() << " Started ! ";
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
    TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::RequestId id = 1;
    initRequest.setId(id);
    qDebug() << " initRequest " << initRequest;
    mClient->request(TextAutoGenerateTextMcpProtocolCore::McpProtocolInitializeRequest::toJson(initRequest));
}

KMcpClientInspectorClientProtocolManager::~KMcpClientInspectorClientProtocolManager() = default;
#include "moc_kmcpclientinspectorclientprotocolmanager.cpp"
