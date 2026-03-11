#pragma once

#include <array>
#include <cstddef>
#include <bit>
#include <memory>
#include <new>
#include <iostream>
#include <gtest/gtest.h>

class Entity
{
public:
    Entity();
    Entity(Entity&& other) noexcept;
   ~Entity();

    Entity& operator=(Entity&& other) noexcept;

    class Implementation*       get()       noexcept;
    class Implementation const* get() const noexcept;

    void test() const;

    Entity(const Entity&)            = delete;
    Entity& operator=(const Entity&) = delete;

private:
    class Implementation;

    alignas(std::max_align_t)
    std::array<std::byte, 16> m_storage;
};

class Entity::Implementation
{
public:
    explicit Implementation(int value = 0) noexcept : m_value(value) {}

    void test() const { std::cout << "m_value = " << m_value << '\n'; }

    int  value()          const noexcept { return m_value; }
    void setValue(int v)        noexcept { m_value = v; }

private:
    int m_value;
};

Entity::Entity()
{
    static_assert(sizeof(Implementation) <= sizeof(m_storage));
    static_assert(alignof(Implementation) <= alignof(std::max_align_t));
    ::new (m_storage.data()) Implementation();
}

Entity::Entity(Entity&& other) noexcept
{
    ::new (m_storage.data()) Implementation(std::move(*other.get()));
}

Entity::~Entity()
{
    std::destroy_at(get());
}

Entity& Entity::operator=(Entity&& other) noexcept
{
    if (this != &other)
    {
        std::destroy_at(get());
        ::new (m_storage.data()) Implementation(std::move(*other.get()));
    }
    return *this;
}

Entity::Implementation* Entity::get() noexcept
{
    return std::launder(std::bit_cast<Implementation*>(m_storage.data()));
}

const Entity::Implementation* Entity::get() const noexcept
{
    return std::launder(std::bit_cast<const Implementation*>(m_storage.data()));
}

void Entity::test() const { get()->test(); }

TEST(EntityTest, DefaultConstructAndGet)
{
    Entity e;
    EXPECT_NE(e.get(), nullptr);
    EXPECT_NE(std::as_const(e).get(), nullptr);
}

TEST(EntityTest, TestMethodCallable)
{
    Entity e;
    EXPECT_NO_THROW(e.test());
}

TEST(EntityTest, MoveConstruct)
{
    Entity e1;
    Entity e2(std::move(e1));
    EXPECT_NE(e2.get(), nullptr);
    EXPECT_NO_THROW(e2.test());
}

TEST(EntityTest, MoveAssign)
{
    Entity e1;
    Entity e2;
    e2 = std::move(e1);
    EXPECT_NE(e2.get(), nullptr);
}

TEST(EntityTest, SelfMoveAssignSafe)
{
    Entity e;
    auto* ptr = e.get();
    e = std::move(e);
    EXPECT_EQ(e.get(), ptr);
}

TEST(EntityTest, NotCopyable)
{
    EXPECT_FALSE(std::is_copy_constructible_v<Entity>);
    EXPECT_FALSE(std::is_copy_assignable_v<Entity>);
}

TEST(EntityTest, MutableGetAccess)
{
    Entity e;
    e.get()->setValue(42);
    EXPECT_EQ(e.get()->value(), 42);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
