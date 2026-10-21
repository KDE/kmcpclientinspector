/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include <QObject>

class KMcpClientInspectorPromptArgumentsWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorPromptArgumentsWidgetTest(QObject *parent = nullptr);
    ~KMcpClientInspectorPromptArgumentsWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldCreateFields();
    void shouldReturnArguments();
};
