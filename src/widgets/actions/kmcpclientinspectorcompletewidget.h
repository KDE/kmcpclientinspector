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
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolResourceTemplate>
class QComboBox;
class QJsonObject;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class QTabWidget;
class JsonPlainTextEditWidget;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolCompleteRequestParams;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorCompleteWidget : public KMcpClientInspectorActionTabPageBase
{
    Q_OBJECT
public:
    enum class ReferenceType : uint8_t {
        Prompt,
        ResourceTemplate,
    };
    Q_ENUM(ReferenceType)

    explicit KMcpClientInspectorCompleteWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                               QWidget *parent = nullptr);
    ~KMcpClientInspectorCompleteWidget() override;

    /*!
     * Update prompts list from prompts/list response \a obj.
     */
    void setPromptsList(const QJsonObject &obj);
    /*!
     * Update resource templates list from resources/templates/list response \a obj.
     */
    void setResourceTemplatesList(const QJsonObject &obj);
    /*!
     * Show completion/complete response \a obj.
     */
    void setResult(const QJsonObject &obj);

    [[nodiscard]] QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolPrompt> prompts() const;
    [[nodiscard]] QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolResourceTemplate> resourceTemplates() const;

    /*!
     * Build completion request params from current values.
     * Return std::nullopt and set \a errorMessage when values are not valid.
     */
    [[nodiscard]] std::optional<TextAutoGenerateTextMcpProtocolCore::McpProtocolCompleteRequestParams> requestParams(QString &errorMessage) const;

    /*!
     * Return variable names used in uri template \a uriTemplate (RFC 6570), e.g. "file:///{path}{?query,page}" gives path, query, page.
     */
    [[nodiscard]] static QStringList uriTemplateVariables(const QString &uriTemplate);

private:
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotComplete();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void updateReferences();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void updateArgumentNames();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void clearResult();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void setRequestInProgress(bool inProgress);
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void updateCompleteButton();
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void showMessage(const QString &message, bool isError);
    [[nodiscard]] LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT ReferenceType referenceType() const;

    QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolPrompt> mPrompts;
    QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolResourceTemplate> mResourceTemplates;
    QComboBox *const mReferenceTypeComboBox;
    QComboBox *const mReferenceComboBox;
    QComboBox *const mArgumentNameComboBox;
    QLineEdit *const mArgumentValueLineEdit;
    QLineEdit *const mContextLineEdit;
    QPushButton *const mCompleteButton;
    QLabel *const mStatusLabel;
    QTabWidget *const mResultTabWidget;
    QListWidget *const mValuesListWidget;
    JsonPlainTextEditWidget *const mJsonPlainTextEditWidget;
    qint64 mPendingRequestId = -1;
};
