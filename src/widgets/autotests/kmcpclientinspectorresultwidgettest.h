/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include <QObject>

class KMcpClientInspectorResultWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorResultWidgetTest(QObject *parent = nullptr);
    ~KMcpClientInspectorResultWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldShowResult();
};
