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
#include "imagesettingsmodel.h"

#include "types/commontypes.h"

#include "translation.h"

using namespace mu::inspector;

ImageSettingsModel::ImageSettingsModel(QObject* parent, IElementRepositoryService* repository)
    : AbstractInspectorModel(parent, repository)
{
    setModelType(InspectorModelType::TYPE_IMAGE);
    setTitle(qtrc("inspector", "Image"));
    setIcon(ui::IconCode::Code::IMAGE_MOUNTAINS);
    createProperties();
}

void ImageSettingsModel::createProperties()
{
    m_isAspectRatioLocked = buildPropertyItem(mu::engraving::Pid::LOCK_ASPECT_RATIO);

    m_shouldScaleToFrameSize
        = buildPropertyItem(mu::engraving::Pid::AUTOSCALE, [this](const mu::engraving::Pid pid, const QVariant& newValue) {
        onPropertyValueChanged(pid, newValue);

        emit requestReloadPropertyItems();
    });

    m_height = buildPropertyItem(mu::engraving::Pid::IMAGE_HEIGHT, [this](const mu::engraving::Pid pid, const QVariant& newValue) {
        onPropertyValueChanged(pid, newValue);

        if (m_isAspectRatioLocked->value().toBool()) {
            emit requestReloadPropertyItems();
        }
    });

    m_width = buildPropertyItem(mu::engraving::Pid::IMAGE_WIDTH, [this](const mu::engraving::Pid pid, const QVariant& newValue) {
        onPropertyValueChanged(pid, newValue);

        if (m_isAspectRatioLocked->value().toBool()) {
            emit requestReloadPropertyItems();
        }
    });

    m_isSizeInSpatiums
        = buildPropertyItem(mu::engraving::Pid::SIZE_IS_SPATIUM, [this](const mu::engraving::Pid pid, const QVariant& newValue) {
        onPropertyValueChanged(pid, newValue);

        emit requestReloadPropertyItems();
    });

    m_isImageFramed = buildPropertyItem(mu::engraving::Pid::IMAGE_FRAMED);
}

void ImageSettingsModel::requestElements()
{
    m_elementList = m_repository->findElementsByType(mu::engraving::ElementType::IMAGE);
}

void ImageSettingsModel::loadProperties()
{
    loadPropertyItem(m_shouldScaleToFrameSize);
    loadPropertyItem(m_height, formatDoubleFunc);
    loadPropertyItem(m_width, formatDoubleFunc);

    loadPropertyItem(m_isAspectRatioLocked);
    loadPropertyItem(m_isSizeInSpatiums);
    loadPropertyItem(m_isImageFramed);

    updateFrameScalingAvailability();
}

void ImageSettingsModel::resetProperties()
{
    m_shouldScaleToFrameSize->resetToDefault();
    m_height->resetToDefault();
    m_width->resetToDefault();
    m_isAspectRatioLocked->resetToDefault();
    m_isSizeInSpatiums->resetToDefault();
    m_isImageFramed->resetToDefault();
}

void ImageSettingsModel::updatePropertiesOnNotationChanged()
{
    loadPropertyItem(m_height, formatDoubleFunc);
    loadPropertyItem(m_width, formatDoubleFunc);
}

PropertyItem* ImageSettingsModel::shouldScaleToFrameSize() const
{
    return m_shouldScaleToFrameSize;
}

PropertyItem* ImageSettingsModel::height() const
{
    return m_height;
}

PropertyItem* ImageSettingsModel::width() const
{
    return m_width;
}

PropertyItem* ImageSettingsModel::isAspectRatioLocked() const
{
    return m_isAspectRatioLocked;
}

PropertyItem* ImageSettingsModel::isSizeInSpatiums() const
{
    return m_isSizeInSpatiums;
}

PropertyItem* ImageSettingsModel::isImageFramed() const
{
    return m_isImageFramed;
}

void ImageSettingsModel::updateFrameScalingAvailability()
{
    bool isAvailable = m_isImageFramed->value().toBool();

    m_shouldScaleToFrameSize->setIsEnabled(isAvailable);
}
