/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorpingwidget.h"
#include "kmcpclientinspector_widget_debug.h"
#include "kmcpclientinspectorclientprotocolmanager.h"
#include <KLocalizedString>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClient>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolPingRequest>
#include <qjsonobject.h>
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
        mProtocolManager->ping();
    });
    mainLayout->addWidget(pingButton);
    mainLayout->addStretch(1);
}

KMcpClientInspectorPingWidget::~KMcpClientInspectorPingWidget() = default;

#include "moc_kmcpclientinspectorpingwidget.cpp"
