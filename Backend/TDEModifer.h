#pragma once

#include "../config.h"

#ifdef RPG_SYSTEM_TDE_AVENTURIA

namespace TDEModifier {
    /**
     * @brief Compute modifier for the German rpg The Dark Eye ("Das Schwarze Auge"), fith edition.
     * @param lep Current hit points ("Lebenspunkte") of the character.
     * @param le Maximal hit points ("Lebensenergie") of the character.
     * @return Modificator due to bad health.
     */
    int computeModifier(const int lep, const int le);
}

#endif // RPG_SYSTEM_TDE_AVENTURIA
