/**
 * @file        callbacks.h
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

#ifndef AZUKIHEART_CALLBACKS_H
#define AZUKIHEART_CALLBACKS_H

#include <libKitsunemimiSakuraNetwork/session.h>
#include <libKitsunemimiJson/json_item.h>

#include <azuki_root.h>
#include <libKitsunemimiCpu/cpu.h>
#include <libKitsunemimiSakuraHardware/cpu_core.h>
#include <libKitsunemimiSakuraHardware/cpu_package.h>
#include <libKitsunemimiSakuraHardware/cpu_thread.h>
#include <libKitsunemimiSakuraHardware/host.h>

#include <libAzukiHeart/azuki_messages.h>

void streamDataCallback(void*,
                        Kitsunemimi::Sakura::Session*,
                        const void*,
                        const uint64_t)
{

}

/**
 * @brief handle errors of message which to requires a response
 *
 * @param msg error-message
 */
inline void
handleFail(const std::string &msg,
           Kitsunemimi::Sakura::Session* session,
           const uint64_t blockerId)
{
    Kitsunemimi::ErrorContainer error;
    error.addMeesage(msg);
    LOG_ERROR(error);

    const std::string ret = "-";
    session->sendResponse(ret.c_str(), ret.size(), blockerId, error);
    return;
}

/**
 * @brief handleSetCpuSpeedRequest
 * @param msg
 * @param session
 * @param blockerId
 */
inline void
handleSetCpuSpeedRequest(const Azuki::SetCpuSpeed_Message &msg)
{
    // TODO: move the setting of the speed correctly to libKitsunemimiSakuraHardware
    std::cout<<"poi2"<<std::endl;

    Kitsunemimi::ErrorContainer error;
    uint64_t numberCpuThreads = 0;
    uint64_t minimumSpeed = 0;
    uint64_t maximumSpeed = 0;

    if(Kitsunemimi::Cpu::getNumberOfCpuThreads(numberCpuThreads, error) == false)
    {

        LOG_ERROR(error);
        return;
    }
    std::cout<<"poi3: "<<numberCpuThreads<<std::endl;

    if(Kitsunemimi::Cpu::getMinimumSpeed(minimumSpeed, 0, error) == false)
    {
        LOG_ERROR(error);
        return;
    }
    if(Kitsunemimi::Cpu::getMaximumSpeed(maximumSpeed, 0, error) == false)
    {
        LOG_ERROR(error);
        return;
    }

    if(msg.speedState == Azuki::SetCpuSpeed_Message::SpeedState::MINIMUM_SPEED)
    {
        for(uint64_t i = 0; i < numberCpuThreads; i++)
        {
            Kitsunemimi::Cpu::setMinimumSpeed(i, minimumSpeed, error);
            Kitsunemimi::Cpu::setMaximumSpeed(i, minimumSpeed, error);
        }
    }
    else if(msg.speedState == Azuki::SetCpuSpeed_Message::SpeedState::MAXIMUM_SPEED)
    {
        for(uint64_t i = 0; i < numberCpuThreads; i++)
        {
            Kitsunemimi::Cpu::setMinimumSpeed(i, maximumSpeed, error);
            Kitsunemimi::Cpu::setMaximumSpeed(i, maximumSpeed, error);
        }
    }
    else
    {
        for(uint64_t i = 0; i < numberCpuThreads; i++) {
            Kitsunemimi::Cpu::resetSpeed(i, error);
        }
    }

    return;
}

/**
 * @brief genericMessageCallback
 * @param session
 * @param data
 * @param dataSize
 * @param blockerId
 */
void
genericCallback(Kitsunemimi::Sakura::Session* session,
                void* data,
                const uint64_t dataSize,
                const uint64_t blockerId)
{
    u_int8_t* u8Data = static_cast<uint8_t*>(data);
    std::cout<<"poi1"<<std::endl;
    switch(u8Data[6])
    {
        case Azuki::SET_CPU_SPEED_MESSAGE_TYPE:
            {
                Azuki::SetCpuSpeed_Message msg;
                if(msg.read(data, dataSize) == false)
                {
                    handleFail("Receive broken set-cpu-speed-message", session, blockerId);
                    return;
                }

                handleSetCpuSpeedRequest(msg);
            }
            break;
        default:
            handleFail("Received unknown generic message", session, blockerId);
            break;
    }

}


#endif // AZUKIHEART_CALLBACKS_H
