#include <gtest/gtest.h>
#include <memory>

struct Entity {
  int x;
  int y;
};

TEST(MemoryManagementTest, SharedPtrAliasing) {
  std::shared_ptr<Entity> entity_ptr = std::make_shared<Entity>();
  entity_ptr->x = 42;
  entity_ptr->y = 100;
  std::shared_ptr<int> field_ptr(entity_ptr, &entity_ptr->x);
  EXPECT_EQ(*field_ptr, 42);
  EXPECT_EQ(entity_ptr.use_count(), 2);
  EXPECT_EQ(field_ptr.use_count(), 2);
  entity_ptr.reset();
  EXPECT_EQ(*field_ptr, 42);
  EXPECT_EQ(field_ptr.use_count(), 1);
  *field_ptr = 55;
  EXPECT_EQ(*field_ptr, 55);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
