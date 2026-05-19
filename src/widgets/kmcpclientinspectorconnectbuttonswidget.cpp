/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorconnectbuttonswidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QPushButton>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorConnectButtonsWidget::KMcpClientInspectorConnectButtonsWidget(QWidget *parent)
    : QWidget{parent}
    , mConnectButton(new QPushButton(i18nc("@action:button", "Connect"), this))
    , mDisconnectButton(new QPushButton(i18nc("@action:button", "DisConnect"), this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});

    mConnectButton->setObjectName("mConnectButton"_L1);
    mDisconnectButton->setObjectName("mDisconnectButton"_L1);

    mConnectButton->setEnabled(false);
    mDisconnectButton->setEnabled(false);

    mainLayout->addWidget(mConnectButton);
    mainLayout->addWidget(mDisconnectButton);
    connect(mConnectButton, &QPushButton::clicked, this, [this]() {
        Q_EMIT connectRequested();
        updateButtons(false, true);
    });
    connect(mDisconnectButton, &QPushButton::clicked, this, [this]() {
        Q_EMIT disconnectRequested();
        updateButtons(true, false);
    });
}

KMcpClientInspectorConnectButtonsWidget::~KMcpClientInspectorConnectButtonsWidget() = default;

void KMcpClientInspectorConnectButtonsWidget::updateButtons(bool connected, bool disconnected)
{
    mConnectButton->setEnabled(connected);
    mDisconnectButton->setEnabled(disconnected);
}

#include "moc_kmcpclientinspectorconnectbuttonswidget.cpp"
