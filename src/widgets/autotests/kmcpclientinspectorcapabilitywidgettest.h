/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include <QObject>

class KMcpClientInspectorCapabilityWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorCapabilityWidgetTest(QObject *parent = nullptr);
    ~KMcpClientInspectorCapabilityWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldEnableConfigureRootsButton();
    void shouldDisableCapabilitiesWhenConnected();
};
