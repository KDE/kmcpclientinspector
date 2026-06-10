/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectortabpage.h"
#include "actions/kmcpclientinspectoractiontabwidget.h"
#include "kmcpclientinspectorserversettingswidget.h"
#include <KLocalizedString>
#include <QJsonObject>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClient>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolInitializeRequest>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorTabPage::KMcpClientInspectorTabPage(const TextAutoGenerateTextMcpProtocolCore::McpServer &server, QWidget *parent)
    : QWidget{parent}
    , mProtocolManager(new TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager(server, this))
    , mServerSettings(new KMcpClientInspectorServerSettingsWidget(server, this))
    , mActionTabWidget(new KMcpClientInspectorActionTabWidget(mProtocolManager, this))
{
    mProtocolManager->setClientName(u"kmcpclientinspector"_s);
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
    connect(mServerSettings,
            &KMcpClientInspectorServerSettingsWidget::startStopRequested,
            mProtocolManager,
            &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::initializeClient);
}

KMcpClientInspectorTabPage::~KMcpClientInspectorTabPage() = default;

#include "moc_kmcpclientinspectortabpage.cpp"
