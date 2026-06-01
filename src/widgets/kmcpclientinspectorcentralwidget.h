/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <KAboutData>
#include <QWidget>
class KMcpClientInspectorTabWidget;
class KMcpClientInspectorManager;
namespace TextAutoGenerateTextMcpProtocolWidgets
{
class McpServerWidget;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorCentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorCentralWidget(const QList<KAboutRelease> &releases, KMcpClientInspectorManager *manager, QWidget *parent = nullptr);
    ~KMcpClientInspectorCentralWidget() override;

    [[nodiscard]] QList<KAboutRelease> releases() const;

private:
    KMcpClientInspectorTabWidget *const mTabWidget;
    TextAutoGenerateTextMcpProtocolWidgets::McpServerWidget *const mMcpServerWidget;
    KMcpClientInspectorManager *const mManager;
    QList<KAboutRelease> mReleases;
};
