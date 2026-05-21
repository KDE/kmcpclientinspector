/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "actions/kmcpclientinspectoractiontabpagebase.h"
#include "libkmcpclientinspectorwidgets_private_export.h"
class KMcpClientInspectorClientProtocolManager;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorPingWidget : public KMcpClientInspectorActionTabPageBase
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorPingWidget(KMcpClientInspectorClientProtocolManager *protocolManager, QWidget *parent = nullptr);
    ~KMcpClientInspectorPingWidget() override;
};
