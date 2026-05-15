/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorserversettingswidget.h"
#include <KLocalizedString>
#include <QPushButton>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolWidgets/AddMcpServerWidget>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorServerSettingsWidget::KMcpClientInspectorServerSettingsWidget(QWidget *parent)
    : QWidget{parent}
    , mMcpServerWidget(new TextAutoGenerateTextMcpProtocolWidgets::AddMcpServerWidget(this))
    , mStartStopButton(new QPushButton(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mMcpServerWidget->setObjectName(u"mMcpServerWidget"_s);
    mainLayout->addWidget(mMcpServerWidget);
    mStartStopButton->setObjectName(u"mStartStopButton"_s);
    mainLayout->addWidget(mStartStopButton);

    mainLayout->addStretch(1);
}

KMcpClientInspectorServerSettingsWidget::~KMcpClientInspectorServerSettingsWidget() = default;

#include "moc_kmcpclientinspectorserversettingswidget.cpp"
