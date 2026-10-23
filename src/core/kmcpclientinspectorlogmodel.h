/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorcore_export.h"
#include <QAbstractListModel>
#include <QList>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>

class LIBKMCPCLIENTINSPECTORCORE_EXPORT KMcpClientInspectorLogModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum LogColumns {
        Identifier,
        Direction,
        MethodType,
        Duration,
        DateTime,
        DurationValue,
        Json,
        LastColumn = Json,
    };
    Q_ENUM(LogColumns)

    explicit KMcpClientInspectorLogModel(QObject *parent = nullptr);
    ~KMcpClientInspectorLogModel() override;

    void addLogInfo(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageInfo info);

    [[nodiscard]] int rowCount(const QModelIndex & = {}) const override;
    void clear();

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageInfo> mLogInfos;
};
