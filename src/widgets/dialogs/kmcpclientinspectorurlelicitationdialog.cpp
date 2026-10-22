/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorurlelicitationdialog.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolElicitRequestURLParams>
using namespace Qt::Literals::StringLiterals;
using namespace TextAutoGenerateTextMcpProtocolCore;
KMcpClientInspectorUrlElicitationDialog::KMcpClientInspectorUrlElicitationDialog(const McpProtocolElicitRequestURLParams &params, QWidget *parent)
    : QDialog(parent)
    , mUrl(QUrl(params.url(), QUrl::StrictMode))
    , mMessageLabel(new QLabel(this))
    , mDomainLabel(new QLabel(this))
    , mUrlLabel(new QLabel(this))
{
    setWindowTitle(i18nc("@title:window", "Server Requests to Open a Web Page"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mMessageLabel->setObjectName(u"mMessageLabel"_s);
    mMessageLabel->setWordWrap(true);
    mMessageLabel->setTextFormat(Qt::PlainText);
    mMessageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mMessageLabel->setText(params.message());
    mainLayout->addWidget(mMessageLabel);

    auto formLayout = new QFormLayout;
    formLayout->setObjectName(u"formLayout"_s);
    mainLayout->addLayout(formLayout);

    // Show domain clearly: user must know which site is opened before accepting
    mDomainLabel->setObjectName(u"mDomainLabel"_s);
    mDomainLabel->setTextFormat(Qt::PlainText);
    mDomainLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    QFont font = mDomainLabel->font();
    font.setBold(true);
    mDomainLabel->setFont(font);
    mDomainLabel->setText(mUrl.host());
    formLayout->addRow(i18nc("@label", "Domain:"), mDomainLabel);

    mUrlLabel->setObjectName(u"mUrlLabel"_s);
    mUrlLabel->setWordWrap(true);
    mUrlLabel->setTextFormat(Qt::PlainText);
    mUrlLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mUrlLabel->setText(params.url());
    formLayout->addRow(i18nc("@label", "URL:"), mUrlLabel);

    const bool urlValid = canOpenUrl(mUrl);
    auto infoLabel = new QLabel(urlValid ? i18n("The page will be opened in your web browser only if you click \"Open\".")
                                         : i18n("This URL is invalid or is not a web page: it cannot be opened."),
                                this);
    infoLabel->setObjectName(u"infoLabel"_s);
    infoLabel->setWordWrap(true);
    mainLayout->addWidget(infoLabel);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(u"buttonBox"_s);
    auto openButton = buttonBox->button(QDialogButtonBox::Ok);
    openButton->setText(i18nc("@action:button", "Open"));
    openButton->setIcon(QIcon::fromTheme(u"internet-web-browser"_s));
    openButton->setEnabled(urlValid);
    // Url must not be opened by mistake
    openButton->setDefault(false);
    openButton->setAutoDefault(false);
    auto declineButton = buttonBox->addButton(i18nc("@action:button", "Decline"), QDialogButtonBox::RejectRole);
    declineButton->setObjectName(u"declineButton"_s);
    mainLayout->addWidget(buttonBox);

    connect(openButton, &QPushButton::clicked, this, [this]() {
        mAction = McpProtocolElicitResult::Action::Accept;
        accept();
    });
    connect(buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &KMcpClientInspectorUrlElicitationDialog::reject);
    connect(declineButton, &QPushButton::clicked, this, &KMcpClientInspectorUrlElicitationDialog::slotDecline);
}

KMcpClientInspectorUrlElicitationDialog::~KMcpClientInspectorUrlElicitationDialog() = default;

bool KMcpClientInspectorUrlElicitationDialog::canOpenUrl(const QUrl &url)
{
    return url.isValid() && !url.host().isEmpty() && (url.scheme() == "https"_L1 || url.scheme() == "http"_L1);
}

void KMcpClientInspectorUrlElicitationDialog::slotDecline()
{
    mAction = McpProtocolElicitResult::Action::Decline;
    reject();
}

QUrl KMcpClientInspectorUrlElicitationDialog::url() const
{
    return mUrl;
}

McpProtocolElicitResult KMcpClientInspectorUrlElicitationDialog::elicitResult() const
{
    // Url mode: no content, user finishes interaction in web browser
    McpProtocolElicitResult result;
    result.setAction(mAction);
    return result;
}

#include "moc_kmcpclientinspectorurlelicitationdialog.cpp"
