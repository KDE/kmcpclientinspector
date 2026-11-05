/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QWidget>

class KMcpClientInspectorConfigureBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorConfigureBaseWidget(QWidget *parent = nullptr);
    ~KMcpClientInspectorConfigureBaseWidget() override;

    virtual void save() = 0;
    virtual void revert() = 0;
    virtual void load() = 0;
};
