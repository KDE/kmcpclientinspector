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
class KMcpClientInspectorClientProtocolManager;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolClient;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorActionTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorActionTabWidget(KMcpClientInspectorClientProtocolManager *protocolManager, QWidget *parent = nullptr);
    ~KMcpClientInspectorActionTabWidget() override;

private:
    KMcpClientInspectorPingWidget *const mPingWidget;
    QTabWidget *const mTabWidget;
};
