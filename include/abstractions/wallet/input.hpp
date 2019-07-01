// Copyright (c) 2018-2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ABSTRACTIONS_WALLET_INPUT
#define ABSTRACTIONS_WALLET_INPUT

#include "outpoint.hpp"
#include "script.hpp"
#include <abstractions/transaction.hpp>

namespace abstractions {
    
    namespace bitcoin {
        
        using input = typename abstractions::input<outpoint, script>::representation;
        
    }
    
} 

#endif
