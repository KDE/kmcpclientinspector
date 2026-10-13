/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "actions/kmcpclientinspectorresultwidget.h"
#include "libkmcpclientinspectorwidgets_private_export.h"
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorListToolsWidget : public KMcpClientInspectorResultWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorListToolsWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                QWidget *parent = nullptr);
    ~KMcpClientInspectorListToolsWidget() override;
};
