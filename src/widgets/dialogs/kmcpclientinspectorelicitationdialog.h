/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QDialog>
#include <QJsonObject>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolElicitResult>
class QLabel;
class KMcpClientInspectorToolArgumentsWidget;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolElicitRequestFormParams;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorElicitationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorElicitationDialog(const TextAutoGenerateTextMcpProtocolCore::McpProtocolElicitRequestFormParams &params,
                                                  QWidget *parent = nullptr);
    ~KMcpClientInspectorElicitationDialog() override;

    /*!
     * Answer to send to server: Cancel until user accepted or declined the request.
     */
    [[nodiscard]] TextAutoGenerateTextMcpProtocolCore::McpProtocolElicitResult elicitResult() const;

private:
    void slotAccept();
    void slotDecline();

    QLabel *const mMessageLabel;
    KMcpClientInspectorToolArgumentsWidget *const mArgumentsWidget;
    TextAutoGenerateTextMcpProtocolCore::McpProtocolElicitResult::Action mAction = TextAutoGenerateTextMcpProtocolCore::McpProtocolElicitResult::Action::Cancel;
    QJsonObject mContent;
};
