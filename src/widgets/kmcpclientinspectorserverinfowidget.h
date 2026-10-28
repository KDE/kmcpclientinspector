/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QWidget>
#include <TextAutoGenerateTextMcpProtocolCore/McpServer>
class QLabel;
class QFormLayout;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorServerInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorServerInfoWidget(QWidget *parent = nullptr);
    ~KMcpClientInspectorServerInfoWidget() override;

    void setServerInfo(const TextAutoGenerateTextMcpProtocolCore::McpServer &server);

private:
    [[nodiscard]] LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT QLabel *createLabel(const QString &objectName);
    QFormLayout *const mMainLayout;
    QLabel *const mName;
    QLabel *const mTransportType;
    QLabel *const mCommand;
    QLabel *const mArguments;
    QLabel *const mEnvironments;
    QLabel *const mServerUrl;
    QLabel *const mRoots;
    QLabel *const mIcon;
};
