/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QWidget>
class KMcpClientInspectorTabWidget;
class KMcpClientInspectorManager;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorCentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorCentralWidget(QWidget *parent = nullptr);
    ~KMcpClientInspectorCentralWidget() override;

private:
    KMcpClientInspectorTabWidget *const mTabWidget;
};
