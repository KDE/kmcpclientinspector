/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include <QObject>

class KMcpClientInspectorCallToolWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorCallToolWidgetTest(QObject *parent = nullptr);
    ~KMcpClientInspectorCallToolWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldUpdateToolsList();
    void shouldShowResult();
    void shouldShowError();
    void shouldConvertResultToHtml();
};
