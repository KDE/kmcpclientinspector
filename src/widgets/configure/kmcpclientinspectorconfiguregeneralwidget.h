/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "kmcpclientinspectorconfigurebasewidget.h"
#include <QStringList>
class KMcpClientInspectorCapabilityWidget;

class KMcpClientInspectorConfigureGeneralWidget : public KMcpClientInspectorConfigureBaseWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorConfigureGeneralWidget(QWidget *parent = nullptr);
    ~KMcpClientInspectorConfigureGeneralWidget() override;

    void save() override;
    void revert() override;
    void load() override;
    void restoreDefaults() override;

private:
    void slotConfigureRoots();
    KMcpClientInspectorCapabilityWidget *const mCapabilityWidget;
    QStringList mRoots;
};
