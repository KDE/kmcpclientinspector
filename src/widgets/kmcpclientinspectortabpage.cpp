/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectortabpage.h"
#include "actions/kmcpclientinspectoractiontabwidget.h"
#include "kmcpclientinspectorserversettingswidget.h"
#include <KLocalizedString>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClient>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorTabPage::KMcpClientInspectorTabPage(const TextAutoGenerateTextMcpProtocolCore::McpServer &server, QWidget *parent)
    : QWidget{parent}
    , mServerSettings(new KMcpClientInspectorServerSettingsWidget(server, this))
    , mActionTabWidget(new KMcpClientInspectorActionTabWidget(this))
    , mServer(server)
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
}

KMcpClientInspectorTabPage::~KMcpClientInspectorTabPage() = default;

void KMcpClientInspectorTabPage::initializeClient()
{
    if (!mClient) {
        // mClient = new TextAutoGenerateTextMcpProtocolCore::McpProtocolClient(mMcpServerWidget->serverInfo().transportType(), this);
    }
    mClient->setSettings(mServer.settings());
}

#include "moc_kmcpclientinspectortabpage.cpp"
