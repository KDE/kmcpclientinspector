/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "actions/kmcpclientinspectoractiontabpagebase.h"
#include "libkmcpclientinspectorwidgets_private_export.h"
class QJsonObject;
class KMcpClientInspectorClientProtocolManager;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorResourceTemplatesWidget : public KMcpClientInspectorActionTabPageBase
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorResourceTemplatesWidget(KMcpClientInspectorClientProtocolManager *protocolManager, QWidget *parent = nullptr);
    ~KMcpClientInspectorResourceTemplatesWidget() override;
    void setResult(const QJsonObject &obj);
};
