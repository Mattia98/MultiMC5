/* Copyright 2013 MultiMC Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "settingsobject.h"
#include "setting.h"
#include "overridesetting.h"

#include <QVariant>

SettingsObject::SettingsObject(QObject *parent) : QObject(parent)
{
}

SettingsObject::~SettingsObject()
{
	m_settings.clear();
}

std::shared_ptr<Setting> SettingsObject::registerOverride(std::shared_ptr<Setting> original)
{
	if (contains(original->id()))
	{
		qDebug(QString("Failed to register setting %1. ID already exists.")
				   .arg(original->id())
				   .toUtf8());
		return nullptr; // Fail
	}
	auto override = std::make_shared<OverrideSetting>(original);
	override->m_storage = this;
	connectSignals(*override);
	m_settings.insert(override->id(), override);
	return override;
}

std::shared_ptr<Setting> SettingsObject::registerSetting(QStringList synonyms, QVariant defVal)
{
	if (synonyms.empty())
		return nullptr;
	if (contains(synonyms.first()))
	{
		qDebug(QString("Failed to register setting %1. ID already exists.")
				   .arg(synonyms.first())
				   .toUtf8());
		return nullptr; // Fail
	}
	auto setting = std::make_shared<Setting>(synonyms, defVal);
	setting->m_storage = this;
	connectSignals(*setting);
	m_settings.insert(setting->id(), setting);
	return setting;
}

/*

bool SettingsObject::registerSetting(Setting *setting)
{
	if (contains(setting->id()))
	{
		qDebug(QString("Failed to register setting %1. ID already exists.")
				   .arg(setting->id())
				   .toUtf8());
		return false; // Fail
	}

	m_settings.insert(setting->id(), setting);
	setting->setParent(this); // Take ownership.

	// Connect signals.
	connectSignals(*setting);

	// qDebug(QString("Registered setting %1.").arg(setting->id()).toUtf8());
	return true;
}
*/
std::shared_ptr<Setting> SettingsObject::getSetting(const QString &id) const
{
	// Make sure there is a setting with the given ID.
	if (!m_settings.contains(id))
		return NULL;

	return m_settings[id];
}

QVariant SettingsObject::get(const QString &id) const
{
	auto setting = getSetting(id);
	return (setting ? setting->get() : QVariant());
}

bool SettingsObject::set(const QString &id, QVariant value)
{
	auto setting = getSetting(id);
	if (!setting)
	{
		qDebug(QString("Error changing setting %1. Setting doesn't exist.").arg(id).toUtf8());
		return false;
	}
	else
	{
		setting->set(value);
		return true;
	}
}

void SettingsObject::reset(const QString &id) const
{
	auto setting = getSetting(id);
	if (setting)
		setting->reset();
}

bool SettingsObject::contains(const QString &id)
{
	return m_settings.contains(id);
}

void SettingsObject::connectSignals(const Setting &setting)
{
	connect(&setting, SIGNAL(settingChanged(const Setting &, QVariant)),
			SLOT(changeSetting(const Setting &, QVariant)));
	connect(&setting, SIGNAL(settingChanged(const Setting &, QVariant)),
			SIGNAL(settingChanged(const Setting &, QVariant)));

	connect(&setting, SIGNAL(settingReset(Setting)), SLOT(resetSetting(const Setting &)));
	connect(&setting, SIGNAL(settingReset(Setting)), SIGNAL(settingReset(const Setting &)));
}
