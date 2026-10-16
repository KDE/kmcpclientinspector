/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectortoolargumentswidgettest.h"
#include "actions/kmcpclientinspectortoolargumentswidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLabel>
#include <QLineEdit>
#include <QStackedWidget>
#include <QTest>
QTEST_MAIN(KMcpClientInspectorToolArgumentsWidgetTest)
using namespace Qt::Literals::StringLiterals;
using TextAutoGenerateTextMcpProtocolCore::McpProtocolTool;

namespace
{
McpProtocolTool::InputSchema schemaFromJson(const QByteArray &json)
{
    return McpProtocolTool::InputSchema::fromJson(QJsonDocument::fromJson(json).object());
}

const QByteArray formSchema = R"({
    "type": "object",
    "properties": {
        "city": {"type": "string", "description": "City name"},
        "days": {"type": "integer", "default": 3},
        "ratio": {"type": "number"},
        "metric": {"type": "boolean"},
        "unit": {"type": "string", "enum": ["celsius", "fahrenheit"]},
        "tags": {"type": "array", "items": {"type": "string"}}
    },
    "required": ["city", "unit"]
})";
}

KMcpClientInspectorToolArgumentsWidgetTest::KMcpClientInspectorToolArgumentsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorToolArgumentsWidgetTest::shouldHaveDefaultValues()
{
    KMcpClientInspectorToolArgumentsWidget w;
    auto mJsonEditorCheckBox = w.findChild<QCheckBox *>(u"mJsonEditorCheckBox"_s);
    QVERIFY(mJsonEditorCheckBox);
    QVERIFY(!mJsonEditorCheckBox->isChecked());
    QVERIFY(mJsonEditorCheckBox->isEnabled());

    auto mStackedWidget = w.findChild<QStackedWidget *>(u"mStackedWidget"_s);
    QVERIFY(mStackedWidget);
    QCOMPARE(mStackedWidget->currentIndex(), 0);

    auto mNoArgumentsLabel = w.findChild<QLabel *>(u"mNoArgumentsLabel"_s);
    QVERIFY(mNoArgumentsLabel);

    auto mFormLayout = w.findChild<QFormLayout *>(u"mFormLayout"_s);
    QVERIFY(mFormLayout);
    QCOMPARE(mFormLayout->rowCount(), 0);

    QVERIFY(!w.jsonEditorActivated());
    QCOMPARE(w.arguments(), QJsonObject());
}

void KMcpClientInspectorToolArgumentsWidgetTest::shouldGenerateForm()
{
    const McpProtocolTool::InputSchema schema = schemaFromJson(formSchema);
    QVERIFY(KMcpClientInspectorToolArgumentsWidget::canGenerateForm(schema));

    KMcpClientInspectorToolArgumentsWidget w;
    w.setInputSchema(schema);
    QVERIFY(!w.jsonEditorActivated());

    auto mFormLayout = w.findChild<QFormLayout *>(u"mFormLayout"_s);
    QCOMPARE(mFormLayout->rowCount(), 6);
    // Required arguments first
    QCOMPARE(mFormLayout->itemAt(0, QFormLayout::FieldRole)->widget()->objectName(), u"city"_s);
    QCOMPARE(mFormLayout->itemAt(1, QFormLayout::FieldRole)->widget()->objectName(), u"unit"_s);

    QVERIFY(w.findChild<QLineEdit *>(u"city"_s));
    QCOMPARE(w.findChild<QLineEdit *>(u"city"_s)->toolTip(), u"City name"_s);
    QCOMPARE(w.findChild<QLineEdit *>(u"days"_s)->text(), u"3"_s);
    QVERIFY(w.findChild<QLineEdit *>(u"ratio"_s));
    QVERIFY(w.findChild<QLineEdit *>(u"tags"_s));
    // Optional boolean has an empty entry
    QCOMPARE(w.findChild<QComboBox *>(u"metric"_s)->count(), 3);
    // Required enum has no empty entry
    QCOMPARE(w.findChild<QComboBox *>(u"unit"_s)->count(), 2);
}

void KMcpClientInspectorToolArgumentsWidgetTest::shouldReturnArgumentsFromForm()
{
    KMcpClientInspectorToolArgumentsWidget w;
    w.setInputSchema(schemaFromJson(formSchema));

    // Default values
    QJsonObject expected{{u"city"_s, QString()}, {u"unit"_s, u"celsius"_s}, {u"days"_s, 3}};
    QCOMPARE(w.arguments(), expected);

    w.findChild<QLineEdit *>(u"city"_s)->setText(u"Paris"_s);
    w.findChild<QLineEdit *>(u"ratio"_s)->setText(u"1.5"_s);
    w.findChild<QComboBox *>(u"metric"_s)->setCurrentIndex(2);
    w.findChild<QComboBox *>(u"unit"_s)->setCurrentIndex(1);
    w.findChild<QLineEdit *>(u"tags"_s)->setText(u"[\"a\", \"b\"]"_s);
    w.findChild<QLineEdit *>(u"days"_s)->clear();

    expected =
        QJsonObject{{u"city"_s, u"Paris"_s}, {u"unit"_s, u"fahrenheit"_s}, {u"ratio"_s, 1.5}, {u"metric"_s, false}, {u"tags"_s, QJsonArray{u"a"_s, u"b"_s}}};
    QCOMPARE(w.arguments(), expected);
}

void KMcpClientInspectorToolArgumentsWidgetTest::shouldReportMissingRequiredArgument()
{
    KMcpClientInspectorToolArgumentsWidget w;
    w.setInputSchema(schemaFromJson(R"({"type": "object", "properties": {"count": {"type": "integer"}}, "required": ["count"]})"));
    QString errorMessage;
    QVERIFY(!w.arguments(&errorMessage).has_value());
    QVERIFY(errorMessage.contains(u"count"_s));

    w.findChild<QLineEdit *>(u"count"_s)->setText(u"42"_s);
    QCOMPARE(w.arguments(), QJsonObject({{u"count"_s, 42}}));
}

void KMcpClientInspectorToolArgumentsWidgetTest::shouldReportInvalidNumber()
{
    KMcpClientInspectorToolArgumentsWidget w;
    w.setInputSchema(schemaFromJson(R"({"type": "object", "properties": {"count": {"type": "integer"}, "obj": {"type": "object"}}})"));
    w.findChild<QLineEdit *>(u"count"_s)->setText(u"foo"_s);
    QString errorMessage;
    QVERIFY(!w.arguments(&errorMessage).has_value());
    QVERIFY(errorMessage.contains(u"count"_s));

    w.findChild<QLineEdit *>(u"count"_s)->clear();
    w.findChild<QLineEdit *>(u"obj"_s)->setText(u"{bla"_s);
    QVERIFY(!w.arguments(&errorMessage).has_value());
    QVERIFY(errorMessage.contains(u"obj"_s));
}

void KMcpClientInspectorToolArgumentsWidgetTest::shouldFallbackToJsonEditor()
{
    const McpProtocolTool::InputSchema schema =
        schemaFromJson(R"({"type": "object", "properties": {"a": {"type": "string"}}, "oneOf": [{"required": ["a"]}, {"required": ["b"]}]})");
    QVERIFY(!KMcpClientInspectorToolArgumentsWidget::canGenerateForm(schema));
    QVERIFY(!KMcpClientInspectorToolArgumentsWidget::canGenerateForm(schemaFromJson(R"({"type": "object", "additionalProperties": true})")));
    QVERIFY(KMcpClientInspectorToolArgumentsWidget::canGenerateForm(schemaFromJson(R"({"type": "object"})")));

    KMcpClientInspectorToolArgumentsWidget w;
    w.setInputSchema(schema);
    QVERIFY(w.jsonEditorActivated());
    QVERIFY(!w.findChild<QCheckBox *>(u"mJsonEditorCheckBox"_s)->isEnabled());
    QCOMPARE(w.findChild<QStackedWidget *>(u"mStackedWidget"_s)->currentIndex(), 1);
    // Template with known properties
    QCOMPARE(w.arguments(), QJsonObject({{u"a"_s, QJsonValue()}}));

    // Back to form for a simple schema
    w.setInputSchema(schemaFromJson(R"({"type": "object"})"));
    QVERIFY(!w.jsonEditorActivated());
    QVERIFY(w.findChild<QCheckBox *>(u"mJsonEditorCheckBox"_s)->isEnabled());
    QVERIFY(w.findChild<QLabel *>(u"mNoArgumentsLabel"_s)->isVisibleTo(&w));
}

void KMcpClientInspectorToolArgumentsWidgetTest::shouldKeepValuesWhenSwitchingEditor()
{
    KMcpClientInspectorToolArgumentsWidget w;
    w.setInputSchema(schemaFromJson(formSchema));
    w.findChild<QLineEdit *>(u"city"_s)->setText(u"Paris"_s);
    const QJsonObject formArgs = *w.arguments();

    w.setJsonEditorActivated(true);
    QCOMPARE(w.findChild<QStackedWidget *>(u"mStackedWidget"_s)->currentIndex(), 1);
    QCOMPARE(w.arguments(), formArgs);

    w.setArguments(QJsonObject{{u"city"_s, u"Lyon"_s}, {u"unit"_s, u"fahrenheit"_s}, {u"metric"_s, true}});
    w.setJsonEditorActivated(false);
    QCOMPARE(w.findChild<QStackedWidget *>(u"mStackedWidget"_s)->currentIndex(), 0);
    QCOMPARE(w.arguments(), QJsonObject({{u"city"_s, u"Lyon"_s}, {u"unit"_s, u"fahrenheit"_s}, {u"metric"_s, true}}));
}

void KMcpClientInspectorToolArgumentsWidgetTest::shouldReportInvalidJson()
{
    KMcpClientInspectorToolArgumentsWidget w;
    w.setJsonEditorActivated(true);
    auto mJsonEditor = w.findChild<JsonPlainTextEditWidget *>(u"mJsonEditor"_s);
    QVERIFY(mJsonEditor);

    // Empty text means no arguments
    mJsonEditor->setPlainText(QString());
    QCOMPARE(w.arguments(), QJsonObject());

    QString errorMessage;
    mJsonEditor->setPlainText(u"{\"a\": "_s);
    QVERIFY(!w.arguments(&errorMessage).has_value());
    QVERIFY(!errorMessage.isEmpty());

    // Not an object
    errorMessage.clear();
    mJsonEditor->setPlainText(u"[1, 2]"_s);
    QVERIFY(!w.arguments(&errorMessage).has_value());
    QVERIFY(!errorMessage.isEmpty());

    mJsonEditor->setPlainText(u"{\"a\": [1, 2]}"_s);
    QCOMPARE(w.arguments(), QJsonObject({{u"a"_s, QJsonArray{1, 2}}}));
}

#include "moc_kmcpclientinspectortoolargumentswidgettest.cpp"
