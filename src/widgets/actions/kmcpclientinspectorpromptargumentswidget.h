/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QList>
#include <QMap>
#include <QWidget>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolPromptArgument>
class QFormLayout;
class QLabel;
class QLineEdit;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorPromptArgumentsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorPromptArgumentsWidget(QWidget *parent = nullptr);
    ~KMcpClientInspectorPromptArgumentsWidget() override;

    void setPromptArguments(const QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolPromptArgument> &arguments);
    void clear();

    /*!
     * Return arguments to send with prompts/get. Empty optional arguments are not added.
     */
    [[nodiscard]] QMap<QString, QString> arguments() const;
    void setArguments(const QMap<QString, QString> &arguments);

    /*!
     * Return names of required arguments without value.
     */
    [[nodiscard]] QStringList missingRequiredArguments() const;

private:
    struct Field {
        QString name;
        bool required = false;
        QLineEdit *lineEdit = nullptr;
    };
    QList<Field> mFields;
    QWidget *const mFormWidget;
    QFormLayout *const mFormLayout;
    QLabel *const mNoArgumentsLabel;
};
