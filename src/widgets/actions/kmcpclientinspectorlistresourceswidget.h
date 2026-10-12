/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "actions/kmcpclientinspectoractiontabpagebase.h"
#include "libkmcpclientinspectorwidgets_private_export.h"
class QJsonObject;
class QTextBrowser;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorListResourcesWidget : public KMcpClientInspectorActionTabPageBase
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorListResourcesWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                    QWidget *parent = nullptr);
    ~KMcpClientInspectorListResourcesWidget() override;

    void setResult(const QJsonObject &obj);

private:
    QTextBrowser *const mTextBrowser;
};
