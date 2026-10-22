/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include <QObject>

class JsonPlainTextEditWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit JsonPlainTextEditWidgetTest(QObject *parent = nullptr);
    ~JsonPlainTextEditWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldChangeText();
    void shouldChangeReadOnly();
};
