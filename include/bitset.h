#include <cstdint>

class Bitset {
private:
    uint64_t summary = 0;
    uint64_t bins[64] = {0};
public:
    inline void set(uint32_t price) {
        uint32_t binIndex = price >> 6;
        uint32_t bitIndex = price & 64;
        bins[binIndex] |= (1ULL << bitIndex);
        summary |= (1ULL << binIndex);
    }

    inline void unset(uint32_t price) {
        uint32_t binIndex = price >> 6;
        uint32_t bitIndex = price & 64;
        bins[binIndex] &= ~(1ULL << bitIndex);
        summary &= ~(1ULL << binIndex);        
    }

    inline int start() {
        if (!summary) return -1;

        uint32_t binIndex = __builtin_ctzll(summary);
        uint32_t bitIndex = __builtin_ctzll(bins[binIndex]);
        
        return (binIndex << 6) | bitIndex;
    }

    inline int end() {
        if (!summary) return -1;
        
        uint32_t binIndex = 63 - __builtin_clzll(summary);
        uint32_t bitIndex = 63 - __builtin_clzll(bins[binIndex]);

        return (binIndex << 6) | bitIndex;
    }
};