/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorpromptargumentswidgettest.h"
#include "actions/kmcpclientinspectorpromptargumentswidget.h"
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTest>
QTEST_MAIN(KMcpClientInspectorPromptArgumentsWidgetTest)
using namespace Qt::Literals::StringLiterals;
using TextAutoGenerateTextMcpProtocolCore::McpProtocolPromptArgument;

namespace
{
McpProtocolPromptArgument createArgument(const QString &name, bool required, const QString &description = {})
{
    McpProtocolPromptArgument argument;
    argument.setName(name);
    argument.setRequired(required);
    if (!description.isEmpty()) {
        argument.setDescription(description);
    }
    return argument;
}
}

KMcpClientInspectorPromptArgumentsWidgetTest::KMcpClientInspectorPromptArgumentsWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void KMcpClientInspectorPromptArgumentsWidgetTest::shouldHaveDefaultValues()
{
    KMcpClientInspectorPromptArgumentsWidget w;
    QVERIFY(w.arguments().isEmpty());
    QVERIFY(w.missingRequiredArguments().isEmpty());

    auto mNoArgumentsLabel = w.findChild<QLabel *>(u"mNoArgumentsLabel"_s);
    QVERIFY(mNoArgumentsLabel);
    QVERIFY(!mNoArgumentsLabel->text().isEmpty());
    QVERIFY(!mNoArgumentsLabel->isHidden());

    auto mFormLayout = w.findChild<QFormLayout *>(u"mFormLayout"_s);
    QVERIFY(mFormLayout);
    QCOMPARE(mFormLayout->rowCount(), 0);
}

void KMcpClientInspectorPromptArgumentsWidgetTest::shouldCreateFields()
{
    KMcpClientInspectorPromptArgumentsWidget w;
    w.setPromptArguments({createArgument(u"language"_s, false), createArgument(u"code"_s, true, u"Code to review"_s)});

    auto mFormLayout = w.findChild<QFormLayout *>(u"mFormLayout"_s);
    QCOMPARE(mFormLayout->rowCount(), 2);
    QVERIFY(w.findChild<QLabel *>(u"mNoArgumentsLabel"_s)->isHidden());

    // Required arguments first
    auto codeLabel = qobject_cast<QLabel *>(mFormLayout->itemAt(0, QFormLayout::LabelRole)->widget());
    QCOMPARE(codeLabel->text(), u"code*:"_s);
    auto code = qobject_cast<QLineEdit *>(mFormLayout->itemAt(0, QFormLayout::FieldRole)->widget());
    QCOMPARE(code->objectName(), u"code"_s);
    QCOMPARE(code->placeholderText(), u"Code to review"_s);
    auto languageLabel = qobject_cast<QLabel *>(mFormLayout->itemAt(1, QFormLayout::LabelRole)->widget());
    QCOMPARE(languageLabel->text(), u"language:"_s);

    // No arguments
    w.setPromptArguments({});
    QCOMPARE(mFormLayout->rowCount(), 0);
    QVERIFY(!w.findChild<QLabel *>(u"mNoArgumentsLabel"_s)->isHidden());
}

void KMcpClientInspectorPromptArgumentsWidgetTest::shouldReturnArguments()
{
    KMcpClientInspectorPromptArgumentsWidget w;
    w.setPromptArguments({createArgument(u"code"_s, true), createArgument(u"language"_s, false)});
    QCOMPARE(w.missingRequiredArguments(), QStringList{u"code"_s});
    // Empty optional argument is not sent
    QCOMPARE(w.arguments(), (QMap<QString, QString>{{u"code"_s, QString()}}));

    w.setArguments({{u"code"_s, u"int main() {}"_s}, {u"language"_s, u"C++"_s}});
    QVERIFY(w.missingRequiredArguments().isEmpty());
    QCOMPARE(w.arguments(), (QMap<QString, QString>{{u"code"_s, u"int main() {}"_s}, {u"language"_s, u"C++"_s}}));
}

#include "moc_kmcpclientinspectorpromptargumentswidgettest.cpp"
