/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#include "kmcpclientinspectormessagewidget.h"

KMcpClientInspectorMessageWidget::KMcpClientInspectorMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setMessageType(KMessageWidget::Error);
    setCloseButtonVisible(true);
    setVisible(false);
}

KMcpClientInspectorMessageWidget::~KMcpClientInspectorMessageWidget() = default;
#include "moc_kmcpclientinspectormessagewidget.cpp"
