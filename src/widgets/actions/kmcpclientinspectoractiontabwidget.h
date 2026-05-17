/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QWidget>

class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorActionTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorActionTabWidget(QWidget *parent = nullptr);
    ~KMcpClientInspectorActionTabWidget() override;
};
