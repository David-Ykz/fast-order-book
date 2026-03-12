#include <cstdint>

struct TradeEvent {
    uint64_t client;
    uint32_t price;
    uint32_t quantity;
    uint32_t ticker;
};
