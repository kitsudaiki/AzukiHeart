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
 * @brief change core-ids of a thread of azuki itself
 *
 * @param threadNames name of the thread-type
 * @param coreId is of the core (physical thread) to bind to
 *
 * @return true, if successfull, false if core-id is out-of-range
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
 * @brief ThreadBinder::changeRemoteCoreIds
 * @param component
 * @param request
 * @param threadNames
 * @param coreId
 */
void
ThreadBinder::changeRemoteCoreIds(const std::string &component,
                                  Kitsunemimi::Hanami::RequestMessage &request,
                                  const std::vector<std::string> &threadNames,
                                  const long coreId)
{
    Kitsunemimi::Hanami::HanamiMessaging* msg = Kitsunemimi::Hanami::HanamiMessaging::getInstance();
    Kitsunemimi::Hanami::ResponseMessage response;

    for(const std::string &name : threadNames)
    {
        Kitsunemimi::ErrorContainer error;

        request.inputValues = "{ \"token\" : \""
                              + *AzukiRoot::componentToken
                              + "\", \"core_id\":"
                              + std::to_string(coreId)
                              + ",\"thread_name\": \""
                              + name
                              + "\"}";

        if(msg->triggerSakuraFile(component, response, request, error) == false) {
            LOG_ERROR(error);
        }

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
        Kitsunemimi::DataMap* threadMapping = requestThreadMapping(*AzukiRoot::componentToken,
                                                                   error);

        // some debug-output
        //std::cout<<"#################################################################"<<std::endl;
        //std::cout<<threadMapping->toString(true)<<std::endl;
        //std::cout<<"#################################################################"<<std::endl;

        Kitsunemimi::Hanami::RequestMessage request;
        request.id = "bind_thread_to_core";
        request.httpType = Kitsunemimi::Hanami::POST_TYPE;

        std::map<std::string,Kitsunemimi:: DataItem*>::const_iterator it;
        for(it = threadMapping->map.begin();
            it != threadMapping->map.end();
            it++)
        {
            const std::vector<std::string> threadNames = it->second->toMap()->getKeys();
            if(it->first == "azuki") {
                changeInternalCoreIds(threadNames, coreId);
            } else {
                changeRemoteCoreIds(it->first, request, threadNames, coreId);
            }
        }

        delete threadMapping;
        sleep(10);
    }
}
