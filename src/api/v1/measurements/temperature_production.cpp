#include "temperature_production.h"
#include <azuki_root.h>
#include <core/temperature_measuring.h>

using namespace Kitsunemimi::Sakura;

TemperatureProduction::TemperatureProduction()
    : Kitsunemimi::Sakura::Blossom("")
{
    //----------------------------------------------------------------------------------------------
    // output
    //----------------------------------------------------------------------------------------------

    registerOutputField("temperature",
                        SAKURA_MAP_TYPE,
                        "");

    //----------------------------------------------------------------------------------------------
    //
    //----------------------------------------------------------------------------------------------
}

/**
 * @brief runTask
 */
bool
TemperatureProduction::runTask(BlossomLeaf &blossomLeaf,
                                const Kitsunemimi::DataMap &,
                                BlossomStatus &,
                                Kitsunemimi::ErrorContainer &)
{
    // creat output
    blossomLeaf.output.insert("temperature", AzukiRoot::temperatureMeasuring->getJson());

    return true;
}
