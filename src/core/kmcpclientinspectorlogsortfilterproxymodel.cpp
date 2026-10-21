/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorlogsortfilterproxymodel.h"
#include "kmcpclientinspectorlogmodel.h"

KMcpClientInspectorLogSortFilterProxyModel::KMcpClientInspectorLogSortFilterProxyModel(QObject *parent)
    : TextAddonsWidgets::SortFilterProxyModelBase{parent}
{
    setSortCaseSensitivity(Qt::CaseInsensitive);
}

KMcpClientInspectorLogSortFilterProxyModel::~KMcpClientInspectorLogSortFilterProxyModel() = default;

bool KMcpClientInspectorLogSortFilterProxyModel::filterAcceptsRow([[maybe_unused]] int source_row, [[maybe_unused]] const QModelIndex &source_parent) const
{
    if (mFilterString.trimmed().isEmpty()) {
        return true;
    }
    const QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
    const QString methodType = sourceIndex.data(KMcpClientInspectorLogModel::MethodType).toString();
    if (contains(methodType)) {
        return true;
    }
    return false;
}
#include "moc_kmcpclientinspectorlogsortfilterproxymodel.cpp"
