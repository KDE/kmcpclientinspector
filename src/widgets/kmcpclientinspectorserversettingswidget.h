/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QWidget>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolPlugin>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolClient;
}
namespace TextAutoGenerateTextMcpProtocolWidgets
{
class AddMcpServerWidget;
}
class QPushButton;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorServerSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorServerSettingsWidget(const TextAutoGenerateTextMcpProtocolCore::McpServer &server, QWidget *parent = nullptr);
    ~KMcpClientInspectorServerSettingsWidget() override;

Q_SIGNALS:
    void startStopRequested(bool started);

private:
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotStopStart();
    TextAutoGenerateTextMcpProtocolWidgets::AddMcpServerWidget *const mMcpServerWidget;
    QPushButton *const mStartStopButton;
    TextAutoGenerateTextMcpProtocolCore::McpServer mServer;
};
