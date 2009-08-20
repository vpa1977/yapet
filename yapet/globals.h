// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2009  Rafael Ostertag
//
// This file is part of YAPET.
//
// YAPET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//

/**
 * @file
 *
 * Header file for global values
 */

#ifndef _GLOBALS_H
#define _GLOBALS_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

namespace YAPET {
    /**
     * Holding values that are globally read- and writable.
     */
    namespace GLOBALS {

        class Globals {
            private:
		static int selected_character_pools;
		static size_t selected_password_length;
                inline Globals() {}
                inline ~Globals() {}
                inline Globals (const Globals&) {}
                inline const Globals& operator= (const Globals&) {
                    return *this;
                }

            public:
		static void setCharacterPools(int cp);
		static int getCharacterPools();
		static void setPasswordLength(size_t pl);
		static size_t getPasswordLength();
        };

    }
}
#endif // _GLOBALS_H
