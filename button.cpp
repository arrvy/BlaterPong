#include "button.hpp"

Button::Button(const char *imagePath, Vector2 imagePosition, float scale)
{
    Image image = LoadImage(imagePath);

    int originalWidth = image.width;
    int originalHeight = image.height;

    int newWidth = static_cast<int>(originalWidth * scale);
    int newHeight = static_cast<int>(originalHeight * scale);

    ImageResize(&image, newWidth, newHeight);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    position = imagePosition;
}

Button::~Button()
{
    UnloadTexture(texture);
}

void Button::Draw(Vector2 mousePos) // Pastikan ini sesuai
{
    // Gambar tombol
    DrawTextureV(texture, position, WHITE);

    // Buat rectangle untuk tombol
    Rectangle rect = {position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height)};

    // Periksa apakah mouse berada di atas tombol
    if (CheckCollisionPointRec(mousePos, rect)) {
        // Gambar outline jika mouse berada di atas tombol
        DrawRectangleLinesEx(rect, 2, WHITE); // Gambar outline dengan ketebalan 2 dan warna putih
    }
}

bool Button::isPressed(Vector2 mousePos, bool mousePressed)
{
    Rectangle rect = {position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height)};

    if(CheckCollisionPointRec(mousePos,rect)&& mousePressed)
    {
        return true;
    }
    return false;
}
