/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlisttoolswidget.h"
#include "kmcpclientinspectorclientprotocolmanager.h"
#include <KLocalizedString>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorListToolsWidget::KMcpClientInspectorListToolsWidget(KMcpClientInspectorClientProtocolManager *protocolManager, QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
    , mTextEdit(new QPlainTextEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto listToolsButton = new QPushButton(i18nc("@action:button", "List Tools"), this);
    listToolsButton->setObjectName(u"listToolsButton"_s);
    connect(listToolsButton, &QPushButton::clicked, this, [this]() {
        mProtocolManager->listTools();
    });
    mainLayout->addWidget(listToolsButton);

    mTextEdit->setObjectName(u"mTextEdit"_s);
    mTextEdit->setReadOnly(true);
    mainLayout->addWidget(mTextEdit);
}

KMcpClientInspectorListToolsWidget::~KMcpClientInspectorListToolsWidget() = default;

#include "moc_kmcpclientinspectorlisttoolswidget.cpp"
