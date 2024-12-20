#include <raylib.h>
#include "button.hpp"
#include <iostream>
using namespace std;

int main ()
{
    InitWindow(800, 600, "BlaterPong");

    SetTargetFPS(60);

    Texture2D background = LoadTexture("Graphics/ft.png");
    Button startButton{"Graphics/start.png", {320, 200}, 2.5};
    Button scoreButton{"Graphics/score.png", {320, 275}, 2.5};
    Button quitButton{"Graphics/quit.png", {320, 350}, 2.5};
    bool exit = false;

    while(WindowShouldClose() == false && exit == false)
    {
        Vector2 mousePosition = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);


        if(startButton.isPressed(mousePosition, mousePressed))
        {
            cout << "Start Button Pressed" << endl;
        }

        else if(scoreButton.isPressed(mousePosition, mousePressed))
        {
            cout << "Score Button Pressed" << endl;
        }

        else if(quitButton.isPressed(mousePosition, mousePressed))
        {
            exit = true;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(background, 0, 0, WHITE);
        startButton.Draw();
        scoreButton.Draw();
        quitButton.Draw();
        EndDrawing();
    }
    CloseWindow();
}
