/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QWidget>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
class KMcpClientInspectorServerSettingsWidget;
class KMcpClientInspectorActionTabWidget;
class KMcpClientInspectorClientProtocolManager;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpServer;
class McpProtocolClient;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorTabPage : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorTabPage(const TextAutoGenerateTextMcpProtocolCore::McpServer &server, QWidget *parent = nullptr);
    ~KMcpClientInspectorTabPage() override;

private:
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void initializeClient(bool started);
    KMcpClientInspectorServerSettingsWidget *const mServerSettings;
    KMcpClientInspectorActionTabWidget *const mActionTabWidget;
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClient *mClient = nullptr;
    TextAutoGenerateTextMcpProtocolCore::McpServer mServer;
    KMcpClientInspectorClientProtocolManager *const mProtocolManager;
};
