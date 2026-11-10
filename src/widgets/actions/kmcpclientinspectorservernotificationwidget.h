/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include "kmcpclientinspectoractiontabpagebase.h"
#include "libkmcpclientinspectorwidgets_private_export.h"
class QComboBox;
class QJsonObject;
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

    void setLoggingLevelResult(const QJsonObject &obj);

private:
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void
    slotLoggingMessageReceived(const TextAutoGenerateTextMcpProtocolCore::McpProtocolLoggingMessageNotificationParams &params);
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotCurrentChanged(const QModelIndex &index);
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotMinimumLevelChanged();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotServerLevelActivated();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT static void fillLevelComboBox(QComboBox *comboBox);
    QComboBox *const mServerLevelComboBox;
    QComboBox *const mLevelComboBox;
    KMcpClientInspectorLogSearchWidget *const mLogSearchWidget;
    QSplitter *const mSplitter;
    QTreeView *const mTreeView;
    JsonPlainTextEditWidget *const mJsonPlainTextEditWidget;
    KMcpClientInspectorServerLogModel *const mServerLogModel;
    KMcpClientInspectorServerLogSortFilterProxyModel *const mServerLogSortFilterProxyModel;
};
