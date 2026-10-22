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
class KMcpClientInspectorCapabilityWidget;
class KMcpClientInspectorConnectButtonsWidget;
class KMcpClientInspectorServerInfoWidget;
class QPushButton;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorServerSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorServerSettingsWidget(const TextAutoGenerateTextMcpProtocolCore::McpServer &server, QWidget *parent = nullptr);
    ~KMcpClientInspectorServerSettingsWidget() override;

Q_SIGNALS:
    void startStopRequested(bool started);
    void serverModified(const TextAutoGenerateTextMcpProtocolCore::McpServer &server);

private:
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotConnectRequested();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotDisconnectRequested();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotUpdateButtons(bool valid);
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotEditServer();
    KMcpClientInspectorServerInfoWidget *const mServerInfoWidget;
    QPushButton *const mEditButton;
    KMcpClientInspectorConnectButtonsWidget *const mConnectButtonsWidget;
    KMcpClientInspectorCapabilityWidget *const mCapabilityWidget;
    TextAutoGenerateTextMcpProtocolCore::McpServer mServer;
};
