/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef MU_AUTOBOT_TESTCASEREPORT_H
#define MU_AUTOBOT_TESTCASEREPORT_H

#include <QFile>
#include <QTextStream>

#include "modularity/ioc.h"
#include "../iautobotconfiguration.h"
#include "io/ifilesystem.h"

#include "../autobottypes.h"
#include "../itestcasecontext.h"

namespace mu::autobot {
class TestCaseReport
{
    INJECT(autobot, IAutobotConfiguration, configuration)
    INJECT(autobot, io::IFileSystem, fileSystem)

public:
    TestCaseReport() = default;

    Ret beginReport(const TestCase& testCase);
    void endReport(bool aborted);

    void onStepStatusChanged(const QString& name, StepStatus status, const ITestCaseContextPtr& ctx);

private:

    QFile m_file;
    QTextStream m_stream;
    bool m_opened = false;
};
}

#endif // MU_AUTOBOT_TESTCASEREPORT_H
