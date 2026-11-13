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

void reflectBallOnAnyPlayerTouch(Ball& ball, Rectangle player1,
                                 Rectangle player2)
{
    // REFLECT BOUNCE BALL
    // FIXME: when the ball touch the player shoulder, it goes crazy,
    // because need to flip it on Y not X

    // bool hasHitPlayerShoulder{CheckCollisionLines(ball.pos, Vector2 endPos1,
    // Vector2 startPos2, Vector2 endPos2, Vector2 *collisionPoint)};
    Vector2 LeftPlayerDownShoulder{.x = player1.x,
                                   .y = player1.y + player1.height};

    Rectangle LeftPlayerDownShoulderRec{.x = LeftPlayerDownShoulder.x,
                                        .y = LeftPlayerDownShoulder.y,
                                        .width = 30,
                                        .height = 30};

    DrawRectangleRec(LeftPlayerDownShoulderRec, RED);

    Vector2 LeftPlayerUpShoulder{.x = player1.x, .y = player1.y};

    Rectangle LeftPlayerUpShoulderRec{.x = LeftPlayerUpShoulder.x,
                                      .y = LeftPlayerUpShoulder.y,
                                      .width = 30,
                                      .height = 30};

    DrawRectangleRec(LeftPlayerUpShoulderRec, RED);

    if (CheckCollisionCircleRec(ball.pos, ball.radius, player1)) {
        // float closestX{Vector2Clamp(ball, {player1.x, player1.y}, Vector2
        // max)

        if (CheckCollisionCircleRec(ball.pos, ball.radius,
                                    LeftPlayerDownShoulderRec)
            || CheckCollisionCircleRec(ball.pos, ball.radius,
                                       LeftPlayerUpShoulderRec)) {
            ball.vel.y = -ball.vel.y;
            return;
        }

        ball.vel.x = -ball.vel.x;
    }

    if (CheckCollisionCircleRec(ball.pos, ball.radius, player2)) {
        ball.vel.x = -ball.vel.x;
    }
}

void reflectBallOnAnyWallTouch(Ball& ball, float gameWindowHeight)
{
    if (ball.pos.y < 0.0f) {
        ball.vel.y = -ball.vel.y;
    }

    if (ball.pos.y > gameWindowHeight) {
        ball.vel.y = -ball.vel.y;
    }
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
        1000.0f);

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

        // Ball Movement
        theBall.pos = Vector2Add(theBall.pos,
                                 Vector2Scale(theBall.vel, GetFrameTime()));

        // Right Player AI
        rightPlayer.y = theBall.pos.y;

        // Ball Relection
        reflectBallOnAnyPlayerTouch(theBall, leftPlayer, rightPlayer);
        reflectBallOnAnyWallTouch(theBall,
                                  static_cast<float>(gameWindowHeight));

        // Player movement
        float playerMovementSpeed{1500 * GetFrameTime()};
        if (IsKeyDown(KEY_W)) {
            leftPlayer.y -= playerMovementSpeed;
        }

        if (IsKeyDown(KEY_S)) {
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
