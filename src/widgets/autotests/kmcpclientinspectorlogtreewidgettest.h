/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include <QObject>

class KMcpClientInspectorLogTreeWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorLogTreeWidgetTest(QObject *parent = nullptr);
    ~KMcpClientInspectorLogTreeWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldAddLogMessage();
    void shouldFilterLog();
    void shouldFilterDirection();
};
