/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libkmcpclientinspectorwidgets_private_export.h"
#include <KSyntaxHighlighting/Repository>
#include <TextCustomEditor/PlainTextEditor>
namespace TextCustomEditor
{
class PlainTextEditorWidget;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT JsonPlainTextEditor : public TextCustomEditor::PlainTextEditor
{
    Q_OBJECT
public:
    explicit JsonPlainTextEditor(QWidget *parent = nullptr);
    ~JsonPlainTextEditor() override;
};

class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT JsonPlainTextEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit JsonPlainTextEditWidget(QWidget *parent = nullptr);
    ~JsonPlainTextEditWidget() override;

    void setPlainText(const QString &str);
    [[nodiscard]] QString toPlainText() const;

    void setReadOnly(bool readOnly);

private:
    TextCustomEditor::PlainTextEditorWidget *const mPlainTextEdit;
    KSyntaxHighlighting::Repository mRepo;
};
