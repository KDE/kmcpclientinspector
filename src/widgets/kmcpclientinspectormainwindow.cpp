/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectormainwindow.h"

KMcpClientInspectorMainWindow::KMcpClientInspectorMainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
{
    setupActions();
    setupGUI();
}

KMcpClientInspectorMainWindow::~KMcpClientInspectorMainWindow() = default;

void KMcpClientInspectorMainWindow::setupActions()
{
}

#include "moc_kmcpclientinspectormainwindow.cpp"
