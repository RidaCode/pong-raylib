#include <iostream>
#include <raylib.h>
#include <string>

// the ball
struct Ball {
    float x;
    float y;
    float radius;
    Color color;
};

bool isBallTouchingAnyPlayer(Ball ball, Rectangle player1, Rectangle player2)
{
    if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, player1)) {
        return true;
    }

    if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, player2)) {
        return true;
    }

    return false;
}

bool hasPlayerScored(Ball ball, Rectangle scoreArea)
{
    return CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, scoreArea);
}

void drawScoreBoard(const int gameWindowWidth, std::string leftScoreBoardNum,
                    std::string rightScoreBoardNum)
{
    DrawText(" - ", gameWindowWidth / 2, 50, 94, WHITE);
    DrawText(leftScoreBoardNum.c_str(), gameWindowWidth / 2 - 200, 50, 94,
             WHITE);
    DrawText(rightScoreBoardNum.c_str(), gameWindowWidth / 2 + 200, 50, 94,
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
    DrawCircleV({ball.x, ball.y}, ball.radius, ball.color);
}

int main(void)
{
    // Current Display
    const int myDisplayWidth{3456};
    const int myDisplayHeight{2160};

    // GameWindow
    const int gameWindowWidth{myDisplayWidth - 100};
    const int gameWindowHeight{myDisplayHeight - 100};

    // Window Initialization
    InitWindow(gameWindowWidth, gameWindowHeight, "Pong first try");
    SetTargetFPS(60);

    // Scoreboard
    int leftPlayerScore{};
    std::string leftScoreBoardNum = std::to_string(leftPlayerScore);

    int rightPlayerScore{};
    std::string rightScoreBoardNum = std::to_string(rightPlayerScore);

    // PLAYERS
    constexpr int playersHeight{500};
    constexpr int playersWidth{30};

    // left player
    constexpr int playerLeftOffset{100};
    Rectangle leftPlayer{};
    leftPlayer.height = playersHeight;
    leftPlayer.width = playersWidth;
    leftPlayer.x = playerLeftOffset;
    leftPlayer.y = gameWindowHeight / 2;

    // right player
    constexpr int playerRightOffset{gameWindowWidth - 130};
    Rectangle rightPlayer;
    rightPlayer.height = playersHeight;
    rightPlayer.width = playersWidth;
    rightPlayer.x = playerRightOffset;
    rightPlayer.y = gameWindowHeight / 2;

    Ball theBall;
    theBall.x = gameWindowWidth / 2;
    theBall.y = gameWindowHeight / 2 + 80;
    theBall.radius = 20.00f;
    theBall.color = WHITE;

    float theBallSpeed{-2000.0f};

    // Score Boundries
    Rectangle leftScoreBoundry;
    leftScoreBoundry.height = gameWindowHeight;
    leftScoreBoundry.width = 2;
    leftScoreBoundry.x = 0;
    leftScoreBoundry.y = 0;

    Rectangle rightScoreBoundry;
    rightScoreBoundry.height = gameWindowHeight;
    rightScoreBoundry.width = 2;
    rightScoreBoundry.x = gameWindowWidth - 2;
    rightScoreBoundry.y = 0;

    // GameLoop updating each frame
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        // Score board top
        std::string leftScoreBoardNum = std::to_string(leftPlayerScore);
        std::string rightScoreBoardNum = std::to_string(rightPlayerScore);

        // Left player
        // DrawRectangle(100, gameWindowHeight / 2, 30, 200, WHITE);
        DrawRectangleRec(leftPlayer, WHITE);

        // Right player
        DrawRectangleRec(rightPlayer, WHITE);

        // No need to draw them, leaving them for DEBUG
        // DrawRectangleRec(leftScoreBoundry, RED);
        // DrawRectangleRec(rightScoreBoundry, RED);

        // Ball
        DrawCircleV({theBall.x, theBall.y}, theBall.radius, theBall.color);

        // Ball Movement
        theBall.x += theBallSpeed * GetFrameTime();

        if (isBallTouchingAnyPlayer(theBall, leftPlayer, rightPlayer)) {
            theBallSpeed = -theBallSpeed;
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
            theBall.x = gameWindowWidth / 2;
            theBall.y = gameWindowHeight / 2 + 80;
        }

        if (hasPlayerScored(theBall, leftScoreBoundry)) {
            rightPlayerScore += 1;
            // restart the game
            theBall.x = gameWindowWidth / 2;
            theBall.y = gameWindowHeight / 2 + 80;
        }

        EndDrawing();
    }
}
