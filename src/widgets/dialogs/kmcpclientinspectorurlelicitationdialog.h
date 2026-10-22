/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QDialog>
#include <QUrl>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolElicitResult>
class QLabel;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolElicitRequestURLParams;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorUrlElicitationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorUrlElicitationDialog(const TextAutoGenerateTextMcpProtocolCore::McpProtocolElicitRequestURLParams &params,
                                                     QWidget *parent = nullptr);
    ~KMcpClientInspectorUrlElicitationDialog() override;

    /*!
     * Answer to send to server: Accept means user agreed to open url(), Cancel until user chose.
     */
    [[nodiscard]] TextAutoGenerateTextMcpProtocolCore::McpProtocolElicitResult elicitResult() const;

    /*!
     * Url to open in web browser when dialog is accepted.
     */
    [[nodiscard]] QUrl url() const;

    /*!
     * Only http(s) urls can be opened.
     */
    [[nodiscard]] static bool canOpenUrl(const QUrl &url);

private:
    void slotDecline();

    const QUrl mUrl;
    QLabel *const mMessageLabel;
    QLabel *const mDomainLabel;
    QLabel *const mUrlLabel;
    TextAutoGenerateTextMcpProtocolCore::McpProtocolElicitResult::Action mAction = TextAutoGenerateTextMcpProtocolCore::McpProtocolElicitResult::Action::Cancel;
};
