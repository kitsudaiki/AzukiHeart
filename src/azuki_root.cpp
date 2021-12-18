/**
 * @file        misaka_root.cpp
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

#include "azuki_root.h"

#include <api/blossom_initializing.h>
#include <core/thread_binder.h>

#include <libKitsunemimiConfig/config_handler.h>
#include <libKitsunemimiCommon/files/text_file.h>
#include <libKitsunemimiJson/json_item.h>
#include <libKitsunemimiJwt/jwt.h>

#include <libKitsunemimiHanamiCommon/component_support.h>
#include <libKitsunemimiHanamiMessaging/hanami_messaging.h>

using namespace Kitsunemimi::Sakura;
using Kitsunemimi::Hanami::SupportedComponents;
using Kitsunemimi::Sakura::SakuraLangInterface;

std::string* AzukiRoot::componentToken = nullptr;

AzukiRoot::AzukiRoot() {}

/**
 * @brief AzukiRoot::init
 * @return
 */
bool
AzukiRoot::init()
{
    initBlossoms();

    SupportedComponents* scomp = SupportedComponents::getInstance();

    Kitsunemimi::Hanami::RequestMessage request;
    request.id = "token/internal";
    request.httpType = Kitsunemimi::Hanami::GET_TYPE;
    request.inputValues = "{\"service_name\":\"azuki\"}";
    Kitsunemimi::ErrorContainer error;

    if(scomp->support[Kitsunemimi::Hanami::MISAKA])
    {
        Kitsunemimi::Hanami::HanamiMessaging* msg = Kitsunemimi::Hanami::HanamiMessaging::getInstance();
        Kitsunemimi::Hanami::ResponseMessage response;

        if(msg->triggerSakuraFile("misaka", response, request, error) == false)
        {
            LOG_ERROR(error);
            return false;
        }

        if(response.success == false) {
            return false;
        }
        Kitsunemimi::Json::JsonItem jsonItem;
        if(jsonItem.parse(response.responseContent, error) == false)
        {
            LOG_ERROR(error);
            return false;
        }

        componentToken = new std::string();
        *componentToken = jsonItem.getItemContent()->toMap()->getStringByKey("token");
        if(*componentToken == "") {
            return false;
        }
    }
    else
    {
        const std::string tokenKeyStr = "-";
        CryptoPP::SecByteBlock tokenKey((unsigned char*)tokenKeyStr.c_str(), tokenKeyStr.size());
        Kitsunemimi::Jwt::Jwt jwt(tokenKey);

        Kitsunemimi::Json::JsonItem jsonItem;
        jsonItem.insert("service_name", "azuki");
        jwt.create_HS256_Token(*componentToken, jsonItem, 0);
    }

    m_threadBinder = new ThreadBinder();
    m_threadBinder->startThread();

    return true;
}
