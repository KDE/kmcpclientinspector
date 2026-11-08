/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include <QObject>

class KMcpClientInspectorCompleteWidgetTest : public QObject
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorCompleteWidgetTest(QObject *parent = nullptr);
    ~KMcpClientInspectorCompleteWidgetTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
    void shouldUpdatePromptsList();
    void shouldUpdateResourceTemplatesList();
    void shouldCreateRequestParams();
    void shouldShowResult();
    void shouldShowError();
    void shouldExtractUriTemplateVariables_data();
    void shouldExtractUriTemplateVariables();
};
