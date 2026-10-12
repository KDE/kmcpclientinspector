/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlisttoolswidget.h"
#include <KLocalizedString>
#include <QJsonObject>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolListToolsResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>

using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorListToolsWidget::KMcpClientInspectorListToolsWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                                       QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
    , mTextBrowser(new QTextBrowser(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto listToolsButton = new QPushButton(i18nc("@action:button", "List Tools"), this);
    listToolsButton->setObjectName(u"listToolsButton"_s);
    connect(listToolsButton, &QPushButton::clicked, this, [this]() {
        mProtocolManager->executeAction(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListTools);
    });
    mainLayout->addWidget(listToolsButton);

    mTextBrowser->setObjectName(u"mTextBrowser"_s);
    mTextBrowser->setReadOnly(true);
    mainLayout->addWidget(mTextBrowser);
}

KMcpClientInspectorListToolsWidget::~KMcpClientInspectorListToolsWidget() = default;

void KMcpClientInspectorListToolsWidget::setResult(const QJsonObject &obj)
{
    const QString json = QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    mTextBrowser->setMarkdown(u"```json\n"_s + json + u"```\n"_s);
}

#include "moc_kmcpclientinspectorlisttoolswidget.cpp"
