/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QWidget>
#include <TextAutoGenerateTextMcpProtocolCore/McpProtocolClientProtocolManager>
class QCheckBox;
namespace TextAutoGenerateTextMcpProtocolCore
{
class McpProtocolClientProtocolManager;
}
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorCapabilityWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorCapabilityWidget(TextAutoGenerateTextMcpProtocolCore::McpProtocolClientProtocolManager *protocolManager,
                                                 QWidget *parent = nullptr);
    ~KMcpClientInspectorCapabilityWidget() override;

    [[nodiscard]] bool elicatationSupport() const;
    [[nodiscard]] bool rootsSupport() const;
    [[nodiscard]] bool samplingSupport() const;

private:
    QCheckBox *const mElicitationSupported;
    QCheckBox *const mRootsSupported;
    QCheckBox *const mSamplingSupported;
};
