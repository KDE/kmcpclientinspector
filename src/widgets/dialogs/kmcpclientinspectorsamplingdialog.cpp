/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorsamplingdialog.h"
#include "misc/jsonplaintexteditwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QJsonDocument>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolCreateMessageRequestParams>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolCreateMessageResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTextContent>
using namespace Qt::Literals::StringLiterals;
using namespace TextAutoGenerateTextMcpProtocolCore;
KMcpClientInspectorSamplingDialog::KMcpClientInspectorSamplingDialog(const McpProtocolCreateMessageRequestParams &params, QWidget *parent)
    : QDialog(parent)
    , mRequestWidget(new JsonPlainTextEditWidget(this))
    , mModelLineEdit(new QLineEdit(u"kmcpclientinspector"_s, this))
    , mStopReasonLineEdit(new QLineEdit(u"endTurn"_s, this))
    , mResponseTextEdit(new QPlainTextEdit(this))
{
    setWindowTitle(i18nc("@title:window", "Server Requests Message Generation"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    auto requestLabel = new QLabel(i18nc("@label", "Request:"), this);
    requestLabel->setObjectName(u"requestLabel"_s);
    mainLayout->addWidget(requestLabel);

    mRequestWidget->setObjectName(u"mRequestWidget"_s);
    mRequestWidget->setReadOnly(true);
    mRequestWidget->setPlainText(QString::fromUtf8(QJsonDocument(McpProtocolCreateMessageRequestParams::toJson(params)).toJson(QJsonDocument::Indented)));
    mainLayout->addWidget(mRequestWidget, 1);

    auto formLayout = new QFormLayout;
    formLayout->setObjectName(u"formLayout"_s);
    mModelLineEdit->setObjectName(u"mModelLineEdit"_s);
    mModelLineEdit->setClearButtonEnabled(true);
    formLayout->addRow(i18nc("@label:textbox", "Model:"), mModelLineEdit);
    mStopReasonLineEdit->setObjectName(u"mStopReasonLineEdit"_s);
    mStopReasonLineEdit->setClearButtonEnabled(true);
    formLayout->addRow(i18nc("@label:textbox", "Stop reason:"), mStopReasonLineEdit);
    mainLayout->addLayout(formLayout);

    auto responseLabel = new QLabel(i18nc("@label:textbox", "Response:"), this);
    responseLabel->setObjectName(u"responseLabel"_s);
    mainLayout->addWidget(responseLabel);

    mResponseTextEdit->setObjectName(u"mResponseTextEdit"_s);
    mResponseTextEdit->setPlaceholderText(i18nc("@info:placeholder", "Write the message generated for the server…"));
    mainLayout->addWidget(mResponseTextEdit, 1);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(u"buttonBox"_s);
    auto sendButton = buttonBox->button(QDialogButtonBox::Ok);
    sendButton->setText(i18nc("@action:button", "Send"));
    sendButton->setEnabled(false);
    buttonBox->button(QDialogButtonBox::Cancel)->setText(i18nc("@action:button", "Reject"));
    mainLayout->addWidget(buttonBox);

    connect(mResponseTextEdit, &QPlainTextEdit::textChanged, this, [this, sendButton]() {
        sendButton->setEnabled(!mResponseTextEdit->toPlainText().trimmed().isEmpty());
    });
    connect(buttonBox, &QDialogButtonBox::accepted, this, &KMcpClientInspectorSamplingDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &KMcpClientInspectorSamplingDialog::reject);
    resize(600, 500);
}

KMcpClientInspectorSamplingDialog::~KMcpClientInspectorSamplingDialog() = default;

McpProtocolCreateMessageResult KMcpClientInspectorSamplingDialog::createMessageResult() const
{
    McpProtocolTextContent content;
    content.setText(mResponseTextEdit->toPlainText());
    McpProtocolCreateMessageResult result;
    result.setContent(content);
    result.setRole(McpProtocolUtils::Role::Assistant);
    result.setModel(mModelLineEdit->text().trimmed());
    if (const QString stopReason = mStopReasonLineEdit->text().trimmed(); !stopReason.isEmpty()) {
        result.setStopReason(stopReason);
    }
    return result;
}

#include "moc_kmcpclientinspectorsamplingdialog.cpp"
