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
    // Rectangle* CollisionObject = nullptr;
};

void reflectBallOnFaceCollision([[maybe_unused]] Ball& ball, Rectangle& player)
{
    // it's ball.pos as first point and ball.pos + ball.vel as second point
    [[maybe_unused]]
    Vector2 nextBallPos{
        Vector2Add(ball.pos, Vector2Scale(ball.vel, GetFrameTime()))};

    DrawLineV(ball.pos, nextBallPos, GREEN);

    // Check if ball next pos touch player
    if (!CheckCollisionCircleRec(nextBallPos, ball.radius, player))
        return;

    // Get the line of Top face of player
    Vector2 topLeftCorner{player.x, player.y};
    Vector2 topRightCorner{player.x + player.width, player.y};
    DrawLineV(topLeftCorner, topRightCorner, RED);

    // Get the line of Bottom face of player
    Vector2 bottomLeftCorner{player.x, player.y + player.height};
    Vector2 bottomRightCorner{player.x + player.width,
                              player.y + player.height};
    DrawLineV(bottomLeftCorner, bottomRightCorner, RED);

    // Get the line of Right face of player
    DrawLineV(topRightCorner, bottomRightCorner, RED);

    // Get the line of Left face of player
    DrawLineV(topLeftCorner, bottomLeftCorner, RED);

    Vector2 collisionPoint;
    // Reflect ball on Right and Left faces of player
    if ((ball.pos.x > player.x + player.width
         && CheckCollisionLines(topRightCorner, bottomRightCorner, ball.pos,
                                nextBallPos, &collisionPoint))
        || (ball.pos.x < player.x
            && CheckCollisionLines(topLeftCorner, bottomLeftCorner, ball.pos,
                                   nextBallPos, &collisionPoint))) {

        Ball ballHit{.pos = {collisionPoint.x, collisionPoint.y},
                     .vel = {0, 0},
                     .radius = ball.radius,
                     .color = RED};

        DrawCircleV(ballHit.pos, ballHit.radius, ballHit.color);
        ball.vel.x = -ball.vel.x;
    }

    // Reflect ball on Top and Bottom faces of player
    if ((ball.pos.y < player.y
         && CheckCollisionLines(topLeftCorner, topRightCorner, ball.pos,
                                nextBallPos, &collisionPoint))
        || (ball.pos.y > player.y + player.height
            && CheckCollisionLines(bottomLeftCorner, bottomRightCorner,
                                   ball.pos, nextBallPos, &collisionPoint))) {

        Ball ballHit{.pos = {collisionPoint.x, collisionPoint.y},
                     .vel = {0, 0},
                     .radius = ball.radius,
                     .color = RED};

        DrawCircleV(ballHit.pos, ballHit.radius, ballHit.color);
        ball.vel.y = -ball.vel.y;
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
            {static_cast<float>(rand()) / static_cast<float>(-RAND_MAX),
             static_cast<float>(rand()) / static_cast<float>(RAND_MAX)}),
        1200.0f);

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
        float rightPlayerCenter{rightPlayer.height / 2};
        rightPlayer.y = theBall.pos.y - rightPlayerCenter;

        // Ball Relection
        reflectBallOnFaceCollision(theBall, leftPlayer);
        reflectBallOnFaceCollision(theBall, rightPlayer);

        [[maybe_unused]]
        float leftPlayerBottomShoulderHitPoint{theBall.pos.y - 210.0f};

        [[maybe_unused]]
        float leftPlayerTopShoulderHitPoint{theBall.pos.y + 10.0f};

        reflectBallOnAnyWallTouch(theBall,
                                  static_cast<float>(gameWindowHeight));

        // Player movement
        float playerMovementSpeed{1000 * GetFrameTime()};
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
