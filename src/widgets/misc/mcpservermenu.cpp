/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "mcpservermenu.h"
#include "kmcpclientinspectormanager.h"

#include <KActionCollection>
#include <KLocalizedString>
#include <QActionGroup>
#include <QMenu>

using namespace Qt::Literals::StringLiterals;
McpServerMenu::McpServerMenu(KMcpClientInspectorManager *manager, QWidget *parent)
    : KActionMenu(parent)
    , mActionGroup(new QActionGroup(this))
    , mManager(manager)
{
    mActionGroup->setExclusive(true);
    setText(i18n("Server"));
    connect(menu(), &QMenu::aboutToShow, this, &McpServerMenu::slotUpdateServerMenu);
}

McpServerMenu::~McpServerMenu() = default;

void McpServerMenu::setActionCollection(KActionCollection *ac)
{
    mActionCollection = ac;
}

void McpServerMenu::slotUpdateServerMenu()
{
    menu()->clear();
    const auto servers = mManager->enabledSortedServers();
    const int serverCount = servers.count();
    if (serverCount == 0) {
        return;
    }
    if (serverCount == 1) {
        auto action = new QAction(servers.constFirst().name(), menu());
        menu()->addAction(action);
        action->setEnabled(false);
    } else {
        for (int i = 0; i < serverCount; ++i) {
            const auto &server = servers.at(i);
            auto action = new QAction(server.name(), menu());
            action->setIcon(server.icon());
            action->setCheckable(true);
            action->setChecked(i == mCurrentServer);
            menu()->addAction(action);
            if (mActionCollection && i < 9) {
                // Start shortcuts at 1 instead of 0 for better ergonomics (0 key is far from 1-9)
                mActionCollection->setDefaultShortcut(action, QKeySequence(u"CTRL+%1"_s.arg(i + 1)));
            }
            connect(action, &QAction::triggered, this, [this, i]() {
                Q_EMIT serverSelected(i);
            });
            mActionGroup->addAction(action);
        }
    }
}

void McpServerMenu::setCurrentServer(int index)
{
    mCurrentServer = index;
}

#include "moc_mcpservermenu.cpp"
