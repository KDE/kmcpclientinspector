/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectormainwindow.h"
#include "kmcpclientinspectorcentralwidget.h"
#include "kmcpclientinspectormanager.h"
#include <KActionCollection>
#include <KNotifyConfigWidget>
#include <KStandardAction>
#include <KStandardActions>
#include <QFontDatabase>
#include <QMenuBar>
#include <QToolButton>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorMainWindow::KMcpClientInspectorMainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
    , mMainWidget(new KMcpClientInspectorCentralWidget(this))
    , mManager(new KMcpClientInspectorManager(this))
{
    mMainWidget->setObjectName(u"mMainWidget"_s);
    setCentralWidget(mMainWidget);
    setupActions();
    setupGUI();
    mManager->loadServers();
}

KMcpClientInspectorMainWindow::~KMcpClientInspectorMainWindow() = default;

void KMcpClientInspectorMainWindow::setupActions()
{
    KActionCollection *ac = actionCollection();
    KStandardActions::quit(this, &KMcpClientInspectorMainWindow::close, ac);
    KStandardActions::configureNotifications(this, &KMcpClientInspectorMainWindow::slotConfigureNotifications, ac);

    mShowFullScreenAction = KStandardAction::fullScreen(nullptr, nullptr, this, ac);
    ac->setDefaultShortcut(mShowFullScreenAction, Qt::Key_F11);
    connect(mShowFullScreenAction, &QAction::toggled, this, &KMcpClientInspectorMainWindow::slotFullScreen);
}

void KMcpClientInspectorMainWindow::slotConfigureNotifications()
{
    KNotifyConfigWidget::configure(this);
}

void KMcpClientInspectorMainWindow::slotFullScreen(bool t)
{
    KToggleFullScreenAction::setFullScreen(this, t);
    QMenuBar *mb = menuBar();
    if (t) {
        auto b = new QToolButton(mb);
        b->setDefaultAction(mShowFullScreenAction);
        b->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored));
        b->setFont(QFontDatabase::systemFont(QFontDatabase::SmallestReadableFont));
        mb->setCornerWidget(b, Qt::TopRightCorner);
        b->setVisible(true);
        b->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    } else {
        QWidget *w = mb->cornerWidget(Qt::TopRightCorner);
        if (w) {
            w->deleteLater();
        }
    }
}

#include "moc_kmcpclientinspectormainwindow.cpp"
