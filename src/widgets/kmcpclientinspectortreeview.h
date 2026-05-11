/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QTreeView>
class KMcpClientInspectorManager;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorTreeView(KMcpClientInspectorManager *manager, QWidget *parent = nullptr);
    ~KMcpClientInspectorTreeView() override;

private:
    KMcpClientInspectorManager *const mManager;
};
