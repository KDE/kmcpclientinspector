/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlistpromptswidget.h"
#include "kmcpclientinspectorclientprotocolmanager.h"
#include <KLocalizedString>
#include <QJsonObject>
#include <QPushButton>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorListPromptsWidget::KMcpClientInspectorListPromptsWidget(KMcpClientInspectorClientProtocolManager *protocolManager, QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto listPromptsButton = new QPushButton(i18nc("@action:button", "List Prompts"), this);
    listPromptsButton->setObjectName(u"listPromptsButton"_s);
    connect(listPromptsButton, &QPushButton::clicked, this, [this]() {
        mProtocolManager->executeAction(KMcpClientInspectorClientProtocolManager::MethodType::ListPrompts);
    });
    mainLayout->addWidget(listPromptsButton);
    mainLayout->addStretch(1);
}

KMcpClientInspectorListPromptsWidget::~KMcpClientInspectorListPromptsWidget() = default;

void KMcpClientInspectorListPromptsWidget::setResult([[maybe_unused]] const QJsonObject &obj)
{
    // TODO
}
#include "moc_kmcpclientinspectorlistpromptswidget.cpp"
