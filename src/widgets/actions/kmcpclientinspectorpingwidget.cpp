/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorpingwidget.h"
#include "kmcpclientinspector_widget_debug.h"
#include <KLocalizedString>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolPingRequest>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorPingWidget::KMcpClientInspectorPingWidget(QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto pingButton = new QPushButton(i18nc("@action:button", "Ping"), this);
    pingButton->setObjectName(u"pingButton"_s);
    connect(pingButton, &QPushButton::clicked, this, [this]() {
        if (mClient) {
            qDebug() << " ping !!!!!!!!!!";
        } else {
            qCWarning(KMCPCLIENTINSPECTOR_WIDGET_LOG) << "Client is not defined it's a bug! ";
        }
    });
    mainLayout->addWidget(pingButton);
}

KMcpClientInspectorPingWidget::~KMcpClientInspectorPingWidget() = default;

#include "moc_kmcpclientinspectorpingwidget.cpp"
