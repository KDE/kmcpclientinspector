/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "libkmcpclientinspectorwidgets_export.h"
#include <KAboutData>
#include <KXmlGuiWindow>
class KToggleFullScreenAction;
class KMcpClientInspectorCentralWidget;
class KMcpClientInspectorManager;
class LIBKMCPCLIENTINSPECTORWIDGETS_EXPORT KMcpClientInspectorMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorMainWindow(const QList<KAboutRelease> &releases, QWidget *parent = nullptr);
    ~KMcpClientInspectorMainWindow() override;

private:
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void setupActions();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotFullScreen(bool t);
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotConfigureNotifications();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotConfigure();
    KMcpClientInspectorManager *const mManager;
    KMcpClientInspectorCentralWidget *const mMainWidget;
    KToggleFullScreenAction *mShowFullScreenAction = nullptr;
};
