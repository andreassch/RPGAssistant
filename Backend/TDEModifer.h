/**
 * Implementation of computations of modifiers due to bad health for The Dark Eye, fifth edition.
 * This module is part of the Role Playing Game Assistant.
 *
 * (c) 2019, 2020 Andreas Schneider (andreas underscore schn at web dot de)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * The Dark Eye, its respective logos, as well as Aventuria, Dere,
 * Myranor, Riesland, Tharun and Uthuria, and their respective logos,
 * are trademarks of Ulisses Spiele GmbH, Waldems.
 */

#pragma once

#include "../config.h"

#ifdef RPG_SYSTEM_TDE_AVENTURIA

namespace TDEModifier {
    /**
     * @brief Compute modifier for The Dark Eye, fith edition.
     * @param lep Current hit points ("Lebenspunkte") of the character.
     * @param le Maximal hit points ("Lebensenergie") of the character.
     * @return Modificator due to bad health.
     */
    int computeModifier(const int lep, const int le);
}

#endif // RPG_SYSTEM_TDE_AVENTURIA
