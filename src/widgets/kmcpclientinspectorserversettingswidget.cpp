/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorserversettingswidget.h"
#include "actions/kmcpclientinspectorcapabilitywidget.h"
#include "kmcpclientinspectorconnectbuttonswidget.h"
#include "kmcpclientinspectorserverinfowidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QPointer>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
#include <TextAutoGenerateTextMcpProtocolWidgets/AddMcpServerDialog>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorServerSettingsWidget::KMcpClientInspectorServerSettingsWidget(const TextAutoGenerateTextMcpProtocolCore::McpServer &server, QWidget *parent)
    : QWidget{parent}
    , mServerInfoWidget(new KMcpClientInspectorServerInfoWidget(this))
    , mEditButton(new QPushButton(QIcon::fromTheme(u"document-edit"_s), i18nc("@action:button", "Edit…"), this))
    , mConnectButtonsWidget(new KMcpClientInspectorConnectButtonsWidget(this))
    , mCapabilityWidget(new KMcpClientInspectorCapabilityWidget(this))
    , mServer(server)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mServerInfoWidget->setObjectName(u"mServerInfoWidget"_s);
    mainLayout->addWidget(mServerInfoWidget);
    mServerInfoWidget->setServerInfo(server);

    mCapabilityWidget->setObjectName(u"mCapabilityWidget"_s);
    mainLayout->addWidget(mCapabilityWidget);

    mEditButton->setObjectName(u"mEditButton"_s);
    auto editLayout = new QHBoxLayout;
    editLayout->setObjectName(u"editLayout"_s);
    editLayout->setContentsMargins({});
    editLayout->addStretch(1);
    editLayout->addWidget(mEditButton);
    mainLayout->addLayout(editLayout);

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
    connect(mEditButton, &QPushButton::clicked, this, &KMcpClientInspectorServerSettingsWidget::slotEditServer);

    slotUpdateButtons(mServer.isValid());
}

KMcpClientInspectorServerSettingsWidget::~KMcpClientInspectorServerSettingsWidget() = default;

void KMcpClientInspectorServerSettingsWidget::slotUpdateButtons([[maybe_unused]] bool valid)
{
    mConnectButtonsWidget->updateButtons(true, false);
}

void KMcpClientInspectorServerSettingsWidget::slotEditServer()
{
    QPointer<TextAutoGenerateTextMcpProtocolWidgets::AddMcpServerDialog> dlg = new TextAutoGenerateTextMcpProtocolWidgets::AddMcpServerDialog(this);
    dlg->setServerInfo(mServer);
    if (dlg->exec()) {
        const TextAutoGenerateTextMcpProtocolCore::McpServer server = dlg->serverInfo();
        if (!(server == mServer)) {
            mServer = server;
            mServerInfoWidget->setServerInfo(mServer);
            Q_EMIT serverModified(mServer);
        }
    }
    delete dlg;
}

void KMcpClientInspectorServerSettingsWidget::slotDisconnectRequested()
{
    mConnectButtonsWidget->updateButtons(true, false);
    mEditButton->setEnabled(true);
    mCapabilityWidget->setEnabled(true);
    Q_EMIT startStopRequested(false);
}

void KMcpClientInspectorServerSettingsWidget::slotConnectRequested()
{
    mConnectButtonsWidget->updateButtons(false, true);
    mEditButton->setEnabled(false);
    mCapabilityWidget->setEnabled(false);
    Q_EMIT startStopRequested(true);
}

#include "moc_kmcpclientinspectorserversettingswidget.cpp"
