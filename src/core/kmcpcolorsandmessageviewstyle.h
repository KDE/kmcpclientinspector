/*
   SPDX-FileCopyrightText: 2022 Nicolas Fella <nicolas.fella@gmx.de>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libkmcpclientinspectorcore_export.h"
#include <TextUtils/TextUtilsColorsAndMessageViewStyle>

class LIBKMCPCLIENTINSPECTORCORE_EXPORT KMcpColorsAndMessageViewStyle : public TextUtils::TextUtilsColorsAndMessageViewStyle
{
    Q_OBJECT
public:
    static KMcpColorsAndMessageViewStyle &self();

    KMcpColorsAndMessageViewStyle();

    [[nodiscard]] const QString &positiveText() const;

    [[nodiscard]] const QString &positiveBackground() const;

    [[nodiscard]] const QString &neutralText() const;

    [[nodiscard]] const QString &neutralBackground() const;

    [[nodiscard]] const QString &negativeText() const;

    [[nodiscard]] const QString &negativeBackground() const;

    [[nodiscard]] const QString &alternateBackground() const;

    [[nodiscard]] const QString &inactiveText() const;

    [[nodiscard]] bool darkTheme() const;

private:
    LIBKMCPCLIENTINSPECTORCORE_NO_EXPORT void slotUpdateColors();
    QString mPositiveText;
    QString mPositiveBackground;
    QString mNeutralText;
    QString mNeutralBackground;
    QString mNegativeText;
    QString mNegativeBackground;
    QString mAlternateBackground;
    QString mInactiveText;
    bool mDarkTheme = false;
};
