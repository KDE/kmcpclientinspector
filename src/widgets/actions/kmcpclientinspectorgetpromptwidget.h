/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "actions/kmcpclientinspectoractiontabpagebase.h"
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QList>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolPrompt>
class QJsonObject;
class QLabel;
class QListWidget;
class QPushButton;
class QTabWidget;
class QTextBrowser;
class QTextDocument;
class JsonPlainTextEditWidget;
class KMcpClientInspectorPromptArgumentsWidget;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolGetPromptResult;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorGetPromptWidget : public KMcpClientInspectorActionTabPageBase
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorGetPromptWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                QWidget *parent = nullptr);
    ~KMcpClientInspectorGetPromptWidget() override;

    /*!
     * Update prompts list from prompts/list response \a obj.
     */
    void setPromptsList(const QJsonObject &obj);
    /*!
     * Show prompts/get response \a obj.
     */
    void setResult(const QJsonObject &obj);

    [[nodiscard]] QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolPrompt> prompts() const;

    /*!
     * Convert \a result to html. Images are added as resources to \a document when it is not null.
     */
    [[nodiscard]] static QString resultToHtml(const TextAutoGenerateTextMcpProtocolCore::McpProtocolGetPromptResult &result, QTextDocument *document = nullptr);

private:
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotCurrentPromptChanged(int row);
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotGetPrompt();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void clearPrompts();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void clearResult();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void setRequestInProgress(bool inProgress);
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void updateGetPromptButton();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void showMessage(const QString &message, bool isError);

    QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolPrompt> mPrompts;
    QPushButton *const mListPromptsButton;
    QListWidget *const mPromptsListWidget;
    QLabel *const mPromptNameLabel;
    QLabel *const mPromptDescriptionLabel;
    KMcpClientInspectorPromptArgumentsWidget *const mArgumentsWidget;
    QPushButton *const mGetPromptButton;
    QLabel *const mStatusLabel;
    QTabWidget *const mResultTabWidget;
    QTextBrowser *const mMessagesBrowser;
    JsonPlainTextEditWidget *const mJsonPlainTextEditWidget;
    qint64 mPendingRequestId = -1;
};
