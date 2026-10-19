/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include <QDateTime>
#include <QMetaType>
class LogInfo
{
    Q_GADGET
public:
    LogInfo();
    ~LogInfo();

    enum Direction : uint8_t {
        Unknown = 0,
        Input,
        Output,
    };
    Q_ENUM(Direction)

    [[nodiscard]] QDateTime dateTime() const;
    void setDateTime(const QDateTime &newDateTime);

    [[nodiscard]] quint64 identifier() const;
    void setIdentifier(quint64 newIdentifier);

    [[nodiscard]] bool operator==(const LogInfo &other) const;

    [[nodiscard]] Direction direction() const;
    void setDirection(Direction newDirection);

private:
    QDateTime mDateTime;
    quint64 mIdentifier = 0;
    Direction mDirection = Direction::Unknown;
};
