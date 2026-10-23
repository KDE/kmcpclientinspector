/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "kmcpclientinspectoractiontabpagebase.h"
#include "libkmcpclientinspectorwidgets_private_export.h"
class QSplitter;
class JsonPlainTextEditWidget;
class KMcpClientInspectorLogTreeWidget;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorLogWidget : public KMcpClientInspectorActionTabPageBase
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorLogWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager, QWidget *parent = nullptr);
    ~KMcpClientInspectorLogWidget() override;

private:
    QSplitter *const mSplitter;
    JsonPlainTextEditWidget *const mJsonPlainTextEditWidget;
    KMcpClientInspectorLogTreeWidget *const mLogTreeWidget;
};
