/**
 * @file        thread_binder.h
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

#ifndef THREADBINDER_H
#define THREADBINDER_H

#include <libKitsunemimiCommon/threading/thread.h>

namespace Kitsunemimi {
namespace Hanami {
struct RequestMessage;
}
}

class ThreadBinder
        : public Kitsunemimi::Thread
{
public:
    ThreadBinder();

protected:
    void run();

private:
    void createInternalToken();
    bool changeInternalCoreIds(const std::vector<std::string> &threadNames, const long coreId);
    void changeRemoteCoreIds(const std::string &component,
                             Kitsunemimi::Hanami::RequestMessage &request,
                             const std::vector<std::string> &threadNames,
                             const long coreId);

    std::string m_token = "";
};

#endif // THREADBINDER_H
