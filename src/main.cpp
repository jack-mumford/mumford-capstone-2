#include <raylib.h>
#include "textures.h"
#include "player.h"
#include "level.h"

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

    Player   player = player_init();
    Platform platforms[MAX_PLATFORMS];
    int      platform_count = 0;
    level_init(platforms, platform_count);

    while (!WindowShouldClose()) {
        // Reset grounded each frame; player_update and check_platform_collision re-set it
        player.is_grounded = false;

        // Update player physics and input
        player_update(player);

        // Platform collision (must run after player_update moves position)
        check_platform_collision(player, platforms, platform_count);

        // Camera follows player
        camera.target   = player.position;
        camera.position = {
            player.position.x,
            player.position.y + 20.0f,
            player.position.z + 10.0f
        };

        // Draw
        BeginDrawing();
        ClearBackground({ 20, 20, 30, 255 });

        BeginMode3D(camera);

        DrawPlane({ 0.0f, 0.0f, 0.0f }, { 20.0f, 20.0f }, DARKGREEN);
        DrawGrid(20, 1.0f);
        level_draw(platforms, platform_count);
        player_draw(player);

        EndMode3D();

        DrawFPS(10, 10);
        DrawText(player.is_grounded ? "GROUNDED" : "AIRBORNE", 10, 30, 20, WHITE);
        EndDrawing();
    }

    UnloadTexture(ground_tex);
    CloseWindow();
    return 0;
}
