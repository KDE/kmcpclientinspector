/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorresultwidget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorResultWidget::KMcpClientInspectorResultWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                                 const QString &buttonText,
                                                                 TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType type,
                                                                 QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
    , mTextBrowser(new QTextBrowser(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto actionButton = new QPushButton(buttonText, this);
    actionButton->setObjectName(u"actionButton"_s);
    connect(actionButton, &QPushButton::clicked, this, [this, type]() {
        mProtocolManager->executeAction(type);
    });
    mainLayout->addWidget(actionButton);

    mTextBrowser->setObjectName(u"mTextBrowser"_s);
    mTextBrowser->setReadOnly(true);
    mainLayout->addWidget(mTextBrowser);
}

KMcpClientInspectorResultWidget::~KMcpClientInspectorResultWidget() = default;

void KMcpClientInspectorResultWidget::setResult(const QJsonObject &obj)
{
    showJson(obj);
}

void KMcpClientInspectorResultWidget::showJson(const QJsonObject &obj)
{
    const QString json = QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    mTextBrowser->setMarkdown(u"```json\n"_s + json + u"```\n"_s);
}

#include "moc_kmcpclientinspectorresultwidget.cpp"
