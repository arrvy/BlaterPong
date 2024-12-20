#include "raylib.h"

struct Ball
{
	float x, y;
	float speedX, speedY;
	float radius;

	void Draw()
	{
		DrawCircle((int) x, (int) y, radius, WHITE);
	}
};

struct Paddle
{
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRect()
	{
		return Rectangle{ x - width / 2, y - height / 2, 10, 100 };
	}

	void Draw()
	{
		DrawRectangleRec(GetRect(), WHITE);
	}
};

static void gameScreen(void);
typedef enum { LOGO = 0, TITLE, CREDITS, GAMEPLAY, ENDING } GameScreen;
static int currentScreen = LOGO;
static int framesCounter = 0;
Ball ball;
Paddle leftPaddle;
Paddle rightPaddle;
const char* winnerText = nullptr;

int main()
{
	InitWindow(800, 600, "BlaterPong");
	SetWindowState(FLAG_VSYNC_HINT);


	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 5;
	ball.speedX = 300;
	ball.speedY = 300;


	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.speed = 500;


	rightPaddle.x = GetScreenWidth() - 50;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.speed = 500;



	while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        gameScreen();
    }
    return 0;
}

static void gameScreen(void){
switch (currentScreen)
    {
        case LOGO:
        {
            // Update LOGO screen
            framesCounter++;

            if (framesCounter > 100)
            {
                framesCounter = 0;
                currentScreen = TITLE;

            }
            BeginDrawing();
            DrawText("LOGO SCREEN", 320, 280, 20, WHITE);
            EndDrawing();
        } break;
        case TITLE:
        {

            if (IsKeyPressed(KEY_ENTER))
            {
                framesCounter = 0;
                currentScreen = GAMEPLAY;
            }
            BeginDrawing();
            DrawText("BlaterPong", 320, 320, 20, WHITE);
            EndDrawing();
        } break;
        case GAMEPLAY:
        {
        ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();

		if (ball.y < 0)
		{
			ball.y = 0;
			ball.speedY*= -1;
		}
		if (ball.y > GetScreenHeight())
		{
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
		}

		if (IsKeyDown(KEY_W)&&leftPaddle.y > 50)
		{
			leftPaddle.y -= leftPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_S)&&leftPaddle.y < GetScreenHeight()-50)
		{
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_UP)&&rightPaddle.y > 50)
		{
			rightPaddle.y -= rightPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_DOWN)&&rightPaddle.y < GetScreenHeight()- 50 )
		{
			rightPaddle.y += rightPaddle.speed * GetFrameTime();
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRect()))
		{
			if (ball.speedX < 0)
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
		}
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect()))
		{
			if (ball.speedX > 0)
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
			}
		}

		if (ball.x < 0)
		{
			winnerText = "Right Player Wins!";
		}
		if (ball.x > GetScreenWidth())
		{
			winnerText = "Left Player Wins!";
		}
		if (winnerText && IsKeyPressed(KEY_SPACE))
		{
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.speedX = 300;
			ball.speedY = 300;
			winnerText = nullptr;
		}


		BeginDrawing();
			ClearBackground(BLACK);

			ball.Draw();
			leftPaddle.Draw();
			rightPaddle.Draw();

			if (winnerText)
			{
				int textWidth = MeasureText(winnerText, 60);
				DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
			}

			DrawFPS(10, 10);
		EndDrawing();



        } break;
        case CREDITS:
        {
framesCounter++;

            if (framesCounter > 300)
            {
                framesCounter = 0;
                currentScreen = TITLE;

            }

        } break;
        case ENDING:
        {
framesCounter++;

            if (framesCounter > 300)
            {
                framesCounter = 0;
                currentScreen = TITLE;

            }
        } break;
        default: break;
    }

}
