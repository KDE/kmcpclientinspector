/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlogmodel.h"

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
    const auto &log = mLogInfos[index.row()];
    switch (role) {
    case Identifier:
    case Qt::DisplayRole:
        return log.identifier;
    default:
        break;
    }
    return {};
}

#include "moc_kmcpclientinspectorlogmodel.cpp"
