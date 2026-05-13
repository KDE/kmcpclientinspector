/*
 * SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorconfiguresettingsdialog.h"

#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QPushButton>
#include <QWindow>

namespace
{
const char myConfigGroupName[] = "KMcpClientInspectorConfigureSettingsDialog";
}
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorConfigureSettingsDialog::KMcpClientInspectorConfigureSettingsDialog(QWidget *parent)
    : KPageDialog(parent)
{
    setWindowTitle(i18nc("@title:window", "Configure KMcpClientInspector"));
    setFaceType(KPageDialog::List);

    buttonBox()->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults);

    connect(buttonBox()->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &KMcpClientInspectorConfigureSettingsDialog::slotAccepted);
    connect(buttonBox()->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &KMcpClientInspectorConfigureSettingsDialog::reject);
    connect(buttonBox()->button(QDialogButtonBox::RestoreDefaults),
            &QPushButton::clicked,
            this,
            &KMcpClientInspectorConfigureSettingsDialog::slotRestoreDefaults);
    readConfig();
    load();
}

KMcpClientInspectorConfigureSettingsDialog::~KMcpClientInspectorConfigureSettingsDialog()
{
    writeConfig();
}

void KMcpClientInspectorConfigureSettingsDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void KMcpClientInspectorConfigureSettingsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void KMcpClientInspectorConfigureSettingsDialog::slotAccepted()
{
}

void KMcpClientInspectorConfigureSettingsDialog::load()
{
}

void KMcpClientInspectorConfigureSettingsDialog::slotRestoreDefaults()
{
}

#include "moc_kmcpclientinspectorconfiguresettingsdialog.cpp"
