/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QDialog>
namespace TextAutoGenerateTextMcpProtocolWidgets
{
class AddMcpServerRootsWidget;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorRootsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorRootsDialog(QWidget *parent = nullptr);
    ~KMcpClientInspectorRootsDialog() override;

    /*!
     * Local directories that server is allowed to access.
     */
    [[nodiscard]] QStringList roots() const;
    void setRoots(const QStringList &roots);

private:
    TextAutoGenerateTextMcpProtocolWidgets::AddMcpServerRootsWidget *const mRootsWidget;
};
