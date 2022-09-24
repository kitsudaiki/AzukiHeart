#include "power_consumption.h"
#include <azuki_root.h>
#include <core/power_measuring.h>

using namespace Kitsunemimi::Sakura;

PowerConsumption::PowerConsumption()
    : Kitsunemimi::Sakura::Blossom("")
{
    //----------------------------------------------------------------------------------------------
    // output
    //----------------------------------------------------------------------------------------------

    registerOutputField("power",
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
PowerConsumption::runTask(BlossomLeaf &blossomLeaf,
                           const Kitsunemimi::DataMap &,
                           BlossomStatus &,
                           Kitsunemimi::ErrorContainer &)
{
    // creat output
    blossomLeaf.output.insert("power", AzukiRoot::powerMeasuring->getJson());

    return true;
}
