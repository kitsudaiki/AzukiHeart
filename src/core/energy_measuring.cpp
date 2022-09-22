/**
 * @file        energy_measuring.cpp
 *
 * @author      Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 * @copyright   Apache License Version 2.0
 *
 *      Copyright 2021 Tobias Anker
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include "energy_measuring.h"
#include <azuki_root.h>
#include <core/value_container.h>

#include <libKitsunemimiSakuraHardware/host.h>
#include <libKitsunemimiSakuraHardware/cpu_core.h>
#include <libKitsunemimiSakuraHardware/cpu_package.h>
#include <libKitsunemimiSakuraHardware/cpu_thread.h>

#include <libKitsunemimiJson/json_item.h>

using namespace Kitsunemimi::Sakura;

PowerMeasuring::PowerMeasuring()
    : Kitsunemimi::Thread("Azuki_EnergyMeasuring")
{
    m_valueContainer = new ValueContainer();
}

PowerMeasuring::~PowerMeasuring()
{
    delete m_valueContainer;
}

Kitsunemimi::DataMap*
PowerMeasuring::getJson()
{
    return m_valueContainer->toJson();
}

/**
 * @brief ThreadBinder::run
 */
void
PowerMeasuring::run()
{

    Kitsunemimi::ErrorContainer error;
    while(m_abort == false)
    {
        double power = 0.0;
        for(uint64_t i = 0; i < AzukiRoot::host->cpuPackages.size(); i++) {
            power += AzukiRoot::host->getPackage(i)->getTotalPackagePower();
        }

        m_valueContainer->addValue(power);

        sleep(1);
    }
}
