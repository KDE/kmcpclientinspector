/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QWidget>
class KMcpClientInspectorServerSettingsWidget;
class KMcpClientInspectorActionTabWidget;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpServer;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorTabPage : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorTabPage(const TextAutoGenerateTextMcpProtocolCore::McpServer &server, QWidget *parent = nullptr);
    ~KMcpClientInspectorTabPage() override;

private:
    KMcpClientInspectorServerSettingsWidget *const mServerSettings;
    KMcpClientInspectorActionTabWidget *const mActionTabWidget;
};
