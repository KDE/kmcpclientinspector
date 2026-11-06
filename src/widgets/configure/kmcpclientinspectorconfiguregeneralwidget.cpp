/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "kmcpclientinspectorconfiguregeneralwidget.h"
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorConfigureGeneralWidget::KMcpClientInspectorConfigureGeneralWidget(QWidget *parent)
    : KMcpClientInspectorConfigureBaseWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
}

KMcpClientInspectorConfigureGeneralWidget::~KMcpClientInspectorConfigureGeneralWidget() = default;

void KMcpClientInspectorConfigureGeneralWidget::save()
{
}

void KMcpClientInspectorConfigureGeneralWidget::revert()
{
}

void KMcpClientInspectorConfigureGeneralWidget::load()
{
}

void KMcpClientInspectorConfigureGeneralWidget::restoreDefaults()
{
}

#include "moc_kmcpclientinspectorconfiguregeneralwidget.cpp"
