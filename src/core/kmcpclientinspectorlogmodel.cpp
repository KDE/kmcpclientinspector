/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlogmodel.h"
#include <KLocalizedString>

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
    const int col = index.column();
    switch (col) {
    case LogRoles::Identifier:
        return log.identifier;
    case LogRoles::MethodType:
    case LogRoles::DateTime:
        // TODO
    default:
        break;
    }
    return {};
}

QVariant KMcpClientInspectorLogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (static_cast<LogRoles>(section)) {
        case LogRoles::Identifier:
            return i18n("IDentifier");
        case LogRoles::MethodType:
            return i18n("Method Name");
        case LogRoles::DateTime:
            return i18n("Time");
        }
    }
    return {};
}

int KMcpClientInspectorLogModel::columnCount([[maybe_unused]] const QModelIndex &parent) const
{
    constexpr int val = static_cast<int>(LogRoles::LastColumn) + 1;
    return val;
}

#include "moc_kmcpclientinspectorlogmodel.cpp"
