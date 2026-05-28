#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>

double lastUpdateTime = 0;

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int main()
{
    const int screenWidth = 700;
    const int screenHeight = 760;
    const int boardOffsetX = 100;
    const int boardOffsetY = 80;
    const int panelWidth = 170;
    const int panelX = boardOffsetX + 300 + 30;
    const int scorePanelY = boardOffsetY;
    const int scorePanelHeight = 80;
    const int nextPanelY = scorePanelY + scorePanelHeight + 80;
    const int nextPanelHeight = 160;
    const int next2PanelY = nextPanelY + nextPanelHeight + 80;
    const int next2PanelHeight = 160;
    const int overlayX = boardOffsetX;
    const int overlayY = boardOffsetY + 80;
    const int overlayWidth = screenWidth - boardOffsetX * 2;
    const int overlayHeight = 260;
    const int playAgainButtonWidth = 180;
    const int playAgainButtonHeight = 34;
    const int playAgainButtonX = overlayX + (overlayWidth - playAgainButtonWidth) / 2;
    const int playAgainButtonY = overlayY + 210;
    Rectangle playAgainButton = {playAgainButtonX, playAgainButtonY, playAgainButtonWidth, playAgainButtonHeight};
    bool inStartScreen = true;
    const int startButtonWidth = 220;
    const int startButtonHeight = 50;
    const int startButtonY = 420;
    const int quitButtonY = startButtonY + startButtonHeight + 20;
    Rectangle startButton = {(screenWidth - startButtonWidth) / 2, startButtonY, startButtonWidth, startButtonHeight};
    Rectangle quitButton = {(screenWidth - startButtonWidth) / 2, quitButtonY, startButtonWidth, startButtonHeight};

    InitWindow(screenWidth, screenHeight, "block bloom");
    SetTargetFPS(60);

    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);

    Game game = Game();

    while (WindowShouldClose() == false)
    {
        UpdateMusicStream(game.music);

        if (inStartScreen)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, startButton))
                {
                    inStartScreen = false;
                    game.Reset();
                    game.gameOver = false;
                }
                else if (CheckCollisionPointRec(mousePoint, quitButton))
                {
                    CloseWindow();
                    return 0;
                }
            }

            BeginDrawing();
            ClearBackground(darkBlue);

            Vector2 titleSize = MeasureTextEx(font, "Block Bloom", 72, 2);
            DrawTextEx(font, "Block Bloom", Vector2{(screenWidth - titleSize.x) / 2.0f, 180.0f}, 72, 2, WHITE);

            Color startButtonColor = CheckCollisionPointRec(GetMousePosition(), startButton) ? SKYBLUE : BLUE;
            DrawRectangleRounded(startButton, 0.3f, 6, startButtonColor);
            Vector2 startTextSize = MeasureTextEx(font, "Play", 32, 2);
            DrawTextEx(font, "Play", Vector2{startButton.x + (startButton.width - startTextSize.x) / 2.0f, startButton.y + (startButton.height - startTextSize.y) / 2.0f}, 32, 2, WHITE);

            Color quitButtonColor = CheckCollisionPointRec(GetMousePosition(), quitButton) ? RED : MAROON;
            DrawRectangleRounded(quitButton, 0.3f, 6, quitButtonColor);
            Vector2 quitTextSize = MeasureTextEx(font, "Quit", 32, 2);
            DrawTextEx(font, "Quit", Vector2{quitButton.x + (quitButton.width - quitTextSize.x) / 2.0f, quitButton.y + (quitButton.height - quitTextSize.y) / 2.0f}, 32, 2, WHITE);

            EndDrawing();
            continue;
        }

        game.HandleInput();
        if (EventTriggered(0.2))
        {
            game.MoveBlockDown();
        }

        if (game.gameOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePoint = GetMousePosition();
            if (CheckCollisionPointRec(mousePoint, playAgainButton))
            {
                game.Reset();
                game.gameOver = false;
            }
        }

        BeginDrawing();
        ClearBackground(darkBlue);
        Vector2 scoreLabelSize = MeasureTextEx(font, "Score", 38, 2);
        DrawTextEx(font, "Score", Vector2{(float)panelX + (panelWidth - scoreLabelSize.x) / 2.0f, (float)scorePanelY - 40.0f}, 38, 2, WHITE);
        Vector2 nextLabelSize = MeasureTextEx(font, "Next", 38, 2);
        DrawTextEx(font, "Shift", Vector2{(float)panelX + (panelWidth - nextLabelSize.x) / 2.0f, (float)nextPanelY - 40.0f}, 38, 2, WHITE);
        Vector2 next2LabelSize = MeasureTextEx(font, "Next 2", 38, 2);
        DrawTextEx(font, "Next", Vector2{(float)panelX + (panelWidth - next2LabelSize.x) / 2.0f, (float)next2PanelY - 40.0f}, 38, 2, WHITE);

        DrawRectangleRounded({panelX, scorePanelY, panelWidth, scorePanelHeight}, 0.3, 6, lightBlue);
        DrawRectangleRounded({panelX, nextPanelY, panelWidth, nextPanelHeight}, 0.3, 6, lightBlue);
        DrawRectangleRounded({panelX, next2PanelY, panelWidth, next2PanelHeight}, 0.3, 6, lightBlue);

        char scoreText[10];
        sprintf(scoreText, "%d", game.score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
        DrawTextEx(font, scoreText, Vector2{(float)panelX + (panelWidth - textSize.x) / 2.0f, (float)scorePanelY + (scorePanelHeight - textSize.y) / 2.0f}, 38, 2, WHITE);

        game.Draw(boardOffsetX, boardOffsetY, panelX, nextPanelY, next2PanelY);

        if (game.gameOver)
        {
            DrawRectangleRounded({overlayX, overlayY, overlayWidth, overlayHeight}, 0.3, 6, Fade(BLACK, 0.75f));
            Vector2 titleSize = MeasureTextEx(font, "GAME OVER", 38, 2);
            DrawTextEx(font, "GAME OVER", Vector2{(screenWidth - titleSize.x) / 2.0f, (float)boardOffsetY + 120.0f}, 38, 2, WHITE);

            char finalScoreText[32];
            sprintf(finalScoreText, "Score: %d", game.score);
            Vector2 finalScoreSize = MeasureTextEx(font, finalScoreText, 28, 2);
            DrawTextEx(font, finalScoreText, Vector2{(screenWidth - finalScoreSize.x) / 2.0f, (float)boardOffsetY + 190.0f}, 28, 2, WHITE);

            Color buttonColor = CheckCollisionPointRec(GetMousePosition(), playAgainButton) ? SKYBLUE : BLUE;
            DrawRectangleRounded(playAgainButton, 0.25f, 6, buttonColor);
            Vector2 buttonTextSize = MeasureTextEx(font, "Play Again", 22, 2);
            DrawTextEx(font, "Play Again", Vector2{playAgainButton.x + (playAgainButton.width - buttonTextSize.x) / 2.0f, playAgainButton.y + (playAgainButton.height - buttonTextSize.y) / 2.0f}, 22, 2, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
}