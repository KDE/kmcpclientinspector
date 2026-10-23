/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include "libkmcpclientinspectorcore_export.h"
#include <TextAddonsWidgets/SortFilterProxyModelBase>
class LIBKMCPCLIENTINSPECTORCORE_EXPORT KMcpClientInspectorLogSortFilterProxyModel : public TextAddonsWidgets::SortFilterProxyModelBase
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorLogSortFilterProxyModel(QObject *parent = nullptr);
    ~KMcpClientInspectorLogSortFilterProxyModel() override;

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    [[nodiscard]] bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};
