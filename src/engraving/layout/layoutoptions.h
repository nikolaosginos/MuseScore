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
#ifndef MU_ENGRAVING_LAYOUTOPTIONS_H
#define MU_ENGRAVING_LAYOUTOPTIONS_H

#include "style/styledef.h"
#include "style/style.h"
#include "libmscore/mscore.h"

namespace mu::engraving {
//---------------------------------------------------------
//   LayoutMode
//    PAGE   The normal page view, honors page and line breaks.
//    LINE   The panoramic view, one long system
//    FLOAT  The "reflow" mode, ignore page and line breaks
//    SYSTEM The "never ending page", page break are turned into line break
//---------------------------------------------------------

enum class LayoutMode : char {
    PAGE, FLOAT, LINE, SYSTEM, HORIZONTAL_FIXED
};

struct LayoutOptions
{
    LayoutMode mode = LayoutMode::PAGE;

    bool showVBox = true;

    // from style
    qreal loWidth = 0;
    qreal loHeight = 0;
    bool firstSystemIndent = true;

    qreal maxFretShiftAbove = 0;
    qreal maxFretShiftBelow = 0;

    VerticalAlignRange verticalAlignRange = VerticalAlignRange::SEGMENT;

    bool isMode(LayoutMode m) const { return mode == m; }
    bool isLinearMode() const { return mode == LayoutMode::LINE || mode == LayoutMode::HORIZONTAL_FIXED; }

    void updateFromStyle(const MStyle& style)
    {
        loWidth = style.styleD(Sid::pageWidth) * DPI;
        loHeight = style.styleD(Sid::pageHeight) * DPI;

        firstSystemIndent = style.styleB(Sid::enableIndentationOnFirstSystem);

        maxFretShiftAbove = style.styleMM(Sid::maxFretShiftAbove);
        maxFretShiftBelow = style.styleMM(Sid::maxFretShiftBelow);

        verticalAlignRange = VerticalAlignRange(style.styleI(Sid::autoplaceVerticalAlignRange));
    }
};
}

#endif // MU_ENGRAVING_LAYOUTOPTIONS_H
