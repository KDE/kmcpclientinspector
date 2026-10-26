/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorlogwidget.h"
#include "actions/kmcpclientinspectorlogtreewidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <QSplitter>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorLogWidget::KMcpClientInspectorLogWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                           QWidget *parent)
    : KMcpClientInspectorActionTabPageBase(protocolManager, parent)
    , mSplitter(new QSplitter(this))
    , mJsonPlainTextEditWidget(new JsonPlainTextEditWidget(this))
    , mLogTreeWidget(new KMcpClientInspectorLogTreeWidget(protocolManager, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mSplitter->setObjectName(u"mSplitter"_s);
    mainLayout->addWidget(mSplitter);
    mSplitter->setOrientation(Qt::Vertical);

    mSplitter->setChildrenCollapsible(false);

    mLogTreeWidget->setObjectName(u"mLogTreeWidget"_s);
    mSplitter->addWidget(mLogTreeWidget);

    mJsonPlainTextEditWidget->setObjectName(u"mJsonPlainTextEditWidget"_s);
    mSplitter->addWidget(mJsonPlainTextEditWidget);
    mJsonPlainTextEditWidget->setReadOnly(true);

    connect(mLogTreeWidget, &KMcpClientInspectorLogTreeWidget::jsonSelected, mJsonPlainTextEditWidget, &JsonPlainTextEditWidget::setPlainText);
    connect(mLogTreeWidget, &KMcpClientInspectorLogTreeWidget::clearRequested, mJsonPlainTextEditWidget, &JsonPlainTextEditWidget::clear);
}

KMcpClientInspectorLogWidget::~KMcpClientInspectorLogWidget() = default;
#include "moc_kmcpclientinspectorlogwidget.cpp"
