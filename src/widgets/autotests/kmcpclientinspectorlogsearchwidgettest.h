/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include <QObject>

class KMcpClientInspectorLogSearchWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorLogSearchWidgetTest(QObject *parent = nullptr);
    ~KMcpClientInspectorLogSearchWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldEmitSearchTextChanged();
    void shouldEmitSearchClearRequested();
};
