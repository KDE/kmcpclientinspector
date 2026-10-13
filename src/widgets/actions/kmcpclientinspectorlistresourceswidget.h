/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "actions/kmcpclientinspectorresultwidget.h"
#include "libkmcpclientinspectorwidgets_private_export.h"
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorListResourcesWidget : public KMcpClientInspectorResultWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorListResourcesWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                    QWidget *parent = nullptr);
    ~KMcpClientInspectorListResourcesWidget() override;

    void setResult(const QJsonObject &obj) override;
};
