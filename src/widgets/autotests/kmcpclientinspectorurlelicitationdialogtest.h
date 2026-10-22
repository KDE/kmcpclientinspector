/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include <QObject>

class KMcpClientInspectorUrlElicitationDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorUrlElicitationDialogTest(QObject *parent = nullptr);
    ~KMcpClientInspectorUrlElicitationDialogTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldReturnResult();
    void shouldNotOpenInvalidUrl();
    void shouldCheckUrl_data();
    void shouldCheckUrl();
};
