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
#ifndef MU_NOTATION_STAFFTEXTPROPERTIESDIALOG_H
#define MU_NOTATION_STAFFTEXTPROPERTIESDIALOG_H

#include "ui_stafftextpropertiesdialog.h"

#include "modularity/ioc.h"
#include "context/iglobalcontext.h"
#include "inotationconfiguration.h"
#include "ui/iuiconfiguration.h"

class QPushButton;
class QToolButton;
class QComboBox;
class QTreeWidgetItem;

namespace mu::engraving {
class StaffTextBase;
}

namespace mu::notation {
class StaffTextPropertiesDialog : public QDialog, public Ui::StaffTextPropertiesDialog
{
    Q_OBJECT

    INJECT(notation, context::IGlobalContext, globalContext)
    INJECT(notation, INotationConfiguration, configuration)
    INJECT(notation, ui::IUiConfiguration, uiConfiguration)

public:
    StaffTextPropertiesDialog(QWidget* parent = nullptr);
    StaffTextPropertiesDialog(const StaffTextPropertiesDialog& other);
    ~StaffTextPropertiesDialog() override;

    static int static_metaTypeId();

private slots:
    void saveValues();
    void channelItemChanged(QTreeWidgetItem*, QTreeWidgetItem*);
    void voiceButtonClicked(int);
    void tabChanged(int tab);
    void setSwingControls(bool);

private:
    void hideEvent(QHideEvent*) override;

    void saveChannel(int channel);

    engraving::StaffTextBase* m_originStaffText = nullptr;
    engraving::StaffTextBase* m_staffText = nullptr;
    QToolButton* m_vb[engraving::VOICES][engraving::VOICES];
    QComboBox* m_channelCombo[engraving::VOICES];
    QPushButton* m_stops[4][16];
    int m_curTabIndex = 0;
};
}

Q_DECLARE_METATYPE(mu::notation::StaffTextPropertiesDialog)

#endif // MU_NOTATION_STAFFTEXTPROPERTIESDIALOG_H
