/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectoractiontabwidget.h"
#include "actions/kmcpclientinspectorcalltoolwidget.h"
#include "actions/kmcpclientinspectorgetpromptwidget.h"
#include "actions/kmcpclientinspectorinitializewidget.h"
#include "actions/kmcpclientinspectorlistpromptswidget.h"
#include "actions/kmcpclientinspectorlistresourceswidget.h"
#include "actions/kmcpclientinspectorlisttoolswidget.h"
#include "actions/kmcpclientinspectorlogwidget.h"
#include "actions/kmcpclientinspectormessagewidget.h"
#include "actions/kmcpclientinspectorpingwidget.h"
#include "actions/kmcpclientinspectorreadresourcewidget.h"
#include "actions/kmcpclientinspectorresourcetemplateswidget.h"
#include "actions/kmcpclientinspectorservernotificationwidget.h"
#include "kmcpclientinspector_widget_debug.h"
#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolProgressNotificationParams>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorActionTabWidget::KMcpClientInspectorActionTabWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                                       QWidget *parent)
    : QWidget{parent}
    , mInitializeWidget(new KMcpClientInspectorInitializeWidget(protocolManager, this))
    , mPingWidget(new KMcpClientInspectorPingWidget(protocolManager, this))
    , mListToolsWidget(new KMcpClientInspectorListToolsWidget(protocolManager, this))
    , mCallToolWidget(new KMcpClientInspectorCallToolWidget(protocolManager, this))
    , mListPromptsWidget(new KMcpClientInspectorListPromptsWidget(protocolManager, this))
    , mGetPromptWidget(new KMcpClientInspectorGetPromptWidget(protocolManager, this))
    , mListResourcesWidget(new KMcpClientInspectorListResourcesWidget(protocolManager, this))
    , mResourceTemplatesWidget(new KMcpClientInspectorResourceTemplatesWidget(protocolManager, this))
    , mReadResourceWidget(new KMcpClientInspectorReadResourceWidget(protocolManager, this))
    , mLogWidget(new KMcpClientInspectorLogWidget(protocolManager, this))
    , mTabWidget(new QTabWidget(this))
    , mMessageWidget(new KMcpClientInspectorMessageWidget(this))
    , mServerNotificationWidget(new KMcpClientInspectorServerNotificationWidget(protocolManager, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mInitializeWidget->setObjectName(u"mInitializeWidget"_s);
    mPingWidget->setObjectName(u"mPingWidget"_s);
    mTabWidget->setObjectName(u"mTabWidget"_s);
    mListToolsWidget->setObjectName(u"mListToolsWidget"_s);
    mCallToolWidget->setObjectName(u"mCallToolWidget"_s);
    mListPromptsWidget->setObjectName(u"mListPromptsWidget"_s);
    mGetPromptWidget->setObjectName(u"mGetPromptWidget"_s);
    mListResourcesWidget->setObjectName(u"mListResourcesWidget"_s);
    mResourceTemplatesWidget->setObjectName(u"mResourceTemplatesWidget"_s);
    mReadResourceWidget->setObjectName(u"mReadResourceWidget"_s);
    mMessageWidget->setObjectName(u"mMessageWidget"_s);
    mServerNotificationWidget->setObjectName(u"mServerNotificationWidget"_s);

    mainLayout->addWidget(mMessageWidget);
    mainLayout->addWidget(mTabWidget);
    mTabWidget->addTab(mInitializeWidget, i18n("Initialize"));
    mTabWidget->addTab(mPingWidget, i18n("Ping"));
    mTabWidget->addTab(mListToolsWidget, i18n("List Tools"));
    mTabWidget->addTab(mCallToolWidget, i18n("Call Tool"));
    mTabWidget->addTab(mListPromptsWidget, i18n("List Prompts"));
    mTabWidget->addTab(mGetPromptWidget, i18n("Get Prompt"));
    mTabWidget->addTab(mListResourcesWidget, i18n("List Resources"));
    mTabWidget->addTab(mResourceTemplatesWidget, i18n("Resource Templates"));
    mTabWidget->addTab(mReadResourceWidget, i18n("Read Resource"));
    mTabWidget->addTab(mLogWidget, i18n("Log"));
    mTabWidget->addTab(mServerNotificationWidget, i18n("Server Notification"));

    mTabWidget->setTabVisible(mTabWidget->indexOf(mResourceTemplatesWidget), false);
    mTabWidget->setTabVisible(mTabWidget->indexOf(mListPromptsWidget), false);
    mTabWidget->setTabVisible(mTabWidget->indexOf(mGetPromptWidget), false);
    mTabWidget->setTabVisible(mTabWidget->indexOf(mListResourcesWidget), false);
    mTabWidget->setTabVisible(mTabWidget->indexOf(mReadResourceWidget), false);
    mTabWidget->setTabVisible(mTabWidget->indexOf(mCallToolWidget), false);

    connect(protocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::error, this, [this](const QString &errorStr) {
        mMessageWidget->setText(errorStr);
        mMessageWidget->animatedShow();
    });

    connect(protocolManager,
            &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::progressReceived,
            this,
            [](qint64 requestId, const TextAutoGenerateTextMcpProtocolCore::McpProtocolProgressNotificationParams &params) {
                qDebug() << " progressReceived*********** id : " << requestId << " params " << params;
            });

    connect(protocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::initialized, this, [this, protocolManager]() {
        mTabWidget->setTabVisible(
            mTabWidget->indexOf(mResourceTemplatesWidget),
            protocolManager->serverSupports(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ResourceTemplates));

        mTabWidget->setTabVisible(
            mTabWidget->indexOf(mListPromptsWidget),
            protocolManager->serverSupports(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListPrompts));

        mTabWidget->setTabVisible(
            mTabWidget->indexOf(mGetPromptWidget),
            protocolManager->serverSupports(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::GetPrompt));

        mTabWidget->setTabVisible(
            mTabWidget->indexOf(mListResourcesWidget),
            protocolManager->serverSupports(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListResources));
        mTabWidget->setTabVisible(
            mTabWidget->indexOf(mReadResourceWidget),
            protocolManager->serverSupports(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ReadResource));

        mTabWidget->setTabVisible(mTabWidget->indexOf(mCallToolWidget),
                                  protocolManager->serverSupports(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::CallTool));
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
                    mCallToolWidget->setToolsList(obj);
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListPrompts:
                    mListPromptsWidget->setResult(obj);
                    mGetPromptWidget->setPromptsList(obj);
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ResourceTemplates:
                    mResourceTemplatesWidget->setResult(obj);
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Initialize:
                    mInitializeWidget->setResult(obj);
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ListResources:
                    mListResourcesWidget->setResult(obj);
                    mReadResourceWidget->setResourcesList(obj);
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::GetPrompt:
                    mGetPromptWidget->setResult(obj);
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::CallTool:
                    mCallToolWidget->setResult(obj);
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ReadResource:
                    mReadResourceWidget->setResult(obj);
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ServerNotification:
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::ServerRequest:
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::SubscribeResource:
                    mReadResourceWidget->setSubscriptionResult(obj, true);
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::UnsubscribeResource:
                    mReadResourceWidget->setSubscriptionResult(obj, false);
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Complete:
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::SetLoggingLevel:
                    break;
                case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType::Unknown:
                    qCWarning(KMCPCLIENTINSPECTOR_WIDGET_LOG) << "It's a bug. MethodType::Unknown must not used.";
                    break;
                }
            });
}

KMcpClientInspectorActionTabWidget::~KMcpClientInspectorActionTabWidget() = default;

#include "moc_kmcpclientinspectoractiontabwidget.cpp"
