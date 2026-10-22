/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QWidget>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolUtils>
class QDialog;
class KMessageWidget;
class KMcpClientInspectorServerSettingsWidget;
class KMcpClientInspectorActionTabWidget;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpServer;
class McpProtocolCreateMessageRequest;
class McpProtocolElicitRequest;
class McpProtocolClientProtocolManager;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorTabPage : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorTabPage(const TextAutoGenerateTextMcpProtocolCore::McpServer &server, QWidget *parent = nullptr);
    ~KMcpClientInspectorTabPage() override;

Q_SIGNALS:
    void started();
    void finished();

private:
    void slotElicitationRequested(const TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::RequestId &id,
                                  const TextAutoGenerateTextMcpProtocolCore::McpProtocolElicitRequest &request);
    void slotSamplingRequested(const TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::RequestId &id,
                               const TextAutoGenerateTextMcpProtocolCore::McpProtocolCreateMessageRequest &request);
    void slotElicitationCompleted(const QString &elicitationId);
    void showUserRequestDialog(QDialog *dialog, const TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::RequestId &id);
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *const mProtocolManager;
    KMcpClientInspectorServerSettingsWidget *const mServerSettings;
    KMcpClientInspectorActionTabWidget *const mActionTabWidget;
    KMessageWidget *const mElicitationCompletedWidget;
};
