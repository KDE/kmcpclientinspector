/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include <QObject>

class KMcpClientInspectorInitializeWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorInitializeWidgetTest(QObject *parent = nullptr);
    ~KMcpClientInspectorInitializeWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldShowResult();
};
