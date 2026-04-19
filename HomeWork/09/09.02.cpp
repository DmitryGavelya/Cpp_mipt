#include <iostream>
#include <memory>
#include <queue>

class Tree {
public:
    struct Node {
        int value;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent;

        Node(int v) : value(v) {}

        ~Node() {
            std::cout << "~Node(" << value << ")\n";
        }
    };

    std::shared_ptr<Node> root;

    void traverse_v1() const {
        if (!root) return;
        std::queue<std::shared_ptr<Node>> q;
        q.push(root);
        while (!q.empty()) {
            auto node = q.front();
            q.pop();
            std::cout << node->value << ' ';
            if (node->left)  q.push(node->left);
            if (node->right) q.push(node->right);
        }
        std::cout << '\n';
    }

    void traverse_v2() const {
        dfs(root);
        std::cout << '\n';
    }

private:
    void dfs(const std::shared_ptr<Node>& node) const {
        if (!node) return;
        std::cout << node->value << ' ';
        dfs(node->left);
        dfs(node->right);
    }
};

int main() {
    {
        Tree tree;

        tree.root = std::make_shared<Tree::Node>(1);

        tree.root->left = std::make_shared<Tree::Node>(2);
        tree.root->right = std::make_shared<Tree::Node>(3);
        tree.root->left->parent = tree.root;
        tree.root->right->parent = tree.root;

        tree.root->left->left   = std::make_shared<Tree::Node>(4);
        tree.root->left->right  = std::make_shared<Tree::Node>(5);
        tree.root->right->left  = std::make_shared<Tree::Node>(6);
        tree.root->right->right = std::make_shared<Tree::Node>(7);

        tree.root->left->left->parent   = tree.root->left;
        tree.root->left->right->parent  = tree.root->left;
        tree.root->right->left->parent  = tree.root->right;
        tree.root->right->right->parent = tree.root->right;

        std::cout << "BFS: ";
        tree.traverse_v1();

        std::cout << "DFS: ";
        tree.traverse_v2();

    }
}
