/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QJsonObject>
#include <QList>
#include <QWidget>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>
#include <optional>
class QCheckBox;
class QFormLayout;
class QLabel;
class QStackedWidget;
class JsonPlainTextEditWidget;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorToolArgumentsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorToolArgumentsWidget(QWidget *parent = nullptr);
    ~KMcpClientInspectorToolArgumentsWidget() override;

    void setInputSchema(const TextAutoGenerateTextMcpProtocolCore::McpProtocolTool::InputSchema &schema);
    void clear();

    /*!
     * Return arguments from form or json editor, std::nullopt if they are invalid (\a errorMessage explains why).
     */
    [[nodiscard]] std::optional<QJsonObject> arguments(QString *errorMessage = nullptr) const;
    void setArguments(const QJsonObject &arguments);

    [[nodiscard]] bool jsonEditorActivated() const;
    void setJsonEditorActivated(bool activated);

    [[nodiscard]] static bool canGenerateForm(const TextAutoGenerateTextMcpProtocolCore::McpProtocolTool::InputSchema &schema);

private:
    enum class FieldType : uint8_t {
        String,
        Integer,
        Number,
        Boolean,
        Enum,
        Json,
    };
    struct Field {
        QString name;
        FieldType type = FieldType::Json;
        bool required = false;
        QWidget *widget = nullptr;
    };
    [[nodiscard]] static FieldType fieldType(const QJsonObject &property);
    [[nodiscard]] std::optional<QJsonObject> formArguments(QString *errorMessage) const;
    [[nodiscard]] std::optional<QJsonObject> jsonArguments(QString *errorMessage) const;
    [[nodiscard]] QWidget *createFieldWidget(const Field &field, const QJsonObject &property);
    void setFieldValue(const Field &field, const QJsonValue &value);
    void clearForm();
    void slotJsonEditorToggled(bool checked);

    QList<Field> mFields;
    QCheckBox *const mJsonEditorCheckBox;
    QStackedWidget *const mStackedWidget;
    QWidget *const mFormWidget;
    QFormLayout *const mFormLayout;
    QLabel *const mNoArgumentsLabel;
    JsonPlainTextEditWidget *const mJsonEditor;
    bool mFormAvailable = true;
};
