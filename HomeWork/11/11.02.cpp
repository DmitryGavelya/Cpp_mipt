#include <benchmark/benchmark.h>
#include <functional>

#if defined(__GNUC__) || defined(__clang__)
    #define NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
    #define NOINLINE __declspec(noinline)
#else
    #define NOINLINE
#endif

NOINLINE int free_function(int x) {
    return x * 2;
}

struct MyClass {
    NOINLINE int member_function(int x) {
        return x * 2;
    }
};

struct Base {
    virtual int virtual_function(int x) = 0;
    virtual ~Base() = default;
};

struct Derived : public Base {
    NOINLINE int virtual_function(int x) override {
        return x * 2;
    }
};

struct Functor {
    NOINLINE int operator()(int x) {
        return x * 2;
    }
};

static void BM_FreeFunction(benchmark::State& state) {
    int val = 0;
    for (auto _ : state) {
        val = free_function(1);
        benchmark::DoNotOptimize(val);
    }
}
BENCHMARK(BM_FreeFunction);

static void BM_MemberFunction(benchmark::State& state) {
    MyClass obj;
    int val = 0;
    for (auto _ : state) {
        val = obj.member_function(1);
        benchmark::DoNotOptimize(val);
    }
}
BENCHMARK(BM_MemberFunction);

static void BM_VirtualFunction(benchmark::State& state) {
    Derived d;
    Base* obj = &d;
    int val = 0;
    for (auto _ : state) {
        val = obj->virtual_function(1);
        benchmark::DoNotOptimize(val);
    }
}
BENCHMARK(BM_VirtualFunction);

static void BM_Functor(benchmark::State& state) {
    Functor f;
    int val = 0;
    for (auto _ : state) {
        val = f(1);
        benchmark::DoNotOptimize(val);
    }
}
BENCHMARK(BM_Functor);

static void BM_LambdaAuto(benchmark::State& state) {
    auto lambda = [](int x) NOINLINE { return x * 2; };
    int val = 0;
    for (auto _ : state) {
        val = lambda(1);
        benchmark::DoNotOptimize(val);
    }
}
BENCHMARK(BM_LambdaAuto);

static void BM_StdFunction(benchmark::State& state) {
    std::function<int(int)> std_func = [](int x) NOINLINE { return x * 2; };
    int val = 0;
    for (auto _ : state) {
        val = std_func(1);
        benchmark::DoNotOptimize(val);
    }
}
BENCHMARK(BM_StdFunction);

BENCHMARK_MAIN();