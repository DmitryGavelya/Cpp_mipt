#include <benchmark/benchmark.h>
#include <charconv>
#include <charconv>
#include <cstring>
#include <string>
#include <string_view>
#include <nlohmann/json.hpp>

///////////////////////////////////////////////////////////////////////////////

static const std::string json_string = R"({"value_int":42,"value_float":3.14159})";
static constexpr int    known_int_pos   = 14;
static constexpr int    known_float_pos = 30;
static constexpr int    float_digits_before = 1;
static constexpr int    float_digits_after  = 5;

///////////////////////////////////////////////////////////////////////////////

int read_int_nlohmann(std::string const & s)
{
    return nlohmann::json::parse(s)["value_int"].get<int>();
}

int read_int_from_chars(std::string const & s)
{
    int value = 0;
    std::from_chars(s.data() + known_int_pos, s.data() + s.size(), value);
    return value;
}

int read_int_stoi(std::string const & s)
{
    return std::stoi(s.substr(known_int_pos));
}

int read_int_manual(std::string const & s)
{
    int value = 0;
    for (auto i = known_int_pos; i < (int)s.size() && s[i] >= '0' && s[i] <= '9'; ++i)
        value = value * 10 + (s[i] - '0');
    return value;
}

///////////////////////////////////////////////////////////////////////////////

float read_float_nlohmann(std::string const & s)
{
    return nlohmann::json::parse(s)["value_float"].get<float>();
}

float read_float_from_chars(std::string const & s)
{
    float value = 0.f;
    std::from_chars(s.data() + known_float_pos, s.data() + s.size(), value);
    return value;
}

float read_float_stof(std::string const & s)
{
    return std::stof(s.substr(known_float_pos));
}

float read_float_manual(std::string const & s)
{
    float integer_part = 0.f, frac_part = 0.f;
    int i = known_float_pos;

    while (i < (int)s.size() && s[i] >= '0' && s[i] <= '9')
        integer_part = integer_part * 10.f + (s[i++] - '0');

    if (i < (int)s.size() && s[i] == '.') ++i;

    float factor = 0.1f;
    while (i < (int)s.size() && s[i] >= '0' && s[i] <= '9')
    {
        frac_part += (s[i++] - '0') * factor;
        factor *= 0.1f;
    }

    return integer_part + frac_part;
}

///////////////////////////////////////////////////////////////////////////////

static std::string make_template() { return json_string; }

std::string write_int_nlohmann(int value)
{
    auto j = nlohmann::json::parse(json_string);
    j["value_int"] = value;
    return j.dump();
}

std::string write_int_to_chars(int value)
{
    auto s = make_template();
    char buf[16];
    auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), value);
    s.replace(known_int_pos, ptr - buf, buf, ptr - buf);
    return s;
}

std::string write_int_to_string(int value)
{
    auto s = make_template();
    auto str = std::to_string(value);
    s.replace(known_int_pos, str.size(), str);
    return s;
}

std::string write_int_manual(int value)
{
    auto s = make_template();
    char buf[16];
    int len = 0;
    int tmp = value;
    if (tmp == 0) { buf[len++] = '0'; }
    else { while (tmp > 0) { buf[len++] = '0' + tmp % 10; tmp /= 10; } }
    std::reverse(buf, buf + len);
    s.replace(known_int_pos, len, buf, len);
    return s;
}

///////////////////////////////////////////////////////////////////////////////

std::string write_float_to_chars(float value)
{
    auto s = make_template();
    char buf[32];
    auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), value,
        std::chars_format::fixed, float_digits_after);
    s.replace(known_float_pos, ptr - buf, buf, ptr - buf);
    return s;
}

std::string write_float_nlohmann(float value)
{
    auto j = nlohmann::json::parse(json_string);
    j["value_float"] = value;
    return j.dump();
}

std::string write_float_to_string(float value)
{
    auto s = make_template();
    auto str = std::to_string(value);
    str = str.substr(0, float_digits_before + 1 + float_digits_after);
    s.replace(known_float_pos, str.size(), str);
    return s;
}

std::string write_float_manual(float value)
{
    auto s = make_template();
    char buf[32];
    int int_part = static_cast<int>(value);
    int len = 0;
    int tmp = int_part;
    if (tmp == 0) { buf[len++] = '0'; }
    else { while (tmp > 0) { buf[len++] = '0' + tmp % 10; tmp /= 10; } }
    std::reverse(buf, buf + len);
    buf[len++] = '.';
    float frac = value - int_part;
    for (int i = 0; i < float_digits_after; ++i)
    {
        frac *= 10.f;
        buf[len++] = '0' + static_cast<int>(frac) % 10;
    }
    s.replace(known_float_pos, len, buf, len);
    return s;
}

///////////////////////////////////////////////////////////////////////////////

#define BM_READ(name, fn) \
static void BM_Read_##name(benchmark::State & state) { \
    for (auto _ : state) benchmark::DoNotOptimize(fn(json_string)); \
} \
BENCHMARK(BM_Read_##name);

#define BM_WRITE(name, fn, val) \
static void BM_Write_##name(benchmark::State & state) { \
    for (auto _ : state) benchmark::DoNotOptimize(fn(val)); \
} \
BENCHMARK(BM_Write_##name);

BM_READ(Int_Nlohmann,    read_int_nlohmann)
BM_READ(Int_FromChars,   read_int_from_chars)
BM_READ(Int_Stoi,        read_int_stoi)
BM_READ(Int_Manual,      read_int_manual)

BM_READ(Float_Nlohmann,  read_float_nlohmann)
BM_READ(Float_FromChars, read_float_from_chars)
BM_READ(Float_Stof,      read_float_stof)
BM_READ(Float_Manual,    read_float_manual)

BM_WRITE(Int_Nlohmann,   write_int_nlohmann,   99)
BM_WRITE(Int_ToChars,    write_int_to_chars,    99)
BM_WRITE(Int_ToString,   write_int_to_string,   99)
BM_WRITE(Int_Manual,     write_int_manual,      99)

BM_WRITE(Float_Nlohmann, write_float_nlohmann,  2.71828f)
BM_WRITE(Float_ToChars,  write_float_to_chars,  2.71828f)
BM_WRITE(Float_ToString, write_float_to_string, 2.71828f)
BM_WRITE(Float_Manual,   write_float_manual,    2.71828f)

BENCHMARK_MAIN();