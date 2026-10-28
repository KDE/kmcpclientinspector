/*
 * SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorconfiguresettingsdialog.h"
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
{
    setWindowTitle(i18nc("@title:window", "Configure KMcpClientInspector"));
    setFaceType(KPageDialog::List);

    const QString mcpServersPageName = i18nc("@title Preferences page name", "MCP Servers");
    mConfigureMcpServersWidgetPage = new KPageWidgetItem(mConfigureMcpServersWidget, mcpServersPageName);
    mConfigureMcpServersWidgetPage->setIcon(QIcon::fromTheme(u"preferences-system-network-server"_s));
    addPage(mConfigureMcpServersWidgetPage);

    buttonBox()->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox()->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &KMcpClientInspectorConfigureSettingsDialog::slotAccepted);
    // Servers are already loaded: reloading them would recreate all tabs (and stop connections)
    connect(this, &QDialog::rejected, mConfigureMcpServersWidget, &KMcpClientInspectorConfigureMcpServersWidget::revert);
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myConfigGroupName), QSize(800, 600));
}

KMcpClientInspectorConfigureSettingsDialog::~KMcpClientInspectorConfigureSettingsDialog() = default;

void KMcpClientInspectorConfigureSettingsDialog::slotAccepted()
{
    mConfigureMcpServersWidget->save();
}

#include "moc_kmcpclientinspectorconfiguresettingsdialog.cpp"
