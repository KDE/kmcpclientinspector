/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorcentralwidget.h"
#include "kmcpclientinspectormanager.h"
#include "kmcpclientinspectortabwidget.h"
#include <QSplitter>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolWidgets/McpServerWidget>

using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorCentralWidget::KMcpClientInspectorCentralWidget(KMcpClientInspectorManager *manager, QWidget *parent)
    : QWidget{parent}
    , mTabWidget(new KMcpClientInspectorTabWidget(manager, this))
    , mMcpServerWidget(new TextAutoGenerateTextMcpProtocolWidgets::McpServerWidget(manager->mcpServerModel(), this))
    , mManager(manager)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    auto splitter = new QSplitter(this);
    splitter->setObjectName(u"splitter"_s);
    splitter->setChildrenCollapsible(false);
    mainLayout->addWidget(splitter);

    mMcpServerWidget->setObjectName(u"mMcpServerWidget"_s);
    mTabWidget->setObjectName(u"mTabWidget"_s);

    splitter->addWidget(mMcpServerWidget);
    splitter->addWidget(mTabWidget);
}

KMcpClientInspectorCentralWidget::~KMcpClientInspectorCentralWidget() = default;
#include "moc_kmcpclientinspectorcentralwidget.cpp"
