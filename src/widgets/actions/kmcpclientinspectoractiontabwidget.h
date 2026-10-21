/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QWidget>
class KMcpClientInspectorInitializeWidget;
class KMcpClientInspectorPingWidget;
class QTabWidget;

class KMcpClientInspectorListToolsWidget;
class KMcpClientInspectorCallToolWidget;
class KMcpClientInspectorListPromptsWidget;
class KMcpClientInspectorGetPromptWidget;
class KMcpClientInspectorListResourcesWidget;
class KMcpClientInspectorResourceTemplatesWidget;
class KMcpClientInspectorLogWidget;
class KMcpClientInspectorServerNotificationWidget;
class KMcpClientInspectorMessageWidget;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolClient;
class McpProtocolClientProtocolManager;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorActionTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorActionTabWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                QWidget *parent = nullptr);
    ~KMcpClientInspectorActionTabWidget() override;

private:
    KMcpClientInspectorInitializeWidget *const mInitializeWidget;
    KMcpClientInspectorPingWidget *const mPingWidget;
    KMcpClientInspectorListToolsWidget *const mListToolsWidget;
    KMcpClientInspectorCallToolWidget *const mCallToolWidget;
    KMcpClientInspectorListPromptsWidget *const mListPromptsWidget;
    KMcpClientInspectorGetPromptWidget *const mGetPromptWidget;
    KMcpClientInspectorListResourcesWidget *const mListResourcesWidget;
    KMcpClientInspectorResourceTemplatesWidget *const mResourceTemplatesWidget;
    KMcpClientInspectorLogWidget *const mLogWidget;
    QTabWidget *const mTabWidget;
    KMcpClientInspectorMessageWidget *const mMessageWidget;
    KMcpClientInspectorServerNotificationWidget *const mServerNotificationWidget;
};
