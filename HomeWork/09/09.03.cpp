#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <gtest/gtest.h>

class Tree {
public:
    struct Node {
        int value;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent;

        Node(int v) : value(v) {
            std::cout << "Node created: " << value << "\n";
        }

        ~Node() {
            std::cout << "Node destroyed: " << value << "\n";
        }
    };

    std::shared_ptr<Node> root;

    void traverse_v1() const {
        if (!root) return;
        std::queue<std::shared_ptr<Node>> q;
        q.push(root);

        while (!q.empty()) {
            auto current = q.front();
            q.pop();
            std::cout << current->value << " ";

            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        std::cout << "\n";
    }

    void traverse_v2() const {
        if (!root) return;
        std::stack<std::shared_ptr<Node>> s;
        s.push(root);

        while (!s.empty()) {
            auto current = s.top();
            s.pop();
            std::cout << current->value << " ";

            if (current->right) s.push(current->right);
            if (current->left) s.push(current->left);
        }
        std::cout << "\n";
    }
};

void add_children(std::shared_ptr<Tree::Node> parent,
                  std::shared_ptr<Tree::Node> left,
                  std::shared_ptr<Tree::Node> right) {
    parent->left = left;
    if (left) left->parent = parent;

    parent->right = right;
    if (right) right->parent = parent;
}

TEST(TreeTest, FullTraversalAndDestruction) {
    testing::internal::CaptureStdout();

    {
        Tree tree;
        auto n1 = std::make_shared<Tree::Node>(1);
        auto n2 = std::make_shared<Tree::Node>(2);
        auto n3 = std::make_shared<Tree::Node>(3);
        auto n4 = std::make_shared<Tree::Node>(4);
        auto n5 = std::make_shared<Tree::Node>(5);
        auto n6 = std::make_shared<Tree::Node>(6);
        auto n7 = std::make_shared<Tree::Node>(7);

        tree.root = n1;
        add_children(n1, n2, n3);
        add_children(n2, n4, n5);
        add_children(n3, n6, n7);

        std::cout << "BFS: ";
        tree.traverse_v1();

        std::cout << "DFS: ";
        tree.traverse_v2();
    }

    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("BFS: 1 2 3 4 5 6 7"), std::string::npos);

    EXPECT_NE(output.find("DFS: 1 2 4 5 3 6 7"), std::string::npos);

    EXPECT_NE(output.find("Node destroyed: 1"), std::string::npos);
    EXPECT_NE(output.find("Node destroyed: 7"), std::string::npos);

    int destroyed_count = 0;
    size_t pos = 0;
    while ((pos = output.find("Node destroyed:", pos)) != std::string::npos) {
        destroyed_count++;
        pos += 1;
    }
    EXPECT_EQ(destroyed_count, 7);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
