#ifndef _CRYPTO_HH
#define _CRYPTO_HH

#include <openssl/evp.h>
#include <memory>

#include "key.hh"
#include "securearray.hh"

namespace yapet {
/**
 * Base class for encryption and decryption.
 */
class Crypto {
   private:
    std::shared_ptr<Key> _key;

   protected:
    virtual const EVP_CIPHER* getCipher() const = 0;

   public:
    /**
     * Initializes the class with the given key, which is used for
     * encryption and decryption.
     *
     * The constructor tries to set the key length of the cipher used to
     * the length of the key provided. If this fails, a \c YAPETException is
     * thrown.
     *
     * @param k the key used for encryption/decryption.
     */
    Crypto(const std::shared_ptr<Key>& key);
    virtual ~Crypto(){};

    Crypto(const Crypto&);
    Crypto& operator=(const Crypto& c);

    Crypto(Crypto&& c);
    Crypto& operator=(Crypto&& c);

    /**
     * Encrypt data using the cipher provided by \c getCipher().
     *
     * @throw YAPETException in case the key length of the cipher cannot be
     * set to the length of the key provided.
     *
     * @throw YAPETEncryptionException in case of cipher errors.
     */
    virtual SecureArray encrypt(const SecureArray& plainText);
    /**
     * Decrypt data using the cipher provided by \c getCipher().
     *
     * @throw YAPETException in case the key length of the cipher cannot be
     * set to the length of the key provided.
     *
     * @throw YAPETEncryptionException in case of cipher errors.
     */
    virtual SecureArray decrypt(const SecureArray& cipherText);
};
}  // namespace yapet

#endif