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

#include "NostalgiaInstance.h"

NostalgiaInstance::NostalgiaInstance(const QString &rootDir, SettingsObject *settings,
									 QObject *parent)
	: OneSixInstance(rootDir, settings, parent)
{
}

QString NostalgiaInstance::getStatusbarDescription()
{
	if (flags() & VersionBrokenFlag)
	{
		return "Nostalgia : " + intendedVersionId() + " (broken)";
	}
	return "Nostalgia : " + intendedVersionId();
}

bool NostalgiaInstance::menuActionEnabled(QString action_name) const
{
	return false;
}
