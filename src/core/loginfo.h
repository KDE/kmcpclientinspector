/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include <QDateTime>
class LogInfo
{
public:
    LogInfo();
    ~LogInfo();

    [[nodiscard]] QDateTime dateTime() const;
    void setDateTime(const QDateTime &newDateTime);

    [[nodiscard]] quint64 identifier() const;
    void setIdentifier(quint64 newIdentifier);

    [[nodiscard]] bool operator==(const LogInfo &other) const;

private:
    QDateTime mDateTime;
    quint64 mIdentifier = 0;
};
