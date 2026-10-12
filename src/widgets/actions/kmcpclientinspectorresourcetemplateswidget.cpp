/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorresourcetemplateswidget.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorResourceTemplatesWidget::KMcpClientInspectorResourceTemplatesWidget(
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
    QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
    , mTextBrowser(new QTextBrowser(this))
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

    mTextBrowser->setObjectName(u"mTextBrowser"_s);
    mTextBrowser->setReadOnly(true);
    mainLayout->addWidget(mTextBrowser);
}

KMcpClientInspectorResourceTemplatesWidget::~KMcpClientInspectorResourceTemplatesWidget() = default;

void KMcpClientInspectorResourceTemplatesWidget::setResult(const QJsonObject &obj)
{
    const QString json = QString::fromUtf8(QJsonDocument(obj["result"_L1].toObject()).toJson(QJsonDocument::Indented));
    mTextBrowser->setMarkdown(u"```json\n"_s + json + u"```\n"_s);
}

#include "moc_kmcpclientinspectorresourcetemplateswidget.cpp"
