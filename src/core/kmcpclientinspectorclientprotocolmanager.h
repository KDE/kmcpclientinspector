/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "libkmcpclientinspectorcore_export.h"
#include <QObject>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolClient;
}
class LIBKMCPCLIENTINSPECTORCORE_EXPORT KMcpClientInspectorClientProtocolManager : public QObject
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorClientProtocolManager(const TextAutoGenerateTextMcpProtocolCore::McpServer &server, QObject *parent = nullptr);
    ~KMcpClientInspectorClientProtocolManager() override;

    void initializeClient(bool started);
    void ping();

    [[nodiscard]] int requestId();

private:
    int mRequestIdentifier = 0;
    TextAutoGenerateTextMcpProtocolCore::McpServer mServer;
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClient *mClient = nullptr;
};
