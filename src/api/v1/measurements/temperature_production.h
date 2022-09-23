#ifndef TEMPERATURECONSUMPTION_H
#define TEMPERATURECONSUMPTION_H

#include <libKitsunemimiSakuraLang/blossom.h>

class TemperatureProduction
        : public Kitsunemimi::Sakura::Blossom
{
public:
    TemperatureProduction();

protected:
    bool runTask(Kitsunemimi::Sakura::BlossomLeaf &blossomLeaf,
                 const Kitsunemimi::DataMap &,
                 Kitsunemimi::Sakura::BlossomStatus &,
                 Kitsunemimi::ErrorContainer &);
};

#endif // TEMPERATURECONSUMPTION_H
