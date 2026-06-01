/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorcentralwidget.h"
#include "kmcpclientinspectormanager.h"
#include "kmcpclientinspectortabwidget.h"
#include <QSplitter>
#include <QVBoxLayout>
#include <TextAddonsWidgets/WhatsNewMessageNgWidget>
#include <TextAddonsWidgets/WhatsNewNgUtils>
#include <TextAutoGenerateTextMcpProtocolWidgets/McpServerWidget>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorCentralWidget::KMcpClientInspectorCentralWidget(const QList<KAboutRelease> &releases, KMcpClientInspectorManager *manager, QWidget *parent)
    : QWidget{parent}
    , mTabWidget(new KMcpClientInspectorTabWidget(manager, this))
    , mMcpServerWidget(new TextAutoGenerateTextMcpProtocolWidgets::McpServerWidget(manager->mcpServerModel(), this))
    , mManager(manager)
    , mReleases(releases)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    QString newFeaturesMD5;
    if (!mReleases.isEmpty()) {
        newFeaturesMD5 = TextAddonsWidgets::WhatsNewNgUtils::createMD5(mReleases.constFirst().untranslatedDescription());
    }
    if (!newFeaturesMD5.isEmpty()) {
#if 0 // TODO
        const QString previousNewFeaturesMD5 = KAIChatGlobalConfig::self()->previousNewFeaturesMD5();
        if (!previousNewFeaturesMD5.isEmpty()) {
            const bool hasNewFeature = (previousNewFeaturesMD5 != newFeaturesMD5);
            if (hasNewFeature) {
                auto whatsNewMessageWidget = new TextAddonsWidgets::WhatsNewMessageNgWidget(this);
                whatsNewMessageWidget->setReleases(mReleases);
                whatsNewMessageWidget->setObjectName(u"whatsNewMessageWidget"_s);
                mainLayout->addWidget(whatsNewMessageWidget);
                KAIChatGlobalConfig::self()->setPreviousNewFeaturesMD5(newFeaturesMD5);
                whatsNewMessageWidget->animatedShow();
            }
        } else {
            KAIChatGlobalConfig::self()->setPreviousNewFeaturesMD5(newFeaturesMD5);
        }
#endif
    }

    auto splitter = new QSplitter(this);
    splitter->setObjectName(u"splitter"_s);
    splitter->setChildrenCollapsible(false);
    mainLayout->addWidget(splitter);

    mMcpServerWidget->setObjectName(u"mMcpServerWidget"_s);
    mTabWidget->setObjectName(u"mTabWidget"_s);

    splitter->addWidget(mMcpServerWidget);
    splitter->addWidget(mTabWidget);
}

KMcpClientInspectorCentralWidget::~KMcpClientInspectorCentralWidget() = default;
#include "moc_kmcpclientinspectorcentralwidget.cpp"
