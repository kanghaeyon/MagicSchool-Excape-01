#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Magic School Escape #01");
    InitAudioDevice();
    SetTargetFPS(60);

    // =========================
    // AUDIO
    // =========================
    bool hasBGM = FileExists("bgm.ogg");
    bool hasScareSound = FileExists("scare.wav");
    bool hasBlanketSound = FileExists("blanket.wav");
    bool hasRugSound = FileExists("rug.wav");
    bool hasDrawerSound = FileExists("drawer.wav");

    Music bgm = { 0 };
    Sound scareSound = { 0 };
    Sound blanketSound = { 0 };
    Sound rugSound = { 0 };
    Sound drawerSound = { 0 };

    if (hasBGM)
    {
        bgm = LoadMusicStream("bgm.ogg");
        SetMusicVolume(bgm, 0.35f);
        PlayMusicStream(bgm);
    }

    if (hasScareSound)
    {
        scareSound = LoadSound("scare.wav");
        SetSoundVolume(scareSound, 0.9f);
    }

    if (hasBlanketSound)
    {
        blanketSound = LoadSound("blanket.wav");
        SetSoundVolume(blanketSound, 0.75f);
    }

    if (hasRugSound)
    {
        rugSound = LoadSound("rug.wav");
        SetSoundVolume(rugSound, 0.75f);
    }

    if (hasDrawerSound)
    {
        drawerSound = LoadSound("drawer.wav");
        SetSoundVolume(drawerSound, 0.8f);
    }

    // =========================
    // PLAYER - YEONMI
    // =========================
    Vector2 player = { 380, 430 };
    float speed = 3.5f;

    // =========================
    // KEY SYSTEM
    // =========================
    bool keyRevealed[3] = { false, false, false };
    bool keyCollected[3] = { false, false, false };
    int keyCount = 0;

    // =========================
    // DOOR / CLEAR
    // =========================
    bool doorOpen = false;
    bool cleared = false;

    // =========================
    // WINDOW SCARE EVENT
    // =========================
    bool windowEventDone = false;
    bool scareActive = false;

    float scareTimer = 0.0f;

    bool step1Done = false;
    bool step2Done = false;
    bool step3Done = false;

    const char *message = "Find 3 hidden keys.";

    Rectangle windowArea = { 90, 85, 150, 90 };

    // =========================
    // GAME LOOP
    // =========================
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        if (hasBGM)
        {
            UpdateMusicStream(bgm);
        }

        // =========================
        // SCARE EVENT
        // =========================
        if (scareActive)
        {
            scareTimer += dt;

            if (scareTimer > 0.35f && !step1Done)
            {
                player.y += 24;
                step1Done = true;
            }

            if (scareTimer > 0.65f && !step2Done)
            {
                player.y += 24;
                step2Done = true;
            }

            if (scareTimer > 0.95f && !step3Done)
            {
                player.y += 24;
                step3Done = true;
            }

            if (player.y > 535)
            {
                player.y = 535;
            }

            if (scareTimer > 2.0f)
            {
                scareActive = false;
                windowEventDone = true;
                message = "YEONMI: ...WHAT THE?!";
            }
        }

        // =========================
        // NORMAL GAME
        // =========================
        if (!cleared && !scareActive)
        {
            // 이동
            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
                player.y -= speed;

            if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
                player.y += speed;

            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
                player.x -= speed;

            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
                player.x += speed;

            // 방 경계
            if (player.x < 55) player.x = 55;
            if (player.x > 745) player.x = 745;
            if (player.y < 150) player.y = 150;
            if (player.y > 535) player.y = 535;

            // =========================
            // WINDOW EVENT
            // =========================
            bool nearWindow =
                player.x > 70 &&
                player.x < 260 &&
                player.y < 230;

            bool clickedWindow =
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(GetMousePosition(), windowArea);

            if (!windowEventDone &&
                ((nearWindow && IsKeyPressed(KEY_E)) || clickedWindow))
            {
                scareActive = true;
                scareTimer = 0.0f;

                step1Done = false;
                step2Done = false;
                step3Done = false;

                message = "...";

                if (hasScareSound)
                {
                    PlaySound(scareSound);
                }
            }

            // =========================
            // E INTERACTION
            // =========================
            else if (IsKeyPressed(KEY_E))
            {
                // -------------------------
                // KEY 1 : BED
                // -------------------------
                if (player.x > 70 &&
                    player.x < 280 &&
                    player.y > 260 &&
                    player.y < 430)
                {
                    if (!keyRevealed[0])
                    {
                        keyRevealed[0] = true;
                        message = "Something was hidden in the bed...";

                        if (hasBlanketSound)
                        {
                            PlaySound(blanketSound);
                        }
                    }
                    else if (!keyCollected[0])
                    {
                        keyCollected[0] = true;
                        keyCount++;
                        message = "KEY ACQUIRED.";
                    }
                    else
                    {
                        message = "Nothing else here.";
                    }
                }

                // -------------------------
                // KEY 2 : DRAWER / DESK
                // -------------------------
                else if (player.x > 540 &&
                         player.x < 730 &&
                         player.y > 220 &&
                         player.y < 420)
                {
                    if (!keyRevealed[1])
                    {
                        keyRevealed[1] = true;
                        message = "A key was hidden in the drawer...";

                        if (hasDrawerSound)
                        {
                            PlaySound(drawerSound);
                        }
                    }
                    else if (!keyCollected[1])
                    {
                        keyCollected[1] = true;
                        keyCount++;
                        message = "KEY ACQUIRED.";
                    }
                    else
                    {
                        message = "The drawer is empty.";
                    }
                }

                // -------------------------
                // KEY 3 : RUG
                // -------------------------
                else if (player.x > 250 &&
                         player.x < 550 &&
                         player.y > 370 &&
                         player.y < 535)
                {
                    if (!keyRevealed[2])
                    {
                        keyRevealed[2] = true;
                        message = "Something is under the rug...";

                        if (hasRugSound)
                        {
                            PlaySound(rugSound);
                        }
                    }
                    else if (!keyCollected[2])
                    {
                        keyCollected[2] = true;
                        keyCount++;
                        message = "KEY ACQUIRED.";
                    }
                    else
                    {
                        message = "Nothing else underneath.";
                    }
                }

                // -------------------------
                // DOOR
                // -------------------------
                else if (player.x > 330 &&
                         player.x < 470 &&
                         player.y < 220)
                {
                    if (keyCount < 3)
                    {
                        message = "LOCKED. Find all 3 keys.";
                    }
                    else if (!doorOpen)
                    {
                        doorOpen = true;
                        message = "The door opened...";
                    }
                }
            }

            // 열린 문으로 들어가면 클리어
            if (doorOpen &&
                player.x > 345 &&
                player.x < 455 &&
                player.y < 160)
            {
                cleared = true;
            }
        }

        // =========================
        // DRAW
        // =========================
        BeginDrawing();

        ClearBackground((Color){ 18, 13, 30, 255 });

        // 바닥
        DrawRectangle(
            40, 120,
            720, 440,
            (Color){ 82, 60, 94, 255 }
        );

        // 벽
        DrawRectangle(
            40, 70,
            720, 100,
            (Color){ 39, 30, 60, 255 }
        );

        // =========================
        // WINDOW
        // =========================
        DrawRectangle(
            90, 85,
            150, 90,
            (Color){ 10, 14, 28, 255 }
        );

        DrawRectangleLines(
            90, 85,
            150, 90,
            (Color){ 140, 120, 175, 255 }
        );

        DrawLine(
            165, 85,
            165, 175,
            (Color){ 140, 120, 175, 255 }
        );

        DrawLine(
            90, 130,
            240, 130,
            (Color){ 140, 120, 175, 255 }
        );

        DrawCircle(
            125, 110,
            18,
            (Color){ 225, 220, 190, 255 }
        );

        // =========================
        // GHOST
        // =========================
        if (scareActive)
        {
            DrawCircle(
                195, 125,
                30,
                (Color){ 225, 225, 220, 255 }
            );

            DrawEllipse(
                195, 135,
                23, 34,
                (Color){ 190, 190, 185, 255 }
            );

            DrawCircle(
                185, 120,
                5,
                RED
            );

            DrawCircle(
                205, 120,
                5,
                RED
            );

            DrawEllipse(
                195, 145,
                8, 13,
                BLACK
            );

            if (((int)(scareTimer * 12)) % 2 == 0)
            {
                DrawRectangle(
                    0, 0,
                    screenWidth,
                    screenHeight,
                    (Color){ 110, 0, 0, 55 }
                );
            }
        }

        // =========================
        // BED
        // =========================
        DrawRectangle(
            80, 290,
            180, 110,
            (Color){ 70, 40, 65, 255 }
        );

        DrawRectangle(
            90, 300,
            160, 90,
            (Color){ 120, 75, 125, 255 }
        );

        DrawRectangle(
            100, 310,
            60, 28,
            (Color){ 205, 195, 210, 255 }
        );

        // =========================
        // DESK
        // =========================
        DrawRectangle(
            560, 280,
            150, 20,
            (Color){ 75, 45, 35, 255 }
        );

        DrawRectangle(
            575, 300,
            15, 100,
            (Color){ 60, 35, 30, 255 }
        );

        DrawRectangle(
            680, 300,
            15, 100,
            (Color){ 60, 35, 30, 255 }
        );

        // =========================
        // MAGIC BOOK
        // =========================
        DrawRectangle(
            600, 250,
            65, 30,
            (Color){ 35, 20, 65, 255 }
        );

        DrawRectangleLines(
            600, 250,
            65, 30,
            GOLD
        );

        DrawCircle(
            632, 265,
            6,
            GOLD
        );

        // =========================
        // RUG
        // =========================
        DrawEllipse(
            400, 450,
            150, 70,
            (Color){ 55, 35, 80, 255 }
        );

        DrawEllipseLines(
            400, 450,
            150, 70,
            (Color){ 135, 100, 175, 255 }
        );

        // =========================
        // DOOR
        // =========================
        if (!doorOpen)
        {
            DrawRectangle(
                335, 70,
                130, 110,
                (Color){ 50, 30, 58, 255 }
            );

            DrawRectangleLines(
                335, 70,
                130, 110,
                (Color){ 155, 125, 180, 255 }
            );

            DrawCircle(
                445, 130,
                5,
                GOLD
            );
        }
        else
        {
            DrawRectangle(
                335, 70,
                130, 110,
                (Color){ 4, 3, 10, 255 }
            );

            DrawRectangle(
                455, 70,
                30, 110,
                (Color){ 50, 30, 58, 255 }
            );

            DrawRectangleLines(
                455, 70,
                30, 110,
                (Color){ 155, 125, 180, 255 }
            );

            DrawCircle(
                462, 130,
                4,
                GOLD
            );
        }

        // =========================
        // REVEALED KEYS
        // =========================
        if (keyRevealed[0] && !keyCollected[0])
        {
            DrawCircle(185, 350, 7, GOLD);
            DrawRectangle(190, 348, 20, 4, GOLD);
            DrawRectangle(205, 348, 4, 9, GOLD);
        }

        if (keyRevealed[1] && !keyCollected[1])
        {
            DrawCircle(630, 315, 7, GOLD);
            DrawRectangle(635, 313, 20, 4, GOLD);
            DrawRectangle(650, 313, 4, 9, GOLD);
        }

        if (keyRevealed[2] && !keyCollected[2])
        {
            DrawCircle(400, 450, 7, GOLD);
            DrawRectangle(405, 448, 20, 4, GOLD);
            DrawRectangle(420, 448, 4, 9, GOLD);
        }

        // =========================
        // YEONMI
        // =========================

        // 몸
        DrawRectangle(
            (int)player.x - 12,
            (int)player.y - 4,
            24, 32,
            (Color){ 30, 30, 35, 255 }
        );

        // 머리
        DrawCircle(
            (int)player.x,
            (int)player.y - 23,
            16,
            (Color){ 175, 125, 95, 255 }
        );

        // 짧은 머리
        DrawRectangle(
            (int)player.x - 13,
            (int)player.y - 39,
            26, 8,
            (Color){ 24, 20, 20, 255 }
        );

        DrawRectangle(
            (int)player.x - 15,
            (int)player.y - 35,
            30, 5,
            (Color){ 24, 20, 20, 255 }
        );

        // 안경
        DrawRectangleLines(
            (int)player.x - 13,
            (int)player.y - 27,
            11, 8,
            BLACK
        );

        DrawRectangleLines(
            (int)player.x + 2,
            (int)player.y - 27,
            11, 8,
            BLACK
        );

        DrawLine(
            (int)player.x - 2,
            (int)player.y - 23,
            (int)player.x + 2,
            (int)player.y - 23,
            BLACK
        );

        // 다리
        DrawRectangle(
            (int)player.x - 10,
            (int)player.y + 27,
            7, 10,
            (Color){ 35, 35, 40, 255 }
        );

        DrawRectangle(
            (int)player.x + 3,
            (int)player.y + 27,
            7, 10,
            (Color){ 35, 35, 40, 255 }
        );

        // 이름
        DrawText(
            "YEONMI",
            (int)player.x - 24,
            (int)player.y + 42,
            13,
            RAYWHITE
        );

        // =========================
        // UI
        // =========================
        DrawText(
            TextFormat("KEYS : %d / 3", keyCount),
            20, 20,
            24,
            GOLD
        );

        DrawText(
            message,
            250, 205,
            18,
            RAYWHITE
        );

        DrawText(
            "MOVE : WASD / ARROW",
            20, 565,
            16,
            RAYWHITE
        );

        DrawText(
            "E : SEARCH",
            650, 565,
            16,
            RAYWHITE
        );

        if (scareActive && scareTimer > 0.9f)
        {
            DrawText(
                "YEONMI: WHAT THE...?!",
                275, 520,
                22,
                RED
            );
        }

        // =========================
        // CLEAR
        // =========================
        if (cleared)
        {
            DrawRectangle(
                0, 0,
                screenWidth,
                screenHeight,
                (Color){ 5, 4, 12, 240 }
            );

            DrawText(
                "ROOM 1 CLEAR!",
                265, 220,
                40,
                GOLD
            );

            DrawText(
                "MAGIC SCHOOL ESCAPE #01",
                245, 285,
                22,
                RAYWHITE
            );

            DrawText(
                "YEONMI ESCAPED.",
                300, 330,
                22,
                RAYWHITE
            );
        }

        EndDrawing();
    }

    // =========================
    // CLEANUP
    // =========================
    if (hasBGM)
    {
        StopMusicStream(bgm);
        UnloadMusicStream(bgm);
    }

    if (hasScareSound)
        UnloadSound(scareSound);

    if (hasBlanketSound)
        UnloadSound(blanketSound);

    if (hasRugSound)
        UnloadSound(rugSound);

    if (hasDrawerSound)
        UnloadSound(drawerSound);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
