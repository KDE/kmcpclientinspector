/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectoractiontabwidget.h"
#include "actions/kmcpclientinspectorlistpromptswidget.h"
#include "actions/kmcpclientinspectorlistresourceswidget.h"
#include "actions/kmcpclientinspectorlisttoolswidget.h"
#include "actions/kmcpclientinspectorpingwidget.h"
#include "actions/kmcpclientinspectorresourcetemplateswidget.h"
#include "kmcpclientinspector_widget_debug.h"
#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorActionTabWidget::KMcpClientInspectorActionTabWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                                       QWidget *parent)
    : QWidget{parent}
    , mPingWidget(new KMcpClientInspectorPingWidget(protocolManager, this))
    , mListToolsWidget(new KMcpClientInspectorListToolsWidget(protocolManager, this))
    , mListPromptsWidget(new KMcpClientInspectorListPromptsWidget(protocolManager, this))
    , mListResourcesWidget(new KMcpClientInspectorListResourcesWidget(protocolManager, this))
    , mResourceTemplatesWidget(new KMcpClientInspectorResourceTemplatesWidget(protocolManager, this))
    , mTabWidget(new QTabWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mPingWidget->setObjectName(u"mPingWidget"_s);
    mTabWidget->setObjectName(u"mTabWidget"_s);
    mListToolsWidget->setObjectName(u"mListToolsWidget"_s);
    mListPromptsWidget->setObjectName(u"mListPromptsWidget"_s);
    mListResourcesWidget->setObjectName(u"mListResourcesWidget"_s);
    mResourceTemplatesWidget->setObjectName(u"mResourceTemplatesWidget"_s);

    mainLayout->addWidget(mTabWidget);
    mTabWidget->addTab(mPingWidget, i18n("Ping"));
    mTabWidget->addTab(mListToolsWidget, i18n("List Tools"));
    mTabWidget->addTab(mListPromptsWidget, i18n("List Prompts"));
    mTabWidget->addTab(mListResourcesWidget, i18n("List Resources"));
    mTabWidget->addTab(mResourceTemplatesWidget, i18n("Resource Templates"));

    mTabWidget->setTabVisible(mTabWidget->indexOf(mResourceTemplatesWidget), false);
    mTabWidget->setTabVisible(mTabWidget->indexOf(mListPromptsWidget), false);
    mTabWidget->setTabVisible(mTabWidget->indexOf(mListResourcesWidget), false);

    connect(protocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::initialized, this, [this, protocolManager]() {
        mTabWidget->setTabVisible(
            mTabWidget->indexOf(mResourceTemplatesWidget),
            protocolManager->serverSupports(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ResourceTemplates));

        mTabWidget->setTabVisible(
            mTabWidget->indexOf(mListPromptsWidget),
            protocolManager->serverSupports(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListPrompts));

        mTabWidget->setTabVisible(
            mTabWidget->indexOf(mListResourcesWidget),
            protocolManager->serverSupports(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListResources));
    });

    connect(protocolManager,
            &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::received,
            this,
            [this](const QJsonObject &obj, TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType type) {
                switch (type) {
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Ping:
                    mPingWidget->setResult(obj);
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListTools:
                    mListToolsWidget->setResult(obj);
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListPrompts:
                    mListPromptsWidget->setResult(obj);
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ResourceTemplates:
                    mResourceTemplatesWidget->setResult(obj);
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Initialize:
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ReadResource:
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListResources:
                    mListResourcesWidget->setResult(obj);
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::GetPrompt:
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ServerNotification:
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::CallTool:
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ServerRequest:
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Unknown:
                    qCWarning(KMCPCLIENTINSPECTOR_WIDGET_LOG) << "It's a bug. MethodType::Unknown must not used.";
                    break;
                }
            });
}

KMcpClientInspectorActionTabWidget::~KMcpClientInspectorActionTabWidget() = default;

#include "moc_kmcpclientinspectoractiontabwidget.cpp"
