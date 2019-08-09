#pragma once

#include <cstdint>

#include "cryptonote_config.h"

namespace config
{

namespace graft
{

constexpr uint8_t STAKE_TRANSACTION_PROCESSING_DB_VERSION = 13;

constexpr uint64_t STAKE_MIN_UNLOCK_TIME = 10;
constexpr uint64_t STAKE_MAX_UNLOCK_TIME_V15 = 5000;
constexpr uint64_t STAKE_MAX_UNLOCK_TIME = 23040;
constexpr uint64_t STAKE_MIN_UNLOCK_TIME_FOR_WALLET = 60;
constexpr uint64_t STAKE_VALIDATION_PERIOD = 6;
constexpr uint64_t TRUSTED_RESTAKING_PERIOD = 6;
constexpr uint64_t SUPERNODE_HISTORY_SIZE = 100;

//  50,000 GRFT –  tier 1
//  90,000 GRFT –  tier 2
//  150,000 GRFT – tier 3
//  250,000 GRFT – tier 4
constexpr uint64_t TIER1_STAKE_AMOUNT = COIN *  50000;
constexpr uint64_t TIER2_STAKE_AMOUNT = COIN *  90000;
constexpr uint64_t TIER3_STAKE_AMOUNT = COIN * 150000;
constexpr uint64_t TIER4_STAKE_AMOUNT = COIN * 250000;

constexpr size_t TIERS_COUNT = 4;

}

}
