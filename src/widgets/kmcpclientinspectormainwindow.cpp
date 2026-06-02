/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectormainwindow.h"
#include "config-kmcpclientinspector.h"
#include "configure/kmcpclientinspectorconfiguresettingsdialog.h"
#include "kmcpclientinspectorcentralwidget.h"
#include "kmcpclientinspectormanager.h"
#include <KActionCollection>
#include <KActionMenu>
#include <KColorSchemeManager>
#include <KColorSchemeMenu>
#include <KLocalizedString>
#include <KNotifyConfigWidget>
#include <KStandardAction>
#include <KStandardActions>
#include <QFontDatabase>
#include <QMenuBar>
#include <QPointer>
#include <QToolButton>
#include <TextAddonsWidgets/WhatsNewNgDialog>
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
#include <TextAddonsWidgets/VerifyNewVersionWidget>
#endif

using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorMainWindow::KMcpClientInspectorMainWindow(const QList<KAboutRelease> &releases, QWidget *parent)
    : KXmlGuiWindow(parent)
    , mManager(new KMcpClientInspectorManager(this))
    , mMainWidget(new KMcpClientInspectorCentralWidget(releases, mManager, this))
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    , mVerifyNewVersionWidget(new TextAddonsWidgets::VerifyNewVersionWidget(this))
#endif

{
    mMainWidget->setObjectName(u"mMainWidget"_s);
    setCentralWidget(mMainWidget);
    setupActions();
    setupGUI();
    mManager->loadServers();
}

KMcpClientInspectorMainWindow::~KMcpClientInspectorMainWindow() = default;

void KMcpClientInspectorMainWindow::setupActions()
{
    KActionCollection *ac = actionCollection();
    KStandardActions::quit(this, &KMcpClientInspectorMainWindow::close, ac);
    KStandardActions::configureNotifications(this, &KMcpClientInspectorMainWindow::slotConfigureNotifications, ac);

    mShowFullScreenAction = KStandardAction::fullScreen(nullptr, nullptr, this, ac);
    ac->setDefaultShortcut(mShowFullScreenAction, Qt::Key_F11);
    connect(mShowFullScreenAction, &QAction::toggled, this, &KMcpClientInspectorMainWindow::slotFullScreen);

    auto manager = KColorSchemeManager::instance();
    ac->addAction(u"colorscheme_menu"_s, KColorSchemeMenu::createMenu(manager, this));
    KStandardActions::preferences(this, &KMcpClientInspectorMainWindow::slotConfigure, ac);

    auto showWhatsNewAction = new QAction(QIcon(u":/kmcpclientinspector/kmcpclientinspector.svg"_s), i18n("What's new"), this);
    ac->addAction(u"whatsnew_kmcpclientinspector"_s, showWhatsNewAction);
    connect(showWhatsNewAction, &QAction::triggered, this, &KMcpClientInspectorMainWindow::slotWhatsNew);
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
    const QString defaultUrlPath = QStringLiteral("https://origin.cdn.kde.org/ci-builds/sdk/kmcpclientinspector/");
    const QString stableBranch = u"0.2"_s;
    bool stableVersion = false;
#if KMCPCLIENTINSPECTOR_STABLE_VERSION
    stableVersion = true;
#endif
    mVerifyNewVersionWidget->generateUrlInfo(stableBranch, defaultUrlPath, stableVersion);
    auto verifyNewVersionAction = mVerifyNewVersionWidget->verifyNewVersionAction();
    ac->addAction(u"verify_check_version"_s, verifyNewVersionAction);
#endif
}

void KMcpClientInspectorMainWindow::slotConfigure()
{
    QPointer<KMcpClientInspectorConfigureSettingsDialog> dlg = new KMcpClientInspectorConfigureSettingsDialog(mManager, this);
    if (dlg->exec()) {
        // TODO
    }
    delete dlg;
}

void KMcpClientInspectorMainWindow::slotConfigureNotifications()
{
    KNotifyConfigWidget::configure(this);
}

void KMcpClientInspectorMainWindow::slotFullScreen(bool t)
{
    KToggleFullScreenAction::setFullScreen(this, t);
    QMenuBar *mb = menuBar();
    if (t) {
        auto b = new QToolButton(mb);
        b->setDefaultAction(mShowFullScreenAction);
        b->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Ignored));
        b->setFont(QFontDatabase::systemFont(QFontDatabase::SmallestReadableFont));
        mb->setCornerWidget(b, Qt::TopRightCorner);
        b->setVisible(true);
        b->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    } else {
        QWidget *w = mb->cornerWidget(Qt::TopRightCorner);
        if (w) {
            w->deleteLater();
        }
    }
}

void KMcpClientInspectorMainWindow::slotWhatsNew()
{
    TextAddonsWidgets::WhatsNewNgDialog dlg(this);
    dlg.setReleases(mMainWidget->releases());
    dlg.exec();
}

#include "moc_kmcpclientinspectormainwindow.cpp"
