/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorserversettingswidget.h"
#include <KLocalizedString>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClient>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
#include <TextAutoGenerateTextMcpProtocolWidgets/AddMcpServerWidget>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorServerSettingsWidget::KMcpClientInspectorServerSettingsWidget(const TextAutoGenerateTextMcpProtocolCore::McpServer &server, QWidget *parent)
    : QWidget{parent}
    , mMcpServerWidget(new TextAutoGenerateTextMcpProtocolWidgets::AddMcpServerWidget(this))
    , mStartStopButton(new QPushButton(i18nc("@action:button", "Start"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mMcpServerWidget->setObjectName(u"mMcpServerWidget"_s);
    mainLayout->addWidget(mMcpServerWidget);
    mMcpServerWidget->setServerInfo(server);
    mStartStopButton->setObjectName(u"mStartStopButton"_s);
    mainLayout->addWidget(mStartStopButton);

    mainLayout->addStretch(1);
    connect(mStartStopButton, &QPushButton::clicked, this, &KMcpClientInspectorServerSettingsWidget::slotStopStart);
    connect(mMcpServerWidget, &TextAutoGenerateTextMcpProtocolWidgets::AddMcpServerWidget::buttonOkEnabled, mStartStopButton, &QPushButton::setEnabled);
}

KMcpClientInspectorServerSettingsWidget::~KMcpClientInspectorServerSettingsWidget() = default;

void KMcpClientInspectorServerSettingsWidget::slotStopStart()
{
    if (!mClient) {
        mClient = new TextAutoGenerateTextMcpProtocolCore::McpProtocolClient(convertTransportType(mMcpServerWidget->serverInfo().transportType()), this);
    }
    // TODO
}

TextAutoGenerateTextMcpProtocolCore::McpProtocolPlugin::ProtocolType
KMcpClientInspectorServerSettingsWidget::convertTransportType(TextAutoGenerateTextMcpProtocolCore::McpServer::TransportType type) const
{
    switch (type) {
    case TextAutoGenerateTextMcpProtocolCore::McpServer::TransportType::Unknown:
        return TextAutoGenerateTextMcpProtocolCore::McpProtocolPlugin::ProtocolType::Unknown;
    case TextAutoGenerateTextMcpProtocolCore::McpServer::TransportType::Sse:
        return TextAutoGenerateTextMcpProtocolCore::McpProtocolPlugin::ProtocolType::Sse;
    case TextAutoGenerateTextMcpProtocolCore::McpServer::TransportType::Stdio:
        return TextAutoGenerateTextMcpProtocolCore::McpProtocolPlugin::ProtocolType::Stdio;
    case TextAutoGenerateTextMcpProtocolCore::McpServer::TransportType::StreamableHttp:
        return TextAutoGenerateTextMcpProtocolCore::McpProtocolPlugin::ProtocolType::StreamableHttp;
    }
    return TextAutoGenerateTextMcpProtocolCore::McpProtocolPlugin::ProtocolType::Unknown;
}

#include "moc_kmcpclientinspectorserversettingswidget.cpp"
