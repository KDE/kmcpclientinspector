/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QDialog>
class QLineEdit;
class QPlainTextEdit;
class JsonPlainTextEditWidget;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolCreateMessageRequestParams;
class McpProtocolCreateMessageResult;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorSamplingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorSamplingDialog(const TextAutoGenerateTextMcpProtocolCore::McpProtocolCreateMessageRequestParams &params,
                                               QWidget *parent = nullptr);
    ~KMcpClientInspectorSamplingDialog() override;

    /*!
     * Generated message written by user, to send when dialog is accepted.
     */
    [[nodiscard]] TextAutoGenerateTextMcpProtocolCore::McpProtocolCreateMessageResult createMessageResult() const;

private:
    JsonPlainTextEditWidget *const mRequestWidget;
    QLineEdit *const mModelLineEdit;
    QLineEdit *const mStopReasonLineEdit;
    QPlainTextEdit *const mResponseTextEdit;
};
