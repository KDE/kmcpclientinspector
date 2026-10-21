/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlogsearchwidget.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QLineEdit>

using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorLogSearchWidget::KMcpClientInspectorLogSearchWidget(QWidget *parent)
    : QWidget{parent}
    , mSearchLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search"));
    mSearchLineEdit->setObjectName(u"mSearchLineEdit"_s);
    mainLayout->addWidget(mSearchLineEdit);
    mSearchLineEdit->setClearButtonEnabled(true);

    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &KMcpClientInspectorLogSearchWidget::searchTextChanged);
    // TODO add more
}

KMcpClientInspectorLogSearchWidget::~KMcpClientInspectorLogSearchWidget() = default;
