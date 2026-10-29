/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlogsearchwidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorLogSearchWidget::KMcpClientInspectorLogSearchWidget(QWidget *parent)
    : QWidget{parent}
    , mSearchLineEdit(new QLineEdit(this))
    , mClearButton(new QPushButton(QIcon::fromTheme(u"edit-clear-history"_s), i18nc("@action:button", "Clear"), this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search"));
    mSearchLineEdit->setObjectName(u"mSearchLineEdit"_s);
    mSearchLineEdit->setClearButtonEnabled(true);
    mainLayout->addWidget(mSearchLineEdit);

    mClearButton->setObjectName(u"mClearButton"_s);
    mainLayout->addWidget(mClearButton);

    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &KMcpClientInspectorLogSearchWidget::searchTextChanged);
    connect(mClearButton, &QPushButton::clicked, this, &KMcpClientInspectorLogSearchWidget::clearRequested);
}

KMcpClientInspectorLogSearchWidget::~KMcpClientInspectorLogSearchWidget() = default;

#include "moc_kmcpclientinspectorlogsearchwidget.cpp"
