/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorinitializewidget.h"
#include "misc/jsonplaintexteditwidget.h"
#include <KLocalizedString>
#include <QFormLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QPlainTextEdit>
#include <QTabWidget>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolInitializeResult>
using namespace Qt::Literals::StringLiterals;
using namespace TextAutoGenerateTextMcpProtocolCore;
KMcpClientInspectorInitializeWidget::KMcpClientInspectorInitializeWidget(McpProtocolClientProtocolManager *protocolManager, QWidget *parent)
    : KMcpClientInspectorActionTabPageBase{protocolManager, parent}
    , mFormLayout(new QFormLayout)
    , mServerName(createLabel(u"mServerName"_s))
    , mServerTitle(createLabel(u"mServerTitle"_s))
    , mServerVersion(createLabel(u"mServerVersion"_s))
    , mServerDescription(createLabel(u"mServerDescription"_s))
    , mServerWebsite(createLabel(u"mServerWebsite"_s))
    , mProtocolVersion(createLabel(u"mProtocolVersion"_s))
    , mCapabilities(createLabel(u"mCapabilities"_s))
    , mInstructions(new QPlainTextEdit(this))
    , mJsonPlainTextEditWidget(new JsonPlainTextEditWidget(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto tabWidget = new QTabWidget(this);
    tabWidget->setObjectName(u"tabWidget"_s);
    mainLayout->addWidget(tabWidget);

    auto infoWidget = new QWidget(tabWidget);
    infoWidget->setObjectName(u"infoWidget"_s);
    auto infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setObjectName(u"infoLayout"_s);

    mFormLayout->setObjectName(u"mFormLayout"_s);
    mFormLayout->setContentsMargins({});
    mFormLayout->addRow(i18nc("@label", "Name:"), mServerName);
    mFormLayout->addRow(i18nc("@label", "Title:"), mServerTitle);
    mFormLayout->addRow(i18nc("@label", "Version:"), mServerVersion);
    mFormLayout->addRow(i18nc("@label", "Description:"), mServerDescription);
    mFormLayout->addRow(i18nc("@label", "Website:"), mServerWebsite);
    mFormLayout->addRow(i18nc("@label", "Protocol version:"), mProtocolVersion);
    mFormLayout->addRow(i18nc("@label", "Capabilities:"), mCapabilities);
    infoLayout->addLayout(mFormLayout);

    mServerDescription->setWordWrap(true);
    mServerWebsite->setTextFormat(Qt::RichText);
    mServerWebsite->setTextInteractionFlags(Qt::TextBrowserInteraction);
    mServerWebsite->setOpenExternalLinks(true);

    auto instructionsLabel = new QLabel(i18nc("@label", "Instructions:"), infoWidget);
    instructionsLabel->setObjectName(u"instructionsLabel"_s);
    infoLayout->addWidget(instructionsLabel);

    mInstructions->setObjectName(u"mInstructions"_s);
    mInstructions->setReadOnly(true);
    mInstructions->setPlaceholderText(i18n("The server did not send instructions."));
    infoLayout->addWidget(mInstructions, 1);

    tabWidget->addTab(infoWidget, i18n("Information"));

    mJsonPlainTextEditWidget->setObjectName(u"mJsonPlainTextEditWidget"_s);
    tabWidget->addTab(mJsonPlainTextEditWidget, i18n("JSON"));

    setInitializeResult({});

    connect(protocolManager, &McpProtocolClientProtocolManager::initialized, this, [this]() {
        setInitializeResult(mProtocolManager->initializeResult());
    });
}

KMcpClientInspectorInitializeWidget::~KMcpClientInspectorInitializeWidget() = default;

QLabel *KMcpClientInspectorInitializeWidget::createLabel(const QString &objectName)
{
    auto label = new QLabel(this);
    label->setObjectName(objectName);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    label->setTextFormat(Qt::PlainText);
    return label;
}

void KMcpClientInspectorInitializeWidget::setResult(const QJsonObject &obj)
{
    mJsonPlainTextEditWidget->setPlainText(QString::fromUtf8(QJsonDocument(obj).toJson(QJsonDocument::Indented)));
}

void KMcpClientInspectorInitializeWidget::setInitializeResult(const McpProtocolInitializeResult &result)
{
    const McpProtocolImplementation serverInfo = result.serverInfo();
    const QString title = serverInfo.title().value_or(QString());
    const QString description = serverInfo.description().value_or(QString());
    const QString websiteUrl = serverInfo.websiteUrl().value_or(QString());

    mServerName->setText(serverInfo.name());
    mServerTitle->setText(title);
    mServerVersion->setText(serverInfo.version());
    mServerDescription->setText(description);
    mServerWebsite->setText(u"<a href=\"%1\">%2</a>"_s.arg(websiteUrl.toHtmlEscaped(), websiteUrl.toHtmlEscaped()));
    mProtocolVersion->setText(result.protocolVersion());
    mCapabilities->setText(capabilitiesToText(result.capabilities()));
    mInstructions->setPlainText(result.instructions().value_or(QString()));

    mFormLayout->setRowVisible(mServerTitle, !title.isEmpty());
    mFormLayout->setRowVisible(mServerDescription, !description.isEmpty());
    mFormLayout->setRowVisible(mServerWebsite, !websiteUrl.isEmpty());
}

QString KMcpClientInspectorInitializeWidget::capabilitiesToText(const McpProtocolServerCapabilities &capabilities)
{
    const auto withOptions = [](const QString &name, const QStringList &options) {
        return options.isEmpty() ? name : u"%1 (%2)"_s.arg(name, options.join(u", "_s));
    };
    const QString listChanged = i18nc("@info server capability option", "list changed notifications");

    QStringList lines;
    if (const auto tools = capabilities.tools(); tools.has_value()) {
        lines.append(withOptions(i18nc("@info server capability", "Tools"), tools->listChanged().value_or(false) ? QStringList{listChanged} : QStringList{}));
    }
    if (const auto prompts = capabilities.prompts(); prompts.has_value()) {
        lines.append(
            withOptions(i18nc("@info server capability", "Prompts"), prompts->listChanged().value_or(false) ? QStringList{listChanged} : QStringList{}));
    }
    if (const auto resources = capabilities.resources(); resources.has_value()) {
        QStringList options;
        if (resources->subscribe().value_or(false)) {
            options.append(i18nc("@info server capability option", "subscribe"));
        }
        if (resources->listChanged().value_or(false)) {
            options.append(listChanged);
        }
        lines.append(withOptions(i18nc("@info server capability", "Resources"), options));
    }
    if (capabilities.logging().has_value()) {
        lines.append(i18nc("@info server capability", "Logging"));
    }
    if (capabilities.completions().has_value()) {
        lines.append(i18nc("@info server capability", "Completions"));
    }
    if (capabilities.tasks().has_value()) {
        lines.append(i18nc("@info server capability", "Tasks"));
    }
    if (const auto experimental = capabilities.experimental(); experimental.has_value()) {
        lines.append(withOptions(i18nc("@info server capability", "Experimental"), experimental->keys()));
    }
    if (lines.isEmpty()) {
        return i18nc("@info server capabilities", "None");
    }
    return lines.join(u'\n');
}

#include "moc_kmcpclientinspectorinitializewidget.cpp"
