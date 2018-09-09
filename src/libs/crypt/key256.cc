// $Id$
//
// Copyright (C) 2018  Rafael Ostertag
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
// Additional permission under GNU GPL version 3 section 7
//
// If you modify this program, or any covered work, by linking or combining it
// with the OpenSSL project's OpenSSL library (or a modified version of that
// library), containing parts covered by the terms of the OpenSSL or SSLeay
// licenses, Rafael Ostertag grants you additional permission to convey the
// resulting work.  Corresponding Source for a non-source form of such a
// combination shall include the source code for the parts of OpenSSL used as
// well as that of the covered work.
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstring>

#include "intl.h"
#include "key256.hh"

using namespace yapet;

namespace {
/**
 * The max length of the blowfish key in bytes (448 bits)
 */
constexpr auto KEY_LENGTH = 32;
/**
 * The length of the initialization vector
 */
constexpr auto IVEC_LENGTH = 16;

inline EVP_MD_CTX* createContext() {
#ifdef HAVE_EVP_MD_CTX_CREATE
    return EVP_MD_CTX_create();
#elif HAVE_EVP_MD_CTX_NEW
    return EVP_MD_CTX_new();
#else
#error "Neither EVP_MD_CTX_create() nor EVP_MD_CTX_new() available"
#endif
}

inline void destroyContext(EVP_MD_CTX* context) {
#ifdef HAVE_EVP_MD_CTX_DESTROY
    EVP_MD_CTX_destroy(context);
#elif HAVE_EVP_MD_CTX_FREE
    EVP_MD_CTX_free(context);
#else
#error "Neither EVP_MD_CTX_destroy() nor EVP_MD_CTX_free() available"
#endif
}

inline SecureArray hash(const SecureArray& text, const EVP_MD* md) {
    if (md == 0) throw YAPET::YAPETException(_("Empty EVP_MD structure"));

    EVP_MD_CTX* mdctx = createContext();
    if (mdctx == nullptr) {
        throw YAPET::YAPETException(_("Error initializing MD context"));
    }

    int retval = EVP_DigestInit_ex(mdctx, md, 0);
    if (retval == 0) {
        destroyContext(mdctx);
        throw YAPET::YAPETException(_("Unable to initialize the digest"));
    }

    retval = EVP_DigestUpdate(mdctx, *text, text.size());
    if (retval == 0) {
        destroyContext(mdctx);
        throw YAPET::YAPETException(_("Unable to update the digest"));
    }

    SecureArray::size_type hashSize = EVP_MD_size(md);
    SecureArray hash{hashSize};

    retval = EVP_DigestFinal(mdctx, *hash, nullptr);
    if (retval == 0) {
        destroyContext(mdctx);
        throw YAPET::YAPETException(_("Unable to finalize the digest"));
    }
    destroyContext(mdctx);

    return hash;
}
}  // namespace

/**
 * Initializes the key and the initialization vector. Make sure you
 * securely destroy the password provided to this method.
 *
 * @param password a pointer to the location the password is
 * stored. The password has to be zero-terminated.
 */
Key256::Key256() : _key{0}, _ivec{IVEC_LENGTH} {}

void Key256::password(const SecureArray& password) {
    SecureArray passwordWithoutZeroTerminator{password.size() - 1};
    passwordWithoutZeroTerminator << password;

    _key = hash(passwordWithoutZeroTerminator, EVP_sha256());

    if (_key.size() != KEY_LENGTH) {
        char tmp[100];
        snprintf(tmp, 100,
                 _("Effective key length of %d does not match expected key "
                   "length %d"),
                 _key.size(), KEY_LENGTH);
        throw YAPET::YAPETException(tmp);
    }

    std::memcpy(*_ivec, *_key, IVEC_LENGTH);
}

Key256::Key256(Key256&& k)
    : _key{std::move(k._key)}, _ivec{std::move(k._ivec)} {}

Key256::Key256(const Key256& k) : _key{k._key}, _ivec{k._ivec} {}

Key256& Key256::operator=(const Key256& k) {
    if (this == &k) return *this;

    _key = k._key;
    _ivec = k._ivec;
    return *this;
}

Key256& Key256::operator=(Key256&& k) {
    if (this == &k) return *this;

    _key = std::move(k._key);
    _ivec = std::move(k._ivec);
    return *this;
}