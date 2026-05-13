/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
namespace TextAutoGenerateTextMcpProtocolWidgets
{
class McpServerWidget;
}
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpServerManager;
}
class KMcpClientInspectorConfigureMcpServersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorConfigureMcpServersWidget(TextAutoGenerateTextMcpProtocolCore::McpServerManager *manager, QWidget *parent = nullptr);
    ~KMcpClientInspectorConfigureMcpServersWidget() override;

    void save();
    void load();
    void restoreToDefaults();

private:
    TextAutoGenerateTextMcpProtocolWidgets::McpServerWidget *const mMcpServerWidget;
    TextAutoGenerateTextMcpProtocolCore::McpServerManager *const mManager;
};
