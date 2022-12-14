/**
 * @file        speed.cpp
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

#include "speed.h"
#include <azuki_root.h>
#include <core/speed_measuring.h>

using namespace Kitsunemimi::Hanami;

Speed::Speed()
    : Kitsunemimi::Hanami::Blossom("Request the speed of the CPU")
{
    //----------------------------------------------------------------------------------------------
    // output
    //----------------------------------------------------------------------------------------------

    registerOutputField("current_speed",
                        SAKURA_MAP_TYPE,
                        "Json-object with current-speed-measurements");

    //----------------------------------------------------------------------------------------------
    //
    //----------------------------------------------------------------------------------------------
}

/**
 * @brief runTask
 */
bool
Speed::runTask(BlossomIO &blossomIO,
               const Kitsunemimi::DataMap &,
               BlossomStatus &,
               Kitsunemimi::ErrorContainer &)
{
    blossomIO.output.insert("current_speed", AzukiRoot::speedMeasuring->getJson());

    return true;
}
