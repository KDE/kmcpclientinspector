/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlistresourceswidget.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorListResourcesWidget::KMcpClientInspectorListResourcesWidget(
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
    QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
    , mTextBrowser(new QTextBrowser(this))
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

    mTextBrowser->setObjectName(u"mTextBrowser"_s);
    mTextBrowser->setReadOnly(true);
    mainLayout->addWidget(mTextBrowser);
}

KMcpClientInspectorListResourcesWidget::~KMcpClientInspectorListResourcesWidget() = default;

void KMcpClientInspectorListResourcesWidget::setResult(const QJsonObject &obj)
{
    const QString json = QString::fromUtf8(QJsonDocument(obj["result"_L1].toObject()).toJson(QJsonDocument::Indented));
    mTextBrowser->setMarkdown(u"```json\n"_s + json + u"```\n"_s);
}
#include "moc_kmcpclientinspectorlistresourceswidget.cpp"
