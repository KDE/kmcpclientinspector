/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include <KXmlGuiWindow>

class KMcpClientInspectorMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorMainWindow(QWidget *parent = nullptr);
    ~KMcpClientInspectorMainWindow() override;
};
