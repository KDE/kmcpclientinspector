/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include "libkmcpclientinspectorcore_export.h"
#include <TextAddonsWidgets/SortFilterProxyModelBase>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolUtils>
class LIBKMCPCLIENTINSPECTORCORE_EXPORT KMcpClientInspectorServerLogSortFilterProxyModel : public TextAddonsWidgets::SortFilterProxyModelBase
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorServerLogSortFilterProxyModel(QObject *parent = nullptr);
    ~KMcpClientInspectorServerLogSortFilterProxyModel() override;

    [[nodiscard]] TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel minimumLevel() const;
    void setMinimumLevel(TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel level);

protected:
    [[nodiscard]] bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel mMinimumLevel =
        TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel::Debug;
};
