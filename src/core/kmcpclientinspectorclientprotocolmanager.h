/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "libkmcpclientinspectorcore_export.h"
#include <QObject>

class LIBKMCPCLIENTINSPECTORCORE_EXPORT KMcpClientInspectorClientProtocolManager : public QObject
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorClientProtocolManager(QObject *parent = nullptr);
    ~KMcpClientInspectorClientProtocolManager() override;
};
