/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "directioncombobox.h"
#include <KLocalizedString>

DirectionComboBox::DirectionComboBox(QWidget *parent)
    : QComboBox(parent)
{
    fill();
}

DirectionComboBox::~DirectionComboBox() = default;

void DirectionComboBox::fill()
{
    addItem(i18n("All"), QVariant::fromValue(KMcpClientInspectorLogModel::DirectionType::All));
    addItem(i18n("Send"), QVariant::fromValue(KMcpClientInspectorLogModel::DirectionType::Sent));
    addItem(i18n("Received"), QVariant::fromValue(KMcpClientInspectorLogModel::DirectionType::Received));
}

KMcpClientInspectorLogModel::DirectionType DirectionComboBox::type() const
{
    return currentData().value<KMcpClientInspectorLogModel::DirectionType>();
}

#include "moc_directioncombobox.cpp"
