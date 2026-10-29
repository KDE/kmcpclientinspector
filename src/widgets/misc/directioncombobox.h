/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "kmcpclientinspectorlogmodel.h"
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QComboBox>

class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT DirectionComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit DirectionComboBox(QWidget *parent = nullptr);
    ~DirectionComboBox() override;

    [[nodiscard]] KMcpClientInspectorLogModel::DirectionType type() const;

private:
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void fill();
};
