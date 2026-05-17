#include <print>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

std::unordered_map<char32_t, std::string> map =
{
    { U'А', "A"   }, { U'а', "a"   },
    { U'Б', "B"   }, { U'б', "b"   },
    { U'В', "V"   }, { U'в', "v"   },
    { U'Г', "G"   }, { U'г', "g"   },
    { U'Д', "D"   }, { U'д', "d"   },
    { U'Е', "E"   }, { U'е', "e"   },
    { U'Ё', "Yo"  }, { U'ё', "yo"  },
    { U'Ж', "Zh"  }, { U'ж', "zh"  },
    { U'З', "Z"   }, { U'з', "z"   },
    { U'И', "I"   }, { U'и', "i"   },
    { U'Й', "J"   }, { U'й', "j"   },
    { U'К', "K"   }, { U'к', "k"   },
    { U'Л', "L"   }, { U'л', "l"   },
    { U'М', "M"   }, { U'м', "m"   },
    { U'Н', "N"   }, { U'н', "n"   },
    { U'О', "O"   }, { U'о', "o"   },
    { U'П', "P"   }, { U'п', "p"   },
    { U'Р', "R"   }, { U'р', "r"   },
    { U'С', "S"   }, { U'с', "s"   },
    { U'Т', "T"   }, { U'т', "t"   },
    { U'У', "U"   }, { U'у', "u"   },
    { U'Ф', "Ph"  }, { U'ф', "ph"  },
    { U'Х', "H"   }, { U'х', "h"   },
    { U'Ц', "Ts"  }, { U'ц', "ts"  },
    { U'Ч', "Ch"  }, { U'ч', "ch"  },
    { U'Ш', "Sh"  }, { U'ш', "sh"  },
    { U'Щ', "Sch" }, { U'щ', "sch" },
    { U'Ы', "I"   }, { U'ы', "i"   },
    { U'ъ', ""    }, { U'ь', "'"   },
    { U'Э', "E"   }, { U'э', "e"   },
    { U'Ю', "Yu"  }, { U'ю', "yu"  },
    { U'Я', "Ya"  }, { U'я', "ya"  },
};

auto transliterate(std::u32string const & input) -> std::string
{
    std::string result;

    for (auto c : input)
    {
        if (auto it = map.find(c); it != map.end())
            result += it->second;
        else
            result += static_cast<char>(c);
    }

    return result;
}

auto to_u32(std::string const & utf8) -> std::u32string
{
    std::u32string result;
    std::size_t i = 0;

    while (i < utf8.size())
    {
        unsigned char c = utf8[i];
        char32_t cp = 0;

        if      (c < 0x80)  { cp = c;                                                  i += 1; }
        else if (c < 0xE0)  { cp = (c & 0x1F) << 6  | (utf8[i+1] & 0x3F);            i += 2; }
        else if (c < 0xF0)  { cp = (c & 0x0F) << 12 | (utf8[i+1] & 0x3F) << 6
                                                     | (utf8[i+2] & 0x3F);             i += 3; }
        else                { cp = (c & 0x07) << 18 | (utf8[i+1] & 0x3F) << 12
                                                     | (utf8[i+2] & 0x3F) << 6
                                                     | (utf8[i+3] & 0x3F);             i += 4; }
        result += cp;
    }

    return result;
}

int main()
{
    std::fstream file("input.txt", std::ios::in);

    if (!file.is_open())
    {
        std::print("main : failed to open input.txt\n");
        return 1;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::print("{}\n", transliterate(to_u32(line)));
    }
}