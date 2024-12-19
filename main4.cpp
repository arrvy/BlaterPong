#include "raylib.h"
#include <stdio.h>



// Struktur untuk bola
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

// Deklarasi fungsi
static void gameScreen(void);

// Enum untuk layar permainan
typedef enum { LOGO = 0, TITLE, CREDITS, GAMEPLAY1,GAMEAI1, GAMEAI2,GAMEAI3, ENDING } GameScreen;
static int currentScreen = LOGO; // Layar saat ini
static int framesCounter = 0; // Penghitung frame
Ball ball; // Objek bola
Paddle leftPaddle; // Paddle kiri
Paddle rightPaddle; // Paddle kanan

const char* winnerText = nullptr; // Teks pemenang

// Resource
static RenderTexture2D screenTarget = { 0 }; // Render texture untuk efek
static Texture2D texLogo = { 0 }; // Tekstur logo

static Sound fxCollision = { 0 };

int main() {
    // Inisialisasi jendela
    InitWindow(800, 600, "BlaterPong");
    SetWindowState(FLAG_VSYNC_HINT); // Mengaktifkan V-Sync
    InitAudioDevice();

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

    // Memuat tekstur logo
    texLogo = LoadTexture("resource/logo.png");

    fxCollision = LoadSound ("resource/Audio/Collision.wav");

    if (texLogo.id == 0) {
        // Jika tekstur gagal dimuat, tampilkan pesan kesalahan
        printf("Failed to load texture: resource/logo.png\n");
    }

    // Inisialisasi render texture
    screenTarget = LoadRenderTexture(800, 600);
    SetTextureFilter(screenTarget.texture, TEXTURE_FILTER_BILINEAR);

    // Loop utama
    while (!WindowShouldClose()) { // Deteksi tombol tutup jendela atau tombol ESC
        gameScreen();
    }

    // Membersihkan sumber daya
    UnloadTexture(texLogo);

    UnloadSound(fxCollision);
    CloseWindow(); // Menutup jendela

    return 0;
}



// Fungsi untuk mengelola tampilan layar permainan
static void gameScreen(void) {
    switch (currentScreen) {
        case LOGO: {
            // Update layar LOGO
            framesCounter++;

            if (framesCounter > 300) {
                framesCounter = 0;
                currentScreen = TITLE; // Pindah ke layar TITLE setelah 100 frame
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
        } break;

        case TITLE: {

            // Menampilkan layar TITLE
            if (IsKeyPressed(KEY_ENTER)) {
                framesCounter = 0;
                currentScreen = GAMEAI1; // Pindah ke layar GAMEPLAY saat ENTER ditekan
            }
            BeginDrawing();
            ClearBackground(BLACK); // Bersihkan latar belakang layar TITLE
            DrawText("BlaterPong", 320, 320, 20, WHITE);
            DrawText("Press 'ENTER' to continue", 320, 360, 15, WHITE); // Tampilkan judul permainan
            EndDrawing();
        } break;

        case GAMEPLAY1: {




            // Update posisi bola
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
            if (IsKeyDown(KEY_W) && leftPaddle.y > 50) {
                leftPaddle.y -= leftPaddle.speed * GetFrameTime();
            }
            if (IsKeyDown(KEY_S) && leftPaddle.y < GetScreenHeight() - 50) {
                leftPaddle.y += leftPaddle.speed * GetFrameTime();
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
            if (ball.x < 0) {
                winnerText = "Right Player Wins!"; // Pemain kanan menang
            }
            if (ball.x > GetScreenWidth()) {
                winnerText = "Left Player Wins!"; // Pemain kiri menang
            }
            if (winnerText && IsKeyPressed(KEY_SPACE)) {
                // Reset permainan jika SPACE ditekan
                ball.x = GetScreenWidth() / 2;
                ball.y = GetScreenHeight() / 2;
                ball.speedX = 300;
                ball.speedY = 300;
                winnerText = nullptr; // Reset teks pemenang
            }

            // Mulai menggambar ke layar utama
            BeginDrawing();
            ClearBackground(BLACK); // Bersihkan latar belakang
            ball.Draw(); // Gambar bola
            leftPaddle.Draw(); // Gambar paddle kiri
            rightPaddle.Draw(); // Gambar paddle kanan

            // Tampilkan teks pemenang jika ada
            if (winnerText) {
                int textWidth = MeasureText(winnerText, 60);
                DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
            }

            DrawFPS(10, 10); // Tampilkan FPS
            EndDrawing();

        } break;
        case GAMEAI1: {



            // Update posisi bola
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
            if (leftPaddle.y > ball.y && (leftPaddle.y - ball.y) > 20 && ball.x < 400 && ball.x > 0 ) // Ketika Paddle berada di bawah bola, maka akan bergerak ke atas. Juga paddle bergerak hanya ketika selisih jaraknya lebih dari 20 dan ketika jarak bola x melewati setengah lapangan
                {
                leftPaddle.y -=  ( (leftPaddle.y - ball.y+10) + ball.x - leftPaddle.x) *1.2 * GetFrameTime();
            }
            if (leftPaddle.y < ball.y && (ball.y -leftPaddle.y) > 20 && ball.x < 400 && ball.x > 0 ) // Ketika Paddle berada di atas bola, maka akan bergerak ke bawah. Juga paddle bergerak hanya ketika selisih jaraknya lebih dari 20 dan ketika jarak bola x melewati setengah lapangan
            {
                leftPaddle.y += ( (ball.y -leftPaddle.y+10) + ball.x - leftPaddle.x ) *1.2* GetFrameTime();
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
            if (ball.x < 0) {
                winnerText = "Right Player Wins!"; // Pemain kanan menang
            }
            if (ball.x > GetScreenWidth()) {
                winnerText = "Left Player Wins!"; // Pemain kiri menang
            }
            if (winnerText && IsKeyPressed(KEY_SPACE)) {
                // Reset permainan jika SPACE ditekan
                ball.x = GetScreenWidth() / 2;
                ball.y = GetScreenHeight() / 2;
                ball.speedX = 300;
                ball.speedY = 300;
                winnerText = nullptr; // Reset teks pemenang
            }

            // Mulai menggambar ke layar utama
            BeginDrawing();
            ClearBackground(BLACK); // Bersihkan latar belakang
            ball.Draw(); // Gambar bola
            leftPaddle.Draw(); // Gambar paddle kiri
            rightPaddle.Draw(); // Gambar paddle kanan

            // Tampilkan teks pemenang jika ada
            if (winnerText) {
                int textWidth = MeasureText(winnerText, 60);
                DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
            }

            DrawFPS(10, 10); // Tampilkan FPS
            EndDrawing();

        } break;
        case GAMEAI2: {


            // Update posisi bola
            ball.x += ball.speedX * GetFrameTime();
            ball.y += ball.speedY * GetFrameTime();

            // Deteksi tabrakan dengan tepi atas dan bawah
            if (ball.y < 0) {
                ball.y = 0;
                ball.speedY *= -1; // Balik arah bola
            }
            if (ball.y > GetScreenHeight()) {
                ball.y = GetScreenHeight();
                ball.speedY *= -1; // Balik arah bola
            }

            // Kontrol paddle kiri
            if (leftPaddle.y > ball.y && (leftPaddle.y - ball.y) > 20 && ball.x < 600 && ball.x > 0 ) // Ketika Paddle berada di bawah bola, maka akan bergerak ke atas. Juga paddle bergerak hanya ketika selisih jaraknya lebih dari 20 dan ketika jarak bola x melewati setengah lapangan
                {
                leftPaddle.y -= leftPaddle.speed/2* GetFrameTime();
            }
            if (leftPaddle.y < ball.y && (ball.y -leftPaddle.y) > 20 && ball.x < 600 && ball.x > 0) // Ketika Paddle berada di atas bola, maka akan bergerak ke bawah. Juga paddle bergerak hanya ketika selisih jaraknya lebih dari 20 dan ketika jarak bola x melewati setengah lapangan
            {
                leftPaddle.y += leftPaddle.speed/2* GetFrameTime();
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
                    ball.speedX *= -1.1f; // Tingkatkan kecepatan bola
                    ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX; // Atur kecepatan Y
                }
            }
            if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect())) {
                if (ball.speedX > 0) {
                    ball.speedX *= -1.1f; // Tingkatkan kecepatan bola
                    ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX; // Atur kecepatan Y
                }
            }

            // Cek jika bola keluar dari layar
            if (ball.x < 0) {
                winnerText = "Right Player Wins!"; // Pemain kanan menang
            }
            if (ball.x > GetScreenWidth()) {
                winnerText = "Left Player Wins!"; // Pemain kiri menang
            }
            if (winnerText && IsKeyPressed(KEY_SPACE)) {
                // Reset permainan jika SPACE ditekan
                ball.x = GetScreenWidth() / 2;
                ball.y = GetScreenHeight() / 2;
                ball.speedX = 300;
                ball.speedY = 300;
                winnerText = nullptr; // Reset teks pemenang
            }

            // Mulai menggambar ke layar utama
            BeginDrawing();
            ClearBackground(BLACK); // Bersihkan latar belakang
            ball.Draw(); // Gambar bola
            leftPaddle.Draw(); // Gambar paddle kiri
            rightPaddle.Draw(); // Gambar paddle kanan

            // Tampilkan teks pemenang jika ada
            if (winnerText) {
                int textWidth = MeasureText(winnerText, 60);
                DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
            }

            DrawFPS(10, 10); // Tampilkan FPS
            EndDrawing();

        } break;

        case GAMEAI3: {


            // Update posisi bola
            ball.x += ball.speedX * GetFrameTime();
            ball.y += ball.speedY * GetFrameTime();

            // Deteksi tabrakan dengan tepi atas dan bawah
            if (ball.y < 0) {
                ball.y = 0;
                ball.speedY *= -1; // Balik arah bola
            }
            if (ball.y > GetScreenHeight()) {
                ball.y = GetScreenHeight();
                ball.speedY *= -1; // Balik arah bola
            }

            // Kontrol paddle kiri
            if (leftPaddle.y > ball.y && (leftPaddle.y - ball.y) > 20 && ball.x < 400 && ball.x > 0 ) // Ketika Paddle berada di bawah bola, maka akan bergerak ke atas. Juga paddle bergerak hanya ketika selisih jaraknya lebih dari 20 dan ketika jarak bola x melewati setengah lapangan
                {
                leftPaddle.y -= (leftPaddle.speed * GetFrameTime());
            }
            if (leftPaddle.y < ball.y && (ball.y -leftPaddle.y) > 20 && ball.x < 400 && ball.x > 0 ) // Ketika Paddle berada di atas bola, maka akan bergerak ke bawah. Juga paddle bergerak hanya ketika selisih jaraknya lebih dari 20 dan ketika jarak bola x melewati setengah lapangan
            {
                leftPaddle.y += (leftPaddle.speed * GetFrameTime());
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
                    ball.speedX *= -1.1f; // Tingkatkan kecepatan bola
                    ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX; // Atur kecepatan Y
                }
            }
            if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect())) {
                if (ball.speedX > 0) {
                    ball.speedX *= -1.1f; // Tingkatkan kecepatan bola
                    ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX; // Atur kecepatan Y
                }
            }

            // Cek jika bola keluar dari layar
            if (ball.x < 0) {
                winnerText = "Right Player Wins!"; // Pemain kanan menang
            }
            if (ball.x > GetScreenWidth()) {
                winnerText = "Left Player Wins!"; // Pemain kiri menang
            }
            if (winnerText && IsKeyPressed(KEY_SPACE)) {
                // Reset permainan jika SPACE ditekan
                ball.x = GetScreenWidth() / 2;
                ball.y = GetScreenHeight() / 2;
                ball.speedX = 300;
                ball.speedY = 300;
                winnerText = nullptr; // Reset teks pemenang
            }

            // Mulai menggambar ke layar utama
            BeginDrawing();
            ClearBackground(BLACK); // Bersihkan latar belakang
            ball.Draw(); // Gambar bola
            leftPaddle.Draw(); // Gambar paddle kiri
            rightPaddle.Draw(); // Gambar paddle kanan

            // Tampilkan teks pemenang jika ada
            if (winnerText) {
                int textWidth = MeasureText(winnerText, 60);
                DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
            }

            DrawFPS(10, 10); // Tampilkan FPS
            EndDrawing();

        } break;

        case CREDITS: {
            framesCounter++;
            if (framesCounter > 300) {
                framesCounter = 0;
                currentScreen = TITLE; // Kembali ke layar TITLE setelah 300 frame
            }
        } break;

        case ENDING: {
            framesCounter++;
            if (framesCounter > 300) {
                framesCounter = 0;
                currentScreen = TITLE; // Kembali ke layar TITLE setelah 300 frame
            }
        } break;

        default: break; // Tidak ada tindakan untuk layar yang tidak dikenal
    }
}
