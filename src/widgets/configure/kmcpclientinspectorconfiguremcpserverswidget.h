/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kmcpclientinspectorconfigurebasewidget.h"
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
namespace TextAutoGenerateTextMcpProtocolWidgets
{
class McpServerWidget;
}
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpServerManager;
}
class KMcpClientInspectorConfigureMcpServersWidget : public KMcpClientInspectorConfigureBaseWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorConfigureMcpServersWidget(TextAutoGenerateTextMcpProtocolCore::McpServerManager *manager, QWidget *parent = nullptr);
    ~KMcpClientInspectorConfigureMcpServersWidget() override;

    void save() override;
    void revert() override;
    void load() override;

private:
    TextAutoGenerateTextMcpProtocolWidgets::McpServerWidget *const mMcpServerWidget;
    TextAutoGenerateTextMcpProtocolCore::McpServerManager *const mManager;
    QList<TextAutoGenerateTextMcpProtocolCore::McpServer> mOriginalServers;
};
