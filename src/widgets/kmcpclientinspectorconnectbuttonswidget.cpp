/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorconnectbuttonswidget.h"
#include <QHBoxLayout>
#include <QPushButton>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorConnectButtonsWidget::KMcpClientInspectorConnectButtonsWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
}

KMcpClientInspectorConnectButtonsWidget::~KMcpClientInspectorConnectButtonsWidget() = default;

#include "moc_kmcpclientinspectorconnectbuttonswidget.cpp"
