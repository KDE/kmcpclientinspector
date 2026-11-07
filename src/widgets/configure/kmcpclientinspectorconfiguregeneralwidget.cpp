/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "kmcpclientinspectorconfiguregeneralwidget.h"
#include "common/kmcpclientinspectorcapabilitywidget.h"
#include "dialogs/kmcpclientinspectorrootsdialog.h"
#include "kmcpclientinspectorglobalconfig.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLabel>
#include <QPointer>
#include <QSpinBox>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorConfigureGeneralWidget::KMcpClientInspectorConfigureGeneralWidget(QWidget *parent)
    : KMcpClientInspectorConfigureBaseWidget{parent}
    , mCapabilityWidget(new KMcpClientInspectorCapabilityWidget(this))
    , mRequestTimeout(new QSpinBox(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mCapabilityWidget->setObjectName(u"mCapabilityWidget"_s);
    mainLayout->addWidget(mCapabilityWidget);

    auto hboxLayout = new QHBoxLayout;
    mainLayout->addLayout(hboxLayout);

    auto label = new QLabel(i18n("Request Timeout:"), this);
    label->setObjectName(u"label"_s);
    mRequestTimeout->setObjectName(u"mRequestTimeout"_s);
    label->setBuddy(mRequestTimeout);

    hboxLayout->addWidget(label);
    hboxLayout->addWidget(mRequestTimeout);
    hboxLayout->addStretch(1);
    mRequestTimeout->setRange(1, 3600);

    mainLayout->addStretch(1);
    connect(mCapabilityWidget,
            &KMcpClientInspectorCapabilityWidget::configureRootsRequested,
            this,
            &KMcpClientInspectorConfigureGeneralWidget::slotConfigureRoots);
}

KMcpClientInspectorConfigureGeneralWidget::~KMcpClientInspectorConfigureGeneralWidget() = default;

void KMcpClientInspectorConfigureGeneralWidget::slotConfigureRoots()
{
    QPointer<KMcpClientInspectorRootsDialog> dlg = new KMcpClientInspectorRootsDialog(this);
    dlg->setRoots(mRoots);
    if (dlg->exec()) {
        mRoots = dlg->roots();
    }
    delete dlg;
}

void KMcpClientInspectorConfigureGeneralWidget::save()
{
    KMcpClientInspectorGlobalConfig::self()->setElicitationSupport(mCapabilityWidget->elicitationSupport());
    KMcpClientInspectorGlobalConfig::self()->setUrlElicitationSupport(mCapabilityWidget->urlElicitationSupport());
    KMcpClientInspectorGlobalConfig::self()->setRootsSupport(mCapabilityWidget->rootsSupport());
    KMcpClientInspectorGlobalConfig::self()->setSamplingSupport(mCapabilityWidget->samplingSupport());
    KMcpClientInspectorGlobalConfig::self()->setRoots(mRoots);
    KMcpClientInspectorGlobalConfig::self()->setRequestTimeoutInSeconds(mRequestTimeout->value());
    KMcpClientInspectorGlobalConfig::self()->save();
}

void KMcpClientInspectorConfigureGeneralWidget::revert()
{
    // TODO
}

void KMcpClientInspectorConfigureGeneralWidget::load()
{
    mRoots = KMcpClientInspectorGlobalConfig::self()->roots();
    mCapabilityWidget->setElicitationSupport(KMcpClientInspectorGlobalConfig::self()->elicitationSupport());
    mCapabilityWidget->setUrlElicitationSupport(KMcpClientInspectorGlobalConfig::self()->urlElicitationSupport());
    mCapabilityWidget->setRootsSupport(KMcpClientInspectorGlobalConfig::self()->rootsSupport());
    mCapabilityWidget->setSamplingSupport(KMcpClientInspectorGlobalConfig::self()->samplingSupport());
    mRequestTimeout->setValue(KMcpClientInspectorGlobalConfig::self()->requestTimeoutInSeconds());
}

void KMcpClientInspectorConfigureGeneralWidget::restoreDefaults()
{
    // TODO
}

#include "moc_kmcpclientinspectorconfiguregeneralwidget.cpp"
