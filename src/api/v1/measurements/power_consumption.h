#ifndef POWERCONSUMPTION_H
#define POWERCONSUMPTION_H

#include <libKitsunemimiSakuraLang/blossom.h>

class PowerConsumption
        : public Kitsunemimi::Sakura::Blossom
{
public:
    PowerConsumption();

protected:
    bool runTask(Kitsunemimi::Sakura::BlossomLeaf &blossomLeaf,
                 const Kitsunemimi::DataMap &,
                 Kitsunemimi::Sakura::BlossomStatus &,
                 Kitsunemimi::ErrorContainer &);
};

#endif // POWERCONSUMPTION_H
