#include <raylib.h>
#include "button.hpp"
#include <iostream>
#include <time.h>
#include <math.h>
#include <list>
#include <fstream>

using namespace std;

enum GameScreen { LOGO = 0, MAIN_MENU, LEVEL_SELECTION, GAMEPLAY, EASY, MEDIUM, HARD };  // Definisikan layar permainan
GameScreen currentScreen = LOGO;

float PaddleSpeed;
bool PaddleUp;
bool PaddleDown;
bool BotUp;
bool BotDown;

static void Gameplay(float speed, bool PaddleConditionUp, bool PaddleConditionDown);
const char* winnerText = nullptr; // Teks pemenang

// Resource
static RenderTexture2D screenTarget = { 0 }; // Render texture untuk efek
static Texture2D texLogo = { 0 }; // Tekstur logo
static Sound fxCollision = { 0 };
static Sound fxPush = { 0 };
static Music ingame = { 0 };

static int framesCounter = 0;

// Skor
int Skor = 0;

int WaktuNow = 0;
int WaktuDulu = 0;
int Time = 0;
int i=0;

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

 //Memuat File HighScore


    list<int> listscore;

// Fungsi untuk memuat skor dari file CSV
void LoadScoresFromCSV() {
    ifstream file("highscore.csv");

    if (!file.is_open()) {
        cout << "Error: Could not open highscore.csv for reading." << endl;
        return; // Keluar dari fungsi jika file tidak dapat dibuka
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            listscore.push_back(stoi(line)); // Tambahkan skor ke listscore
        }
    }
    file.close();
}

// Fungsi untuk menyimpan skor ke file CSV
void SaveScoresToCSV() {
    ofstream scores("highscore.csv", ios::trunc); // Buka file dengan mode truncate untuk menghapus isi sebelumnya

    if (!scores.is_open()) {
        cout << "Error: Could not open highscore.csv for writing." << endl;
        return; // Keluar dari fungsi jika file tidak dapat dibuka
    }

    for (int score : listscore) {
        scores << score << endl; // Tulis setiap skor ke file
    }
    scores.close();
}

int main() {
    // Inisialisasi jendela
    InitWindow(800, 600, "BlaterPong");
    SetTargetFPS(60);
    InitAudioDevice();


    // Memuat resource
    Texture2D background = LoadTexture("resources/ft.png");
    Button startButton{"resources/start.png", {320, 200}, 2.5};
    Button scoreButton{"resources/score.png", {320, 275}, 2.5};
    Button quitButton{"resources/quit.png", {320, 350}, 2.5};
    Button level1Button{"resources/level1.png", {320, 200}, 2.5};
    Button level2Button{"resources/level2.png", {320, 275}, 2.5};
    Button level3Button{"resources/level3.png", {320, 350}, 2.5};
    Button backButton{"resources/back.png", {10, 10}, 1.5}; // Tombol kembali untuk pemilihan level

    // Inisialisasi bola
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

    // Memuat tekstur dan suara
    texLogo = LoadTexture("resources/logo.png");
    fxCollision = LoadSound("resources/Audio/Collision.wav");
    fxPush = LoadSound("resources/Audio/push.wav");
    ingame = LoadMusicStream("resources/Audio/ingame.mp3");

    // Memuat skor dari file CSV
    LoadScoresFromCSV();

    //Waktu
    WaktuDulu = 0;

    // Status layar awal
    bool exit = false;
    screenTarget = LoadRenderTexture(800, 600);
    SetTextureFilter(screenTarget.texture, TEXTURE_FILTER_BILINEAR);

    while (!WindowShouldClose() && !exit) {
        Vector2 mousePosition = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        BotUp = leftPaddle.y > ball.y && (leftPaddle.y - ball.y) > 20 && ball.x < 400 && ball.x > 0 ;
        BotDown = leftPaddle.y < ball.y && (ball.y - leftPaddle.y) > 20 && ball.x < 400 && ball.x > 0;

        UpdateMusicStream(ingame);

        switch (currentScreen) {
            case LOGO: {
                // Memperbarui layar LOGO
                framesCounter++;

                if (framesCounter > 300) {
                    framesCounter = 0;
                    currentScreen = MAIN_MENU; // Pindah ke layar MENU UTAMA setelah 300 frame
                }

                // Mulai menggambar ke render texture
                BeginTextureMode(screenTarget);
                ClearBackground(BLACK); // Bersihkan latar belakang

                DrawTexture(texLogo, (GetScreenWidth() - texLogo.width) / 2, (GetScreenHeight() - texLogo.height) / 2, WHITE); // Gambar logo di tengah
                EndTextureMode(); // Selesai menggambar ke render texture

                // Mulai menggambar ke layar utama
                BeginDrawing();
                ClearBackground(BLACK); // Bersihkan latar belakang layar utama
                DrawTexture(screenTarget.texture, 0, 0, WHITE); // Gambar render texture ke layar utama
                EndDrawing();
                PlayMusicStream(ingame);
            } break;

            case MAIN_MENU: {
                // Menangani penekanan tombol di menu utama
                if (startButton.isPressed(mousePosition, mousePressed)) {
                    PlaySound(fxPush);
                    cout << "Tombol Mulai Ditekan" << endl;
                    currentScreen = LEVEL_SELECTION;  // Pindah ke layar pemilihan level
                } else if (scoreButton.isPressed(mousePosition, mousePressed)) {
                    PlaySound(fxPush);
                    cout << "Tombol Skor Ditekan" << endl;
                    // Pindah ke layar skor (jika diperlukan)
                } else if (quitButton.isPressed(mousePosition, mousePressed)) {
                    PlaySound(fxPush);
                    exit = true;  // Keluar dari permainan
                }



                // Gambar layar menu utama
                BeginDrawing();
                ClearBackground(BLACK);
                DrawTexture(background, 0, 0, WHITE);
                startButton.Draw();
                scoreButton.Draw();
                quitButton.Draw();
                EndDrawing();
            } break;

            case LEVEL_SELECTION: {
                // Menangani penekanan tombol di layar pemilihan level
                if (level1Button.isPressed(mousePosition, mousePressed)) {
                    PlaySound(fxPush);
                    cout << "Level 1 Dipilih" << endl;
                    currentScreen = EASY; // Pindah ke gameplay untuk Level 1
                } else if (level2Button.isPressed(mousePosition, mousePressed)) {
                    PlaySound(fxPush);
                    cout << "Level 2 Dipilih" << endl;
                    currentScreen = MEDIUM; // Pindah ke gameplay untuk Level 2
                } else if (level3Button.isPressed(mousePosition, mousePressed)) {
                    PlaySound(fxPush);
                    cout << "Level 3 Dipilih" << endl;
                    currentScreen = HARD; // Pindah ke gameplay untuk Level 3
                } else if (backButton.isPressed(mousePosition, mousePressed)) {
                    PlaySound(fxPush);
                    cout << "Tombol Kembali Ditekan" << endl;
                    currentScreen = MAIN_MENU;  // Kembali ke menu utama
                }

                // Gambar layar pemilihan level
                BeginDrawing();
                ClearBackground(BLACK);
                DrawTexture(background, 0, 0, WHITE);
                level1Button.Draw();
                level2Button.Draw();
                level3Button.Draw();
                backButton.Draw();
                EndDrawing();
            } break;

            case EASY: {
                PaddleSpeed = 0*sqrt(2*(ball.x - leftPaddle.x) * (ball.x - leftPaddle.x) + (ball.y - leftPaddle.y) * (ball.y - leftPaddle.y)) * 1.2 * GetFrameTime();
                PaddleUp = BotUp;
                PaddleDown = BotDown;
                Gameplay(PaddleSpeed, PaddleUp, PaddleDown);
            } break;

            case MEDIUM: {
                PaddleSpeed = leftPaddle.speed / 1.5 * GetFrameTime();
                PaddleUp = BotUp;
                PaddleDown = BotDown;
                Gameplay(PaddleSpeed, PaddleUp, PaddleDown);
            } break;

            case HARD: {
                PaddleSpeed = (leftPaddle.speed * GetFrameTime());
                PaddleUp = BotUp;
                PaddleDown = BotDown;
                Gameplay(PaddleSpeed, PaddleUp, PaddleDown);
            } break;
        }
    }
    // Menyimpan skor ke file CSV sebelum keluar
    SaveScoresToCSV();


    UnloadTexture(texLogo);
    UnloadSound(fxPush);
    UnloadSound(fxCollision);
    StopMusicStream(ingame);
    CloseWindow();  // Tutup jendela dan konteks
}

static void Gameplay(float speed, bool PaddleConditionUp, bool PaddleConditionDown) {
    //Timer
    if (WaktuDulu ==0){
    WaktuDulu = GetTime();
    }
    WaktuNow = clock()/CLOCKS_PER_SEC;
    Time = WaktuNow-WaktuDulu;




    // Memperbarui posisi bola
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
    if (ball.x < 0 && Skor <= 4) {
        ball.x = GetScreenWidth() / 2;
        ball.y = GetScreenHeight() / 2;
        ball.speedX = 300;
        ball.speedY = 0;
    }
    if (Skor == 5 || Skor > 5) {
        i++;
        winnerText = "Player Wins!";

         if(i< 2){
            listscore.push_front(Time);
            listscore.sort();
            SaveScoresToCSV(); // Simpan skor baru ke file CSV
            cout << "Skor: " << listscore.front() << endl;
         }
    }
    if (ball.x > GetScreenWidth()) {
        winnerText = "Game Over"; // Pemain kiri menang
        Skor = 0;
    }
    if (winnerText && IsKeyPressed(KEY_ENTER)) {
            WaktuDulu = WaktuNow;
        // Reset permainan jika ENTER ditekan
        ball.x = GetScreenWidth() / 2;
        ball.y = GetScreenHeight() / 2;
        ball.speedX = 300;
        ball.speedY = 300;
        winnerText = nullptr; // Reset teks pemenang
        Skor=0;
        i=0;
    }

    // Mulai menggambar ke layar utama
    BeginDrawing();
    ClearBackground(DARKBLUE); // Latar belakang biru tua

    // Gambar garis tengah lapangan (garis putus-putus manual)
    int midScreenX = GetScreenWidth() / 2; // Lebar layar diambil secara dinamis
    for (int i = 10; i < GetScreenHeight(); i += 20) {
        DrawRectangle(midScreenX - 2, i, 4, 10, WHITE);
    }

    ball.Draw(); // Gambar bola
    leftPaddle.Draw(); // Gambar paddle kiri
    rightPaddle.Draw(); // Gambar paddle kanan
    DrawText(TextFormat("%i/5", Skor), (GetScreenWidth()) / 4 - 20, 20, 80, ORANGE);
    DrawText(TextFormat("%i", Time), (3 * GetScreenWidth()) / 4 - 20, 20, 80, RAYWHITE);

    // Tampilkan teks pemenang jika ada
    if (winnerText) {
        int textWidth = MeasureText(winnerText, 60);
        DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
    }

    DrawFPS(10, 10); // Tampilkan FPS
    if (IsKeyPressed(KEY_SPACE)) {
            WaktuDulu = 0;
        ball.x = GetScreenWidth() / 2;
        ball.y = GetScreenHeight() / 2;
        ball.speedX = 300;
        ball.speedY = 300;
        winnerText = nullptr; // Reset teks pemenang
        currentScreen = MAIN_MENU; // Kembali ke menu utama
            Skor=0;
            i=0;
    }
    EndDrawing();
    Time = 0;
}
