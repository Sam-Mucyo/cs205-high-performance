#include <iostream>

// DISCLAIMER: this code contains a race condition
class Counter
{
public:
    Counter() : count_(0) {}

    void increment() { ++count_; }
    unsigned int get_count() const { return count_; }

private:
    unsigned int count_;
};

int main(void)
{
    Counter counter;
#pragma omp parallel for
    for (int i = 0; i < 10000; ++i) {
        counter.increment();
    }
    std::cout << counter.get_count() << '\n';
    return 0;
}
