#include <cstddef>
#include <new>
#include <print>
#include <gtest/gtest.h>

template <typename D>
class Entity
{
public:
    static void* operator new(std::size_t size)
    {
        std::print("Entity::operator new\n");
        return ::operator new(size);
    }

    static void operator delete(void* x)
    {
        std::print("Entity::operator delete\n");
        ::operator delete(x);
    }

    static void* operator new[](std::size_t size)
    {
        std::print("Entity::operator new[]\n");
        return ::operator new[](size);
    }

    static void operator delete[](void* x)
    {
        std::print("Entity::operator delete[]\n");
        ::operator delete[](x);
    }

    static void* operator new(std::size_t size, const std::nothrow_t& nt) noexcept
    {
        std::print("Entity::operator new (nothrow)\n");
        return ::operator new(size, nt);
    }

    static void operator delete(void* x, const std::nothrow_t&) noexcept
    {
        std::print("Entity::operator delete (nothrow)\n");
        ::operator delete(x);
    }

    static void* operator new[](std::size_t size, const std::nothrow_t& nt) noexcept
    {
        std::print("Entity::operator new[] (nothrow)\n");
        return ::operator new[](size, nt);
    }

    static void operator delete[](void* x, const std::nothrow_t&) noexcept
    {
        std::print("Entity::operator delete[] (nothrow)\n");
        ::operator delete[](x);
    }

protected:
    Entity()  = default;
    ~Entity() = default;
};

class Client : private Entity<Client>
{
private:
    using base_t = Entity<Client>;

public:
    Client()  { std::print("Client:: Client\n"); }
    ~Client() { std::print("Client::~Client\n"); }

    using base_t::operator new,
          base_t::operator delete,
          base_t::operator new[],
          base_t::operator delete[];
};

TEST(EntityTest, ScalarNewDelete)
{
    Client* p = new Client;
    EXPECT_NE(p, nullptr);
    delete p;
}

TEST(EntityTest, ScalarNothrowNewDelete)
{
    Client* p = new (std::nothrow) Client;
    EXPECT_NE(p, nullptr);
    delete p;
}

TEST(EntityTest, ArrayNewDelete)
{
    Client* p = new Client[3];
    EXPECT_NE(p, nullptr);
    delete[] p;
}

TEST(EntityTest, ArrayNothrowNewDelete)
{
    Client* p = new (std::nothrow) Client[3];
    EXPECT_NE(p, nullptr);
    delete[] p;
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
