/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QWidget>
class QPushButton;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorConnectButtonsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorConnectButtonsWidget(QWidget *parent = nullptr);
    ~KMcpClientInspectorConnectButtonsWidget() override;

    void updateButtons(bool connected, bool disconnected);
Q_SIGNALS:
    void connectRequested();
    void disconnectRequested();

private:
    QPushButton *const mConnectButton;
    QPushButton *const mDisconnectButton;
};
