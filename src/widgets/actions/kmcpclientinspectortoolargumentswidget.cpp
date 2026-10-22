/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectortoolargumentswidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <KLocalizedString>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>
#include <QStackedWidget>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;

namespace
{
// Parse any json value (QJsonDocument only accepts objects and arrays)
std::optional<QJsonValue> parseJsonValue(const QString &text)
{
    QJsonParseError error;
    const QJsonDocument doc = QJsonDocument::fromJson(u"[%1]"_s.arg(text).toUtf8(), &error);
    if (error.error != QJsonParseError::NoError || doc.array().count() != 1) {
        return std::nullopt;
    }
    return doc.array().at(0);
}

QString jsonValueToText(const QJsonValue &value)
{
    const QByteArray json = QJsonDocument(QJsonArray{value}).toJson(QJsonDocument::Compact);
    // Remove "[" and "]"
    return QString::fromUtf8(json.mid(1, json.size() - 2));
}
}

KMcpClientInspectorToolArgumentsWidget::KMcpClientInspectorToolArgumentsWidget(QWidget *parent)
    : QWidget{parent}
    , mJsonEditorCheckBox(new QCheckBox(i18nc("@option:check", "Edit arguments as JSON"), this))
    , mStackedWidget(new QStackedWidget(this))
    , mFormWidget(new QWidget(this))
    , mFormLayout(new QFormLayout(mFormWidget))
    , mNoArgumentsLabel(new QLabel(i18n("This tool has no arguments."), this))
    , mJsonEditor(new JsonPlainTextEditWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mJsonEditorCheckBox->setObjectName(u"mJsonEditorCheckBox"_s);
    mainLayout->addWidget(mJsonEditorCheckBox);
    connect(mJsonEditorCheckBox, &QCheckBox::toggled, this, &KMcpClientInspectorToolArgumentsWidget::slotJsonEditorToggled);

    mStackedWidget->setObjectName(u"mStackedWidget"_s);
    mainLayout->addWidget(mStackedWidget);

    auto formPage = new QWidget(this);
    auto formPageLayout = new QVBoxLayout(formPage);
    formPageLayout->setContentsMargins({});
    mNoArgumentsLabel->setObjectName(u"mNoArgumentsLabel"_s);
    formPageLayout->addWidget(mNoArgumentsLabel);

    mFormWidget->setObjectName(u"mFormWidget"_s);
    mFormLayout->setObjectName(u"mFormLayout"_s);
    auto scrollArea = new QScrollArea(this);
    scrollArea->setObjectName(u"scrollArea"_s);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(mFormWidget);
    formPageLayout->addWidget(scrollArea);

    mJsonEditor->setObjectName(u"mJsonEditor"_s);
    mJsonEditor->setReadOnly(false);

    mStackedWidget->addWidget(formPage);
    mStackedWidget->addWidget(mJsonEditor);
    clear();
}

KMcpClientInspectorToolArgumentsWidget::~KMcpClientInspectorToolArgumentsWidget() = default;

bool KMcpClientInspectorToolArgumentsWidget::canGenerateForm(const TextAutoGenerateTextMcpProtocolCore::McpProtocolTool::InputSchema &schema)
{
    // These keywords describe the arguments object globally: a form by property can't represent them
    const QJsonObject additionalProperties = schema.additionalProperties();
    for (const auto &key : {"oneOf"_L1, "anyOf"_L1, "allOf"_L1, "$ref"_L1, "patternProperties"_L1, "if"_L1}) {
        if (additionalProperties.contains(key)) {
            return false;
        }
    }
    // Free-form object without known properties
    if (!schema.properties().has_value() || schema.properties()->isEmpty()) {
        const QJsonValue additional = additionalProperties.value("additionalProperties"_L1);
        return additional.isUndefined() || additional == QJsonValue(false);
    }
    return true;
}

KMcpClientInspectorToolArgumentsWidget::FieldType KMcpClientInspectorToolArgumentsWidget::fieldType(const QJsonObject &property)
{
    if (property.value("enum"_L1).isArray()) {
        return FieldType::Enum;
    }
    QString type;
    const QJsonValue typeValue = property.value("type"_L1);
    if (typeValue.isString()) {
        type = typeValue.toString();
    } else if (typeValue.isArray()) {
        // e.g. ["string", "null"]: use first non null type
        const QJsonArray types = typeValue.toArray();
        for (const auto &t : types) {
            if (t.toString() != "null"_L1) {
                if (!type.isEmpty()) {
                    // Several types: user must write json
                    return FieldType::Json;
                }
                type = t.toString();
            }
        }
    }
    if (type == "string"_L1) {
        return FieldType::String;
    } else if (type == "integer"_L1) {
        return FieldType::Integer;
    } else if (type == "number"_L1) {
        return FieldType::Number;
    } else if (type == "boolean"_L1) {
        return FieldType::Boolean;
    }
    return FieldType::Json;
}

QWidget *KMcpClientInspectorToolArgumentsWidget::createFieldWidget(const Field &field, const QJsonObject &property)
{
    switch (field.type) {
    case FieldType::Boolean: {
        auto comboBox = new QComboBox(mFormWidget);
        if (!field.required) {
            comboBox->addItem(QString(), QVariant());
        }
        comboBox->addItem(u"true"_s, true);
        comboBox->addItem(u"false"_s, false);
        return comboBox;
    }
    case FieldType::Enum: {
        auto comboBox = new QComboBox(mFormWidget);
        if (!field.required) {
            comboBox->addItem(QString(), QVariant());
        }
        const QJsonArray values = property.value("enum"_L1).toArray();
        for (const auto &value : values) {
            comboBox->addItem(value.isString() ? value.toString() : jsonValueToText(value), value.toVariant());
        }
        return comboBox;
    }
    case FieldType::String:
    case FieldType::Integer:
    case FieldType::Number:
    case FieldType::Json: {
        auto lineEdit = new QLineEdit(mFormWidget);
        lineEdit->setClearButtonEnabled(true);
        switch (field.type) {
        case FieldType::Integer:
            lineEdit->setPlaceholderText(i18nc("@info:placeholder", "Integer"));
            break;
        case FieldType::Number:
            lineEdit->setPlaceholderText(i18nc("@info:placeholder", "Number"));
            break;
        case FieldType::Json:
            lineEdit->setPlaceholderText(i18nc("@info:placeholder", "JSON value"));
            break;
        default:
            break;
        }
        return lineEdit;
    }
    }
    return nullptr;
}

void KMcpClientInspectorToolArgumentsWidget::clearForm()
{
    while (mFormLayout->rowCount() > 0) {
        mFormLayout->removeRow(0);
    }
    mFields.clear();
}

void KMcpClientInspectorToolArgumentsWidget::clear()
{
    clearForm();
    mFormAvailable = true;
    mNoArgumentsLabel->setVisible(true);
    mJsonEditor->setPlainText(u"{}"_s);
    mJsonEditorCheckBox->setEnabled(true);
    setJsonEditorActivated(false);
}

void KMcpClientInspectorToolArgumentsWidget::setNoArgumentsText(const QString &text)
{
    mNoArgumentsLabel->setText(text);
}

void KMcpClientInspectorToolArgumentsWidget::setInputSchema(const TextAutoGenerateTextMcpProtocolCore::McpProtocolTool::InputSchema &schema)
{
    clear();
    mFormAvailable = canGenerateForm(schema);
    const QMap<QString, QJsonObject> properties = schema.properties().value_or(QMap<QString, QJsonObject>{});
    const QStringList required = schema.required().value_or(QStringList{});

    if (mFormAvailable) {
        // Required arguments first
        QStringList names;
        names.reserve(properties.count());
        for (auto it = properties.constBegin(); it != properties.constEnd(); ++it) {
            if (required.contains(it.key())) {
                names.append(it.key());
            }
        }
        for (auto it = properties.constBegin(); it != properties.constEnd(); ++it) {
            if (!required.contains(it.key())) {
                names.append(it.key());
            }
        }

        for (const QString &name : std::as_const(names)) {
            const QJsonObject property = properties.value(name);
            Field field;
            field.name = name;
            field.type = fieldType(property);
            field.required = required.contains(name);
            field.widget = createFieldWidget(field, property);
            field.widget->setObjectName(name);
            const QString description = property.value("description"_L1).toString();
            field.widget->setToolTip(description);

            auto label = new QLabel(field.required ? u"%1*:"_s.arg(name) : u"%1:"_s.arg(name), mFormWidget);
            label->setToolTip(description);
            mFormLayout->addRow(label, field.widget);
            mFields.append(field);
            if (const QJsonValue defaultValue = property.value("default"_L1); !defaultValue.isUndefined()) {
                setFieldValue(field, defaultValue);
            }
        }
        mNoArgumentsLabel->setVisible(mFields.isEmpty());
        mJsonEditorCheckBox->setVisible(!mFields.isEmpty());
    } else {
        // Fallback: template with known properties
        QJsonObject templateObj;
        for (auto it = properties.constBegin(); it != properties.constEnd(); ++it) {
            templateObj.insert(it.key(), it.value().contains("default"_L1) ? it.value().value("default"_L1) : QJsonValue());
        }
        mJsonEditor->setPlainText(QString::fromUtf8(QJsonDocument(templateObj).toJson(QJsonDocument::Indented)));
    }
    mJsonEditorCheckBox->setEnabled(mFormAvailable);
    setJsonEditorActivated(!mFormAvailable);
}

void KMcpClientInspectorToolArgumentsWidget::setFieldValue(const Field &field, const QJsonValue &value)
{
    if (auto comboBox = qobject_cast<QComboBox *>(field.widget)) {
        const int index = comboBox->findData(value.toVariant());
        // Unknown value: select empty entry when it exists
        comboBox->setCurrentIndex(index >= 0 ? index : 0);
    } else if (auto lineEdit = qobject_cast<QLineEdit *>(field.widget)) {
        if (value.isUndefined() || (value.isNull() && field.type != FieldType::Json)) {
            lineEdit->clear();
        } else if (field.type == FieldType::String && value.isString()) {
            lineEdit->setText(value.toString());
        } else {
            lineEdit->setText(jsonValueToText(value));
        }
    }
}

void KMcpClientInspectorToolArgumentsWidget::setArguments(const QJsonObject &arguments)
{
    if (mJsonEditorCheckBox->isChecked()) {
        mJsonEditor->setPlainText(QString::fromUtf8(QJsonDocument(arguments).toJson(QJsonDocument::Indented)));
    } else {
        for (const Field &field : std::as_const(mFields)) {
            setFieldValue(field, arguments.value(field.name));
        }
    }
}

std::optional<QJsonObject> KMcpClientInspectorToolArgumentsWidget::arguments(QString *errorMessage) const
{
    return mJsonEditorCheckBox->isChecked() ? jsonArguments(errorMessage) : formArguments(errorMessage);
}

std::optional<QJsonObject> KMcpClientInspectorToolArgumentsWidget::formArguments(QString *errorMessage) const
{
    const auto setError = [errorMessage](const QString &message) {
        if (errorMessage) {
            *errorMessage = message;
        }
    };
    QJsonObject obj;
    for (const Field &field : std::as_const(mFields)) {
        if (auto comboBox = qobject_cast<QComboBox *>(field.widget)) {
            const QJsonValue value = QJsonValue::fromVariant(comboBox->currentData());
            if (!value.isNull()) {
                obj.insert(field.name, value);
            }
            continue;
        }
        const auto lineEdit = qobject_cast<QLineEdit *>(field.widget);
        if (!lineEdit) {
            continue;
        }
        const QString text = lineEdit->text();
        if (field.type == FieldType::String) {
            // Empty string is a valid value for a required argument
            if (!text.isEmpty() || field.required) {
                obj.insert(field.name, text);
            }
            continue;
        }
        const QString trimmed = text.trimmed();
        if (trimmed.isEmpty()) {
            if (field.required) {
                setError(i18n("Argument \"%1\" is required.", field.name));
                return std::nullopt;
            }
            continue;
        }
        bool ok = false;
        switch (field.type) {
        case FieldType::Integer: {
            const qint64 value = trimmed.toLongLong(&ok);
            if (!ok) {
                setError(i18n("Argument \"%1\" must be an integer.", field.name));
                return std::nullopt;
            }
            obj.insert(field.name, value);
            break;
        }
        case FieldType::Number: {
            const double value = trimmed.toDouble(&ok);
            if (!ok) {
                setError(i18n("Argument \"%1\" must be a number.", field.name));
                return std::nullopt;
            }
            obj.insert(field.name, value);
            break;
        }
        default: {
            const std::optional<QJsonValue> value = parseJsonValue(trimmed);
            if (!value.has_value()) {
                setError(i18n("Argument \"%1\" is not a valid JSON value.", field.name));
                return std::nullopt;
            }
            obj.insert(field.name, *value);
            break;
        }
        }
    }
    return obj;
}

std::optional<QJsonObject> KMcpClientInspectorToolArgumentsWidget::jsonArguments(QString *errorMessage) const
{
    const QString text = mJsonEditor->toPlainText().trimmed();
    if (text.isEmpty()) {
        return QJsonObject();
    }
    QJsonParseError error;
    const QJsonDocument doc = QJsonDocument::fromJson(text.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        if (errorMessage) {
            *errorMessage = i18n("Invalid JSON arguments: %1 (offset %2).", error.errorString(), error.offset);
        }
        return std::nullopt;
    }
    if (!doc.isObject()) {
        if (errorMessage) {
            *errorMessage = i18n("Arguments must be a JSON object.");
        }
        return std::nullopt;
    }
    return doc.object();
}

bool KMcpClientInspectorToolArgumentsWidget::jsonEditorActivated() const
{
    return mJsonEditorCheckBox->isChecked();
}

void KMcpClientInspectorToolArgumentsWidget::setJsonEditorActivated(bool activated)
{
    if (mJsonEditorCheckBox->isChecked() == activated) {
        // Make sure that page is correct (toggled() is not emitted)
        mStackedWidget->setCurrentIndex(activated ? 1 : 0);
        return;
    }
    mJsonEditorCheckBox->setChecked(activated);
}

void KMcpClientInspectorToolArgumentsWidget::slotJsonEditorToggled(bool checked)
{
    // Keep values when switching between form and json editor
    if (checked) {
        if (const std::optional<QJsonObject> args = formArguments(nullptr); args.has_value() && mFormAvailable) {
            mJsonEditor->setPlainText(QString::fromUtf8(QJsonDocument(*args).toJson(QJsonDocument::Indented)));
        }
    } else if (const std::optional<QJsonObject> args = jsonArguments(nullptr); args.has_value()) {
        for (const Field &field : std::as_const(mFields)) {
            setFieldValue(field, args->value(field.name));
        }
    }
    mStackedWidget->setCurrentIndex(checked ? 1 : 0);
}

#include "moc_kmcpclientinspectortoolargumentswidget.cpp"
