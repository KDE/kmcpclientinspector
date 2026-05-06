/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QTabWidget>

class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorTabWidget(QWidget *parent = nullptr);
    ~KMcpClientInspectorTabWidget() override;
};
