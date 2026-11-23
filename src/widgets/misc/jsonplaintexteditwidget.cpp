/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "jsonplaintexteditwidget.h"
#include "kmcpclientinspector_widget_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <KStandardActions>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>
#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <QPointer>
#include <QVBoxLayout>
#include <TextCustomEditor/PlainTextEditorWidget>
using namespace Qt::Literals::StringLiterals;

Q_GLOBAL_STATIC(KSyntaxHighlighting::Repository, s_syntaxRepository)

namespace
{
[[nodiscard]] bool saveToFile(const QString &fileName, const QString &text, QString &errorStr)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        errorStr = file.errorString();
        return false;
    }
    if (file.write(text.toUtf8()) == -1) {
        errorStr = file.errorString();
        return false;
    }
    return true;
}

void saveTextAs(const QString &text, const QString &filter, QWidget *parent)
{
    QPointer<QFileDialog> fdlg(new QFileDialog(parent, QString(), QString(), filter));
    fdlg->setAcceptMode(QFileDialog::AcceptSave);
    if (fdlg->exec() == QDialog::Accepted && fdlg) {
        const QString fileName = fdlg->selectedFiles().at(0);
        QString errorStr;
        if (!saveToFile(fileName, text, errorStr)) {
            KMessageBox::error(parent,
                               i18n("Could not write the file %1:\n"
                                    "\"%2\" is the detailed error description.",
                                    fileName,
                                    errorStr),
                               i18nc("@title:window", "Save File Error"));
        }
    }
    delete fdlg;
}
}

JsonPlainTextEditor::JsonPlainTextEditor(QWidget *parent)
    : TextCustomEditor::PlainTextEditor(parent)
{
    setReadOnly(true);
    setSearchSupport(true);
}

void JsonPlainTextEditor::addExtraMenuEntry(QMenu *menu, QPoint pos)
{
    TextCustomEditor::PlainTextEditor::addExtraMenuEntry(menu, pos);
    if (isReadOnly() && !document()->isEmpty()) {
        menu->addSeparator();
        QAction *saveAsAction = KStandardActions::saveAs(this, &JsonPlainTextEditor::slotSaveAs, menu);
        menu->addAction(saveAsAction);
    }
}

void JsonPlainTextEditor::slotSaveAs()
{
    const QString filter = i18n("JSON Files (*.json);;All Files (*)");
    saveTextAs(toPlainText(), filter, this);
}

JsonPlainTextEditor::~JsonPlainTextEditor() = default;

JsonPlainTextEditWidget::JsonPlainTextEditWidget(QWidget *parent)
    : QWidget{parent}
    , mPlainTextEdit(new TextCustomEditor::PlainTextEditorWidget(new JsonPlainTextEditor(this), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mPlainTextEdit->setObjectName(u"mPlainTextEdit"_s);
    mPlainTextEdit->setReadOnly(true);
    mainLayout->addWidget(mPlainTextEdit);

    const KSyntaxHighlighting::Definition def = s_syntaxRepository->definitionForName(u"Json"_s);
    if (!def.isValid()) {
        qCWarning(KMCPCLIENTINSPECTOR_WIDGET_LOG) << "Invalid definition name";
    }

    auto hl = new KSyntaxHighlighting::SyntaxHighlighter(mPlainTextEdit->editor()->document());
    hl->setTheme((palette().color(QPalette::Base).lightness() < 128) ? s_syntaxRepository->defaultTheme(KSyntaxHighlighting::Repository::DarkTheme)
                                                                     : s_syntaxRepository->defaultTheme(KSyntaxHighlighting::Repository::LightTheme));
    hl->setDefinition(def);
}

JsonPlainTextEditWidget::~JsonPlainTextEditWidget() = default;

void JsonPlainTextEditWidget::setPlainText(const QString &str)
{
    mPlainTextEdit->setPlainText(str);
}

QString JsonPlainTextEditWidget::toPlainText() const
{
    return mPlainTextEdit->toPlainText();
}

void JsonPlainTextEditWidget::setReadOnly(bool readOnly)
{
    mPlainTextEdit->setReadOnly(readOnly);
}

void JsonPlainTextEditWidget::clear()
{
    mPlainTextEdit->clear();
}

#include "moc_jsonplaintexteditwidget.cpp"
