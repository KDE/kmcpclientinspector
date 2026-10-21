/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorpromptargumentswidget.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
using namespace TextAutoGenerateTextMcpProtocolCore;

KMcpClientInspectorPromptArgumentsWidget::KMcpClientInspectorPromptArgumentsWidget(QWidget *parent)
    : QWidget{parent}
    , mFormWidget(new QWidget(this))
    , mFormLayout(new QFormLayout(mFormWidget))
    , mNoArgumentsLabel(new QLabel(i18n("This prompt has no arguments."), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mNoArgumentsLabel->setObjectName(u"mNoArgumentsLabel"_s);
    mainLayout->addWidget(mNoArgumentsLabel);

    mFormWidget->setObjectName(u"mFormWidget"_s);
    mFormLayout->setObjectName(u"mFormLayout"_s);
    auto scrollArea = new QScrollArea(this);
    scrollArea->setObjectName(u"scrollArea"_s);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(mFormWidget);
    mainLayout->addWidget(scrollArea);
    clear();
}

KMcpClientInspectorPromptArgumentsWidget::~KMcpClientInspectorPromptArgumentsWidget() = default;

void KMcpClientInspectorPromptArgumentsWidget::clear()
{
    while (mFormLayout->rowCount() > 0) {
        mFormLayout->removeRow(0);
    }
    mFields.clear();
    mNoArgumentsLabel->setVisible(true);
}

void KMcpClientInspectorPromptArgumentsWidget::setPromptArguments(const QList<McpProtocolPromptArgument> &arguments)
{
    clear();
    // Required arguments first
    QList<McpProtocolPromptArgument> sortedArguments;
    sortedArguments.reserve(arguments.count());
    for (const McpProtocolPromptArgument &argument : arguments) {
        if (argument.required().value_or(false)) {
            sortedArguments.append(argument);
        }
    }
    for (const McpProtocolPromptArgument &argument : arguments) {
        if (!argument.required().value_or(false)) {
            sortedArguments.append(argument);
        }
    }

    for (const McpProtocolPromptArgument &argument : std::as_const(sortedArguments)) {
        if (argument.name().isEmpty()) {
            continue;
        }
        Field field;
        field.name = argument.name();
        field.required = argument.required().value_or(false);
        field.lineEdit = new QLineEdit(mFormWidget);
        field.lineEdit->setObjectName(field.name);
        field.lineEdit->setClearButtonEnabled(true);
        const QString description = argument.description().value_or(QString());
        field.lineEdit->setPlaceholderText(description);
        field.lineEdit->setToolTip(description);

        const QString title = argument.title().value_or(QString());
        const QString labelText = (title.isEmpty() || title == field.name) ? field.name : u"%1 (%2)"_s.arg(title, field.name);
        auto label = new QLabel(field.required ? u"%1*:"_s.arg(labelText) : u"%1:"_s.arg(labelText), mFormWidget);
        label->setToolTip(description);
        mFormLayout->addRow(label, field.lineEdit);
        mFields.append(field);
    }
    mNoArgumentsLabel->setVisible(mFields.isEmpty());
}

QMap<QString, QString> KMcpClientInspectorPromptArgumentsWidget::arguments() const
{
    QMap<QString, QString> args;
    for (const Field &field : std::as_const(mFields)) {
        const QString text = field.lineEdit->text();
        if (!text.isEmpty() || field.required) {
            args.insert(field.name, text);
        }
    }
    return args;
}

void KMcpClientInspectorPromptArgumentsWidget::setArguments(const QMap<QString, QString> &arguments)
{
    for (const Field &field : std::as_const(mFields)) {
        field.lineEdit->setText(arguments.value(field.name));
    }
}

QStringList KMcpClientInspectorPromptArgumentsWidget::missingRequiredArguments() const
{
    QStringList names;
    for (const Field &field : std::as_const(mFields)) {
        if (field.required && field.lineEdit->text().isEmpty()) {
            names.append(field.name);
        }
    }
    return names;
}

#include "moc_kmcpclientinspectorpromptargumentswidget.cpp"
