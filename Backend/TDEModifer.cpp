#include "TDEModifer.h"

#ifdef RPG_SYSTEM_TDE_AVENTURIA
#include <cmath>

int TDEModifier::computeModifier(const int lep, const int le)
{
    if (lep <= 5)
        return 4;
    else if (static_cast<float>(lep) <= round(static_cast<float>(le)*0.25))
        return 3;
    else if (static_cast<float>(lep) <= round(static_cast<float>(le)*0.5))
        return 2;
    else if (static_cast<float>(lep) <= round(static_cast<float>(le)*0.75))
        return 1;
    else
        return 0;
}
#endif // RPG_SYSTEM_TDE_AVENTURIA
