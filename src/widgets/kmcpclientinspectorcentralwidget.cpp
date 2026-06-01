/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorcentralwidget.h"
#include "kmcpclientinspectorglobalconfig.h"
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
        const QString previousNewFeaturesMD5 = KMcpClientInspectorGlobalConfig::self()->previousNewFeaturesMD5();
        if (!previousNewFeaturesMD5.isEmpty()) {
            const bool hasNewFeature = (previousNewFeaturesMD5 != newFeaturesMD5);
            if (hasNewFeature) {
                auto whatsNewMessageWidget = new TextAddonsWidgets::WhatsNewMessageNgWidget(this);
                whatsNewMessageWidget->setReleases(mReleases);
                whatsNewMessageWidget->setObjectName(u"whatsNewMessageWidget"_s);
                mainLayout->addWidget(whatsNewMessageWidget);
                KMcpClientInspectorGlobalConfig::self()->setPreviousNewFeaturesMD5(newFeaturesMD5);
                KMcpClientInspectorGlobalConfig::self()->save();
                whatsNewMessageWidget->animatedShow();
            }
        } else {
            KMcpClientInspectorGlobalConfig::self()->setPreviousNewFeaturesMD5(newFeaturesMD5);
            KMcpClientInspectorGlobalConfig::self()->save();
        }
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

QList<KAboutRelease> KMcpClientInspectorCentralWidget::releases() const
{
    return mReleases;
}

#include "moc_kmcpclientinspectorcentralwidget.cpp"
