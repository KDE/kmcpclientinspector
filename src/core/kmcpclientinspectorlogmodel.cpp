/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlogmodel.h"
#include <KLocalizedString>
#include <QLocale>
#include <QMetaEnum>

using namespace Qt::Literals::StringLiterals;

KMcpClientInspectorLogModel::KMcpClientInspectorLogModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

KMcpClientInspectorLogModel::~KMcpClientInspectorLogModel() = default;

int KMcpClientInspectorLogModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0; // flat model
    }
    return mLogInfos.count();
}

void KMcpClientInspectorLogModel::addLogInfo(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageInfo info)
{
    const int rowCount = mLogInfos.count();
    beginInsertRows({}, rowCount, rowCount);
    mLogInfos.append(std::move(info));
    endInsertRows();
}

void KMcpClientInspectorLogModel::clear()
{
    beginResetModel();
    mLogInfos.clear();
    endResetModel();
}

QVariant KMcpClientInspectorLogModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= mLogInfos.count()) {
        return {};
    }
    if (role != Qt::DisplayRole) {
        return {};
    }

    const auto &log = mLogInfos.at(index.row());
    switch (static_cast<LogColumns>(index.column())) {
    case LogColumns::Identifier:
        if (log.identifier < 0) {
            return {};
        }
        return log.identifier;
    case LogColumns::MethodType:
        return QString::fromLatin1(
            QMetaEnum::fromType<TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType>().valueToKey(static_cast<int>(log.type)));
    case LogColumns::DateTime:
        return QLocale().toString(log.dateTime.time(), u"hh:mm:ss.zzz"_s);
    case LogColumns::Direction:
        switch (log.direction) {
        case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageDirection::Sent:
            return i18n("Sent");
        case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageDirection::Received:
            return i18n("Received");
        }
        break;
    case LogColumns::DirectionValue:
        switch (log.direction) {
        case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageDirection::Sent:
            return QVariant::fromValue(DirectionType::Sent);
        case TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageDirection::Received:
            return QVariant::fromValue(DirectionType::Received);
        }
        break;
    case LogColumns::Duration:
        if (log.elapsedMs < 0) {
            return {};
        }
        return i18n("%1 ms", log.elapsedMs);
    case LogColumns::DurationValue:
        if (log.elapsedMs < 0) {
            return 0;
        }
        return log.elapsedMs;
    case LogColumns::Json:
        return log.message;
    }
    return {};
}

QVariant KMcpClientInspectorLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<LogColumns>(section)) {
        case LogColumns::Identifier:
            return i18n("Identifier");
        case LogColumns::MethodType:
            return i18n("Method Name");
        case LogColumns::DateTime:
            return i18n("Time");
        case LogColumns::Duration:
            return i18n("Duration");
        case LogColumns::Direction:
            return i18n("Direction"); // or remove it
        case LogColumns::Json:
        case LogColumns::DurationValue:
        case LogColumns::DirectionValue:
            break;
        }
    }
    return {};
}

int KMcpClientInspectorLogModel::columnCount([[maybe_unused]] const QModelIndex &parent) const
{
    constexpr int val = static_cast<int>(LogColumns::LastColumn) + 1;
    return val;
}

#include "moc_kmcpclientinspectorlogmodel.cpp"
