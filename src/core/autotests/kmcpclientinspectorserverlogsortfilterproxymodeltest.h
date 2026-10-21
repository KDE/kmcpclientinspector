/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <QObject>

class KMcpClientInspectorServerLogSortFilterProxyModelTest : public QObject
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorServerLogSortFilterProxyModelTest(QObject *parent = nullptr);
    ~KMcpClientInspectorServerLogSortFilterProxyModelTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldFilterByMinimumLevel();
    void shouldFilterByText();
};
