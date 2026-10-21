/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <QWidget>
class QLineEdit;
class KMcpClientInspectorLogSearchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorLogSearchWidget(QWidget *parent = nullptr);
    ~KMcpClientInspectorLogSearchWidget() override;

Q_SIGNALS:
    void searchTextChanged(const QString &str);

private:
    QLineEdit *const mSearchLineEdit;
};
