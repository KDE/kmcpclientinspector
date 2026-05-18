/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectoractiontabwidget.h"
#include "actions/kmcpclientinspectorpingwidget.h"
#include <KLocalizedString>
#include <QTabWidget>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorActionTabWidget::KMcpClientInspectorActionTabWidget(QWidget *parent)
    : QWidget{parent}
    , mPingWidget(new KMcpClientInspectorPingWidget(this))
    , mTabWidget(new QTabWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mPingWidget->setObjectName(u"mPingWidget"_s);
    mTabWidget->setObjectName(u"mTabWidget"_s);

    mainLayout->addWidget(mTabWidget);
    mTabWidget->addTab(mPingWidget, i18n("Ping"));
}

KMcpClientInspectorActionTabWidget::~KMcpClientInspectorActionTabWidget() = default;

void KMcpClientInspectorActionTabWidget::setClient(TextAutoGenerateTextMcpProtocolCore::McpProtocolClient *newClient)
{
    mPingWidget->setClient(newClient);
    // TODO add more
}

#include "moc_kmcpclientinspectoractiontabwidget.cpp"
