#ifndef MY_RING_BUFFER_H
#define MY_RING_BUFFER_H

#include <atomic>
using namespace std;

template <typename T, size_t Cap>
class my_ring_buffer {
    T data[Cap];
    //atomic<size_t> head{0}, tail{0}, write{0};
    atomic<size_t> head, tail, write;

public:
    my_ring_buffer() = default;
    my_ring_buffer(const my_ring_buffer&) = delete;
    my_ring_buffer& operator=(const my_ring_buffer&) = delete;
    my_ring_buffer& operator=(const my_ring_buffer&) volatile = delete;

    bool push(const T& val)
    {
        size_t t, w;
        do {
            t = tail.load(memory_order_relaxed);
            if ((t+1) % Cap == head.load(memory_order_acquire))
                return false;
        } while (!tail.compare_exchange_weak(t, (t+1)%Cap, memory_order_relaxed));

        data[t] = val;
        do {
            w = t;
        } while (!write.compare_exchange_weak(w, (w+1)%Cap, memory_order_release, memory_order_relaxed));

        return true;   
    }

    bool pop(T& val)
    {
        size_t h;
        do {
            h = head.load(memory_order_relaxed);
            if (h == write.load(memory_order_acquire))
                return false;
            val = data[h];
        } while (!head.compare_exchange_weak(h, (h+1)%Cap, memory_order_release, memory_order_relaxed));
        return true;
    }
};

#endif //MY_RING_BUFFER_H
