/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlistresourceswidget.h"
#include <KLocalizedString>
#include <QJsonObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolListResourcesResult>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorListResourcesWidget::KMcpClientInspectorListResourcesWidget(
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
    QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto listResourcesButton = new QPushButton(i18nc("@action:button", "List Resources"), this);
    listResourcesButton->setObjectName(u"listResourcesButton"_s);
    connect(listResourcesButton, &QPushButton::clicked, this, [this]() {
        mProtocolManager->executeAction(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListResources);
    });
    mainLayout->addWidget(listResourcesButton);
    mainLayout->addStretch(1);
}

KMcpClientInspectorListResourcesWidget::~KMcpClientInspectorListResourcesWidget() = default;

void KMcpClientInspectorListResourcesWidget::setResult(const QJsonObject &obj)
{
    auto result = TextAutoGenerateTextMcpProtocolCore::McpProtocolListResourcesResult::fromJson(obj["result"_L1].toObject());
    qDebug() << " result : " << result;
    // TODO
}
#include "moc_kmcpclientinspectorlistresourceswidget.cpp"
