/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "jsonplaintexteditwidgettest.h"
#include "misc/jsonplaintexteditwidget.h"
#include <QTest>
#include <QVBoxLayout>
#include <TextCustomEditor/PlainTextEditorWidget>
QTEST_MAIN(JsonPlainTextEditWidgetTest)
using namespace Qt::Literals::StringLiterals;
JsonPlainTextEditWidgetTest::JsonPlainTextEditWidgetTest(QObject *parent)
    : QObject{parent}
{
}

void JsonPlainTextEditWidgetTest::shouldHaveDefaultValues()
{
    const JsonPlainTextEditWidget w;
    QVERIFY(w.toPlainText().isEmpty());

    auto mainLayout = w.findChild<QVBoxLayout *>(u"mainLayout"_s);
    QVERIFY(mainLayout);
    QCOMPARE(mainLayout->contentsMargins(), QMargins());

    auto mPlainTextEdit = w.findChild<TextCustomEditor::PlainTextEditorWidget *>(u"mPlainTextEdit"_s);
    QVERIFY(mPlainTextEdit);
    QVERIFY(mPlainTextEdit->isReadOnly());
    QVERIFY(qobject_cast<JsonPlainTextEditor *>(mPlainTextEdit->editor()));
}

void JsonPlainTextEditWidgetTest::shouldChangeText()
{
    JsonPlainTextEditWidget w;
    const QString json = u"{\n    \"foo\": \"bar\"\n}\n"_s;
    w.setPlainText(json);
    QCOMPARE(w.toPlainText(), json);

    w.setPlainText({});
    QVERIFY(w.toPlainText().isEmpty());
}

void JsonPlainTextEditWidgetTest::shouldChangeReadOnly()
{
    JsonPlainTextEditWidget w;
    auto mPlainTextEdit = w.findChild<TextCustomEditor::PlainTextEditorWidget *>(u"mPlainTextEdit"_s);
    w.setReadOnly(false);
    QVERIFY(!mPlainTextEdit->isReadOnly());
    w.setReadOnly(true);
    QVERIFY(mPlainTextEdit->isReadOnly());
}

#include "moc_jsonplaintexteditwidgettest.cpp"
