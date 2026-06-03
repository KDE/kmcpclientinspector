/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectoractiontabwidget.h"
#include "actions/kmcpclientinspectorlistpromptswidget.h"
#include "actions/kmcpclientinspectorlisttoolswidget.h"
#include "actions/kmcpclientinspectorpingwidget.h"
#include "actions/kmcpclientinspectorresourcetemplateswidget.h"
#include "kmcpclientinspector_widget_debug.h"
#include "kmcpclientinspectorclientprotocolmanager.h"
#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorActionTabWidget::KMcpClientInspectorActionTabWidget(KMcpClientInspectorClientProtocolManager *protocolManager, QWidget *parent)
    : QWidget{parent}
    , mPingWidget(new KMcpClientInspectorPingWidget(protocolManager, this))
    , mListToolsWidget(new KMcpClientInspectorListToolsWidget(protocolManager, this))
    , mListPromptsWidget(new KMcpClientInspectorListPromptsWidget(protocolManager, this))
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
    mResourceTemplatesWidget->setObjectName(u"mResourceTemplatesWidget"_s);

    mainLayout->addWidget(mTabWidget);
    mTabWidget->addTab(mPingWidget, i18n("Ping"));
    mTabWidget->addTab(mListToolsWidget, i18n("List Tools"));
    mTabWidget->addTab(mListPromptsWidget, i18n("List Prompts"));
    mTabWidget->addTab(mResourceTemplatesWidget, i18n("Resource Templates"));
    connect(protocolManager,
            &KMcpClientInspectorClientProtocolManager::received,
            this,
            [this](const QJsonObject &obj, KMcpClientInspectorClientProtocolManager::MethodType type) {
                switch (type) {
                case KMcpClientInspectorClientProtocolManager::MethodType::Ping:
                    break;
                case KMcpClientInspectorClientProtocolManager::MethodType::ListTools:
                    mListToolsWidget->setResult(obj);
                    break;
                case KMcpClientInspectorClientProtocolManager::MethodType::ListPrompts:
                    mListPromptsWidget->setResult(obj);
                    break;
                case KMcpClientInspectorClientProtocolManager::MethodType::ResourceTemplates:
                    mResourceTemplatesWidget->setResult(obj);
                    break;
                case KMcpClientInspectorClientProtocolManager::MethodType::Unknown:
                    qCWarning(KMCPCLIENTINSPECTOR_WIDGET_LOG) << "IT's a bug. MethodType::Unknown must not used.";
                    break;
                }
            });
}

KMcpClientInspectorActionTabWidget::~KMcpClientInspectorActionTabWidget() = default;

#include "moc_kmcpclientinspectoractiontabwidget.cpp"
