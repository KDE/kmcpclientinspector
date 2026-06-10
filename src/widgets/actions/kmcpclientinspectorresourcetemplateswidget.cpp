/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorresourcetemplateswidget.h"
#include <KLocalizedString>
#include <QJsonObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorResourceTemplatesWidget::KMcpClientInspectorResourceTemplatesWidget(
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
    QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto resourceTemplatesButton = new QPushButton(i18nc("@action:button", "Resource Templates"), this);
    resourceTemplatesButton->setObjectName(u"resourceTemplatesButton"_s);
    connect(resourceTemplatesButton, &QPushButton::clicked, this, [this]() {
        mProtocolManager->executeAction(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ResourceTemplates);
    });
    mainLayout->addWidget(resourceTemplatesButton);
    mainLayout->addStretch(1);
}

KMcpClientInspectorResourceTemplatesWidget::~KMcpClientInspectorResourceTemplatesWidget() = default;

void KMcpClientInspectorResourceTemplatesWidget::setResult([[maybe_unused]] const QJsonObject &obj)
{
    // TODO
}

#include "moc_kmcpclientinspectorresourcetemplateswidget.cpp"
