/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorlogsortfilterproxymodel.h"
#include "kmcpclientinspectorlogmodel.h"

KMcpClientInspectorLogSortFilterProxyModel::KMcpClientInspectorLogSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
    setSortCaseSensitivity(Qt::CaseInsensitive);
}

KMcpClientInspectorLogSortFilterProxyModel::~KMcpClientInspectorLogSortFilterProxyModel() = default;

bool KMcpClientInspectorLogSortFilterProxyModel::filterAcceptsRow([[maybe_unused]] int source_row, [[maybe_unused]] const QModelIndex &source_parent) const
{
    // TODO
    return true;
}
#include "moc_kmcpclientinspectorlogsortfilterproxymodel.cpp"
