/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QWidget>
class KMcpClientInspectorPingWidget;
class QTabWidget;

class KMcpClientInspectorListToolsWidget;
class KMcpClientInspectorListPromptsWidget;
class KMcpClientInspectorResourceTemplatesWidget;
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
    KMcpClientInspectorPingWidget *const mPingWidget;
    KMcpClientInspectorListToolsWidget *const mListToolsWidget;
    KMcpClientInspectorListPromptsWidget *const mListPromptsWidget;
    KMcpClientInspectorResourceTemplatesWidget *const mResourceTemplatesWidget;
    QTabWidget *const mTabWidget;
};
