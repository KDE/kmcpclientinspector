/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorserversettingswidget.h"
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorServerSettingsWidget::KMcpClientInspectorServerSettingsWidget(QWidget *parent)
    : QWidget{parent}
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});
}

KMcpClientInspectorServerSettingsWidget::~KMcpClientInspectorServerSettingsWidget() = default;

#include "moc_kmcpclientinspectorserversettingswidget.cpp"
