/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlistpromptswidget.h"
#include <KLocalizedString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorListPromptsWidget::KMcpClientInspectorListPromptsWidget(
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
    QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
    , mTextBrowser(new QTextBrowser(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto listPromptsButton = new QPushButton(i18nc("@action:button", "List Prompts"), this);
    listPromptsButton->setObjectName(u"listPromptsButton"_s);
    connect(listPromptsButton, &QPushButton::clicked, this, [this]() {
        mProtocolManager->executeAction(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListPrompts);
    });
    mainLayout->addWidget(listPromptsButton);

    mTextBrowser->setObjectName(u"mTextBrowser"_s);
    mTextBrowser->setReadOnly(true);
    mainLayout->addWidget(mTextBrowser);
}

KMcpClientInspectorListPromptsWidget::~KMcpClientInspectorListPromptsWidget() = default;

void KMcpClientInspectorListPromptsWidget::setResult(const QJsonObject &obj)
{
    const QString json = QString::fromUtf8(QJsonDocument(obj["result"_L1].toObject()).toJson(QJsonDocument::Indented));
    mTextBrowser->setMarkdown(u"```json\n"_s + json + u"```\n"_s);
}
#include "moc_kmcpclientinspectorlistpromptswidget.cpp"
