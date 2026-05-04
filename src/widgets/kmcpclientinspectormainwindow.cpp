/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectormainwindow.h"
#include "kmcpclientinspectorcentralwidget.h"
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorMainWindow::KMcpClientInspectorMainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
    , mMainWidget(new KMcpClientInspectorCentralWidget(this))
{
    mMainWidget->setObjectName(u"mMainWidget"_s);
    setCentralWidget(mMainWidget);
    setupActions();
    setupGUI();
}

KMcpClientInspectorMainWindow::~KMcpClientInspectorMainWindow() = default;

void KMcpClientInspectorMainWindow::setupActions()
{
    KActionCollection *ac = actionCollection();
    // TODO
}

#include "moc_kmcpclientinspectormainwindow.cpp"
