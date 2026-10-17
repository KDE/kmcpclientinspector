/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorserverinfowidget.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QLabel>
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorServerInfoWidget::KMcpClientInspectorServerInfoWidget(QWidget *parent)
    : QWidget{parent}
    , mMainLayout(new QFormLayout(this))
    , mName(createLabel(u"mName"_s))
    , mTransportType(createLabel(u"mTransportType"_s))
    , mCommand(createLabel(u"mCommand"_s))
    , mArguments(createLabel(u"mArguments"_s))
    , mEnvironments(createLabel(u"mEnvironments"_s))
    , mServerUrl(createLabel(u"mServerUrl"_s))
    , mRoots(createLabel(u"mRoots"_s))
{
    mMainLayout->setObjectName(u"mainLayout"_s);
    mMainLayout->setContentsMargins({});

    mMainLayout->addRow(i18nc("@label", "Name:"), mName);
    mMainLayout->addRow(i18nc("@label", "Type:"), mTransportType);
    mMainLayout->addRow(i18nc("@label", "Command:"), mCommand);
    mMainLayout->addRow(i18nc("@label", "Arguments:"), mArguments);
    mMainLayout->addRow(i18nc("@label", "Environment:"), mEnvironments);
    mMainLayout->addRow(i18nc("@label", "Url:"), mServerUrl);
    mMainLayout->addRow(i18nc("@label", "Roots:"), mRoots);
}

KMcpClientInspectorServerInfoWidget::~KMcpClientInspectorServerInfoWidget() = default;

QLabel *KMcpClientInspectorServerInfoWidget::createLabel(const QString &objectName)
{
    auto label = new QLabel(this);
    label->setObjectName(objectName);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    label->setTextFormat(Qt::PlainText);
    return label;
}

void KMcpClientInspectorServerInfoWidget::setServerInfo(const TextAutoGenerateTextMcpProtocolCore::McpServer &server)
{
    const auto type = server.transportType();
    const auto settings = server.settings();
    const bool isStdio = (type == TextAutoGenerateTextMcpProtocolCore::McpProtocolPlugin::TransportType::Stdio);

    mName->setText(server.name());
    mTransportType->setText(TextAutoGenerateTextMcpProtocolCore::McpServer::transportTypeI18n(type));

    mCommand->setText(settings.command());
    mArguments->setText(settings.arguments());
    QStringList environments;
    const auto envs = settings.environments();
    for (const auto &[key, value] : envs.asKeyValueRange()) {
        environments.append(u"%1=%2"_s.arg(key, value));
    }
    mEnvironments->setText(environments.join(u'\n'));
    mServerUrl->setText(settings.serverUrl().toDisplayString());
    mRoots->setText(settings.roots().join(u'\n'));

    mMainLayout->setRowVisible(mCommand, isStdio);
    mMainLayout->setRowVisible(mArguments, isStdio);
    mMainLayout->setRowVisible(mEnvironments, isStdio);
    mMainLayout->setRowVisible(mServerUrl, !isStdio);
}

#include "moc_kmcpclientinspectorserverinfowidget.cpp"
