/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "libkmcpclientinspectorwidgets_private_export.h"
#include <TextCustomEditor/PlainTextEditor>
class QMenu;
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

protected:
    void addExtraMenuEntry(QMenu *menu, QPoint pos) override;

private:
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void slotSaveAs();
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

    void clear();

private:
    TextCustomEditor::PlainTextEditorWidget *const mPlainTextEdit;
};
