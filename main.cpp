#include <raylib.h>
#include "button.hpp"
#include <iostream>
#include <time.h>
#include <math.h>
#include <fstream>


using namespace std;

enum GameScreen { LOGO = 0, MAIN_MENU, LEVEL_SELECTION, GAMEPLAY, EASY, MEDIUM, HARD };  // Define the screens
GameScreen currentScreen = LOGO;

float PaddleSpeed;
bool PaddleUp;
bool PaddleDown;
bool BotUp  ;
bool BotDown;


static void Gameplay (float speed, bool PaddleConditionUp, bool  PaddleConditionDown );
const char* winnerText = nullptr; // Teks pemenang
// Resource
static RenderTexture2D screenTarget = { 0 }; // Render texture untuk efek
static Texture2D texLogo = { 0 }; // Tekstur logo
static Sound fxCollision = { 0 };
static Sound fxPush = { 0 };
static Music ingame = { 0 };

static int framesCounter = 0;

//Skor
int Skor = 0;
int Time = 0;

struct Ball {
    float x, y;      // Posisi bola
    float speedX, speedY; // Kecepatan bola
    float radius;    // Jari-jari bola

    // Fungsi untuk menggambar bola
    void Draw() {
        DrawCircle((int)x, (int)y, radius, WHITE);
    }
};

// Struktur untuk paddle
struct Paddle {
    float x, y;      // Posisi paddle
    float speed;     // Kecepatan paddle
    float width, height; // Lebar dan tinggi paddle

    // Mengembalikan rectangle untuk paddle
    Rectangle GetRect() {
        return Rectangle{ x - width / 2, y - height / 2, width, height };
    }

    // Fungsi untuk menggambar paddle
    void Draw() {
        DrawRectangleRec(GetRect(), WHITE);
    }
};

Ball ball; // Objek bola
Paddle leftPaddle; // Paddle kiri
Paddle rightPaddle; // Paddle kanan

int main ()
{
    // Initialize the window
    InitWindow(800, 600, "BlaterPong");
    SetTargetFPS(60);
    InitAudioDevice();

    // Load resources
    Texture2D background = LoadTexture("resources/ft.png");
    Button startButton{"resources/start.png", {320, 200}, 2.5};
    Button scoreButton{"resources/score.png", {320, 275}, 2.5};
    Button quitButton{"resources/quit.png", {320, 350}, 2.5};
    Button level1Button{"resources/level1.png", {320, 200}, 2.5};
    Button level2Button{"resources/level2.png", {320, 275}, 2.5};
    Button level3Button{"resources/level3.png", {320, 350}, 2.5};
    Button backButton{"resources/back.png", {10, 10}, 1.5}; // Back button for level selection

    ball.x = GetScreenWidth() / 2.0f;
    ball.y = GetScreenHeight() / 2.0f;
    ball.radius = 5;
    ball.speedX = 300;
    ball.speedY = 300;

    // Inisialisasi paddle kiri
    leftPaddle.x = 50;
    leftPaddle.y = GetScreenHeight() / 2;
    leftPaddle.width = 10;
    leftPaddle.height = 100;
    leftPaddle.speed = 500;

    // Inisialisasi paddle kanan
    rightPaddle.x = GetScreenWidth() - 50;
    rightPaddle.y = GetScreenHeight() / 2;
    rightPaddle.width = 10;
    rightPaddle.height = 100;
    rightPaddle.speed = 500;

    // Memuat tekstur logo
    texLogo = LoadTexture("resources/logo.png");
    fxCollision = LoadSound ("resources/Audio/Collision.wav");
    fxPush = LoadSound ("resources/Audio/push.wav");
    ingame = LoadMusicStream ("resources/Audio/ingame.mp3");

    // Initial screen state

    bool exit = false;
    screenTarget = LoadRenderTexture(800, 600);
    SetTextureFilter(screenTarget.texture, TEXTURE_FILTER_BILINEAR);

    while (!WindowShouldClose() && !exit)
    {
        Vector2 mousePosition = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        BotUp  = leftPaddle.y > ball.y && (leftPaddle.y - ball.y) > 20 && ball.x < 400 && ball.x > 0;
        BotDown= leftPaddle.y < ball.y && (ball.y -leftPaddle.y) > 20 && ball.x < 400 && ball.x > 0;

        UpdateMusicStream(ingame);

        switch (currentScreen) {
            case LOGO: {
            // Update layar LOGO
            framesCounter++;

            if (framesCounter > 300) {
                framesCounter = 0;
                currentScreen = MAIN_MENU; // Pindah ke layar TITLE setelah 100 frame
            }

            // Mulai menggambar ke render texture
            BeginTextureMode(screenTarget);
            ClearBackground(BLACK); // Bersihkan latar belakang

            DrawTexture(texLogo, (GetScreenWidth() - texLogo.width) / 2, (GetScreenHeight() - texLogo.height) / 2, WHITE); // Gambar logo di tengah atas
            EndTextureMode(); // Selesai menggambar ke render texture

            // Mulai menggambar ke layar utama
            BeginDrawing();
            ClearBackground(BLACK); // Bersihkan latar belakang layar utama
            DrawTexture(screenTarget.texture, 0, 0, WHITE); // Gambar render texture ke layar utama
            EndDrawing();
            PlayMusicStream(ingame);
            } break;
            case MAIN_MENU: {
                // Handle button presses in the main menu

                if (startButton.isPressed(mousePosition, mousePressed))
                {PlaySound(fxPush);
                    cout << "Start Button Pressed" << endl;
                    currentScreen = LEVEL_SELECTION;  // Transition to the level selection screen
                }
                else if (scoreButton.isPressed(mousePosition, mousePressed))
                {PlaySound(fxPush);
                    cout << "Score Button Pressed" << endl;
                    // Transition to the score screen (if needed)
                }
                else if (quitButton.isPressed(mousePosition, mousePressed))
                {PlaySound(fxPush);
                    exit = true;  // Exit the game
                }

                // Draw the main menu screen
                BeginDrawing();
                ClearBackground(BLACK);
                DrawTexture(background, 0, 0, WHITE);
                startButton.Draw();
                scoreButton.Draw();
                quitButton.Draw();
                EndDrawing();
                }
                break;

            case LEVEL_SELECTION: {
                // Handle button presses in the level selection screen
                if (level1Button.isPressed(mousePosition, mousePressed))
                {PlaySound(fxPush);
                    cout << "Level 1 Selected" << endl;
                    // Transition to the gameplay for Level 1
                    currentScreen = EASY;
                }
                else if (level2Button.isPressed(mousePosition, mousePressed))
                {PlaySound(fxPush);
                    cout << "Level 2 Selected" << endl;
                    // Transition to the gameplay for Level 2
                    currentScreen = MEDIUM;
                }
                else if (level3Button.isPressed(mousePosition, mousePressed))
                {PlaySound(fxPush);
                    cout << "Level 3 Selected" << endl;
                    // Transition to the gameplay for Level 3
                    currentScreen = HARD;
                }
                else if (backButton.isPressed(mousePosition, mousePressed))
                {PlaySound(fxPush);
                    cout << "Back Button Pressed" << endl;
                    currentScreen = MAIN_MENU;  // Go back to the main menu
                }

                // Draw the level selection screen
                BeginDrawing();
                ClearBackground(BLACK);
                DrawTexture(background, 0, 0, WHITE);
                level1Button.Draw();
                level2Button.Draw();
                level3Button.Draw();
                backButton.Draw();
                EndDrawing();
                }
                break;

            case EASY: {
                PaddleSpeed = sqrt((ball.x - leftPaddle.x)*(ball.x - leftPaddle.x)+(ball.y - leftPaddle.x)*(ball.y - leftPaddle.x)) * GetFrameTime();
                PaddleUp = BotUp;
                PaddleDown= BotDown;
                Gameplay (PaddleSpeed,PaddleUp, PaddleDown );
                }
                break;

            case MEDIUM: {
                PaddleSpeed = leftPaddle.speed/1.5* GetFrameTime();
                PaddleUp = BotUp;
                PaddleDown= BotDown;
                Gameplay (PaddleSpeed,PaddleUp, PaddleDown );
                }
                break;

            case HARD: {
                PaddleSpeed = (leftPaddle.speed * GetFrameTime());
                PaddleUp = BotUp;
                PaddleDown= BotDown;
                Gameplay (PaddleSpeed,PaddleUp, PaddleDown );
                }
                break;
        }
    }
    UnloadTexture(texLogo);
    UnloadSound(fxPush);
    UnloadSound(fxCollision);
    StopMusicStream(ingame);
    CloseWindow();  // Close the window and OpenGL context
}

static void Gameplay (float speed, bool PaddleConditionUp, bool  PaddleConditionDown ){
        Time = GetTime();
            // Update posisi bola
            StopMusicStream(ingame);
            ball.x += ball.speedX * GetFrameTime();
            ball.y += ball.speedY * GetFrameTime();

            // Deteksi tabrakan dengan tepi atas dan bawah
            if (ball.y < 0) {
                PlaySound(fxCollision);
                ball.y = 0;
                ball.speedY *= -1; // Balik arah bola
            }
            if (ball.y > GetScreenHeight()) {
               PlaySound(fxCollision);
                ball.y = GetScreenHeight();
                ball.speedY *= -1; // Balik arah bola
            }

            // Kontrol paddle kiri
            if (PaddleConditionUp) {
                leftPaddle.y -= speed;
            }
            if (PaddleConditionDown) {
                leftPaddle.y += speed;
            }

            // Kontrol paddle kanan
            if (IsKeyDown(KEY_UP) && rightPaddle.y > 50) {
                rightPaddle.y -= rightPaddle.speed * GetFrameTime();
            }
            if (IsKeyDown(KEY_DOWN) && rightPaddle.y < GetScreenHeight() - 50) {
                rightPaddle.y += rightPaddle.speed * GetFrameTime();
            }

            // Deteksi tabrakan bola dengan paddle
            if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRect())) {
                if (ball.speedX < 0) {
                        PlaySound(fxCollision);
                    ball.speedX *= -1.1f; // Tingkatkan kecepatan bola
                    ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX; // Atur kecepatan Y
                }
            }
            if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect())) {
                if (ball.speedX > 0) {
                        PlaySound(fxCollision);
                    ball.speedX *= -1.1f; // Tingkatkan kecepatan bola
                    ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX; // Atur kecepatan Y
                }
            }


            // Cek jika bola keluar dari layar
            if (ball.x < 0 && Skor < 5) {
                Skor++;


            }
            if(ball.x < 0 && Skor <= 4){
            ball.x = GetScreenWidth() / 2;
                ball.y = GetScreenHeight() / 2;
                ball.speedX = 300;
                ball.speedY = 300;
            }
            if( Skor == 5 || Skor > 5){
                winnerText = "Player Wins!";
            }
            if (ball.x > GetScreenWidth()) {
                winnerText = "Game Over"; // Pemain kiri menang
                Skor = 0;
            }
            if (winnerText && IsKeyPressed(KEY_ENTER)) {
                // Reset permainan jika SPACE ditekan
                ball.x = GetScreenWidth() / 2;
                ball.y = GetScreenHeight() / 2;
                ball.speedX = 300;
                ball.speedY = 300;
                winnerText = nullptr; // Reset teks pemenang
            }

            // Mulai menggambar ke layar utama
            BeginDrawing();
           ClearBackground(DARKBLUE)    ;  // Latar belakang ungu

        // Gambar garis tengah lapangan (dotted line manual)
        int midScreenX = GetScreenWidth() / 2; // Lebar layar diambil secara dinamis
        DrawRectangle(midScreenX - 2, 10, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 30, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 50, 4, 10, WHITE); //Jangan Dihapus
        DrawRectangle(midScreenX - 2, 70, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 90, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 110, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 130, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 150, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 170, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 190, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 210, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 230, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 250, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 270, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 290, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 310, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 330, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 350, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 370, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 390, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 410, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 430, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 450, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 470, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 490, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 510, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 530, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 550, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 570, 4, 10, WHITE);
        DrawRectangle(midScreenX - 2, 590, 4, 10, WHITE);


             // Bersihkan latar belakang
            ball.Draw(); // Gambar bola
            leftPaddle.Draw(); // Gambar paddle kiri
            rightPaddle.Draw(); // Gambar paddle kanan
            DrawText(TextFormat("%i", Skor), ( GetScreenWidth()) / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", Time), (3 * GetScreenWidth()) / 4 - 20, 20, 80, WHITE);
            // Tampilkan teks pemenang jika ada
            if (winnerText) {
                int textWidth = MeasureText(winnerText, 60);
                DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
            }

            DrawFPS(10, 10); // Tampilkan FPS
            if (IsKeyPressed(KEY_SPACE))
                {


                    ball.x = GetScreenWidth() / 2;
                ball.y = GetScreenHeight() / 2;
                ball.speedX = 300;
                ball.speedY = 300;
                winnerText = nullptr; // Reset teks pemenang

                    currentScreen=MAIN_MENU;

                }
            EndDrawing();
            Time = 0;

        }
