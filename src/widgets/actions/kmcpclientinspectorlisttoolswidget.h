/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "actions/kmcpclientinspectoractiontabpagebase.h"
#include "libkmcpclientinspectorwidgets_private_export.h"
class QPlainTextEdit;
class QJsonObject;
class KMcpClientInspectorClientProtocolManager;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorListToolsWidget : public KMcpClientInspectorActionTabPageBase
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorListToolsWidget(KMcpClientInspectorClientProtocolManager *protocolManager, QWidget *parent = nullptr);
    ~KMcpClientInspectorListToolsWidget() override;

    void setResult(const QJsonObject &obj);

private:
    QPlainTextEdit *const mTextEdit;
};
