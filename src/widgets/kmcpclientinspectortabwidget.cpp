/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectortabwidget.h"
#include "kmcpclientinspectormanager.h"

KMcpClientInspectorTabWidget::KMcpClientInspectorTabWidget(KMcpClientInspectorManager *manager, QWidget *parent)
    : QTabWidget(parent)
    , mManager(manager)
{
    loadServers();
}

KMcpClientInspectorTabWidget::~KMcpClientInspectorTabWidget() = default;

void KMcpClientInspectorTabWidget::loadServers()
{
    // TODO
}

#include "moc_kmcpclientinspectortabwidget.cpp"
