/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectormainwindow.h"
#include "kmcpclientinspectorcentralwidget.h"
#include <KActionCollection>
#include <KNotifyConfigWidget>
#include <KStandardAction>
#include <KStandardActions>
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
    KStandardActions::quit(this, &KMcpClientInspectorMainWindow::close, ac);
    KStandardActions::configureNotifications(this, &KMcpClientInspectorMainWindow::slotConfigureNotifications, ac);
    // TODO
}

void KMcpClientInspectorMainWindow::slotConfigureNotifications()
{
    KNotifyConfigWidget::configure(this);
}

#include "moc_kmcpclientinspectormainwindow.cpp"
