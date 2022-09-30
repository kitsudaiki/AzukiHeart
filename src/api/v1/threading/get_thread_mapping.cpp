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

#include "get_thread_mapping.h"

#include <libKitsunemimiHanamiCommon/enums.h>
#include <libKitsunemimiHanamiCommon/uuid.h>
#include <libKitsunemimiHanamiCommon/component_support.h>
#include <libKitsunemimiHanamiNetwork/hanami_messaging.h>

#include <azuki_root.h>
#include <core/thread_binder.h>

using namespace Kitsunemimi::Sakura;
using Kitsunemimi::Hanami::SupportedComponents;
using Kitsunemimi::Sakura::SakuraLangInterface;

GetThreadMapping::GetThreadMapping()
    : Kitsunemimi::Sakura::Blossom("Get Mapping of the all threads of all components "
                                   "to its bound cpu-core")
{
    //----------------------------------------------------------------------------------------------
    // output
    //----------------------------------------------------------------------------------------------

    registerOutputField("thread_map",
                        SAKURA_MAP_TYPE,
                        "Map with all thread-names and its core-id as json-string.");

    //----------------------------------------------------------------------------------------------
    //
    //----------------------------------------------------------------------------------------------
}

/**
 * @brief runTask
 */
bool
GetThreadMapping::runTask(BlossomIO &blossomIO,
                          const Kitsunemimi::DataMap &,
                          BlossomStatus &,
                          Kitsunemimi::ErrorContainer &)
{
    blossomIO.output.insert("thread_map", AzukiRoot::threadBinder->getMapping());

    return true;
}
