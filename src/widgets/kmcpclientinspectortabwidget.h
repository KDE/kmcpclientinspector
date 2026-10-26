/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QTabWidget>
class KMcpClientInspectorManager;
class KMcpClientInspectorTabPage;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpServer;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorTabWidget(KMcpClientInspectorManager *manager, QWidget *parent = nullptr);
    ~KMcpClientInspectorTabWidget() override;

    // Synchronize tabs with servers model, existing pages are kept
    void loadServers();

private:
    [[nodiscard]] LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT KMcpClientInspectorTabPage *createPage(const TextAutoGenerateTextMcpProtocolCore::McpServer &server);
    KMcpClientInspectorManager *const mManager;
};
