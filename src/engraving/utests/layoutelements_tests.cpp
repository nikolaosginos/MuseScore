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

#include <gtest/gtest.h>

#include "libmscore/measure.h"
#include "libmscore/page.h"
#include "libmscore/rest.h"
#include "libmscore/masterscore.h"
#include "libmscore/staff.h"
#include "libmscore/system.h"
#include "libmscore/tuplet.h"

#include "utils/scorerw.h"
#include "utils/scorecomp.h"

#include "log.h"

static const QString ALL_ELEMENTS_DATA_DIR("all_elements_data/");

using namespace mu::engraving;

class LayoutElementsTests : public ::testing::Test
{
public:
    void tstLayoutAll(QString file);
};

//---------------------------------------------------------
//   isLayoutDone
//    For use with Score::scanElements in tstLayoutAll
//    sets data (treated as bool*) to false if element is
//    not laid out, otherwise doesn't change the value of
//    data.
//---------------------------------------------------------

static void isLayoutDone(void* data, EngravingItem* e)
{
    bool* result = static_cast<bool*>(data);
    if (e->isTuplet()) {
        Tuplet* t = toTuplet(e);
        if (!t->hasBracket() || !t->number()) {
            // in this case tuplet will not have valid bbox.
            // TODO: how to check this case?
            return;
        }
    }
    if (e->isTimeSig()) {
        const Staff* st = e->staff();
        if (!st->staffType(e->tick())->genTimesig()) {
            // Some staff types require not to have a time
            // signature displayed. This is a valid exception.
            return;
        }
    }
    if (e->isRest() && toRest(e)->shouldNotBeDrawn()) {
        // another valid exception
        return;
    }
    // If layout of element is done it (usually?) has a valid
    // bounding box (bbox).
    if (e->visible() && !e->bbox().isValid()) {
        (*result) = false;
        // Print some info about the element to make test more useful...
        if (Measure* m = toMeasure(e->findMeasure())) {
            LOGD("Layout of %s is not done (page %zu, measure %d)", e->typeName(), m->system()->page()->no() + 1,
                 m->no() + 1);
        } else {
            LOGD("Layout of %s is not done", e->typeName());
        }
    }
}

//---------------------------------------------------------
//   tstLayoutAll
//    Test that all elements in the score are laid out
//---------------------------------------------------------

void LayoutElementsTests::tstLayoutAll(QString file)
{
    MasterScore* score = ScoreRW::readScore(ALL_ELEMENTS_DATA_DIR + file);
    // readScore should also do layout of the score

    for (LayoutMode mode : { LayoutMode::PAGE }) {
        score->setLayoutMode(mode);
        bool layoutDone = true;
        for (Score* s : score->scoreList()) {
            s->scanElements(&layoutDone, isLayoutDone, /* all */ true);
            EXPECT_TRUE(layoutDone);
        }
    }
}

TEST_F(LayoutElementsTests, tstLayoutElements)
{
    tstLayoutAll("layout_elements.mscx");
}

TEST_F(LayoutElementsTests, tstLayoutTablature)
{
    tstLayoutAll("layout_elements_tab.mscx");
}

TEST_F(LayoutElementsTests, tstLayoutMoonlight)
{
    tstLayoutAll("moonlight.mscx");
}

// FIXME goldberg.mscx does not pass the test because of some
// TimeSig and Clef elements. Need to check it later!
TEST_F(LayoutElementsTests, DISABLED_tstLayoutGoldberg)
{
    tstLayoutAll("goldberg.mscx");
}
