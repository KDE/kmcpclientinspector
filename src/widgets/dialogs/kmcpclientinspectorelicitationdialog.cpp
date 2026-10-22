/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorelicitationdialog.h"
#include "actions/kmcpclientinspectortoolargumentswidget.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolElicitRequestFormParams>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolTool>
using namespace Qt::Literals::StringLiterals;
using namespace TextAutoGenerateTextMcpProtocolCore;
KMcpClientInspectorElicitationDialog::KMcpClientInspectorElicitationDialog(const McpProtocolElicitRequestFormParams &params, QWidget *parent)
    : QDialog(parent)
    , mMessageLabel(new QLabel(this))
    , mArgumentsWidget(new KMcpClientInspectorToolArgumentsWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Server Requests Information"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mMessageLabel->setObjectName(u"mMessageLabel"_s);
    mMessageLabel->setWordWrap(true);
    mMessageLabel->setTextFormat(Qt::PlainText);
    mMessageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mMessageLabel->setText(params.message());
    mainLayout->addWidget(mMessageLabel);

    mArgumentsWidget->setObjectName(u"mArgumentsWidget"_s);
    mArgumentsWidget->setNoArgumentsText(i18n("No information is requested."));
    // Requested schema is a restricted json schema: use same form as tool arguments
    mArgumentsWidget->setInputSchema(
        McpProtocolTool::InputSchema::fromJson(McpProtocolElicitRequestFormParams::RequestedSchema::toJson(params.requestedSchema())));
    mainLayout->addWidget(mArgumentsWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(u"buttonBox"_s);
    buttonBox->button(QDialogButtonBox::Ok)->setText(i18nc("@action:button", "Accept"));
    auto declineButton = buttonBox->addButton(i18nc("@action:button", "Decline"), QDialogButtonBox::RejectRole);
    declineButton->setObjectName(u"declineButton"_s);
    mainLayout->addWidget(buttonBox);

    connect(buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &KMcpClientInspectorElicitationDialog::slotAccept);
    connect(buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &KMcpClientInspectorElicitationDialog::reject);
    connect(declineButton, &QPushButton::clicked, this, &KMcpClientInspectorElicitationDialog::slotDecline);
    resize(500, 400);
}

KMcpClientInspectorElicitationDialog::~KMcpClientInspectorElicitationDialog() = default;

void KMcpClientInspectorElicitationDialog::slotAccept()
{
    QString errorMessage;
    const std::optional<QJsonObject> content = mArgumentsWidget->arguments(&errorMessage);
    if (!content.has_value()) {
        KMessageBox::error(this, errorMessage, i18nc("@title:window", "Invalid Information"));
        return;
    }
    mContent = *content;
    mAction = McpProtocolElicitResult::Action::Accept;
    accept();
}

void KMcpClientInspectorElicitationDialog::slotDecline()
{
    mAction = McpProtocolElicitResult::Action::Decline;
    reject();
}

McpProtocolElicitResult KMcpClientInspectorElicitationDialog::elicitResult() const
{
    if (mAction == McpProtocolElicitResult::Action::Accept) {
        // fromJson converts json values to ElicitResultContentValue
        return McpProtocolElicitResult::fromJson(QJsonObject{
            {u"action"_s, McpProtocolElicitResult::convertActionToString(mAction)},
            {u"content"_s, mContent},
        });
    }
    McpProtocolElicitResult result;
    result.setAction(mAction);
    return result;
}

#include "moc_kmcpclientinspectorelicitationdialog.cpp"
