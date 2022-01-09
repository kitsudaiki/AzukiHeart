/**
 * @file        thread_binder.cpp
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

#include "thread_binder.h"
#include <azuki_root.h>
#include <core/functions/request_thread_mapping.h>

#include <libKitsunemimiHanamiCommon/enums.h>
#include <libKitsunemimiHanamiMessaging/hanami_messaging.h>

#include <libKitsunemimiJwt/jwt.h>
#include <libKitsunemimiJson/json_item.h>
#include <libKitsunemimiCommon/threading/thread.h>
#include <libKitsunemimiCommon/threading/thread_handler.h>

ThreadBinder::ThreadBinder()
    : Kitsunemimi::Thread("Azuki_ThreadBinder")
{
}

/**
 * @brief change core-ids of the threads of azuki itself
 *
 * @param threadNames name of the thread-type
 * @param coreId is of the core (physical thread) to bind to
 *
 * @return true, if successful, false if core-id is out-of-range
 */
bool
ThreadBinder::changeInternalCoreIds(const std::vector<std::string> &threadNames,
                                    const long coreId)
{
    Kitsunemimi::ThreadHandler* threadHandler = Kitsunemimi::ThreadHandler::getInstance();
    for(const std::string &name : threadNames)
    {
        const std::vector<Kitsunemimi::Thread*> threads = threadHandler->getThreads(name);
        for(Kitsunemimi::Thread* thread : threads)
        {
            if(thread->bindThreadToCore(coreId) == false) {
                return false;
            }
        }
    }

    return true;
}

/**
 * @brief tell another component a new thread-binding
 *
 * @param component name of the component which has to be modified
 * @param request base-request for the trigger
 * @param threadNames name of the thread-type
 * @param coreId is of the core (physical thread) to bind to
 */
void
ThreadBinder::changeRemoteCoreIds(const std::string &component,
                                  Kitsunemimi::Hanami::RequestMessage &request,
                                  const std::vector<std::string> &threadNames,
                                  const long coreId)
{
    Kitsunemimi::Hanami::HanamiMessaging* msg = Kitsunemimi::Hanami::HanamiMessaging::getInstance();

    for(const std::string &name : threadNames)
    {
        Kitsunemimi::ErrorContainer error;
        Kitsunemimi::Hanami::ResponseMessage response;

        // set values for thread-binding
        request.inputValues = "{ \"token\":\""       + *AzukiRoot::componentToken + "\""
                              ", \"core_id\":"       + std::to_string(coreId)     + ""
                              ", \"thread_name\":\"" + name                       + "\"}";

        // trigger remote-action for thread-binding
        if(msg->triggerSakuraFile(component, response, request, error) == false) {
            LOG_ERROR(error);
        }

        // check response
        if(response.success == false)
        {
            error.addMeesage(response.responseContent);
            LOG_ERROR(error);
        }
    }
}

/**
 * @brief ThreadBinder::run
 */
void
ThreadBinder::run()
{
    long coreId = 0;

    while(m_abort == false)
    {
        Kitsunemimi::ErrorContainer error;
        Kitsunemimi::DataMap completeMap;

        // get thread-mapping of all components
        if(requestThreadMapping(&completeMap, *AzukiRoot::componentToken, error) == false) {
            LOG_ERROR(error);
        }

        // debug-output
        //std::cout<<"#################################################################"<<std::endl;
        //std::cout<<threadMapping->toString(true)<<std::endl;
        //std::cout<<"#################################################################"<<std::endl;

        // create request for thread-binding
        Kitsunemimi::Hanami::RequestMessage request;
        request.id = "bind_thread_to_core";
        request.httpType = Kitsunemimi::Hanami::POST_TYPE;

        // update thread-binding for all components
        std::map<std::string,Kitsunemimi:: DataItem*>::const_iterator it;
        for(it = completeMap.map.begin();
            it != completeMap.map.end();
            it++)
        {
            const std::vector<std::string> threadNames = it->second->toMap()->getKeys();
            if(it->first == "azuki") {
                changeInternalCoreIds(threadNames, coreId);
            } else {
                changeRemoteCoreIds(it->first, request, threadNames, coreId);
            }
        }

        sleep(10);
    }
}
