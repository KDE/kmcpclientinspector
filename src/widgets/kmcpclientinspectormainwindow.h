/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "libkmcpclientinspectorwidgets_export.h"
#include <KXmlGuiWindow>
class KToggleFullScreenAction;
class KMcpClientInspectorCentralWidget;
class LIBKMCPCLIENTINSPECTORWIDGETS_EXPORT KMcpClientInspectorMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorMainWindow(QWidget *parent = nullptr);
    ~KMcpClientInspectorMainWindow() override;

private:
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void setupActions();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotFullScreen(bool t);
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotConfigureNotifications();
    KMcpClientInspectorCentralWidget *const mMainWidget;
    KToggleFullScreenAction *mShowFullScreenAction = nullptr;
};
