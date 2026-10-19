/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "loginfo.h"

LogInfo::LogInfo() = default;
LogInfo::~LogInfo() = default;

QDateTime LogInfo::dateTime() const
{
    return mDateTime;
}

void LogInfo::setDateTime(const QDateTime &newDateTime)
{
    mDateTime = newDateTime;
}

quint64 LogInfo::identifier() const
{
    return mIdentifier;
}

void LogInfo::setIdentifier(quint64 newIdentifier)
{
    mIdentifier = newIdentifier;
}

bool LogInfo::operator==(const LogInfo &other) const = default;
