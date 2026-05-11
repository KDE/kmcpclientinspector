/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectortreeview.h"

KMcpClientInspectorTreeView::KMcpClientInspectorTreeView(KMcpClientInspectorManager *manager, QWidget *parent)
    : QTreeView(parent)
    , mManager(manager)
{
}

KMcpClientInspectorTreeView::~KMcpClientInspectorTreeView() = default;

#include "moc_kmcpclientinspectortreeview.cpp"
