/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorpingwidget.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorPingWidget::KMcpClientInspectorPingWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                             QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
    , mTextBrowser(new QTextBrowser(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto pingButton = new QPushButton(i18nc("@action:button", "Ping"), this);
    pingButton->setObjectName(u"pingButton"_s);
    connect(pingButton, &QPushButton::clicked, this, [this]() {
        mProtocolManager->executeAction(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Ping);
    });
    mainLayout->addWidget(pingButton);

    mTextBrowser->setObjectName(u"mTextBrowser"_s);
    mTextBrowser->setReadOnly(true);
    mainLayout->addWidget(mTextBrowser);
}

KMcpClientInspectorPingWidget::~KMcpClientInspectorPingWidget() = default;

void KMcpClientInspectorPingWidget::setResult(const QJsonObject &obj)
{
    const QString json = QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    mTextBrowser->setMarkdown(u"```json\n"_s + json + u"```\n"_s);
}

#include "moc_kmcpclientinspectorpingwidget.cpp"
