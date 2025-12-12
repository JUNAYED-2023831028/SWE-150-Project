#include <SFML/Graphics.hpp>
#include <queue>
#include <vector>
#include <iostream>
#include <string> 

using namespace sf;
using namespace std;

const int TILE = 32;
const int W = 20;
const int H = 17;

int MAP[H][W] =
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,0,1,1,0,1,0,1,1,1,0,1,1,1,0,1},
    {1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1},
    {1,0,1,0,1,1,1,1,0,1,0,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,0,1},
    {1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1},
    {1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,0,1},
    {1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,0,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,0,1},
    {1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1},
    {1,1,1,0,1,1,1,1,0,1,0,1,1,1,0,1,0,1,1,1},
    {1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1},
    {1,0,1,1,1,0,1,1,0,1,0,1,1,1,0,1,1,1,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

vector<Vector2i> bfsPath(Vector2i start, Vector2i goal)
{
    queue<Vector2i> q;
    vector<vector<bool>> visited(H, vector<bool>(W, false));
    vector<vector<Vector2i>> parent(H, vector<Vector2i>(W, { -1,-1 }));

    q.push(start);
    visited[start.y][start.x] = true;

    int dx[4] = { 1,-1,0,0 };
    int dy[4] = { 0,0,1,-1 };

    while (!q.empty())
    {
        auto v = q.front(); q.pop();
        if (v == goal) break;

        for (int i = 0; i < 4; i++)
        {
            int nx = v.x + dx[i];
            int ny = v.y + dy[i];

            if (nx >= 0 && nx < W && ny >= 0 && ny < H &&
                !visited[ny][nx] && MAP[ny][nx] == 0)
            {
                visited[ny][nx] = true;
                parent[ny][nx] = v;
                q.push({ nx,ny });
            }
        }
    }

    vector<Vector2i> path;
    if (!visited[goal.y][goal.x]) return path;

    Vector2i cur = goal;
    while (cur != start)
    {
        path.push_back(cur);
        cur = parent[cur.y][cur.x];
    }
    reverse(path.begin(), path.end());
    return path;
}

void resetGame(Vector2i& player, vector<Vector2i>& enemies,
    vector<vector<bool>>& ball, int& score,
    bool& gameOver, bool& gameWin, int& totalBalls)
{
    player = { 1,1 };
    enemies = { {18,1}, {18,14} };
    score = 0;
    gameOver = false;
    gameWin = false;
    totalBalls = 0;
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            if (MAP[y][x] == 0) {
                ball[y][x] = true;
                totalBalls++;
            }
    cout << "GAME RESET. Total Balls: " << totalBalls << endl;
}

int main()
{
    RenderWindow window(VideoMode(W * TILE, H * TILE), "Pacman");
    window.setFramerateLimit(60);

    Vector2i player = { 1,1 };
    vector<Vector2i> enemies = { {18,1},{18,14} };
    vector<vector<bool>> ball(H, vector<bool>(W, false));
    int totalBalls = 0;
    int score = 0;
    
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            if (MAP[y][x] == 0) {
                ball[y][x] = true;
                totalBalls++;
            }

    int playerMoveTimer = 0, enemyMoveTimer = 0;
    bool gameOver = false, gameWin = false;

    Font font;
    bool hasFont = true;
    if (!font.loadFromFile("arial.ttf")) {
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            cout << "CRITICAL ERROR: No font found. Text will be invisible." << endl;
            hasFont = false;
        }
    }

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed) window.close();
            if ((gameOver || gameWin) && e.type == Event::KeyPressed && e.key.code == Keyboard::R)
            {
                resetGame(player, enemies, ball, score, gameOver, gameWin, totalBalls);
            }
        }

        if (!gameOver && !gameWin)
        {
            playerMoveTimer++;
            enemyMoveTimer++;

            if (playerMoveTimer >= 8) {
                if (Keyboard::isKeyPressed(Keyboard::Up) && MAP[player.y - 1][player.x] == 0) player.y--;
                else if (Keyboard::isKeyPressed(Keyboard::Down) && MAP[player.y + 1][player.x] == 0) player.y++;
                else if (Keyboard::isKeyPressed(Keyboard::Left) && MAP[player.y][player.x - 1] == 0) player.x--;
                else if (Keyboard::isKeyPressed(Keyboard::Right) && MAP[player.y][player.x + 1] == 0) player.x++;
                playerMoveTimer = 0;
            }

            if (ball[player.y][player.x]) {
                ball[player.y][player.x] = false;
                score++;
                cout << "Score: " << score << " / " << totalBalls << endl;
                if (score >= totalBalls) {
                    gameWin = true;
                    cout << "WINNER! WINNER!" << endl;
                }
            }

            if (enemyMoveTimer >= 40) {
                for (auto& en : enemies) {
                    auto path = bfsPath(en, player);
                    if (!path.empty()) en = path[0];
                    if (en == player) gameOver = true;
                }
                enemyMoveTimer = 0;
            }
        }

        if (gameWin) window.clear(Color(0, 100, 0));
        else if (gameOver) window.clear(Color(100, 0, 0));
        else window.clear(Color::Black);

        RectangleShape cell(Vector2f(TILE, TILE));
        for (int y = 0; y < H; y++)
            for (int x = 0; x < W; x++)
            {
                if (MAP[y][x] == 1) cell.setFillColor(Color(40, 40, 40));
                else cell.setFillColor(Color::Transparent);
                cell.setPosition(x * TILE, y * TILE);
                window.draw(cell);

                if (ball[y][x]) {
                    CircleShape dot(4);
                    dot.setFillColor(Color::Yellow);
                    dot.setPosition(x * TILE + 12, y * TILE + 12);
                    window.draw(dot);
                }
            }

        CircleShape p(14);
        p.setFillColor(Color::Green);
        p.setPosition(player.x * TILE + 2, player.y * TILE + 2);
        window.draw(p);

        for (auto& en : enemies) {
            CircleShape g(14);
            g.setFillColor(Color::Red);
            g.setPosition(en.x * TILE + 2, en.y * TILE + 2);
            window.draw(g);
        }

        if ((gameOver || gameWin) && hasFont) {
            Text text;
            text.setFont(font);
            text.setCharacterSize(40);
            text.setFillColor(Color::White);
            if (gameWin) text.setString("CONGRATULATIONS!\nPress R");
            else text.setString("GAME OVER!\nPress R");
            text.setPosition(80, 200);
            window.draw(text);
        }

        window.display();
    }
    return 0;
}