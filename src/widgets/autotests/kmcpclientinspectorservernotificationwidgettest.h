/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include <QObject>

class KMcpClientInspectorServerNotificationWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorServerNotificationWidgetTest(QObject *parent = nullptr);
    ~KMcpClientInspectorServerNotificationWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldAddLoggingMessage();
    void shouldFilterByLevel();
    void shouldClearMessages();
};
