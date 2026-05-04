/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_export.h"
#include <QWidget>

class LIBKMCPCLIENTINSPECTORWIDGETS_EXPORT KMcpClientInspectorCentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorCentralWidget(QWidget *parent = nullptr);
    ~KMcpClientInspectorCentralWidget() override;
};
