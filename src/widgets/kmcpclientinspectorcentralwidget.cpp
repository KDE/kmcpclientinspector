/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorcentralwidget.h"
#include "kmcpclientinspectortabwidget.h"
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorCentralWidget::KMcpClientInspectorCentralWidget(QWidget *parent)
    : QWidget{parent}
    , mTabWidget(new KMcpClientInspectorTabWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mainLayout->addWidget(mTabWidget);
}

KMcpClientInspectorCentralWidget::~KMcpClientInspectorCentralWidget() = default;
#include "moc_kmcpclientinspectorcentralwidget.cpp"
