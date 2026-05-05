/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorcentralwidget.h"
#include "kmcpclientinspectortreeview.h"
#include <QPlainTextEdit>
#include <QSplitter>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorCentralWidget::KMcpClientInspectorCentralWidget(QWidget *parent)
    : QWidget{parent}
    , mTreeView(new KMcpClientInspectorTreeView(this))
    , mTextEdit(new QPlainTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    auto splitter = new QSplitter(this);
    splitter->setObjectName("splitter"_L1);
    splitter->setChildrenCollapsible(false);
    mTreeView->setObjectName("mTreeView"_L1);
    splitter->addWidget(mTreeView);

    mainLayout->addWidget(splitter);

    mTextEdit->setObjectName("mTextEdit"_L1);
    mTextEdit->setReadOnly(true);
    splitter->addWidget(mTextEdit);
}

KMcpClientInspectorCentralWidget::~KMcpClientInspectorCentralWidget() = default;
#include "moc_kmcpclientinspectorcentralwidget.cpp"
