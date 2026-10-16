/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorinitializewidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QVBoxLayout>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorInitializeWidget::KMcpClientInspectorInitializeWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                                         QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
    , mJsonPlainTextEditWidget(new JsonPlainTextEditWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mJsonPlainTextEditWidget->setObjectName(u"mJsonPlainTextEditWidget"_s);
    mainLayout->addWidget(mJsonPlainTextEditWidget);
}

KMcpClientInspectorInitializeWidget::~KMcpClientInspectorInitializeWidget() = default;

void KMcpClientInspectorInitializeWidget::setResult(const QJsonObject &obj)
{
    mJsonPlainTextEditWidget->setPlainText(QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented)));
}

#include "moc_kmcpclientinspectorinitializewidget.cpp"
