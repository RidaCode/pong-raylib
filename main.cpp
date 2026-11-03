#include <iostream>
#include <raylib.h>
#include <string>

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
    int leftSidePlayerScore{};
    std::string leftScoreBoardNum = std::to_string(leftSidePlayerScore);

    int rightSidePlayerScore{};
    std::string rightScoreBoardNum = std::to_string(rightSidePlayerScore);

    // PLAYERS
    // left player
    Rectangle leftSidePlayerRec;
    leftSidePlayerRec.height = 200;
    leftSidePlayerRec.width = 30;
    leftSidePlayerRec.x = 100;
    leftSidePlayerRec.y = gameWindowHeight / 2;

    // right player
    Rectangle rightSidePlayerRec;
    rightSidePlayerRec.height = 200;
    rightSidePlayerRec.width = 30;
    rightSidePlayerRec.x = gameWindowWidth - 130;
    rightSidePlayerRec.y = gameWindowHeight / 2;

    // the ball
    struct Circle {
        float x;
        float y;
        float radius;
        Color color;
    };

    Circle theBall;
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
        std::string leftScoreBoardNum = std::to_string(leftSidePlayerScore);
        std::string rightScoreBoardNum = std::to_string(rightSidePlayerScore);

        DrawText(" - ", gameWindowWidth / 2, 50, 94, WHITE);
        DrawText(leftScoreBoardNum.c_str(), gameWindowWidth / 2 - 200, 50, 94,
                 WHITE);
        DrawText(rightScoreBoardNum.c_str(), gameWindowWidth / 2 + 200, 50, 94,
                 WHITE);

        // Left player
        // DrawRectangle(100, gameWindowHeight / 2, 30, 200, WHITE);
        DrawRectangleRec(leftSidePlayerRec, WHITE);

        // Right player
        DrawRectangleRec(rightSidePlayerRec, WHITE);

        // No need to draw them, leaving them for DEBUG
        // DrawRectangleRec(leftScoreBoundry, RED);
        // DrawRectangleRec(rightScoreBoundry, RED);

        // Ball
        DrawCircleV({theBall.x, theBall.y}, theBall.radius, theBall.color);

        // Ball Movement
        theBall.x += theBallSpeed * GetFrameTime();

        // left side player
        if (CheckCollisionCircleRec({theBall.x, theBall.y}, theBall.radius,
                                    leftSidePlayerRec)) {
            theBallSpeed = -theBallSpeed;
        }

        // right side player
        if (CheckCollisionCircleRec({theBall.x, theBall.y}, theBall.radius,
                                    rightSidePlayerRec)) {
            theBallSpeed = -theBallSpeed;
        }

        // Player movement
        if (IsKeyDown(KEY_W)) {
            rightSidePlayerRec.y -= 1500 * GetFrameTime();
            leftSidePlayerRec.y -= 1500 * GetFrameTime();
        }

        if (IsKeyDown(KEY_S)) {
            rightSidePlayerRec.y += 1500 * GetFrameTime();
            leftSidePlayerRec.y += 1500 * GetFrameTime();
        }

        if (CheckCollisionCircleRec({theBall.x, theBall.y}, theBall.radius,
                                    rightScoreBoundry)) {
            leftSidePlayerScore += 1;
            // restart the game
            theBall.x = gameWindowWidth / 2;
            theBall.y = gameWindowHeight / 2 + 80;
        }

        if (CheckCollisionCircleRec({theBall.x, theBall.y}, theBall.radius,
                                    leftScoreBoundry)) {
            rightSidePlayerScore += 1;
            // restart the game
            theBall.x = gameWindowWidth / 2;
            theBall.y = gameWindowHeight / 2 + 80;
        }

        EndDrawing();
    }
}
