/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "actions/kmcpclientinspectoractiontabpagebase.h"
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
class QJsonObject;
class JsonPlainTextEditWidget;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorResultWidget : public KMcpClientInspectorActionTabPageBase
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorResultWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                             const QString &buttonText,
                                             TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::MethodType type,
                                             QWidget *parent = nullptr);
    ~KMcpClientInspectorResultWidget() override;

    virtual void setResult(const QJsonObject &obj);

protected:
    void showJson(const QJsonObject &obj);

private:
    JsonPlainTextEditWidget *const mJsonPlainTextEditWidget;
};
