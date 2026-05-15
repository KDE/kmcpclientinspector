/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QWidget>
namespace TextAutoGenerateTextMcpProtocolWidgets
{
class AddMcpServerWidget;
}
class QPushButton;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorServerSettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorServerSettingsWidget(QWidget *parent = nullptr);
    ~KMcpClientInspectorServerSettingsWidget() override;

private:
    TextAutoGenerateTextMcpProtocolWidgets::AddMcpServerWidget *const mMcpServerWidget;
    QPushButton *const mStartStopButton;
};
