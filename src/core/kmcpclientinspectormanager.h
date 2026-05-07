/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "libkmcpclientinspectorcore_export.h"
#include <QObject>
#include <TextAutoGenerateTextMcpProtocolCore/McpServerManager>

class LIBKMCPCLIENTINSPECTORCORE_EXPORT KMcpClientInspectorManager : public TextAutoGenerateTextMcpProtocolCore::McpServerManager
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorManager(QObject *parent = nullptr);
    ~KMcpClientInspectorManager() override;

    [[nodiscard]] QString serverConfigFileName() const override;
};
