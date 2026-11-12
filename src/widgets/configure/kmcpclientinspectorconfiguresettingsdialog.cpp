/*
 * SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorconfiguresettingsdialog.h"
#include "configure/kmcpclientinspectorconfiguregeneralwidget.h"
#include "configure/kmcpclientinspectorconfiguremcpserverswidget.h"
#include <KLocalizedString>
#include <QPushButton>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

namespace
{
const char myConfigGroupName[] = "KMcpClientInspectorConfigureSettingsDialog";
}
using namespace Qt::Literals::StringLiterals;
KMcpClientInspectorConfigureSettingsDialog::KMcpClientInspectorConfigureSettingsDialog(TextAutoGenerateTextMcpProtocolCore::McpServerManager *manager,
                                                                                       QWidget *parent)
    : KPageDialog(parent)
    , mConfigureMcpServersWidget(new KMcpClientInspectorConfigureMcpServersWidget(manager, this))
    , mConfigureGeneralWidget(new KMcpClientInspectorConfigureGeneralWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Configure KMcpClientInspector"));
    setFaceType(KPageDialog::List);

    const QString generalPageName = i18nc("@title Preferences page name", "General");
    auto pageItem = new KPageWidgetItem(mConfigureGeneralWidget, generalPageName);
    pageItem->setIcon(QIcon(u":/kmcpclientinspector/kmcpclientinspector.svg"_s));
    addPage(pageItem);

    const QString mcpServersPageName = i18nc("@title Preferences page name", "MCP Servers");
    mConfigureMcpServersWidgetPage = new KPageWidgetItem(mConfigureMcpServersWidget, mcpServersPageName);
    mConfigureMcpServersWidgetPage->setIcon(QIcon::fromTheme(u"preferences-system-network-server"_s));
    addPage(mConfigureMcpServersWidgetPage);

    mConfigureMcpServersWidget->load();
    mConfigureGeneralWidget->load();

    buttonBox()->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::RestoreDefaults);

    connect(buttonBox()->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &KMcpClientInspectorConfigureSettingsDialog::slotAccepted);
    connect(buttonBox()->button(QDialogButtonBox::RestoreDefaults),
            &QPushButton::clicked,
            this,
            &KMcpClientInspectorConfigureSettingsDialog::slotRestoreDefaults);
    // Servers are already loaded: reloading them would recreate all tabs (and stop connections)
    connect(this, &QDialog::rejected, this, &KMcpClientInspectorConfigureSettingsDialog::slotReverted);
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myConfigGroupName), QSize(800, 600));
}

KMcpClientInspectorConfigureSettingsDialog::~KMcpClientInspectorConfigureSettingsDialog() = default;

void KMcpClientInspectorConfigureSettingsDialog::slotReverted()
{
    mConfigureMcpServersWidget->revert();
    mConfigureGeneralWidget->revert();
}

void KMcpClientInspectorConfigureSettingsDialog::slotAccepted()
{
    mConfigureMcpServersWidget->save();
    mConfigureGeneralWidget->save();
}

void KMcpClientInspectorConfigureSettingsDialog::slotRestoreDefaults()
{
    mConfigureMcpServersWidget->restoreDefaults();
    mConfigureGeneralWidget->restoreDefaults();
}

#include "moc_kmcpclientinspectorconfiguresettingsdialog.cpp"
