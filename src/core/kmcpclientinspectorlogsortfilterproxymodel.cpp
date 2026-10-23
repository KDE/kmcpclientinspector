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

bool KMcpClientInspectorLogSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mFilterString.trimmed().isEmpty()) {
        return true;
    }
    const QModelIndex sourceIndex = sourceModel()->index(source_row, KMcpClientInspectorLogModel::MethodType, source_parent);
    const QString methodType = sourceIndex.data().toString();
    if (contains(methodType)) {
        return true;
    }
    return false;
}

bool KMcpClientInspectorLogSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (!sourceModel()) {
        return false;
    }
    const int leftColumn{left.column()};
    if (leftColumn == KMcpClientInspectorLogModel::Duration) {
        const QModelIndex leftDurationModelIndex = sourceModel()->index(left.row(), KMcpClientInspectorLogModel::DurationValue, left.parent());
        const QModelIndex rightDurationModelIndex = sourceModel()->index(right.row(), KMcpClientInspectorLogModel::DurationValue, right.parent());
        return leftDurationModelIndex.data().toLongLong() < rightDurationModelIndex.data().toLongLong();
    }
    return TextAddonsWidgets::SortFilterProxyModelBase::lessThan(left, right);
}
#include "moc_kmcpclientinspectorlogsortfilterproxymodel.cpp"
