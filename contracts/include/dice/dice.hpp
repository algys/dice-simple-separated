#pragma once

#include <game-contract-sdk/game_base.hpp>

namespace dice {

using eosio::name;
using eosio::asset;
using bytes = std::vector<char>;
using eosio::checksum256;

struct action_type {
    uint8_t value;
};

class dice: public game_sdk::game {
public:
    static constexpr uint16_t min_bet_param_type = 0;
    static constexpr uint16_t max_bet_param_type = 1;
    static constexpr uint16_t max_payout_param_type = 2;

    static constexpr uint8_t roll_action_type = 0;

public:
    struct [[eosio::table("roll")]] roll_row {
        uint64_t ses_id;
        uint32_t number;

        uint64_t primary_key() const { return ses_id; }
    };
    using roll_table = eosio::multi_index<"roll"_n, roll_row>;

public:
    dice(name receiver, name code, eosio::datastream<const char*> ds):
        game(receiver, code, ds),
        rolls(_self, _self.value)
    { }

    virtual void on_new_game(uint64_t ses_id) final;

    virtual void on_action(uint64_t ses_id, uint16_t type, std::vector<uint32_t> params) final;

    virtual void on_random(uint64_t ses_id, checksum256 rand) final;

    virtual void on_finish(uint64_t ses_id) final;

private:
    void check_params(uint64_t ses_id);
    void check_bet(uint64_t ses_id);
    asset calc_max_win(uint64_t ses_id, uint32_t num);
    uint32_t rand_range(const checksum256& rand, uint32_t lower, uint32_t upper);

private:
    roll_table rolls;
};

} // namespace dice
