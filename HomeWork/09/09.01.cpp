#include <gtest/gtest.h>
#include <iostream>
#include <source_location>
#include <string>

class Tracer {
public:
    explicit Tracer(std::source_location loc = std::source_location::current()) noexcept
        : loc_(loc) {
        std::cout << "-> " << loc_.function_name()
                  << " (" << loc_.file_name() << ":" << loc_.line() << ")\n";
    }

    ~Tracer() noexcept {
        std::cout << "<- " << loc_.function_name()
                  << " (" << loc_.file_name() << ":" << loc_.line() << ")\n";
    }

    Tracer(const Tracer&) = delete;
    Tracer& operator=(const Tracer&) = delete;
    Tracer(Tracer&&) = delete;
    Tracer& operator=(Tracer&&) = delete;

private:
    std::source_location loc_;
};

#ifndef NDEBUG
#define TRACE_CONCAT_INNER(a, b) a##b
#define TRACE_CONCAT(a, b) TRACE_CONCAT_INNER(a, b)
#define trace() ::Tracer TRACE_CONCAT(_tracer_, __LINE__){std::source_location::current()}
#else
#define trace() ((void)0)
#endif

namespace {
static void Inner() { trace(); }
static void Outer() { trace(); Inner(); }
}  // namespace

#ifndef NDEBUG
TEST(Tracer, PrintsPairedMessages) {
    testing::internal::CaptureStdout();
    Outer();
    const std::string out = testing::internal::GetCapturedStdout();

    const auto first_enter = out.find("->");
    const auto first_exit = out.find("<-");
    ASSERT_NE(first_enter, std::string::npos);
    ASSERT_NE(first_exit, std::string::npos);
    EXPECT_LT(first_enter, first_exit);

    const auto inner_pos = out.find("Inner");
    const auto outer_pos = out.find("Outer");
    EXPECT_NE(inner_pos, std::string::npos);
    EXPECT_NE(outer_pos, std::string::npos);
}
#else
TEST(Tracer, DisabledWithNDebug) {
    testing::internal::CaptureStdout();
    Outer();
    const std::string out = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(out.empty());
}
#endif

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
