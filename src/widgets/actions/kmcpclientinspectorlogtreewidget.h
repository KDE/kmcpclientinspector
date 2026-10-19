/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include <QWidget>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
class QLineEdit;
class QTreeView;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolClientProtocolManager;
}
class KMcpClientInspectorLogModel;
class KMcpClientInspectorLogTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorLogTreeWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                              QWidget *parent = nullptr);
    ~KMcpClientInspectorLogTreeWidget() override;

private:
    void slotMessageLogged(const TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageInfo &info);
    QTreeView *const mTreeView;
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *const mProtocolManager;
    KMcpClientInspectorLogModel *const mLogModel;
};
