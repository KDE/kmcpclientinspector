/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include "kmcpclientinspectoractiontabpagebase.h"
#include "libkmcpclientinspectorwidgets_private_export.h"
class QComboBox;
class QModelIndex;
class QPushButton;
class QSplitter;
class QTreeView;
class JsonPlainTextEditWidget;
class KMcpClientInspectorLogSearchWidget;
class KMcpClientInspectorServerLogModel;
class KMcpClientInspectorServerLogSortFilterProxyModel;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolLoggingMessageNotificationParams;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorServerNotificationWidget : public KMcpClientInspectorActionTabPageBase
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorServerNotificationWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                         QWidget *parent = nullptr);
    ~KMcpClientInspectorServerNotificationWidget() override;

private:
    void slotLoggingMessageReceived(const TextAutoGenerateTextMcpProtocolCore::McpProtocolLoggingMessageNotificationParams &params);
    void slotCurrentChanged(const QModelIndex &index);
    void slotMinimumLevelChanged();
    QComboBox *const mLevelComboBox;
    KMcpClientInspectorLogSearchWidget *const mLogSearchWidget;
    QPushButton *const mClearButton;
    QSplitter *const mSplitter;
    QTreeView *const mTreeView;
    JsonPlainTextEditWidget *const mJsonPlainTextEditWidget;
    KMcpClientInspectorServerLogModel *const mServerLogModel;
    KMcpClientInspectorServerLogSortFilterProxyModel *const mServerLogSortFilterProxyModel;
};
