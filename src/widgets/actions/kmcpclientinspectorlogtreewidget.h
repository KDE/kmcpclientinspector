/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QWidget>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
class QLineEdit;
class QTreeView;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolClientProtocolManager;
}
class KMcpClientInspectorLogModel;
class KMcpClientInspectorLogSortFilterProxyModel;
class KMcpClientInspectorLogSearchWidget;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorLogTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorLogTreeWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                              QWidget *parent = nullptr);
    ~KMcpClientInspectorLogTreeWidget() override;

Q_SIGNALS:
    void jsonSelected(const QString &json);
    void clearRequested();

private:
    void slotCurrentChanged(const QModelIndex &index);
    void slotMessageLogged(const TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MessageInfo &info);
    QTreeView *const mTreeView;
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *const mProtocolManager;
    KMcpClientInspectorLogModel *const mLogModel;
    KMcpClientInspectorLogSortFilterProxyModel *const mLogSortFilterProxyModel;
    KMcpClientInspectorLogSearchWidget *const mLogSearchWidget;
};
