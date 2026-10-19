/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectorlogsortfilterproxymodel.h"

KMcpClientInspectorLogSortFilterProxyModel::KMcpClientInspectorLogSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
}

KMcpClientInspectorLogSortFilterProxyModel::~KMcpClientInspectorLogSortFilterProxyModel() = default;
#include "moc_kmcpclientinspectorlogsortfilterproxymodel.cpp"
