/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kmcpclientinspectorlogmodel.h"

KMcpClientInspectorLogModel::KMcpClientInspectorLogModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

KMcpClientInspectorLogModel::~KMcpClientInspectorLogModel() = default;
