/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorserverlogmodel.h"
#include <KLocalizedString>
#include <QBrush>
#include <QColor>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLocale>

using namespace Qt::Literals::StringLiterals;

KMcpClientInspectorServerLogModel::KMcpClientInspectorServerLogModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

KMcpClientInspectorServerLogModel::~KMcpClientInspectorServerLogModel() = default;

void KMcpClientInspectorServerLogModel::addLoggingMessage(const QDateTime &dateTime,
                                                          const TextAutoGenerateTextMcpProtocolCore::McpProtocolLoggingMessageNotificationParams &params)
{
    const int rowCount = mServerLogInfos.count();
    beginInsertRows({}, rowCount, rowCount);
    mServerLogInfos.append({dateTime, params});
    endInsertRows();
}

void KMcpClientInspectorServerLogModel::clear()
{
    beginResetModel();
    mServerLogInfos.clear();
    endResetModel();
}

int KMcpClientInspectorServerLogModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mServerLogInfos.count();
}

int KMcpClientInspectorServerLogModel::columnCount([[maybe_unused]] const QModelIndex &parent) const
{
    constexpr int val = static_cast<int>(ServerLogColumns::LastColumn) + 1;
    return val;
}

QString KMcpClientInspectorServerLogModel::messageText(const QJsonValue &data)
{
    if (data.isString()) {
        return data.toString();
    }
    if (data.isObject()) {
        return QString::fromUtf8(QJsonDocument(data.toObject()).toJson(QJsonDocument::Compact));
    }
    if (data.isArray()) {
        return QString::fromUtf8(QJsonDocument(data.toArray()).toJson(QJsonDocument::Compact));
    }
    return data.toVariant().toString();
}

QVariant KMcpClientInspectorServerLogModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mServerLogInfos.count()) {
        return {};
    }

    const auto &log = mServerLogInfos.at(index.row());
    const auto level = log.params.level();
    if (role == LoggingLevelRole) {
        return static_cast<int>(level);
    }
    if (role == Qt::ForegroundRole) {
        switch (level) {
        case TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel::Warning:
            return QBrush(QColor(0xf6, 0x74, 0x00));
        case TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel::Error:
        case TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel::Critical:
        case TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel::Alert:
        case TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel::Emergency:
            return QBrush(QColor(0xda, 0x44, 0x53));
        case TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel::Debug:
        case TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel::Info:
        case TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel::Notice:
        case TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::LoggingLevel::Unknown:
            break;
        }
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    switch (static_cast<ServerLogColumns>(index.column())) {
    case ServerLogColumns::DateTime:
        return QLocale().toString(log.dateTime.time(), u"hh:mm:ss.zzz"_s);
    case ServerLogColumns::Level:
        return TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::convertLoggingLevelToString(level);
    case ServerLogColumns::Logger:
        return log.params.logger().value_or(QString());
    case ServerLogColumns::Message:
        return messageText(log.params.data());
    case ServerLogColumns::Json:
        return TextAutoGenerateTextMcpProtocolCore::McpProtocolLoggingMessageNotificationParams::toJson(log.params);
    }
    return {};
}

QVariant KMcpClientInspectorServerLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<ServerLogColumns>(section)) {
        case ServerLogColumns::DateTime:
            return i18n("Time");
        case ServerLogColumns::Level:
            return i18n("Level");
        case ServerLogColumns::Logger:
            return i18n("Logger");
        case ServerLogColumns::Message:
            return i18n("Message");
        case ServerLogColumns::Json:
            break;
        }
    }
    return {};
}

#include "moc_kmcpclientinspectorserverlogmodel.cpp"
