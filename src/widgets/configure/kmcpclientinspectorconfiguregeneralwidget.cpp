/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "kmcpclientinspectorconfiguregeneralwidget.h"
#include "common/kmcpclientinspectorcapabilitywidget.h"
#include "kmcpclientinspectorglobalconfig.h"
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorConfigureGeneralWidget::KMcpClientInspectorConfigureGeneralWidget(QWidget *parent)
    : KMcpClientInspectorConfigureBaseWidget{parent}
    , mCapabilityWidget(new KMcpClientInspectorCapabilityWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mCapabilityWidget->setObjectName(u"mCapabilityWidget"_s);
    mainLayout->addWidget(mCapabilityWidget);
    mainLayout->addStretch(1);
}

KMcpClientInspectorConfigureGeneralWidget::~KMcpClientInspectorConfigureGeneralWidget() = default;

void KMcpClientInspectorConfigureGeneralWidget::save()
{
    KMcpClientInspectorGlobalConfig::self()->setElicitationSupport(mCapabilityWidget->elicitationSupport());
    KMcpClientInspectorGlobalConfig::self()->setUrlElicitationSupport(mCapabilityWidget->urlElicitationSupport());
    KMcpClientInspectorGlobalConfig::self()->setRootsSupport(mCapabilityWidget->rootsSupport());
    KMcpClientInspectorGlobalConfig::self()->setSamplingSupport(mCapabilityWidget->samplingSupport());
}

void KMcpClientInspectorConfigureGeneralWidget::revert()
{
    // TODO
}

void KMcpClientInspectorConfigureGeneralWidget::load()
{
    mCapabilityWidget->setElicitationSupport(KMcpClientInspectorGlobalConfig::self()->elicitationSupport());
    mCapabilityWidget->setUrlElicitationSupport(KMcpClientInspectorGlobalConfig::self()->urlElicitationSupport());
    mCapabilityWidget->setRootsSupport(KMcpClientInspectorGlobalConfig::self()->rootsSupport());
    mCapabilityWidget->setSamplingSupport(KMcpClientInspectorGlobalConfig::self()->samplingSupport());
}

void KMcpClientInspectorConfigureGeneralWidget::restoreDefaults()
{
    // TODO
}

#include "moc_kmcpclientinspectorconfiguregeneralwidget.cpp"
