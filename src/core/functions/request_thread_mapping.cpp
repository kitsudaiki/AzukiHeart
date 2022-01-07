/**
 * @file        request_thread_mapping.cpp
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

#include "request_thread_mapping.h"

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

/**
 * @brief request thread-mapping of another component
 *
 * @param completeMap pointer for the result to attach the thread-mapping of the requested component
 * @param component name of the component of which the thread-mapping should be requested
 * @param request request for getting the thread-mapping of the remote-component
 * @param error reference for error-output
 *
 * @return true, if successful, else false
 */
bool
requestComponent(Kitsunemimi::DataMap* completeMap,
                 const std::string &component,
                 const Kitsunemimi::Hanami::RequestMessage &request,
                 Kitsunemimi::ErrorContainer &error)
{
    // make request
    Kitsunemimi::Hanami::HanamiMessaging* msg = Kitsunemimi::Hanami::HanamiMessaging::getInstance();
    Kitsunemimi::Hanami::ResponseMessage response;
    if(msg->triggerSakuraFile(component, response, request, error) == false) {
        return false;
    }

    // check request-result
    if(response.success == false) {
        return false;
    }

    // parse response
    Kitsunemimi::Json::JsonItem jsonItem;
    if(jsonItem.parse(response.responseContent, error) == false) {
        return false;
    }

    // check if response has valid content
    if(jsonItem.get("thread_map").getItemContent()->isMap() == false) {
        return false;
    }

    // add part to the complete map
    completeMap->insert(component, jsonItem.get("thread_map").getItemContent()->copy()->toMap());

    return true;
}

/**
 * @brief request the thread-mapping of azuki itself
 *
 * @param completeMap pointer for the result to attach the thread-mapping of azuki
 *
 * @return always true
 */
bool
makeInternalRequest(Kitsunemimi::DataMap* completeMap,
                    Kitsunemimi::ErrorContainer &)
{
    Kitsunemimi::ThreadHandler* threadHandler = Kitsunemimi::ThreadHandler::getInstance();
    const std::vector<std::string> names = threadHandler->getRegisteredNames();
    Kitsunemimi::DataMap* result = new Kitsunemimi::DataMap();

    for(const std::string &name : names)
    {
        const std::vector<Kitsunemimi::Thread*> threads = threadHandler->getThreads(name);
        Kitsunemimi::DataArray* threadArray = new Kitsunemimi::DataArray();
        for(Kitsunemimi::Thread* thread : threads)
        {
            const std::vector<uint64_t> coreIds = thread->getCoreIds();
            Kitsunemimi::DataArray* cores = new Kitsunemimi::DataArray();
            for(const uint64_t coreId : coreIds) {
                cores->append(new Kitsunemimi::DataValue(static_cast<long>(coreId)));
            }
            threadArray->append(cores);
        }
        result->insert(name, threadArray);
    }

    completeMap->insert("azuki", result);

    return true;
}

/**
 * @brief get thread-mapping of all components
 *
 * @param completeMap map with mapping of all threads of all components
 * @param token token for the access to the other components
 * @param error reference for error-output
 *
 * @return true, if successful, else false
 */
bool
requestThreadMapping(Kitsunemimi::DataMap* completeMap,
                     const std::string &token,
                     Kitsunemimi::ErrorContainer &error)
{
    // create request
    Kitsunemimi::Hanami::RequestMessage request;
    request.id = "get_thread_mapping";
    request.httpType = Kitsunemimi::Hanami::GET_TYPE;
    request.inputValues = "{ \"token\" : \"" + token + "\"}";

    SupportedComponents* scomp = SupportedComponents::getInstance();

    //----------------------------------------------------------------------------------------------
    // request from azuki itself
    if(makeInternalRequest(completeMap, error) == false) {
        return false;
    }
    //----------------------------------------------------------------------------------------------
    if(scomp->support[Kitsunemimi::Hanami::KYOUKO]
            && requestComponent(completeMap, "kyouko", request, error) == false)
    {
        return false;
    }
    //----------------------------------------------------------------------------------------------
    if(scomp->support[Kitsunemimi::Hanami::MISAKA]
            && requestComponent(completeMap, "misaka", request, error) == false)
    {
        return false;
    }
    //----------------------------------------------------------------------------------------------
    if(scomp->support[Kitsunemimi::Hanami::SAGIRI]
            && requestComponent(completeMap, "sagiri", request, error) == false)
    {
        return false;
    }
    //----------------------------------------------------------------------------------------------
    if(scomp->support[Kitsunemimi::Hanami::NAGATO]
            && requestComponent(completeMap, "nagato", request, error) == false)
    {
        return false;
    }
    //----------------------------------------------------------------------------------------------
    if(scomp->support[Kitsunemimi::Hanami::IZUNA]
            && requestComponent(completeMap, "izuna", request, error) == false)
    {
        return false;
    }
    //----------------------------------------------------------------------------------------------
    if(requestComponent(completeMap, "torii", request, error) == false) {
        return false;
    }
    //----------------------------------------------------------------------------------------------

    return true;
}
