/*
 * SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <KPageDialog>
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpServerManager;
}
class KMcpClientInspectorConfigureMcpServersWidget;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorConfigureSettingsDialog : public KPageDialog
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorConfigureSettingsDialog(TextAutoGenerateTextMcpProtocolCore::McpServerManager *manager, QWidget *parent = nullptr);
    ~KMcpClientInspectorConfigureSettingsDialog() override;

private:
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void readConfig();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void writeConfig();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotAccepted();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void load();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotRestoreDefaults();
    KMcpClientInspectorConfigureMcpServersWidget *const mConfigureMcpServersWidget;
    KPageWidgetItem *mConfigureMcpServersWidgetPage = nullptr;
};
