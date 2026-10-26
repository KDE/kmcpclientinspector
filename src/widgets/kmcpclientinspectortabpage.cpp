/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectortabpage.h"
#include "actions/kmcpclientinspectoractiontabwidget.h"
#include "dialogs/kmcpclientinspectorelicitationdialog.h"
#include "dialogs/kmcpclientinspectorsamplingdialog.h"
#include "dialogs/kmcpclientinspectorurlelicitationdialog.h"
#include "kmcpclientinspectorserversettingswidget.h"
#include <KLocalizedString>
#include <KMessageWidget>
#include <QDesktopServices>
#include <QDir>
#include <QJsonObject>
#include <QPushButton>
#include <QSplitter>
#include <QUrl>
#include <QVBoxLayout>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClient>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolCreateMessageRequest>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolCreateMessageResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolElicitRequest>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolElicitRequestFormParams>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolElicitRequestURLParams>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolElicitResult>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolInitializeRequest>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolRoot>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
using namespace Qt::Literals::StringLiterals;
namespace
{
QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolRoot> convertToRoots(const QStringList &paths)
{
    QList<TextAutoGenerateTextMcpProtocolCore::McpProtocolRoot> roots;
    roots.reserve(paths.count());
    for (const QString &path : paths) {
        if (path.trimmed().isEmpty()) {
            continue;
        }
        const QString cleanPath = QDir::cleanPath(QDir::fromNativeSeparators(path.trimmed()));
        TextAutoGenerateTextMcpProtocolCore::McpProtocolRoot root;
        root.setUri(QUrl::fromLocalFile(cleanPath).toString());
        if (const QString name = QDir(cleanPath).dirName(); !name.isEmpty()) {
            root.setName(name);
        }
        roots.append(std::move(root));
    }
    return roots;
}
}

KMcpClientInspectorTabPage::KMcpClientInspectorTabPage(const TextAutoGenerateTextMcpProtocolCore::McpServer &server, QWidget *parent)
    : QWidget{parent}
    , mProtocolManager(new TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager(server, this))
    , mServerSettings(new KMcpClientInspectorServerSettingsWidget(server, this))
    , mActionTabWidget(new KMcpClientInspectorActionTabWidget(mProtocolManager, this))
    , mElicitationCompletedWidget(new KMessageWidget(this))
{
    mProtocolManager->setClientName(u"kmcpclientinspector"_s);
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout"_L1);
    mainLayout->setContentsMargins({});
    mainLayout->setSpacing(0);

    mElicitationCompletedWidget->setObjectName(u"mElicitationCompletedWidget"_s);
    mElicitationCompletedWidget->setMessageType(KMessageWidget::Positive);
    mElicitationCompletedWidget->setCloseButtonVisible(true);
    mElicitationCompletedWidget->setWordWrap(true);
    mElicitationCompletedWidget->setVisible(false);
    mainLayout->addWidget(mElicitationCompletedWidget);

    auto splitter = new QSplitter(this);
    splitter->setObjectName(u"splitter"_s);
    splitter->setChildrenCollapsible(false);
    mainLayout->addWidget(splitter);

    mServerSettings->setObjectName(u"mServerSettings"_s);
    splitter->addWidget(mServerSettings);

    mActionTabWidget->setObjectName(u"mActionTabWidget"_s);
    splitter->addWidget(mActionTabWidget);
    connect(mServerSettings,
            &KMcpClientInspectorServerSettingsWidget::serverModified,
            this,
            [this](const TextAutoGenerateTextMcpProtocolCore::McpServer &server) {
                std::ignore = mProtocolManager->setServer(server);
                Q_EMIT serverModified(server);
            });
    connect(mServerSettings, &KMcpClientInspectorServerSettingsWidget::startStopRequested, this, [this](bool started) {
        if (started) {
            // Server can be modified while connected (roots, preferences): it's applied when connecting
            std::ignore = mProtocolManager->setServer(mServerSettings->server());
            mProtocolManager->setElicitationSupported(mServerSettings->elicitationSupport());
            mProtocolManager->setUrlElicitationSupported(mServerSettings->urlElicitationSupport());
            mProtocolManager->setRootsSupported(mServerSettings->rootsSupport());
            mProtocolManager->setRoots(convertToRoots(mServerSettings->roots()));
            mProtocolManager->setSamplingSupported(mServerSettings->samplingSupport());
            mProtocolManager->initializeClient();
        } else {
            mProtocolManager->stopClient();
        }
    });
    // Server is informed that roots list changed when client is already initialized
    connect(mServerSettings, &KMcpClientInspectorServerSettingsWidget::rootsChanged, this, [this](const QStringList &roots) {
        mProtocolManager->setRoots(convertToRoots(roots));
    });
    connect(mProtocolManager,
            &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::elicitationRequested,
            this,
            &KMcpClientInspectorTabPage::slotElicitationRequested);
    connect(mProtocolManager,
            &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::samplingRequested,
            this,
            &KMcpClientInspectorTabPage::slotSamplingRequested);
    connect(mProtocolManager,
            &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::elicitationCompleted,
            this,
            &KMcpClientInspectorTabPage::slotElicitationCompleted);
    connect(mProtocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::finished, mElicitationCompletedWidget, [this]() {
        mElicitationCompletedWidget->animatedHide();
    });
    connect(mProtocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::finished, this, [this]() {
        Q_EMIT finished();
        mServerSettings->wasDisconnected();
    });
    connect(mProtocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::started, this, &KMcpClientInspectorTabPage::started);
}

KMcpClientInspectorTabPage::~KMcpClientInspectorTabPage() = default;

TextAutoGenerateTextMcpProtocolCore::McpServer KMcpClientInspectorTabPage::server() const
{
    return mServerSettings->server();
}

void KMcpClientInspectorTabPage::setServer(const TextAutoGenerateTextMcpProtocolCore::McpServer &server)
{
    mServerSettings->setServer(server);
    // Refused when client is started: it will be used at next connection
    std::ignore = mProtocolManager->setServer(server);
}

void KMcpClientInspectorTabPage::slotElicitationRequested(const TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::RequestId &id,
                                                          const TextAutoGenerateTextMcpProtocolCore::McpProtocolElicitRequest &request)
{
    const TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::ElicitRequestParams params = request.params();
    if (const auto urlParams = std::get_if<TextAutoGenerateTextMcpProtocolCore::McpProtocolElicitRequestURLParams>(&params)) {
        auto dialog = new KMcpClientInspectorUrlElicitationDialog(*urlParams, this);
        connect(dialog, &QDialog::finished, this, [this, dialog, id](int result) {
            // Url is opened only after user accepted it
            if (result == QDialog::Accepted) {
                QDesktopServices::openUrl(dialog->url());
            }
            mProtocolManager->respondToElicitation(id, dialog->elicitResult());
        });
        showUserRequestDialog(dialog, id);
        return;
    }
    auto dialog = new KMcpClientInspectorElicitationDialog(std::get<TextAutoGenerateTextMcpProtocolCore::McpProtocolElicitRequestFormParams>(params), this);
    connect(dialog, &QDialog::finished, this, [this, dialog, id]() {
        mProtocolManager->respondToElicitation(id, dialog->elicitResult());
    });
    showUserRequestDialog(dialog, id);
}

void KMcpClientInspectorTabPage::slotElicitationCompleted(const QString &elicitationId)
{
    mElicitationCompletedWidget->setText(
        i18n("The server reports that the interaction in the web browser is complete (elicitation \"%1\"). You can retry the request.", elicitationId));
    mElicitationCompletedWidget->animatedShow();
}

void KMcpClientInspectorTabPage::slotSamplingRequested(const TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::RequestId &id,
                                                       const TextAutoGenerateTextMcpProtocolCore::McpProtocolCreateMessageRequest &request)
{
    auto dialog = new KMcpClientInspectorSamplingDialog(request.params(), this);
    connect(dialog, &QDialog::finished, this, [this, dialog, id](int result) {
        if (result == QDialog::Accepted) {
            mProtocolManager->respondToSampling(id, dialog->createMessageResult());
        } else {
            mProtocolManager->rejectSampling(id);
        }
    });
    showUserRequestDialog(dialog, id);
}

void KMcpClientInspectorTabPage::showUserRequestDialog(QDialog *dialog, const TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::RequestId &id)
{
    // Server cancelled request or connection is closed: close dialog without answering
    const auto closeDialog = [this, dialog]() {
        disconnect(dialog, &QDialog::finished, this, nullptr);
        dialog->deleteLater();
    };
    connect(mProtocolManager,
            &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::userRequestCancelled,
            dialog,
            [id, closeDialog](const TextAutoGenerateTextMcpProtocolCore::McpProtocolUtils::RequestId &cancelledId) {
                if (cancelledId == id) {
                    closeDialog();
                }
            });
    connect(mProtocolManager, &TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager::finished, dialog, closeDialog);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

#include "moc_kmcpclientinspectortabpage.cpp"
