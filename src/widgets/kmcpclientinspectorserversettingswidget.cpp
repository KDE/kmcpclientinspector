/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorserversettingswidget.h"
#include "actions/kmcpclientinspectorcapabilitywidget.h"
#include "dialogs/kmcpclientinspectorrootsdialog.h"
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
    connect(mCapabilityWidget,
            &KMcpClientInspectorCapabilityWidget::configureRootsRequested,
            this,
            &KMcpClientInspectorServerSettingsWidget::slotConfigureRoots);

    slotUpdateButtons(mServer.isValid());
}

KMcpClientInspectorServerSettingsWidget::~KMcpClientInspectorServerSettingsWidget() = default;

void KMcpClientInspectorServerSettingsWidget::slotUpdateButtons(bool valid)
{
    if (valid) {
        mConnectButtonsWidget->updateButtons(true, false);
    } else {
        mConnectButtonsWidget->updateButtons(false, false);
    }
}

void KMcpClientInspectorServerSettingsWidget::slotEditServer()
{
    QPointer<TextAutoGenerateTextMcpProtocolWidgets::AddMcpServerDialog> dlg = new TextAutoGenerateTextMcpProtocolWidgets::AddMcpServerDialog(this);
    dlg->setServerInfo(mServer);
    if (dlg->exec()) {
        const TextAutoGenerateTextMcpProtocolCore::McpServer server = dlg->serverInfo();
        if (server != mServer) {
            mServer = server;
            mServerInfoWidget->setServerInfo(mServer);
            Q_EMIT serverModified(mServer);
            slotUpdateButtons(mServer.isValid());
        }
    }
    delete dlg;
}

void KMcpClientInspectorServerSettingsWidget::slotConfigureRoots()
{
    QPointer<KMcpClientInspectorRootsDialog> dlg = new KMcpClientInspectorRootsDialog(this);
    dlg->setRoots(roots());
    if (dlg->exec()) {
        if (const QStringList newRoots = dlg->roots(); newRoots != roots()) {
            auto settings = mServer.settings();
            settings.setRoots(newRoots);
            mServer.setSettings(settings);
            mServerInfoWidget->setServerInfo(mServer);
            Q_EMIT serverModified(mServer);
            Q_EMIT rootsChanged(newRoots);
        }
    }
    delete dlg;
}

void KMcpClientInspectorServerSettingsWidget::slotDisconnectRequested()
{
    slotUpdateButtons(mServer.isValid());
    mEditButton->setEnabled(true);
    mCapabilityWidget->setConnected(false);
    Q_EMIT startStopRequested(false);
}

void KMcpClientInspectorServerSettingsWidget::slotConnectRequested()
{
    mConnectButtonsWidget->updateButtons(false, true);
    mEditButton->setEnabled(false);
    mCapabilityWidget->setConnected(true);
    Q_EMIT startStopRequested(true);
}

bool KMcpClientInspectorServerSettingsWidget::elicitationSupport() const
{
    return mCapabilityWidget->elicitationSupport();
}

bool KMcpClientInspectorServerSettingsWidget::urlElicitationSupport() const
{
    return mCapabilityWidget->urlElicitationSupport();
}

bool KMcpClientInspectorServerSettingsWidget::rootsSupport() const
{
    return mCapabilityWidget->rootsSupport();
}

bool KMcpClientInspectorServerSettingsWidget::samplingSupport() const
{
    return mCapabilityWidget->samplingSupport();
}

QStringList KMcpClientInspectorServerSettingsWidget::roots() const
{
    return mServer.settings().roots();
}

void KMcpClientInspectorServerSettingsWidget::wasDisconnected()
{
    slotDisconnectRequested();
}

#include "moc_kmcpclientinspectorserversettingswidget.cpp"
