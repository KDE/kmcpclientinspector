/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "libkmcpclientinspectorcore_export.h"
#include <QString>
class QCommandLineParser;
class LIBKMCPCLIENTINSPECTORCORE_EXPORT KMcpClientInspectorCommandLineParser
{
public:
    enum class OptionParser : uint8_t {
        SelfTest,
    };
    explicit KMcpClientInspectorCommandLineParser(QCommandLineParser *parser);
    ~KMcpClientInspectorCommandLineParser();

    [[nodiscard]] static QString optionParserFromEnum(OptionParser e);

private:
    LIBKMCPCLIENTINSPECTORCORE_NO_EXPORT void initializeCommandLine(QCommandLineParser *parser);
};
