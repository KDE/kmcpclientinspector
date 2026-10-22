/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorcapabilitywidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorCapabilityWidget::KMcpClientInspectorCapabilityWidget(QWidget *parent)
    : QWidget{parent}
    , mElicitationSupported(new QCheckBox(i18nc("@option:check", "Elicitation"), this))
    , mUrlElicitationSupported(new QCheckBox(i18nc("@option:check", "URL elicitation"), this))
    , mRootsSupported(new QCheckBox(i18nc("@option:check", "Roots"), this))
    , mSamplingSupported(new QCheckBox(i18nc("@option:check", "Sampling"), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto groupBox = new QGroupBox(i18n("Capabilities"), this);
    groupBox->setObjectName(u"groupBox"_s);
    mainLayout->addWidget(groupBox);

    auto vbox = new QVBoxLayout;
    groupBox->setLayout(vbox);

    mElicitationSupported->setObjectName(u"mElicitationSupported"_s);
    mElicitationSupported->setChecked(true);
    vbox->addWidget(mElicitationSupported);

    mUrlElicitationSupported->setObjectName(u"mUrlElicitationSupported"_s);
    mUrlElicitationSupported->setChecked(true);
    vbox->addWidget(mUrlElicitationSupported);

    mRootsSupported->setObjectName(u"mRootsSupported"_s);
    mRootsSupported->setChecked(true);
    vbox->addWidget(mRootsSupported);

    mSamplingSupported->setObjectName(u"mSamplingSupported"_s);
    mSamplingSupported->setChecked(true);
    vbox->addWidget(mSamplingSupported);
}

KMcpClientInspectorCapabilityWidget::~KMcpClientInspectorCapabilityWidget() = default;

bool KMcpClientInspectorCapabilityWidget::elicitationSupport() const
{
    return mElicitationSupported->isChecked();
}

bool KMcpClientInspectorCapabilityWidget::urlElicitationSupport() const
{
    return mUrlElicitationSupported->isChecked();
}

bool KMcpClientInspectorCapabilityWidget::rootsSupport() const
{
    return mRootsSupported->isChecked();
}

bool KMcpClientInspectorCapabilityWidget::samplingSupport() const
{
    return mSamplingSupported->isChecked();
}

#include "moc_kmcpclientinspectorcapabilitywidget.cpp"
