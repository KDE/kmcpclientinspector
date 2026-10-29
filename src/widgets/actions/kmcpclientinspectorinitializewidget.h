/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "actions/kmcpclientinspectoractiontabpagebase.h"
#include "libkmcpclientinspectorwidgets_private_export.h"
class JsonPlainTextEditWidget;
class QFormLayout;
class QLabel;
class QTextEdit;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolInitializeResult;
class McpProtocolServerCapabilities;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorInitializeWidget : public KMcpClientInspectorActionTabPageBase
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorInitializeWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                 QWidget *parent = nullptr);
    ~KMcpClientInspectorInitializeWidget() override;

    void setResult(const QJsonObject &obj);
    void setInitializeResult(const TextAutoGenerateTextMcpProtocolCore::McpProtocolInitializeResult &result);

    [[nodiscard]] static QString capabilitiesToText(const TextAutoGenerateTextMcpProtocolCore::McpProtocolServerCapabilities &capabilities);

private:
    [[nodiscard]] LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT QLabel *createLabel(const QString &objectName);
    QFormLayout *const mFormLayout;
    QLabel *const mServerName;
    QLabel *const mServerTitle;
    QLabel *const mServerVersion;
    QLabel *const mServerDescription;
    QLabel *const mServerWebsite;
    QLabel *const mProtocolVersion;
    QLabel *const mCapabilities;
    QTextEdit *const mInstructions;
    JsonPlainTextEditWidget *const mJsonPlainTextEditWidget;
};
