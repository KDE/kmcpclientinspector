/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorcore_export.h"
#include "loginfo.h"
#include <QAbstractListModel>
#include <QList>

class LIBKMCPCLIENTINSPECTORCORE_EXPORT KMcpClientInspectorLogModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorLogModel(QObject *parent = nullptr);
    ~KMcpClientInspectorLogModel() override;

    void addLogInfo(LogInfo info);

    [[nodiscard]] int rowCount(const QModelIndex & = {}) const override;
    void clear();

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

private:
    QList<LogInfo> mLogInfos;
};
