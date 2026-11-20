/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include <KActionMenu>

#include "libkmcpclientinspectorwidgets_private_export.h"
class QActionGroup;
class KActionCollection;
class KMcpClientInspectorManager;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT McpServerMenu : public KActionMenu
{
    Q_OBJECT
public:
    explicit McpServerMenu(KMcpClientInspectorManager *manager, QWidget *parent = nullptr);
    ~McpServerMenu() override;

    void setActionCollection(KActionCollection *ac);

    void slotUpdateServerMenu();

    void setCurrentServer(int index);

Q_SIGNALS:
    void serverSelected(int index);

private:
    KActionCollection *mActionCollection = nullptr;
    QActionGroup *const mActionGroup;
    KMcpClientInspectorManager *const mManager;
    int mCurrentServer = 0;
};
