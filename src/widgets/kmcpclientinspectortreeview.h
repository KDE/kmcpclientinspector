/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <QTreeView>

class KMcpClientInspectorTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorTreeView(QWidget *parent = nullptr);
    ~KMcpClientInspectorTreeView() override;
};
