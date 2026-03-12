#include <raylib.h>
#include "textures.h"

int main() {
    InitWindow(800, 600, "4D Pixel Game");
    SetTargetFPS(60);

    Camera3D camera = {};
    camera.position   = { 0.0f, 20.0f, 10.0f };
    camera.target     = { 0.0f,  0.0f,  0.0f };
    camera.up         = { 0.0f,  1.0f,  0.0f };
    camera.fovy       = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Texture2D ground_tex = generate_ground_texture();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground({ 20, 20, 30, 255 });

        BeginMode3D(camera);

        // Draw ground plane
        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 20.0f, 20.0f }, WHITE);
        DrawGrid(20, 1.0f);

        EndMode3D();

        DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadTexture(ground_tex);
    CloseWindow();
    return 0;
}
