/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorlogsortfilterproxymodel.h"

KMcpClientInspectorLogSortFilterProxyModel::KMcpClientInspectorLogSortFilterProxyModel(QObject *parent)
    : TextAddonsWidgets::SortFilterProxyModelBase{parent}
{
    setSortCaseSensitivity(Qt::CaseInsensitive);
}

KMcpClientInspectorLogSortFilterProxyModel::~KMcpClientInspectorLogSortFilterProxyModel() = default;

bool KMcpClientInspectorLogSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (mDirectionType != KMcpClientInspectorLogModel::DirectionType::All) {
        const QModelIndex sourceIndex = sourceModel()->index(source_row, KMcpClientInspectorLogModel::LogColumns::DirectionValue, source_parent);
        if (mDirectionType != sourceIndex.data().value<KMcpClientInspectorLogModel::DirectionType>()) {
            return false;
        }
    }
    if (mFilterString.trimmed().isEmpty()) {
        return true;
    }
    const QModelIndex sourceIndex = sourceModel()->index(source_row, KMcpClientInspectorLogModel::LogColumns::MethodType, source_parent);
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
    if (leftColumn == KMcpClientInspectorLogModel::LogColumns::Duration) {
        const QModelIndex leftDurationModelIndex = sourceModel()->index(left.row(), KMcpClientInspectorLogModel::LogColumns::DurationValue, left.parent());
        const QModelIndex rightDurationModelIndex = sourceModel()->index(right.row(), KMcpClientInspectorLogModel::LogColumns::DurationValue, right.parent());
        return leftDurationModelIndex.data().toLongLong() < rightDurationModelIndex.data().toLongLong();
    }
    return TextAddonsWidgets::SortFilterProxyModelBase::lessThan(left, right);
}

KMcpClientInspectorLogModel::DirectionType KMcpClientInspectorLogSortFilterProxyModel::directionType() const
{
    return mDirectionType;
}

void KMcpClientInspectorLogSortFilterProxyModel::setDirectionType(KMcpClientInspectorLogModel::DirectionType newDirectionType)
{
    if (mDirectionType != newDirectionType) {
        beginFilterChange();
        mDirectionType = newDirectionType;
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
    }
}
#include "moc_kmcpclientinspectorlogsortfilterproxymodel.cpp"
