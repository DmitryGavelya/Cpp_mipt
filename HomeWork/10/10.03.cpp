#include <gtest/gtest.h>
#include <vector>
#include <iostream>

class GameOfLife {
public:
    GameOfLife(int width, int height) : width(width), height(height) {
        board.resize(height, std::vector<bool>(width, false));
    }

    void set_pattern(const std::vector<std::pair<int, int>>& alive_cells) {
        for (const auto& cell : alive_cells) {
            if (cell.first >= 0 && cell.first < height && cell.second >= 0 && cell.second < width) {
                board[cell.first][cell.second] = true;
            }
        }
    }

    void next_generation() {
        std::vector<std::vector<bool>> next_board = board;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int alive_neighbors = count_alive_neighbors(i, j);
                if (board[i][j]) {
                    if (alive_neighbors < 2 || alive_neighbors > 3) {
                        next_board[i][j] = false;
                    }
                } else {
                    if (alive_neighbors == 3) {
                        next_board[i][j] = true;
                    }
                }
            }
        }
        board = next_board;
    }

    void print() const {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                std::cout << (board[i][j] ? '*' : '.') << " ";
            }
            std::cout << "\n";
        }
        std::cout << std::string(width * 2 - 1, '-') << "\n";
    }

    bool is_alive(int row, int col) const {
        if (row >= 0 && row < height && col >= 0 && col < width) {
            return board[row][col];
        }
        return false;
    }

private:
    int width;
    int height;
    std::vector<std::vector<bool>> board;

    int count_alive_neighbors(int row, int col) const {
        int count = 0;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue;
                int r = row + i;
                int c = col + j;
                if (r >= 0 && r < height && c >= 0 && c < width) {
                    if (board[r][c]) count++;
                }
            }
        }
        return count;
    }
};

TEST(GameOfLifeTest, BlinkerOscillator) {
    GameOfLife game(10, 10);
    game.set_pattern({{4, 4}, {4, 5}, {4, 6}});

    std::cout << "Generation 0:\n";
    game.print();

    EXPECT_FALSE(game.is_alive(3, 5));
    EXPECT_TRUE(game.is_alive(4, 4));
    EXPECT_TRUE(game.is_alive(4, 5));
    EXPECT_TRUE(game.is_alive(4, 6));
    EXPECT_FALSE(game.is_alive(5, 5));

    game.next_generation();
    std::cout << "Generation 1:\n";
    game.print();

    EXPECT_TRUE(game.is_alive(3, 5));
    EXPECT_FALSE(game.is_alive(4, 4));
    EXPECT_TRUE(game.is_alive(4, 5));
    EXPECT_FALSE(game.is_alive(4, 6));
    EXPECT_TRUE(game.is_alive(5, 5));

    game.next_generation();
    std::cout << "Generation 2:\n";
    game.print();

    EXPECT_FALSE(game.is_alive(3, 5));
    EXPECT_TRUE(game.is_alive(4, 4));
    EXPECT_TRUE(game.is_alive(4, 5));
    EXPECT_TRUE(game.is_alive(4, 6));
    EXPECT_FALSE(game.is_alive(5, 5));
}

TEST(GameOfLifeTest, BlockStillLife) {
    GameOfLife game(10, 10);
    game.set_pattern({{1, 1}, {1, 2}, {2, 1}, {2, 2}});

    for (int i = 0; i < 3; ++i) {
        EXPECT_TRUE(game.is_alive(1, 1));
        EXPECT_TRUE(game.is_alive(1, 2));
        EXPECT_TRUE(game.is_alive(2, 1));
        EXPECT_TRUE(game.is_alive(2, 2));
        game.next_generation();
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
