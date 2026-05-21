/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectortabpage.h"
#include "actions/kmcpclientinspectoractiontabwidget.h"
#include "kmcpclientinspectorclientprotocolmanager.h"
#include "kmcpclientinspectorserversettingswidget.h"
#include <KLocalizedString>
#include <QJsonObject>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClient>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolInitializeRequest>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorTabPage::KMcpClientInspectorTabPage(const TextAutoGenerateTextMcpProtocolCore::McpServer &server, QWidget *parent)
    : QWidget{parent}
    , mServerSettings(new KMcpClientInspectorServerSettingsWidget(server, this))
    , mActionTabWidget(new KMcpClientInspectorActionTabWidget(this))
    , mServer(server)
    , mProtocolManager(new KMcpClientInspectorClientProtocolManager(server, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    auto splitter = new QSplitter(this);
    splitter->setObjectName(u"splitter"_s);
    splitter->setChildrenCollapsible(false);
    mainLayout->addWidget(splitter);

    mServerSettings->setObjectName(u"mServerSettings"_s);
    splitter->addWidget(mServerSettings);

    mActionTabWidget->setObjectName(u"mActionTabWidget"_s);
    splitter->addWidget(mActionTabWidget);
    connect(mServerSettings, &KMcpClientInspectorServerSettingsWidget::startStopRequested, this, &KMcpClientInspectorTabPage::initializeClient);
}

KMcpClientInspectorTabPage::~KMcpClientInspectorTabPage() = default;

void KMcpClientInspectorTabPage::initializeClient(bool started)
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
        mActionTabWidget->setClient(mClient);
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

#include "moc_kmcpclientinspectortabpage.cpp"
