/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "actions/kmcpclientinspectoractiontabpagebase.h"
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QList>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolUtils>
class QJsonObject;
class QLabel;
class QListWidget;
class QPushButton;
class QTabWidget;
class QTextBrowser;
class QTextDocument;
class JsonPlainTextEditWidget;
class KMcpClientInspectorToolArgumentsWidget;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolCallToolResult;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorCallToolWidget : public KMcpClientInspectorActionTabPageBase
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorCallToolWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                               QWidget *parent = nullptr);
    ~KMcpClientInspectorCallToolWidget() override;

    /*!
     * Update tools list from tools/list response \a obj.
     */
    void setToolsList(const QJsonObject &obj);
    /*!
     * Show tools/call response \a obj.
     */
    void setResult(const QJsonObject &obj);

    [[nodiscard]] QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolTool> tools() const;

    /*!
     * Convert \a result to html. Images are added as resources to \a document when it is not null.
     */
    [[nodiscard]] static QString resultToHtml(const TextAutoGenerateTextMcpProtocolCore::McpProtocolCallToolResult &result, QTextDocument *document = nullptr);
    /*!
     * Convert content \a block to html. Images are added as resources to \a document when it is not null,
     * \a imageIndex is used to create a unique url for them.
     */
    [[nodiscard]] static QString
    contentBlockToHtml(const TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::ContentBlock &block, QTextDocument *document, int &imageIndex);

private:
    void slotCurrentToolChanged(int row);
    void slotCallTool();
    void clearTools();
    void clearResult();
    void setCallInProgress(bool inProgress);
    void updateCallButton();
    void showMessage(const QString &message, bool isError);

    QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolTool> mTools;
    QPushButton *const mListToolsButton;
    QListWidget *const mToolsListWidget;
    QLabel *const mToolNameLabel;
    QLabel *const mToolDescriptionLabel;
    KMcpClientInspectorToolArgumentsWidget *const mArgumentsWidget;
    QPushButton *const mCallToolButton;
    QLabel *const mStatusLabel;
    QTabWidget *const mResultTabWidget;
    QTextBrowser *const mContentBrowser;
    JsonPlainTextEditWidget *const mJsonPlainTextEditWidget;
    qint64 mPendingRequestId = -1;
};
