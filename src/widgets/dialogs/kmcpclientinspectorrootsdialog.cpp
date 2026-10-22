/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorrootsdialog.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolWidgets/AddMcpServerRootsWidget>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorRootsDialog::KMcpClientInspectorRootsDialog(QWidget *parent)
    : QDialog(parent)
    , mRootsWidget(new TextAutoGenerateTextMcpProtocolWidgets::AddMcpServerRootsWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Configure Roots"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mRootsWidget->setObjectName(u"mRootsWidget"_s);
    mainLayout->addWidget(mRootsWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(u"buttonBox"_s);
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &KMcpClientInspectorRootsDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &KMcpClientInspectorRootsDialog::reject);
}

KMcpClientInspectorRootsDialog::~KMcpClientInspectorRootsDialog() = default;

QStringList KMcpClientInspectorRootsDialog::roots() const
{
    return mRootsWidget->roots();
}

void KMcpClientInspectorRootsDialog::setRoots(const QStringList &roots)
{
    mRootsWidget->setRoots(roots);
}

#include "moc_kmcpclientinspectorrootsdialog.cpp"
