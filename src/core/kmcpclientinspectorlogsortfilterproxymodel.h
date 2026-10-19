/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include <QSortFilterProxyModel>

class KMcpClientInspectorLogSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorLogSortFilterProxyModel(QObject *parent = nullptr);
    ~KMcpClientInspectorLogSortFilterProxyModel() override;
};
