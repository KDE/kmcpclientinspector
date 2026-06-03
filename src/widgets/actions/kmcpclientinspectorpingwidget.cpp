/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorpingwidget.h"
#include "kmcpclientinspectorclientprotocolmanager.h"
#include <KLocalizedString>
#include <QPushButton>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorPingWidget::KMcpClientInspectorPingWidget(KMcpClientInspectorClientProtocolManager *protocolManager, QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto pingButton = new QPushButton(i18nc("@action:button", "Ping"), this);
    pingButton->setObjectName(u"pingButton"_s);
    connect(pingButton, &QPushButton::clicked, this, [this]() {
        mProtocolManager->executeAction(KMcpClientInspectorClientProtocolManager::MethodType::Ping);
    });
    mainLayout->addWidget(pingButton);
    mainLayout->addStretch(1);
}

KMcpClientInspectorPingWidget::~KMcpClientInspectorPingWidget() = default;

#include "moc_kmcpclientinspectorpingwidget.cpp"
