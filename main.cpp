#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>

// the ball
struct Ball {
    Vector2 pos;
    Vector2 vel;
    float radius;
    Color color;
};

bool isBallTouchingAnyPlayer(Ball ball, Rectangle player1, Rectangle player2)
{
    if (CheckCollisionCircleRec(ball.pos, ball.radius, player1)) {
        // float closestX{Vector2Clamp(ball, {player1.x, player1.y}, Vector2
        // max)
        return true;
    }

    if (CheckCollisionCircleRec(ball.pos, ball.radius, player2)) {
        return true;
    }

    return false;
}

bool isBallTouchingAnyWall(Ball ball, float gameWindowHeight)
{
    if (ball.pos.y < 0.0f)
        return true;

    if (ball.pos.y > gameWindowHeight)
        return true;

    return false;
}

bool hasPlayerScored(Ball ball, Rectangle scoreArea)
{
    return CheckCollisionCircleRec(ball.pos, ball.radius, scoreArea);
}

void drawScoreBoard(const int gameWindowWidth,
                    std::string_view leftScoreBoardNum,
                    std::string_view rightScoreBoardNum)
{
    DrawText(" - ", gameWindowWidth / 2, 50, 94, WHITE);
    DrawText(leftScoreBoardNum.data(), gameWindowWidth / 2 - 200, 50, 94,
             WHITE);
    DrawText(rightScoreBoardNum.data(), gameWindowWidth / 2 + 200, 50, 94,
             WHITE);
}

void drawPlayers(Rectangle leftPlayer, Color leftPlayerColor,
                 Rectangle rightPlayer, Color rightPlayerColor)
{
    DrawRectangleRec(leftPlayer, leftPlayerColor);
    DrawRectangleRec(rightPlayer, rightPlayerColor);
}

void drawBall(Ball ball)
{
    DrawCircleV(ball.pos, ball.radius, ball.color);
}

int main(void)
{

    // Window Initialization
    InitWindow(100, 100, "Pong first try");
    SetTargetFPS(60);

    // Current Display
    int currentMonitorIdx = GetCurrentMonitor();
    int displayWidth = GetMonitorWidth(currentMonitorIdx);
    int displayHeight = GetMonitorHeight(currentMonitorIdx);

    // GameWindow
    int gameWindowWidth{displayWidth - 200};
    int gameWindowHeight{displayHeight - 200};

    SetWindowSize(gameWindowWidth, gameWindowHeight);
    SetWindowPosition((displayWidth - gameWindowWidth) / 2,
                      (displayHeight - gameWindowHeight) / 2);

    // Scoreboard
    int leftPlayerScore{};
    std::string leftScoreBoardNum = std::to_string(leftPlayerScore);

    int rightPlayerScore{};
    std::string rightScoreBoardNum = std::to_string(rightPlayerScore);

    // PLAYERS
    constexpr int playersHeight{200};
    constexpr int playersWidth{30};

    // left player
    constexpr int playerLeftOffset{100};
    Rectangle leftPlayer{};
    leftPlayer.height = playersHeight;
    leftPlayer.width = playersWidth;
    leftPlayer.x = playerLeftOffset;
    leftPlayer.y = static_cast<float>(gameWindowHeight) / 2.0f;

    // right player
    const float playerRightOffset{static_cast<float>(gameWindowWidth) - 130.0f};
    Rectangle rightPlayer;
    rightPlayer.height = playersHeight;
    rightPlayer.width = playersWidth;
    rightPlayer.x = playerRightOffset;
    rightPlayer.y = static_cast<float>(gameWindowHeight) / 2.0f;

    Ball theBall;

    // starting position
    Vector2 centerOfGameWindow{static_cast<float>(gameWindowWidth) / 2.0f,
                               static_cast<float>(gameWindowHeight) / 2.0f};
    theBall.pos = centerOfGameWindow;

    // random angled velocity
    theBall.vel = Vector2Scale(
        Vector2Normalize(
            {static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
             static_cast<float>(rand()) / static_cast<float>(RAND_MAX)}),
        2000.0f);

    theBall.radius = 20.00f;
    theBall.color = WHITE;

    // Score Boundries
    Rectangle leftScoreBoundry;
    leftScoreBoundry.height = static_cast<float>(gameWindowHeight);
    leftScoreBoundry.width = 2.0f;
    leftScoreBoundry.x = 0;
    leftScoreBoundry.y = 0;

    Rectangle rightScoreBoundry;
    rightScoreBoundry.height = static_cast<float>(gameWindowHeight);
    rightScoreBoundry.width = 2.0f;
    rightScoreBoundry.x = static_cast<float>(gameWindowWidth) - 2.0f;
    rightScoreBoundry.y = 0;

    // GameLoop updating each frame
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        // Score board top
        leftScoreBoardNum = std::to_string(leftPlayerScore);
        rightScoreBoardNum = std::to_string(rightPlayerScore);

        // Drawing
        drawScoreBoard(gameWindowWidth, leftScoreBoardNum, rightScoreBoardNum);
        drawPlayers(leftPlayer, BLUE, rightPlayer, RED);
        drawBall(theBall);

        // No need to draw them, leaving them for DEBUG
        // DrawRectangleRec(leftScoreBoundry, RED);
        // DrawRectangleRec(rightScoreBoundry, RED);

        // Ball Movement
        theBall.pos = Vector2Add(theBall.pos,
                                 Vector2Scale(theBall.vel, GetFrameTime()));

        if (isBallTouchingAnyPlayer(theBall, leftPlayer, rightPlayer)) {
            // REFLECT BOUNCE BALL
            // FIXME: when the ball touch the player shoulder, it goes crazy,
            // because need to flip it on Y not X
            theBall.vel.x = -theBall.vel.x;
        }

        if (isBallTouchingAnyWall(theBall,
                                  static_cast<float>(gameWindowHeight))) {
            // REFLECT BOUNCE BALL
            theBall.vel.y = -theBall.vel.y;
        }

        // Player movement
        float playerMovementSpeed{1500 * GetFrameTime()};
        if (IsKeyDown(KEY_W)) {
            rightPlayer.y -= playerMovementSpeed;
            leftPlayer.y -= playerMovementSpeed;
        }

        if (IsKeyDown(KEY_S)) {
            rightPlayer.y += playerMovementSpeed;
            leftPlayer.y += playerMovementSpeed;
        }

        // Scoring Mechanics
        if (hasPlayerScored(theBall, rightScoreBoundry)) {
            leftPlayerScore += 1;
            // restart the game
            theBall.pos = centerOfGameWindow;
        }

        if (hasPlayerScored(theBall, leftScoreBoundry)) {
            rightPlayerScore += 1;
            // restart the game
            theBall.pos = centerOfGameWindow;
        }

        EndDrawing();
    }
}
