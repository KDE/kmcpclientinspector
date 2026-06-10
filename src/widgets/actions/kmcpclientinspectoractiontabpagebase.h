/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QWidget>
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolClientProtocolManager;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorActionTabPageBase : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorActionTabPageBase(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                  QWidget *parent = nullptr);
    ~KMcpClientInspectorActionTabPageBase() override;

protected:
    TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *const mProtocolManager;
};
