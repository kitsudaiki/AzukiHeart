/**
 * @file        create_token.h
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

#include "get_system_info.h"

#include <libKitsunemimiHanamiCommon/enums.h>

#include <libKitsunemimiSakuraHardware/host.h>

#include <libKitsunemimiJson/json_item.h>

using namespace Kitsunemimi::Sakura;

GetSystemInfo::GetSystemInfo()
    : Kitsunemimi::Sakura::Blossom("Get all available information of the local system.")
{
    registerOutputField("info", SAKURA_MAP_TYPE, "All available information of the local system.");
}

/**
 * @brief runTask
 */
bool
GetSystemInfo::runTask(BlossomLeaf &blossomLeaf,
                       const Kitsunemimi::DataMap &,
                       BlossomStatus &status,
                       Kitsunemimi::ErrorContainer &error)
{
    // get host-info
    Kitsunemimi::Sakura::Host host;
    if(host.initHost(error) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        error.addMeesage("Failed to get system-information.");
        return false;
    }

    // parse-info
    Kitsunemimi::Json::JsonItem parsedPayload;
    if(parsedPayload.parse(host.toJsonString(), error) == false)
    {
        error.addMeesage("Failed to parse json with system-information.");
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    // creat output
    blossomLeaf.output.insert("info", parsedPayload.stealItemContent());

    return true;
}
