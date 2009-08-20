// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008, 2009  Rafael Ostertag
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

#ifndef _UIEXCEPTION_H
#define _UIEXCEPTION_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_EXCEPTION
# include <exception>
#endif

#ifdef HAVE_STRING
# include <string>
#endif

#include "../intl.h"

namespace YAPET {
    namespace UI {
        /**
         * @brief User interface exception.
         *
         * Exception class used by the user interface.
         */
        class UIException : public std::exception {
            private:
                std::string message;

            public:
                inline UIException() throw() : exception(),
                        message (_ ("Generic UI exception") ) {}
                inline UIException (std::string msg) throw() : exception(),
                        message (msg) {}
                inline UIException (const UIException& ex) throw() {
                    message = ex.message;
                }
                inline virtual ~UIException() throw() { /* empty */ }
                inline const UIException& operator= (const UIException& ex)
                throw() {
                    if (this == &ex) return *this;

                    message = ex.message;
                    return *this;
                }
                inline virtual const char* what() const throw() {
                    return message.c_str();
                }
        };

    }
}
#endif // _UIEXCEPTION_H
