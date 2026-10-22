/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include <QObject>

class KMcpClientInspectorElicitationDialogTest : public QObject
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorElicitationDialogTest(QObject *parent = nullptr);
    ~KMcpClientInspectorElicitationDialogTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldReturnResult();
};
