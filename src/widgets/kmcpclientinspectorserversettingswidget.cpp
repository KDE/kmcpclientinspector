/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorserversettingswidget.h"
#include "kmcpclientinspectorconnectbuttonswidget.h"
#include <KLocalizedString>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
#include <TextAutoGenerateTextMcpProtocolWidgets/AddMcpServerWidget>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorServerSettingsWidget::KMcpClientInspectorServerSettingsWidget(const TextAutoGenerateTextMcpProtocolCore::McpServer &server, QWidget *parent)
    : QWidget{parent}
    , mMcpServerWidget(new TextAutoGenerateTextMcpProtocolWidgets::AddMcpServerWidget(this))
    , mConnectButtonsWidget(new KMcpClientInspectorConnectButtonsWidget(this))
    , mServer(server)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mMcpServerWidget->setObjectName(u"mMcpServerWidget"_s);
    mainLayout->addWidget(mMcpServerWidget);
    mMcpServerWidget->setServerInfo(server);
    mConnectButtonsWidget->setObjectName(u"mConnectButtonsWidget"_s);
    mainLayout->addWidget(mConnectButtonsWidget);

    mainLayout->addStretch(1);
    connect(mConnectButtonsWidget,
            &KMcpClientInspectorConnectButtonsWidget::connectRequested,
            this,
            &KMcpClientInspectorServerSettingsWidget::slotConnectRequested);
    connect(mConnectButtonsWidget,
            &KMcpClientInspectorConnectButtonsWidget::disconnectRequested,
            this,
            &KMcpClientInspectorServerSettingsWidget::slotDisconnectRequested);
    connect(mMcpServerWidget,
            &TextAutoGenerateTextMcpProtocolWidgets::AddMcpServerWidget::buttonOkEnabled,
            this,
            &KMcpClientInspectorServerSettingsWidget::slotUpdateButtons);

    slotUpdateButtons(mServer.isValid());
}

KMcpClientInspectorServerSettingsWidget::~KMcpClientInspectorServerSettingsWidget() = default;

void KMcpClientInspectorServerSettingsWidget::slotUpdateButtons([[maybe_unused]] bool valid)
{
    mConnectButtonsWidget->updateButtons(true, false);
}

void KMcpClientInspectorServerSettingsWidget::slotDisconnectRequested()
{
    mConnectButtonsWidget->updateButtons(true, false);
    Q_EMIT startStopRequested(false);
}

void KMcpClientInspectorServerSettingsWidget::slotConnectRequested()
{
    mConnectButtonsWidget->updateButtons(false, true);
    Q_EMIT startStopRequested(true);
}

#include "moc_kmcpclientinspectorserversettingswidget.cpp"
