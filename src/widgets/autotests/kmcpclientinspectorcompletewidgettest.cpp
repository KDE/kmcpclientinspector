/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorcompletewidgettest.h"
#include "actions/kmcpclientinspectorcompletewidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <QComboBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QTest>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolCompleteRequestParams>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolPromptReference>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolResourceTemplateReference>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
QTEST_MAIN(KMcpClientInspectorCompleteWidgetTest)
using namespace Qt::Literals::StringLiterals;
using namespace TextAutoGenerateTextMcpProtocolCore;

namespace
{
QJsonObject jsonFromText(const QByteArray &json)
{
    return QJsonDocument::fromJson(json).object();
}

const QByteArray promptsListResponse = R"({
    "jsonrpc": "2.0",
    "id": 2,
    "result": {
        "prompts": [
            {"name": "greeting"},
            {"name": "code_review", "arguments": [{"name": "language"}, {"name": "code"}]}
        ]
    }
})";

const QByteArray resourceTemplatesListResponse = R"({
    "jsonrpc": "2.0",
    "id": 3,
    "result": {
        "resourceTemplates": [
            {"name": "files", "uriTemplate": "file:///{path}{?query,page}"}
        ]
    }
})";
}

KMcpClientInspectorCompleteWidgetTest::KMcpClientInspectorCompleteWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorCompleteWidgetTest::shouldHaveDefaultValues()
{
    McpProtocolClientProtocolManager manager(McpServer{});
    KMcpClientInspectorCompleteWidget w(&manager);
    QVERIFY(w.prompts().isEmpty());
    QVERIFY(w.resourceTemplates().isEmpty());

    auto mReferenceTypeComboBox = w.findChild<QComboBox *>(u"mReferenceTypeComboBox"_s);
    QVERIFY(mReferenceTypeComboBox);
    QCOMPARE(mReferenceTypeComboBox->count(), 2);
    QCOMPARE(mReferenceTypeComboBox->currentIndex(), 0);

    auto mReferenceComboBox = w.findChild<QComboBox *>(u"mReferenceComboBox"_s);
    QVERIFY(mReferenceComboBox);
    QVERIFY(mReferenceComboBox->isEditable());
    QCOMPARE(mReferenceComboBox->count(), 0);

    auto mArgumentNameComboBox = w.findChild<QComboBox *>(u"mArgumentNameComboBox"_s);
    QVERIFY(mArgumentNameComboBox);
    QVERIFY(mArgumentNameComboBox->isEditable());
    QCOMPARE(mArgumentNameComboBox->count(), 0);

    auto mArgumentValueLineEdit = w.findChild<QLineEdit *>(u"mArgumentValueLineEdit"_s);
    QVERIFY(mArgumentValueLineEdit);
    QVERIFY(mArgumentValueLineEdit->text().isEmpty());
    QVERIFY(mArgumentValueLineEdit->isClearButtonEnabled());

    auto mContextLineEdit = w.findChild<QLineEdit *>(u"mContextLineEdit"_s);
    QVERIFY(mContextLineEdit);
    QVERIFY(mContextLineEdit->text().isEmpty());
    QVERIFY(!mContextLineEdit->placeholderText().isEmpty());

    auto mCompleteButton = w.findChild<QPushButton *>(u"mCompleteButton"_s);
    QVERIFY(mCompleteButton);
    QVERIFY(!mCompleteButton->text().isEmpty());
    QVERIFY(!mCompleteButton->isEnabled());

    auto mStatusLabel = w.findChild<QLabel *>(u"mStatusLabel"_s);
    QVERIFY(mStatusLabel);
    QVERIFY(mStatusLabel->text().isEmpty());

    auto mResultTabWidget = w.findChild<QTabWidget *>(u"mResultTabWidget"_s);
    QVERIFY(mResultTabWidget);
    QCOMPARE(mResultTabWidget->count(), 2);

    auto mValuesListWidget = w.findChild<QListWidget *>(u"mValuesListWidget"_s);
    QVERIFY(mValuesListWidget);
    QCOMPARE(mValuesListWidget->count(), 0);

    auto mJsonPlainTextEditWidget = w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s);
    QVERIFY(mJsonPlainTextEditWidget);
}

void KMcpClientInspectorCompleteWidgetTest::shouldUpdatePromptsList()
{
    McpProtocolClientProtocolManager manager(McpServer{});
    KMcpClientInspectorCompleteWidget w(&manager);
    w.setPromptsList(jsonFromText(promptsListResponse));
    QCOMPARE(w.prompts().count(), 2);

    auto mReferenceComboBox = w.findChild<QComboBox *>(u"mReferenceComboBox"_s);
    QCOMPARE(mReferenceComboBox->count(), 2);
    // Sorted
    QCOMPARE(mReferenceComboBox->itemText(0), u"code_review"_s);
    QCOMPARE(mReferenceComboBox->itemText(1), u"greeting"_s);
    QCOMPARE(mReferenceComboBox->currentText(), u"code_review"_s);

    auto mArgumentNameComboBox = w.findChild<QComboBox *>(u"mArgumentNameComboBox"_s);
    QCOMPARE(mArgumentNameComboBox->count(), 2);
    QCOMPARE(mArgumentNameComboBox->itemText(0), u"language"_s);
    QCOMPARE(mArgumentNameComboBox->itemText(1), u"code"_s);

    mReferenceComboBox->setCurrentIndex(1);
    QCOMPARE(mArgumentNameComboBox->count(), 0);

    // Error response keeps list
    w.setPromptsList(jsonFromText(R"({"jsonrpc": "2.0", "id": 4, "error": {"code": -32601, "message": "Method not found"}})"));
    QCOMPARE(w.prompts().count(), 2);
}

void KMcpClientInspectorCompleteWidgetTest::shouldUpdateResourceTemplatesList()
{
    McpProtocolClientProtocolManager manager(McpServer{});
    KMcpClientInspectorCompleteWidget w(&manager);
    w.setPromptsList(jsonFromText(promptsListResponse));
    w.setResourceTemplatesList(jsonFromText(resourceTemplatesListResponse));
    QCOMPARE(w.resourceTemplates().count(), 1);

    auto mReferenceComboBox = w.findChild<QComboBox *>(u"mReferenceComboBox"_s);
    // Prompt reference type is still selected
    QCOMPARE(mReferenceComboBox->count(), 2);

    auto mReferenceTypeComboBox = w.findChild<QComboBox *>(u"mReferenceTypeComboBox"_s);
    mReferenceTypeComboBox->setCurrentIndex(1);
    QCOMPARE(mReferenceComboBox->count(), 1);
    QCOMPARE(mReferenceComboBox->currentText(), u"file:///{path}{?query,page}"_s);

    auto mArgumentNameComboBox = w.findChild<QComboBox *>(u"mArgumentNameComboBox"_s);
    QCOMPARE(mArgumentNameComboBox->count(), 3);
    QCOMPARE(mArgumentNameComboBox->itemText(0), u"path"_s);
    QCOMPARE(mArgumentNameComboBox->itemText(1), u"query"_s);
    QCOMPARE(mArgumentNameComboBox->itemText(2), u"page"_s);
}

void KMcpClientInspectorCompleteWidgetTest::shouldCreateRequestParams()
{
    McpProtocolClientProtocolManager manager(McpServer{});
    KMcpClientInspectorCompleteWidget w(&manager);
    QString errorMessage;
    QVERIFY(!w.requestParams(errorMessage).has_value());
    QVERIFY(!errorMessage.isEmpty());

    w.setPromptsList(jsonFromText(promptsListResponse));
    auto mArgumentValueLineEdit = w.findChild<QLineEdit *>(u"mArgumentValueLineEdit"_s);
    mArgumentValueLineEdit->setText(u"py"_s);

    errorMessage.clear();
    std::optional<McpProtocolCompleteRequestParams> params = w.requestParams(errorMessage);
    QVERIFY(params.has_value());
    QVERIFY(errorMessage.isEmpty());
    QVERIFY(std::holds_alternative<McpProtocolPromptReference>(params->ref()));
    QCOMPARE(std::get<McpProtocolPromptReference>(params->ref()).name(), u"code_review"_s);
    QCOMPARE(params->argument().name(), u"language"_s);
    QCOMPARE(params->argument().value(), u"py"_s);
    QVERIFY(!params->context().has_value());

    auto mContextLineEdit = w.findChild<QLineEdit *>(u"mContextLineEdit"_s);
    mContextLineEdit->setText(u"not json"_s);
    QVERIFY(!w.requestParams(errorMessage).has_value());
    QVERIFY(!errorMessage.isEmpty());

    errorMessage.clear();
    mContextLineEdit->setText(u"{\"code\": 42}"_s);
    QVERIFY(!w.requestParams(errorMessage).has_value());
    QVERIFY(!errorMessage.isEmpty());

    errorMessage.clear();
    mContextLineEdit->setText(u"{\"code\": \"print()\"}"_s);
    params = w.requestParams(errorMessage);
    QVERIFY(params.has_value());
    QVERIFY(params->context().has_value());
    const QMap<QString, QString> contextArguments = params->context()->arguments().value_or(QMap<QString, QString>{});
    QCOMPARE(contextArguments, (QMap<QString, QString>{{u"code"_s, u"print()"_s}}));

    w.setResourceTemplatesList(jsonFromText(resourceTemplatesListResponse));
    auto mReferenceTypeComboBox = w.findChild<QComboBox *>(u"mReferenceTypeComboBox"_s);
    mReferenceTypeComboBox->setCurrentIndex(1);
    mContextLineEdit->clear();
    params = w.requestParams(errorMessage);
    QVERIFY(params.has_value());
    QVERIFY(std::holds_alternative<McpProtocolResourceTemplateReference>(params->ref()));
    QCOMPARE(std::get<McpProtocolResourceTemplateReference>(params->ref()).uri(), u"file:///{path}{?query,page}"_s);
    QCOMPARE(params->argument().name(), u"path"_s);
}

void KMcpClientInspectorCompleteWidgetTest::shouldShowResult()
{
    McpProtocolClientProtocolManager manager(McpServer{});
    KMcpClientInspectorCompleteWidget w(&manager);
    w.setResult(jsonFromText(R"({
        "jsonrpc": "2.0",
        "id": 5,
        "result": {"completion": {"values": ["python", "pytorch"], "total": 10, "hasMore": true}}
    })"));

    auto mValuesListWidget = w.findChild<QListWidget *>(u"mValuesListWidget"_s);
    QCOMPARE(mValuesListWidget->count(), 2);
    QCOMPARE(mValuesListWidget->item(0)->text(), u"python"_s);
    QCOMPARE(mValuesListWidget->item(1)->text(), u"pytorch"_s);

    auto mStatusLabel = w.findChild<QLabel *>(u"mStatusLabel"_s);
    QVERIFY(!mStatusLabel->text().isEmpty());

    auto mResultTabWidget = w.findChild<QTabWidget *>(u"mResultTabWidget"_s);
    QCOMPARE(mResultTabWidget->currentWidget(), mValuesListWidget);

    auto mJsonPlainTextEditWidget = w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s);
    QVERIFY(mJsonPlainTextEditWidget->toPlainText().contains(u"pytorch"_s));

    // Double-click uses value
    Q_EMIT mValuesListWidget->itemDoubleClicked(mValuesListWidget->item(1));
    auto mArgumentValueLineEdit = w.findChild<QLineEdit *>(u"mArgumentValueLineEdit"_s);
    QCOMPARE(mArgumentValueLineEdit->text(), u"pytorch"_s);
}

void KMcpClientInspectorCompleteWidgetTest::shouldShowError()
{
    McpProtocolClientProtocolManager manager(McpServer{});
    KMcpClientInspectorCompleteWidget w(&manager);
    w.setResult(jsonFromText(R"({"jsonrpc": "2.0", "id": 5, "error": {"code": -32602, "message": "Invalid params"}})"));

    auto mValuesListWidget = w.findChild<QListWidget *>(u"mValuesListWidget"_s);
    QCOMPARE(mValuesListWidget->count(), 0);

    auto mStatusLabel = w.findChild<QLabel *>(u"mStatusLabel"_s);
    QVERIFY(mStatusLabel->text().contains(u"Invalid params"_s));

    auto mResultTabWidget = w.findChild<QTabWidget *>(u"mResultTabWidget"_s);
    auto mJsonPlainTextEditWidget = w.findChild<JsonPlainTextEditWidget *>(u"mJsonPlainTextEditWidget"_s);
    QCOMPARE(mResultTabWidget->currentWidget(), mJsonPlainTextEditWidget);
}

void KMcpClientInspectorCompleteWidgetTest::shouldExtractUriTemplateVariables_data()
{
    QTest::addColumn<QString>("uriTemplate");
    QTest::addColumn<QStringList>("variables");

    QTest::newRow("empty") << QString() << QStringList();
    QTest::newRow("novariable") << u"file:///foo"_s << QStringList();
    QTest::newRow("simple") << u"file:///{path}"_s << QStringList{u"path"_s};
    QTest::newRow("query") << u"file:///{path}{?query,page}"_s << QStringList{u"path"_s, u"query"_s, u"page"_s};
    QTest::newRow("modifiers") << u"repo://{+owner}/{name:3}{/segments*}"_s << QStringList{u"owner"_s, u"name"_s, u"segments"_s};
    QTest::newRow("duplicate") << u"a://{x}/{x}"_s << QStringList{u"x"_s};
}

void KMcpClientInspectorCompleteWidgetTest::shouldExtractUriTemplateVariables()
{
    QFETCH(QString, uriTemplate);
    QFETCH(QStringList, variables);
    QCOMPARE(KMcpClientInspectorCompleteWidget::uriTemplateVariables(uriTemplate), variables);
}

#include "moc_kmcpclientinspectorcompletewidgettest.cpp"
