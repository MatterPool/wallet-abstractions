// Copyright (c) 2018-2019 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef ABSTRACTIONS_WALLET_KEYS
#define ABSTRACTIONS_WALLET_KEYS

#include <abstractions/crypto/secp256k1.hpp>
#include <abstractions/crypto/hash/sha256.hpp>
#include <abstractions/crypto/hash/ripemd160.hpp>
#include <data/crypto/one_way.hpp>
#include "tag.hpp"
#include "hash.hpp"
#include "address.hpp"

namespace abstractions::bitcoin {
    
    struct pubkey;
    struct uncompressed_pubkey;
    
    using signature = secp256k1::signature;
    
    struct secret {
        secp256k1::secret Secret;
        secret() : Secret{} {}
        secret(const secp256k1::secret& s) : Secret{s} {}
        
        bool valid() const {
            return Secret.valid();
        }
        
        pubkey to_public() const;
        uncompressed_pubkey to_public_uncompressed() const;
        
        secret operator+(const secret&) const;
        secret operator*(const secret&) const;
        
        secret& operator=(const secret& s);
        
        explicit secret(string wif);
        
        bitcoin::address address() const;
        string write();
    };
    
    struct pubkey : public tag {
        secp256k1::compressed_pubkey Pubkey;
        pubkey() : Pubkey{} {}
        pubkey(const secp256k1::compressed_pubkey p) : Pubkey{p} {}
        
        bool valid() const {
            return Pubkey.valid();
        }
        
        pubkey operator+(const pubkey&) const;
        pubkey operator*(const secret&) const;
        
        pubkey& operator=(const pubkey& p);
        
        explicit pubkey(string hex);
        
        bitcoin::address address() const;
        string write();
    };
    
    struct uncompressed_pubkey : public tag {
        secp256k1::uncompressed_pubkey Pubkey;
        uncompressed_pubkey() : Pubkey{} {}
        uncompressed_pubkey(const secp256k1::uncompressed_pubkey p) : Pubkey{p} {}
        
        bool valid() const {
            return Pubkey.valid();
        }
        
        uncompressed_pubkey operator+(const uncompressed_pubkey&) const;
        uncompressed_pubkey operator*(const secret&) const;
        
        explicit uncompressed_pubkey(string hex);
        
        bitcoin::address address() const;
        string write();
    };
    
    constexpr data::math::module<pubkey, secret> is_module{};
    constexpr data::crypto::signature_scheme<secret, pubkey, const sha256::digest, signature> is_signature_scheme{};
    
    namespace wif {
        // 52 characters base58, starts with a 'K' or 'L'
        bool read(const string&, secret&);
        string write(secret&);
    }
}

namespace data::crypto {
    template <> struct to_public<
        abstractions::bitcoin::secret, 
        abstractions::bitcoin::uncompressed_pubkey>
            : function::abstract<abstractions::bitcoin::uncompressed_pubkey, const abstractions::bitcoin::secret&> {
        abstractions::bitcoin::uncompressed_pubkey operator()(
            const abstractions::bitcoin::secret& s) const noexcept final override {
            return s.to_public_uncompressed();
        }
    };
    
    template <> struct to_public<
        abstractions::bitcoin::secret, 
        abstractions::bitcoin::pubkey>
            : function::abstract<abstractions::bitcoin::pubkey, const abstractions::bitcoin::secret&> {
        abstractions::bitcoin::pubkey operator()(
            const abstractions::bitcoin::secret& s) const noexcept final override {
            return s.to_public();
        }
    };
}

namespace abstractions::bitcoin {
    inline pubkey secret::to_public() const {
        return pubkey{Secret.to_public_compressed()};
    }
    
    inline secret secret::operator+(const secret& s) const {
        return secret{Secret + s.Secret};
    }
    
    inline secret secret::operator*(const secret& s) const {
        return secret{Secret * s.Secret};
    }
    
    inline pubkey pubkey::operator+(const pubkey& p) const {
        return pubkey{Pubkey + p.Pubkey};
    }
    
    inline pubkey pubkey::operator*(const secret& s) const {
        return pubkey{Pubkey * s.Secret};
    }
    
    inline uncompressed_pubkey uncompressed_pubkey::operator+(const uncompressed_pubkey& p) const {
        return uncompressed_pubkey{Pubkey + p.Pubkey};
    }
    
    inline uncompressed_pubkey uncompressed_pubkey::operator*(const secret& s) const {
        return uncompressed_pubkey{Pubkey * s.Secret};
    }
    
    inline bitcoin::address pubkey::address() const {
        return secp256k1::address(Pubkey);
    }
    
    inline bitcoin::address uncompressed_pubkey::address() const {
        return secp256k1::address(Pubkey);
    }
    
    inline bitcoin::address secret::address() const {
        return secp256k1::address_compressed(Secret);
    }



} 

#endif

