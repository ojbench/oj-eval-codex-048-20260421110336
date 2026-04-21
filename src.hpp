#ifndef T3_ACMOJ2455_TETRIS_HPP
#define T3_ACMOJ2455_TETRIS_HPP

#include <iostream>
#include <vector>
#include <string>

namespace sjtu {

    //// Warning 所有已给出的模板不可修改，你仅可以在模板中标明的位置添加代码。

    class XorShift {
    private:
        unsigned int x, y, z, w;

        void xor_shift32() {
            unsigned int t = x ^ (x << 11);
            x = y;
            y = z;
            z = w;
            w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        }

    public:
        explicit XorShift(unsigned int seed) {
            x = seed;
            y = seed;
            z = seed;
            w = seed;
            xor_shift32();
        }

        unsigned int next() {
            xor_shift32();
            return w;
        }
    };

    struct Tetris {
        struct Tetromino {
            struct Shape {
                std::vector<std::vector<int>> shape; // 骨牌的形状
                int rotateX, rotateY; // 骨牌下一次旋转过后 @shape[0][0] 所代表位置的坐标的变化
                // TODO 添加任何你需要的成员变量
            };
            constexpr static char shapeAlphas[7] = {'I', 'T', 'O', 'S', 'Z', 'J', 'L'}; // 7 种骨牌对应的大写英文字母
            const static Shape rotateShapes[7][4]; // 7 种骨牌的形状及其对应的四种旋转状态
            int x, y; // 骨牌的 @shape[0][0] 的坐标，以区域左上角为坐标原点，+x 方向向右，+y 方向向下，本题模板注释中所提到的“坐标”均在此坐标系下
                      // 此处 @shape[0][0] 实际上就是覆盖此骨牌的最小矩形的左上角
            int index; // 骨牌种类的在 @shapeAlphas 和 @rotateShapes 中的索引
            // TODO 添加任何你需要的成员变量
            int rotation; // 当前旋转状态 [0..3]
        };

        const static int WIDTH = 10; // 游戏区域的宽
        const static int HEIGHT = 20; // 游戏区域的高

        XorShift rand; // 生成骨牌的随机数生成器
        Tetromino cur; // 当前正在下落的骨牌
        // TODO 添加任何你需要的成员变量
        std::vector<std::vector<int>> field; // 已锁定方块：HEIGHT x WIDTH，1 有方块，0 无
        int score; // 计分

        /** 构造函数
         * @param seed 生成骨牌的种子
         */
        Tetris(int seed);

        /** 生成新的骨牌
         *  你需要按照题面所述，在指定位置生成新的骨牌，
         *  同时检查是否满足结束游戏的条件。
         *  @return 是否成功生成新的骨牌，不成功则代表游戏结束
         */
        bool new_tetris();

        /** 骨牌下落
         *  你需要按照题面所述，模拟骨牌的下落。
         *  @return 是否成功下落，不成功则代表骨牌将被“锁定”
         */
        bool dropping();

        /** 操作骨牌
         *  @param operation 操作符，包括 "w" 顺时针旋转 90 度，"a" 左移，"d" 右移，"s" 加速向下坠落一格，"space" 直接下落到底，"e" 无操作。
         *  你需要按照题面所述，模拟骨牌的移动、旋转和直接下落到底。
         *  @return 操作是否成功
         */
        bool move_tetris(const std::string &operation);

        /** 将骨牌“锁定”
         *  你需要按照题面所述，将骨牌“锁定”在方块堆中。
         */
        void place_tetris();

        /** 清除满行
         *  你需要按照题面所述，清除所有被方块填满的行。
         */
        void clear_lines();

        /** 打印区域方块分布图
         *  你需要按照题面所述，打印出当前方块分布图，包括已“锁定”的方块和正在下落的骨牌。
         */
        void print_field();

        /** 检查游戏是否结束
         *  你需要按照题面所述，检查游戏是否结束。
         *  此处仅用检查方块堆是否触及区域顶部。
         *  @return 游戏是否结束
         */
        bool game_over();

        /** 检查骨牌是否与方块堆相撞
         *  你需要按照题面所述，检查骨牌是否与方块堆相撞。
         *  @param next 骨牌的下一个位置，需要检查是否与方块堆相撞
         *  @return 是否与方块堆相撞
         */
        bool check_collision(const Tetromino &next);

        /** 运行游戏
         *  你需要按照题面所述的游戏流程利用上述函数接口编写此函数。
         */
        void run();

        // TODO 添加任何你需要的成员函数

    };

    // 为了让大家在机考的时候少写点恶心的坐标运算，这里直接给大家把表打好，可以在理解之后直接使用
    // 当然也可以不使用给出的模板自己实现
    /* 样例解释：
     *  以 @Tetris::Tetrimino::rotateShapes[0][0] 为例，
     *  成员变量 @Shape::shape 内层 std::vector 存储每一行的所有方块，外层 std::vector 存储所有行，此例中存储的数据为 1 行 4 列的横向的“I”类骨牌；
     *  成员变量 @Shape::rotateX = 1, @Shape::rotateY = -1 代表 @Shape::shape[0][0]（即覆盖该骨牌的最小矩形的左上角）旋转后的坐标变化为 (1, -1)。
     *  rotateShapes[0][0]      rotateShapes[0][1]
     *       0 0 0 0                0 1 0 0
     *       1 1 1 1       \to      0 1 0 0
     *       0 0 0 0                0 1 0 0
     *       0 0 0 0                0 1 0 0
     *  原本的 @Tetrimino::x 和 @Tetrimino::y 分别变化了 1 和 -1。
     */
    const Tetris::Tetromino::Shape Tetris::Tetromino::rotateShapes[7][4] = {
            {
                    {{{1, 1, 1, 1}},         1, -1},
                    {{{1},    {1},    {1}, {1}}, -1, 2},
                    {{{1, 1, 1, 1}},         2, -2},
                    {{{1},    {1},    {1}, {1}}, -2, 1},
            }, // I
            {
                    {{{1, 1, 1}, {0, 1, 0}}, 0, -1},
                    {{{0, 1}, {1, 1}, {0, 1}},   0, 0},
                    {{{0, 1, 0}, {1, 1, 1}}, 1, 0},
                    {{{1, 0}, {1, 1}, {1, 0}},   -1, 1},
            }, // T
            {
                    {{{1, 1},    {1, 1}},    0, 0},
                    {{{1, 1},    {1, 1}},    0, 0},
                    {{{1, 1},    {1, 1}},    0, 0},
                    {{{1, 1},    {1, 1}},    0, 0}
            }, // O
            {
                    {{{0, 1, 1}, {1, 1, 0}}, 0, -1},
                    {{{1, 0}, {1, 1}, {0, 1}},   0, 0},
                    {{{0, 1, 1}, {1, 1, 0}}, 1, 0},
                    {{{1, 0}, {1, 1}, {0, 1}},   -1, 1}
            }, // S
            {
                    {{{1, 1, 0}, {0, 1, 1}}, 0, -1},
                    {{{0, 1}, {1, 1}, {1, 0}},   0, 0},
                    {{{1, 1, 0}, {0, 1, 1}}, 1, 0},
                    {{{0, 1}, {1, 1}, {1, 0}},   -1, 1}
            }, // Z
            {
                    {{{1, 0, 0}, {1, 1, 1}}, 1, 0},
                    {{{1, 1},    {1, 0}, {1, 0}}, -1, 1},
                    {{{1, 1, 1}, {0, 0, 1}}, 0, -1},
                    {{{0, 1},    {0, 1}, {1, 1}},   0, 0}
            }, // J
            {
                    {{{0, 0, 1}, {1, 1, 1}}, 1, 0},
                    {{{1, 0},    {1, 0}, {1, 1}},   -1, 1},
                    {{{1, 1, 1}, {1, 0, 0}}, 0, -1},
                    {{{1, 1},    {0, 1}, {0, 1}},   0, 0}
            } // L
    };

    Tetris::Tetris(int seed) : rand(seed) {
        // TODO
        field.assign(HEIGHT, std::vector<int>(WIDTH, 0));
        score = 0;
        cur.index = 0;
        cur.rotation = 0;
        cur.x = 0;
        cur.y = 0;
    }

    static bool in_bounds(int x, int y) {
        return x >= 0 && x < Tetris::WIDTH && y >= 0 && y < Tetris::HEIGHT;
    }

    bool Tetris::new_tetris() {
        // 随机生成骨牌
        cur.index = rand.next() % 7;
        cur.rotation = 0;
        // 生成位置：区域最顶部中间位置，按当前形状宽度居中
        int w = Tetromino::rotateShapes[cur.index][cur.rotation].shape[0].size();
        cur.x = (WIDTH >> 1) - (w >> 1);
        cur.y = 0;

        // 输出生成的骨牌种类并换行
        std::cout << Tetromino::shapeAlphas[cur.index] << '\n';

        // 检查生成是否与方块堆冲突（结束条件 b）
        if (check_collision(cur)) {
            return false;
        }
        return true;
    }

    bool Tetris::dropping() {
        Tetromino next = cur;
        next.y += 1;
        if (!check_collision(next)) {
            cur = next;
            return true; // 成功下落
        }
        return false; // 碰撞，不能下落
    }

    bool Tetris::move_tetris(const std::string &operation) {
        if (operation == "e") return true; // 无操作一定成功

        if (operation == "a" || operation == "d" || operation == "s") {
            Tetromino next = cur;
            if (operation == "a") next.x -= 1;
            else if (operation == "d") next.x += 1;
            else if (operation == "s") next.y += 1;
            if (!check_collision(next)) { cur = next; return true; }
            return false;
        }

        if (operation == "w") {
            Tetromino next = cur;
            next.rotation = (next.rotation + 1) & 3;
            // 根据表调整左上角坐标变化
            const auto &prevShape = Tetromino::rotateShapes[cur.index][cur.rotation];
            next.x += prevShape.rotateX;
            next.y += prevShape.rotateY;
            if (!check_collision(next)) { cur = next; return true; }
            return false;
        }

        if (operation == "space") {
            // 直接下落到底，一定成功
            Tetromino next = cur;
            while (true) {
                Tetromino tmp = next;
                tmp.y += 1;
                if (check_collision(tmp)) break;
                next = tmp;
            }
            cur = next;
            return true;
        }

        // 非法操作：题面要求在输入阶段重新输入直到合法，这里作为接口返回失败
        return false;
    }

    bool Tetris::check_collision(const Tetromino &t) {
        const auto &shape = Tetromino::rotateShapes[t.index][t.rotation].shape;
        int h = (int)shape.size();
        int w = (int)shape[0].size();
        for (int dy = 0; dy < h; ++dy) {
            for (int dx = 0; dx < w; ++dx) {
                if (shape[dy][dx]) {
                    int gx = t.x + dx;
                    int gy = t.y + dy;
                    if (!in_bounds(gx, gy)) return true; // 越界视为碰撞
                    if (field[gy][gx]) return true; // 与已锁定方块碰撞
                }
            }
        }
        return false;
    }

    void Tetris::place_tetris() {
        const auto &shape = Tetromino::rotateShapes[cur.index][cur.rotation].shape;
        int h = (int)shape.size();
        int w = (int)shape[0].size();
        for (int dy = 0; dy < h; ++dy) {
            for (int dx = 0; dx < w; ++dx) {
                if (shape[dy][dx]) {
                    int gx = cur.x + dx;
                    int gy = cur.y + dy;
                    if (in_bounds(gx, gy)) field[gy][gx] = 1;
                }
            }
        }
    }

    void Tetris::clear_lines() {
        int cleared = 0;
        for (int y = 0; y < HEIGHT; ++y) {
            bool full = true;
            for (int x = 0; x < WIDTH; ++x) {
                if (!field[y][x]) { full = false; break; }
            }
            if (full) {
                ++cleared;
                // 下方行上移：从 y 往上整体下落一格
                for (int yy = y; yy > 0; --yy) field[yy] = field[yy - 1];
                field[0].assign(WIDTH, 0);
            }
        }
        if (cleared == 1) score += 100;
        else if (cleared == 2) score += 300;
        else if (cleared == 3) score += 600;
        else if (cleared == 4) score += 1000;
    }

    void Tetris::print_field() {
        // 打印 HEIGHT x WIDTH 矩阵，包含锁定方块与当前骨牌
        // 先复制字段
        std::vector<std::vector<int>> board = field;
        const auto &shape = Tetromino::rotateShapes[cur.index][cur.rotation].shape;
        int h = (int)shape.size();
        int w = (int)shape[0].size();
        for (int dy = 0; dy < h; ++dy) {
            for (int dx = 0; dx < w; ++dx) {
                if (shape[dy][dx]) {
                    int gx = cur.x + dx;
                    int gy = cur.y + dy;
                    if (in_bounds(gx, gy)) board[gy][gx] = 1;
                }
            }
        }
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                if (x) std::cout << ' ';
                std::cout << board[y][x];
            }
            std::cout << '\n';
        }
    }

    bool Tetris::game_over() {
        // 条件(a)：最高方块在最顶部一行
        for (int x = 0; x < WIDTH; ++x) if (field[0][x]) return true;
        return false;
    }

    void Tetris::run() {
        // 启动游戏：立即生成一个骨牌并输出其种类
        if (!new_tetris()) {
            std::cout << "Game Over!\n";
            std::cout << "Your score: " << score << '\n';
            return;
        }

        std::string op;
        while (true) {
            // 每个单位时间：打印当前区域分布
            print_field();
            // 读取并校验合法操作（循环直到合法）
            while (true) {
                if (!(std::cin >> op)) return; // 输入结束直接返回
                if (op == "w" || op == "a" || op == "d" || op == "s" || op == "e" || op == "space" || op == "esc") break;
                // 非法则继续读
            }

            if (op == "esc") {
                std::cout << "See you!\n";
                std::cout << "Your score: " << score << '\n';
                return;
            }

            bool ok = move_tetris(op);
            if (ok) std::cout << "Successful operation!\n";
            else std::cout << "Failed operation!\n";

            // 骨牌下降一格
            if (dropping()) {
                continue; // 进入下一个单位时间
            } else {
                // 不能下落则锁定
                place_tetris();
                // 清除满行并计分
                clear_lines();
                // 检查结束条件(a)
                if (game_over()) {
                    // 打印此时的方块分布图
                    print_field();
                    std::cout << "Game Over!\n";
                    std::cout << "Your score: " << score << '\n';
                    return;
                }
                // 生成新骨牌并输出种类
                if (!new_tetris()) {
                    // 条件(b)触发：无需打印分布图
                    std::cout << "Game Over!\n";
                    std::cout << "Your score: " << score << '\n';
                    return;
                }
            }
        }
    }

}

#endif //T3_ACMOJ2455_TETRIS_HPP

