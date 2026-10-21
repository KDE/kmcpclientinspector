/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include <KMessageWidget>

#include "libkmcpclientinspectorwidgets_private_export.h"
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorMessageWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorMessageWidget(QWidget *parent = nullptr);
    ~KMcpClientInspectorMessageWidget() override;
};
