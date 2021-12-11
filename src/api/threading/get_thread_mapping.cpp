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
#include <libKitsunemimiHanamiMessaging/hanami_messaging.h>

#include <libKitsunemimiCommon/threading/thread.h>
#include <libKitsunemimiCommon/threading/thread_handler.h>
#include <libKitsunemimiJson/json_item.h>

using namespace Kitsunemimi::Sakura;
using Kitsunemimi::Hanami::SupportedComponents;
using Kitsunemimi::Sakura::SakuraLangInterface;

GetThreadMapping::GetThreadMapping()
    : Kitsunemimi::Sakura::Blossom("Get Mapping of the all threads of all components "
                                   "to its bound cpu-core")
{
    registerOutputField("thread_map",
                        SAKURA_MAP_TYPE,
                        "Map with all thread-names and its core-id as json-string.");
}

/**
 * @brief handleResponse
 * @param completeDocumentation
 * @param response
 */
bool
requestComponent(Kitsunemimi::DataMap* completeMap,
                 const std::string &component,
                 const Kitsunemimi::Hanami::RequestMessage &request,
                 Kitsunemimi::ErrorContainer &error)
{
    Kitsunemimi::Hanami::HanamiMessaging* msg = Kitsunemimi::Hanami::HanamiMessaging::getInstance();
    Kitsunemimi::Hanami::ResponseMessage response;

    if(msg->triggerSakuraFile(component, response, request, error) == false) {
        return false;
    }

    if(response.success == false) {
        return false;
    }

    Kitsunemimi::Json::JsonItem jsonItem;
    if(jsonItem.parse(response.responseContent, error) == false) {
        return false;
    }

    std::string rstDocu;
    Kitsunemimi::DataMap* compMap = jsonItem.get("thread_map").getItemContent()->copy()->toMap();

    completeMap->insert(component, compMap);

    return true;
}

/**
 * @brief makeInternalRequest
 * @param completeDocumentation
 * @param error
 * @return
 */
bool
makeInternalRequest(Kitsunemimi::DataMap* completeMap,
                    Kitsunemimi::ErrorContainer &error)
{
    Kitsunemimi::ThreadHandler* threadHandler = Kitsunemimi::ThreadHandler::getInstance();
    const std::vector<std::string> names = threadHandler->getRegisteredNames();
    Kitsunemimi::DataMap* result = new Kitsunemimi::DataMap();

    for(const std::string &name : names)
    {
        const std::vector<Kitsunemimi::Thread*> threads = threadHandler->getThreads(name);
        Kitsunemimi::DataArray* threadArray = new Kitsunemimi::DataArray();
        for(Kitsunemimi::Thread* thread : threads) {
            threadArray->append(new Kitsunemimi::DataValue(thread->getCoreId()));
        }
        result->insert(name, threadArray);
    }

    completeMap->insert("azuki", result);

    return true;
}

/**
 * @brief runTask
 */
bool
GetThreadMapping::runTask(BlossomLeaf &blossomLeaf,
                          const Kitsunemimi::DataMap &context,
                          BlossomStatus &status,
                          Kitsunemimi::ErrorContainer &error)
{
    const std::string roles = context.getStringByKey("user_roles");

    Kitsunemimi::Hanami::RequestMessage request;
    request.id = "get_thread_mapping";
    request.httpType = Kitsunemimi::Hanami::GET_TYPE;
    const std::string token = context.getStringByKey("token");
    request.inputValues = "{ \"token\" : \"" + token + "\"}";

    Kitsunemimi::DataMap* completeMap = new Kitsunemimi::DataMap();

    SupportedComponents* scomp = SupportedComponents::getInstance();

    if(makeInternalRequest(completeMap, error) == false)
    {
        status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
        return false;
    }

    if(scomp->support[Kitsunemimi::Hanami::KYOUKO])
    {
        if(requestComponent(completeMap, "kyouko", request, error) == false)
        {
            status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
            return false;
        }
    }

    if(scomp->support[Kitsunemimi::Hanami::MISAKA])
    {
        if(requestComponent(completeMap, "misaka", request, error) == false)
        {
            status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
            return false;
        }
    }

    if(scomp->support[Kitsunemimi::Hanami::SAGIRI])
    {
        if(requestComponent(completeMap, "sagiri", request, error) == false)
        {
            status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
            return false;
        }
    }

    if(scomp->support[Kitsunemimi::Hanami::NAGATO])
    {
        if(requestComponent(completeMap, "nagato", request, error) == false)
        {
            status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
            return false;
        }
    }

    if(scomp->support[Kitsunemimi::Hanami::IZUNA])
    {
        if(requestComponent(completeMap, "izuna", request, error) == false)
        {
            status.statusCode = Kitsunemimi::Hanami::INTERNAL_SERVER_ERROR_RTYPE;
            return false;
        }
    }

    blossomLeaf.output.insert("thread_map", completeMap);

    return true;
}
