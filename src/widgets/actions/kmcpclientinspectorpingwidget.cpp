/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorpingwidget.h"
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorPingWidget::KMcpClientInspectorPingWidget(QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
    // TODO
}

KMcpClientInspectorPingWidget::~KMcpClientInspectorPingWidget() = default;

#include "moc_kmcpclientinspectorpingwidget.cpp"
