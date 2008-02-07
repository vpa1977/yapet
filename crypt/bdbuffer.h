// -*- c++ -*-
//
// $Id$
//
// YAPET -- Yet Another Password Encryption Tool
// Copyright (C) 2008  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _BDBUFFER_H
#define _BDBUFFER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif

#ifdef HAVE_STDLIB_h
# include <stdlib.h>
#endif

#ifdef HAVE_STDEXCEPT
# include <stdexcept>
#endif

#include "yapetexception.h"

namespace YAPET {
    /**
     * @brief A wrapper-class for allocating and securely dellocating
     * memory
     *
     * The BDBuffer class is a wrapper-class for allocating and
     * deallocating memory for data of unsigned 8bit integers.
     *
     * Its primary intend is to make sure the memory is cleared after
     * deallocation. It does so by zero'ing out the entire buffer upon
     * destruction of the object.
     *
     * The class does not manage access to the allocated buffer by
     * providing methods for accessing the memory. The pointer to the
     * allocated memory can be obtained by casting to an \c uint8_t
     * pointer.
     */
    class BDBuffer {
	private:
	    /**
	     * @brief Size of allocated memory chunk
	     *
	     * Holds the size of the allocated memory chunk in bytes.
	     */
	    uint32_t _size;

	    /**
	     * @brief pointer to the allocated memory
	     *
	     * Holds the pointer to the allocated memory.
	     */
	    uint8_t* data;

	protected:
	    //! Allocates memory of a given size
	    uint8_t* alloc_mem(uint32_t s) throw(YAPETException);
	    //! Clears and frees memory
	    void free_mem(uint8_t* d, uint32_t s);

	public:
	    //! Initializes the object with a given size of memory
	    BDBuffer(uint32_t is) throw(YAPETException);
	    BDBuffer();
	    BDBuffer(const BDBuffer& ed) throw(YAPETException);
	    //! Destructor
	    ~BDBuffer();

	    //! Resize the memory to a given size
	    void resize(uint32_t ns) throw(YAPETException);
	    /**
	     * @brief Get the size of the buffer
	     *
	     * Returns the size of the allocated memory chunk used as
	     * buffer.
	     *
	     * @return the size of the allocated memory in bytes.
	     */
	    uint32_t size() const { return _size; }

	    //! Access a location inside the memory chunk
	    uint8_t* at(uint32_t pos) throw(std::out_of_range);
	    //! Access a location inside the memory chunk
	    const uint8_t* at(uint32_t pos) const throw(std::out_of_range);

	    //! Returns the pointer to the memory chunk
	    const uint8_t* operator()() const { return data; }
	    //! Returns the pointer to the memory chunk
	    uint8_t* operator()() { return data; }
	    
	    //! Returns the pointer to the memory chunk
	    operator uint8_t*() { return data; }
	    //! Returns the pointer to the memory chunk
	    operator const uint8_t*() const { return data; }

	    //! Returns the pointer to the memory chunk
	    operator const void*() { return (void*)data; }

	    const BDBuffer& operator=(const BDBuffer& ed);
	    
    };
	
}

#endif // _BDBUFFER_H

