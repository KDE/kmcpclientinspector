/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorcore_export.h"
#include <QAbstractListModel>
#include <QDateTime>
#include <QList>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolLoggingMessageNotificationParams>

class LIBKMCPCLIENTINSPECTORCORE_EXPORT KMcpClientInspectorServerLogModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ServerLogColumns {
        DateTime,
        Level,
        Logger,
        Message,
        Json,
        LastColumn = Json,
    };
    Q_ENUM(ServerLogColumns)

    enum ServerLogRoles {
        LoggingLevelRole = Qt::UserRole + 1,
    };

    explicit KMcpClientInspectorServerLogModel(QObject *parent = nullptr);
    ~KMcpClientInspectorServerLogModel() override;

    void addLoggingMessage(const QDateTime &dateTime, const TextAutoGenerateTextMcpProtocolCore::McpProtocolLoggingMessageNotificationParams &params);
    void clear();

    [[nodiscard]] int rowCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    struct ServerLogInfo {
        QDateTime dateTime;
        TextAutoGenerateTextMcpProtocolCore::McpProtocolLoggingMessageNotificationParams params;
    };
    [[nodiscard]] static QString messageText(const QJsonValue &data);
    QList<ServerLogInfo> mServerLogInfos;
};
