/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorlogtreewidget.h"
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;

KMcpClientInspectorLogTreeWidget::KMcpClientInspectorLogTreeWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
}

KMcpClientInspectorLogTreeWidget::~KMcpClientInspectorLogTreeWidget() = default;
#include "moc_kmcpclientinspectorlogtreewidget.cpp"
