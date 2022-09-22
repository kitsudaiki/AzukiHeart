/**
 * @file        value_container.cpp
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

#include "value_container.h"

/**
 * @brief ValueContainer::ValueContainer
 */
ValueContainer::ValueContainer()
{
    m_valueSections.push_back(ValueSection(60));
    m_valueSections.push_back(ValueSection(60));
    m_valueSections.push_back(ValueSection(24));
    m_valueSections.push_back(ValueSection(100));
}

/**
 * @brief ValueContainer::addValue
 * @param newValue
 */
void
ValueContainer::addValue(const float newValue)
{
    addValue(newValue, 0);
}

/**
 * @brief ValueContainer::addValue
 * @param newValue
 * @param sectionId
 */
void
ValueContainer::addValue(const float newValue, const uint64_t sectionId)
{
    // break-condition
    if(sectionId >= m_valueSections.size()) {
        return;
    }

    // set new value
    ValueSection* currentSection = &m_valueSections[sectionId];
    currentSection->values[currentSection->pos] = newValue;
    currentSection->pos++;

    // handle overflow
    if(currentSection->pos >= currentSection->values.size())
    {
        currentSection->pos = 0;

        // calc overflow-value
        float valueOverflow = 0.0f;
        for(const float val : currentSection->values) {
            valueOverflow += val;
        }
        valueOverflow /= currentSection->values.size();

        addValue(valueOverflow, sectionId + 1);
    }
}

/**
 * @brief ValueContainer::toJsonString
 * @param result
 */
void
ValueContainer::toJsonString(std::string &result)
{
    result.append("{");
    result.append("\"seconds\":[");
    appendSectionToJsonString(result, 0);
    result.append("],\n\"minutes\":[");
    appendSectionToJsonString(result, 1);
    result.append("],\n\"hours\":[");
    appendSectionToJsonString(result, 2);
    result.append("],\n\"days\":[");
    appendSectionToJsonString(result, 3);
    result.append("]}");
}

/**
 * @brief ValueContainer::toJson
 * @param result
 */
Kitsunemimi::DataMap*
ValueContainer::toJson()
{
    Kitsunemimi::DataMap* result = new Kitsunemimi::DataMap();
    result->insert("seconds",appendSectionToJson(0));
    result->insert("minutes",appendSectionToJson(1));
    result->insert("hours",appendSectionToJson(2));
    result->insert("days",appendSectionToJson(3));
    return result;
}

/**
 * @brief ValueContainer::appendSectionToJson
 * @param result
 * @param sectionId
 */
void
ValueContainer::appendSectionToJsonString(std::string &result, const uint64_t sectionId)
{
    // precheck
    if(sectionId >= m_valueSections.size()) {
        return;
    }

    ValueSection* tempValueSection = &m_valueSections[sectionId];
    uint64_t pos = tempValueSection->pos;
    for(uint64_t i = 0; i < tempValueSection->values.size(); i++)
    {
        if(i > 0) {
            result.append(",");
        }
        result.append(std::to_string(tempValueSection->values.at(pos)));
        pos = (pos + 1) % tempValueSection->values.size();
    }
}

/**
 * @brief ValueContainer::appendSectionToJson
 * @param result
 * @param sectionId
 */
Kitsunemimi::DataArray*
ValueContainer::appendSectionToJson(const uint64_t sectionId)
{
    // precheck
    if(sectionId >= m_valueSections.size()) {
        return nullptr;
    }

    Kitsunemimi::DataArray* valueList = new Kitsunemimi::DataArray();
    ValueSection* tempValueSection = &m_valueSections[sectionId];
    uint64_t pos = tempValueSection->pos;
    for(uint64_t i = 0; i < tempValueSection->values.size(); i++)
    {
        valueList->append(new Kitsunemimi::DataValue(tempValueSection->values.at(pos)));
        pos = (pos + 1) % tempValueSection->values.size();
    }

    return valueList;
}
