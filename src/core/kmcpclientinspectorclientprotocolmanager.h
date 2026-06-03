/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "libkmcpclientinspectorcore_export.h"
#include <QHash>
#include <QObject>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
class QJsonObject;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolClient;
}
class LIBKMCPCLIENTINSPECTORCORE_EXPORT KMcpClientInspectorClientProtocolManager : public QObject
{
    Q_OBJECT
public:
    enum class MethodType : uint8_t {
        Unknown = 0,
        Ping,
        ListTools,
        ListPrompts,
        ResourceTemplates,
    };
    Q_ENUM(MethodType)
    explicit KMcpClientInspectorClientProtocolManager(const TextAutoGenerateTextMcpProtocolCore::McpServer &server, QObject *parent = nullptr);
    ~KMcpClientInspectorClientProtocolManager() override;

    void initializeClient(bool started);

    [[nodiscard]] int requestId();

    void executeAction(MethodType type);

Q_SIGNALS:
    void started();
    void received(const QJsonObject &obj, KMcpClientInspectorClientProtocolManager::MethodType type);
    void error(const QString &str);

private:
    LIBKMCPCLIENTINSPECTORCORE_NO_EXPORT void ping();
    LIBKMCPCLIENTINSPECTORCORE_NO_EXPORT void listTools();
    LIBKMCPCLIENTINSPECTORCORE_NO_EXPORT void listPrompts();
    LIBKMCPCLIENTINSPECTORCORE_NO_EXPORT void resouceTemplates();
    [[nodiscard]] LIBKMCPCLIENTINSPECTORCORE_NO_EXPORT KMcpClientInspectorClientProtocolManager::MethodType checkMethodType(const QJsonObject &obj) const;

    int mRequestIdentifier = 0;
    TextAutoGenerateTextMcpProtocolCore::McpServer mServer;
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClient *mClient = nullptr;
    QHash<int, KMcpClientInspectorClientProtocolManager::MethodType> mMapIdentifier;
};
