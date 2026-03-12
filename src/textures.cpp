#include "textures.h"
#include <raylib.h>

Texture2D generate_ground_texture() {
    Image img = GenImageChecked(16, 16, 4, 4, DARKGREEN, GREEN);
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    SetTextureFilter(tex, TEXTURE_FILTER_POINT);
    return tex;
}

Texture2D generate_player_texture() {
    Image img = GenImageColor(16, 16, BLUE);

    // Simple pixel face: two white eyes
    ImageDrawPixel(&img, 4, 5, WHITE);
    ImageDrawPixel(&img, 5, 5, WHITE);
    ImageDrawPixel(&img, 10, 5, WHITE);
    ImageDrawPixel(&img, 11, 5, WHITE);

    // Mouth
    for (int x = 5; x <= 10; x++) {
        ImageDrawPixel(&img, x, 11, WHITE);
    }
    ImageDrawPixel(&img, 4, 10, WHITE);
    ImageDrawPixel(&img, 11, 10, WHITE);

    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    SetTextureFilter(tex, TEXTURE_FILTER_POINT);
    return tex;
}

Texture2D generate_platform_texture() {
    Image img = GenImageChecked(16, 16, 4, 4, BROWN, DARKBROWN);
    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);
    SetTextureFilter(tex, TEXTURE_FILTER_POINT);
    return tex;
}
