#include <cstdint>
#include <atomic>
#include <type_traits>

using namespace std;

template<typename T>
class Buffer {
private:
    static constexpr uint32_t capacity = 1024;
    static constexpr uint32_t mask = capacity - 1;
    alignas(64) atomic_uint32_t producerIndex;
    alignas(64) atomic_uint32_t consumerIndex;
    alignas(64) T data[capacity];
public:
    Buffer() : producerIndex(0), consumerIndex(0) {}

    inline void produce(const T& value) {
        const uint32_t p = producerIndex.load(memory_order_relaxed);
        const uint32_t next = (p + 1) & mask;

        data[p] = value;
        producerIndex.store(next, memory_order_release);
    }
    
    inline bool consume(T& output) {
        const uint32_t c = consumerIndex.load(memory_order_relaxed);
        if (c == producerIndex.load(memory_order_acquire)) return false;
        output = data[c];
        consumerIndex.store((c + 1) & mask, memory_order_release);
        return true;
    }
};
