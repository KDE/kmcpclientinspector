/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpcolorsandmessageviewstyle.h"

KMcpColorsAndMessageViewStyle::KMcpColorsAndMessageViewStyle()
{
    connect(this, &KMcpColorsAndMessageViewStyle::needToUpdateColors, this, &KMcpColorsAndMessageViewStyle::slotUpdateColors);
    slotUpdateColors();
}

KMcpColorsAndMessageViewStyle &KMcpColorsAndMessageViewStyle::self()
{
    static KMcpColorsAndMessageViewStyle c;
    return c;
}

void KMcpColorsAndMessageViewStyle::slotUpdateColors()
{
    mPositiveText = schemeView().foreground(KColorScheme::PositiveText).color().name();
    mPositiveBackground = schemeView().background(KColorScheme::PositiveBackground).color().name();
    mNeutralText = schemeView().foreground(KColorScheme::NeutralText).color().name();
    mNeutralBackground = schemeView().background(KColorScheme::NeutralBackground).color().name();
    mNegativeText = schemeView().foreground(KColorScheme::NegativeText).color().name();
    mNegativeBackground = schemeView().background(KColorScheme::NegativeBackground).color().name();
    const QColor alternateBackgroundColor = schemeView().background(KColorScheme::AlternateBackground).color();
    mDarkTheme = alternateBackgroundColor.lightness() < 128;
    mAlternateBackground = alternateBackgroundColor.name();
    mInactiveText = schemeView().foreground(KColorScheme::InactiveText).color().name();
}

bool KMcpColorsAndMessageViewStyle::darkTheme() const
{
    return mDarkTheme;
}

const QString &KMcpColorsAndMessageViewStyle::inactiveText() const
{
    return mInactiveText;
}

const QString &KMcpColorsAndMessageViewStyle::alternateBackground() const
{
    return mAlternateBackground;
}

const QString &KMcpColorsAndMessageViewStyle::negativeBackground() const
{
    return mNegativeBackground;
}

const QString &KMcpColorsAndMessageViewStyle::negativeText() const
{
    return mNegativeText;
}

const QString &KMcpColorsAndMessageViewStyle::neutralBackground() const
{
    return mNeutralBackground;
}

const QString &KMcpColorsAndMessageViewStyle::neutralText() const
{
    return mNeutralText;
}

const QString &KMcpColorsAndMessageViewStyle::positiveBackground() const
{
    return mPositiveBackground;
}

const QString &KMcpColorsAndMessageViewStyle::positiveText() const
{
    return mPositiveText;
}

#include "moc_kmcpcolorsandmessageviewstyle.cpp"
