/**
 * @file        blossom_initializing.h
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

#ifndef AZUKIHEART_BLOSSOM_INITIALIZING_H
#define AZUKIHEART_BLOSSOM_INITIALIZING_H

#include <libKitsunemimiSakuraLang/sakura_lang_interface.h>
#include <libKitsunemimiCommon/logger.h>

#include <libKitsunemimiHanamiEndpoints/endpoint.h>

#include <api/v1/system_info/get_system_info.h>
#include <api/v1/threading/get_thread_mapping.h>

using Kitsunemimi::Sakura::SakuraLangInterface;

void
initBlossoms()
{
    Kitsunemimi::Hanami::Endpoint* endpoints = Kitsunemimi::Hanami::Endpoint::getInstance();
    SakuraLangInterface* interface = SakuraLangInterface::getInstance();

    assert(interface->addBlossom("system", "get_info", new GetSystemInfo()));
    assert(endpoints->addEndpoint("v1/system_info",
                                  Kitsunemimi::Hanami::GET_TYPE,
                                  Kitsunemimi::Hanami::BLOSSOM_TYPE,
                                  "system",
                                  "get_info"));

    assert(interface->addBlossom("threading", "get_mapping", new GetThreadMapping()));
    assert(endpoints->addEndpoint("v1/threading",
                                  Kitsunemimi::Hanami::GET_TYPE,
                                  Kitsunemimi::Hanami::BLOSSOM_TYPE,
                                  "threading",
                                  "get_mapping"));
}

#endif // AZUKIHEART_BLOSSOM_INITIALIZING_H
