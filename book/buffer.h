#include <cstdint>
#include <atomic>
#include <type_traits>

using namespace std;

template<typename T>
class Buffer {
private:
    static constexpr uint32_t capacity = 4096;
    static constexpr uint32_t mask = capacity - 1;
    alignas(64) atomic_uint32_t producerIndex;
    alignas(64) atomic_uint32_t consumerIndex;
    alignas(64) T data[capacity] = {0};
public:
    Buffer() : producerIndex(0), consumerIndex(0) {}

    inline void produce(const T& value) {
        const uint32_t p = producerIndex.load(memory_order_relaxed);
        const uint32_t next = (p + 1) & mask;

        data[p] = value;
        producerIndex.store(next, memory_order_release);
    }

    inline void produceTwo(const T& v1, const T& v2) {
        const uint32_t p = producerIndex.load(memory_order_relaxed);
        const uint32_t next = (p + 1) & mask;
        const uint32_t nextNext = (p + 2) & mask;

        data[p] = v1;
        data[next] = v2;

        producerIndex.store(nextNext, memory_order_release);
    }
    
    inline bool consume(T& output) {
        const uint32_t c = consumerIndex.load(memory_order_relaxed);
        if (c == producerIndex.load(memory_order_acquire)) return false;
        output = data[c];
        consumerIndex.store((c + 1) & mask, memory_order_release);
        return true;
    }

    inline bool peek(T& output) {
        const uint32_t c = consumerIndex.load(memory_order_relaxed);
        if (c == producerIndex.load(memory_order_acquire)) return false;
        output = data[c];
        return true;
    }
    
    inline void clear() {
        data[capacity] = {0};
        producerIndex = 0;
        consumerIndex = 0;
    }
};
