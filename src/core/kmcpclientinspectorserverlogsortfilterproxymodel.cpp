/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorserverlogsortfilterproxymodel.h"
#include "kmcpclientinspectorserverlogmodel.h"

KMcpClientInspectorServerLogSortFilterProxyModel::KMcpClientInspectorServerLogSortFilterProxyModel(QObject *parent)
    : TextAddonsWidgets::SortFilterProxyModelBase{parent}
{
    setSortCaseSensitivity(Qt::CaseInsensitive);
}

KMcpClientInspectorServerLogSortFilterProxyModel::~KMcpClientInspectorServerLogSortFilterProxyModel() = default;

TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel KMcpClientInspectorServerLogSortFilterProxyModel::minimumLevel() const
{
    return mMinimumLevel;
}

void KMcpClientInspectorServerLogSortFilterProxyModel::setMinimumLevel(TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel level)
{
    if (mMinimumLevel != level) {
        beginFilterChange();
        mMinimumLevel = level;
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
    }
}

bool KMcpClientInspectorServerLogSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const QModelIndex levelIndex = sourceModel()->index(source_row, KMcpClientInspectorServerLogModel::Level, source_parent);
    const auto level = static_cast<TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel>(
        levelIndex.data(KMcpClientInspectorServerLogModel::LoggingLevelRole).toInt());
    // Unknown is the last enum value: never hide it behind the level filter
    if (level != TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel::Unknown && level < mMinimumLevel) {
        return false;
    }
    if (mFilterString.trimmed().isEmpty()) {
        return true;
    }
    const QString logger = sourceModel()->index(source_row, KMcpClientInspectorServerLogModel::Logger, source_parent).data().toString();
    const QString message = sourceModel()->index(source_row, KMcpClientInspectorServerLogModel::Message, source_parent).data().toString();
    return contains(logger) || contains(message);
}
#include "moc_kmcpclientinspectorserverlogsortfilterproxymodel.cpp"
