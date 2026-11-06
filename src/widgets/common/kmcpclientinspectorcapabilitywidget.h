/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include "libkmcpclientinspectorwidgets_private_export.h"
#include <QWidget>
class QCheckBox;
class QToolButton;
class LIBKMCPCLIENTINSPECTORWIDGETS_TESTS_EXPORT KMcpClientInspectorCapabilityWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KMcpClientInspectorCapabilityWidget(QWidget *parent = nullptr);
    ~KMcpClientInspectorCapabilityWidget() override;

    [[nodiscard]] bool elicitationSupport() const;
    [[nodiscard]] bool urlElicitationSupport() const;
    [[nodiscard]] bool rootsSupport() const;
    [[nodiscard]] bool samplingSupport() const;

    /*!
     * Capabilities can't be changed when client is connected, but roots can still be modified.
     */
    void setConnected(bool connected);

Q_SIGNALS:
    void configureRootsRequested();

private:
    LIBKMCPCLIENTINSPECTORWIDGETS_NO_EXPORT void updateRootsButton();
    QCheckBox *const mElicitationSupported;
    QCheckBox *const mUrlElicitationSupported;
    QCheckBox *const mRootsSupported;
    QCheckBox *const mSamplingSupported;
    QToolButton *const mConfigureRootsButton;
};
