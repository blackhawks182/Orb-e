#include "cannon_throw.h"

Vector2 rightEdgeMidpoint(Rectangle rect, Vector2 origin, float rotation)
{
    float rotationRadians = rotation * DEG2RAD;
    Vector2 unrotatedRectPoint = { rect.width, rect.height / 2.0f };
    
    Vector2 rotatedRectPoint;
    rotatedRectPoint.x = (unrotatedRectPoint.x * cosf(rotationRadians) - unrotatedRectPoint.y * sinf(rotationRadians)) + rect.x; //xnew = xold * costheta - yold * sintheta
    rotatedRectPoint.y = (unrotatedRectPoint.x * sinf(rotationRadians) + unrotatedRectPoint.y * cosf(rotationRadians)) + rect.y; //ynew = xold * sintheta + yold * costheta
    return rotatedRectPoint;
}


// Runs the game loop and switches between game states.

void startCannonThrow(void){

    obstacle obstacles[10];

    int obstacleSet = GetRandomValue(0, 9);
    int obstacleCount = 0;

    Sound releaseSound = LoadSound("assets/release.mp3");
    Sound hitSound = LoadSound("assets/hit.mp3");
    SetSoundVolume(releaseSound, 0.5f);
    SetSoundVolume(hitSound, 0.3f);

    if (obstacleSet == 0){
        obstacleCount = 5;

        obstacles[0].rectangle = (Rectangle){450, 200, 80, 80};
        obstacles[0].velocity = (Vector2){0, 0};
        obstacles[0].type = 0;
        obstacles[0].active = 1;
        obstacles[0].leftbound = 450;
        obstacles[0].rightbound = 530;

        obstacles[1].rectangle = (Rectangle){650, 150, 120, 40};
        obstacles[1].velocity = (Vector2){80, 0};
        obstacles[1].type = 1;
        obstacles[1].active = 1;
        obstacles[1].leftbound = 550;
        obstacles[1].rightbound = 900;

        obstacles[2].rectangle = (Rectangle){850, 300, 50, 120};
        obstacles[2].velocity = (Vector2){0, -120};
        obstacles[2].type = 1;
        obstacles[2].active = 1;
        obstacles[2].leftbound = 850;
        obstacles[2].rightbound = 900;

        obstacles[3].rectangle = (Rectangle){550, 450, 150, 50};
        obstacles[3].velocity = (Vector2){0, 0};
        obstacles[3].type = 0;
        obstacles[3].active = 1;
        obstacles[3].leftbound = 550;
        obstacles[3].rightbound = 700;

        obstacles[4].rectangle = (Rectangle){950, 180, 100, 60};
        obstacles[4].velocity = (Vector2){-140, 0};
        obstacles[4].type = 1;
        obstacles[4].active = 1;
        obstacles[4].leftbound = 800;
        obstacles[4].rightbound = 1100;
    }

    if (obstacleSet == 1){
        obstacleCount = 5;

        obstacles[0].rectangle = (Rectangle){450, 200, 50, 50};
        obstacles[0].velocity = (Vector2){100, 0};
        obstacles[0].type = 1;
        obstacles[0].active = 1;
        obstacles[0].leftbound = 400;
        obstacles[0].rightbound = 700;

        obstacles[1].rectangle = (Rectangle){700, 200, 50, 120};
        obstacles[1].velocity = (Vector2){0, 80};
        obstacles[1].type = 1;
        obstacles[1].active = 1;
        obstacles[1].leftbound = 700;
        obstacles[1].rightbound = 750;

        obstacles[2].rectangle = (Rectangle){950, 300, 100, 50};
        obstacles[2].velocity = (Vector2){-180, 0};
        obstacles[2].type = 1;
        obstacles[2].active = 1;
        obstacles[2].leftbound = 800;
        obstacles[2].rightbound = 1100;

        obstacles[3].rectangle = (Rectangle){550, 450, 140, 50};
        obstacles[3].velocity = (Vector2){0, 0};
        obstacles[3].type = 0;
        obstacles[3].active = 1;
        obstacles[3].leftbound = 550;
        obstacles[3].rightbound = 690;

        obstacles[4].rectangle = (Rectangle){900, 400, 60, 60};
        obstacles[4].velocity = (Vector2){0, 150};
        obstacles[4].type = 1;
        obstacles[4].active = 1;
        obstacles[4].leftbound = 900;
        obstacles[4].rightbound = 960;
    }

    if (obstacleSet == 2){
        obstacleCount = 6;

        obstacles[0].rectangle = (Rectangle){450, 180, 70, 70};
        obstacles[0].velocity = (Vector2){100, 0};
        obstacles[0].type = 1;
        obstacles[0].active = 1;
        obstacles[0].leftbound = 400;
        obstacles[0].rightbound = 650;

        obstacles[1].rectangle = (Rectangle){700, 180, 80, 80};
        obstacles[1].velocity = (Vector2){0, 120};
        obstacles[1].type = 1;
        obstacles[1].active = 1;
        obstacles[1].leftbound = 700;
        obstacles[1].rightbound = 780;

        obstacles[2].rectangle = (Rectangle){900, 180, 60, 60};
        obstacles[2].velocity = (Vector2){-140, 0};
        obstacles[2].type = 1;
        obstacles[2].active = 1;
        obstacles[2].leftbound = 750;
        obstacles[2].rightbound = 1050;

        obstacles[3].rectangle = (Rectangle){500, 400, 120, 50};
        obstacles[3].velocity = (Vector2){0, 0};
        obstacles[3].type = 0;
        obstacles[3].active = 1;
        obstacles[3].leftbound = 500;
        obstacles[3].rightbound = 620;

        obstacles[4].rectangle = (Rectangle){800, 350, 50, 100};
        obstacles[4].velocity = (Vector2){0, -100};
        obstacles[4].type = 1;
        obstacles[4].active = 1;
        obstacles[4].leftbound = 800;
        obstacles[4].rightbound = 850;

        obstacles[5].rectangle = (Rectangle){1000, 400, 100, 40};
        obstacles[5].velocity = (Vector2){-100, 0};
        obstacles[5].type = 1;
        obstacles[5].active = 1;
        obstacles[5].leftbound = 900;
        obstacles[5].rightbound = 1150;
    }

    if (obstacleSet == 3){
        obstacleCount = 4;

        obstacles[0].rectangle = (Rectangle){500, 180, 50, 100};
        obstacles[0].velocity = (Vector2){120, 0};
        obstacles[0].type = 1;
        obstacles[0].active = 1;
        obstacles[0].leftbound = 450;
        obstacles[0].rightbound = 750;

        obstacles[1].rectangle = (Rectangle){800, 180, 100, 50};
        obstacles[1].velocity = (Vector2){0, 100};
        obstacles[1].type = 1;
        obstacles[1].active = 1;
        obstacles[1].leftbound = 800;
        obstacles[1].rightbound = 900;

        obstacles[2].rectangle = (Rectangle){550, 400, 80, 80};
        obstacles[2].velocity = (Vector2){150, 0};
        obstacles[2].type = 1;
        obstacles[2].active = 1;
        obstacles[2].leftbound = 500;
        obstacles[2].rightbound = 850;

        obstacles[3].rectangle = (Rectangle){950, 400, 120, 60};
        obstacles[3].velocity = (Vector2){0, 0};
        obstacles[3].type = 0;
        obstacles[3].active = 1;
        obstacles[3].leftbound = 950;
        obstacles[3].rightbound = 1070;
    }

    if (obstacleSet == 4){
        obstacleCount = 6;

        obstacles[0].rectangle = (Rectangle){450, 150, 60, 60};
        obstacles[0].velocity = (Vector2){80, 0};
        obstacles[0].type = 1;
        obstacles[0].active = 1;
        obstacles[0].leftbound = 400;
        obstacles[0].rightbound = 650;

        obstacles[1].rectangle = (Rectangle){700, 150, 50, 100};
        obstacles[1].velocity = (Vector2){0, 70};
        obstacles[1].type = 1;
        obstacles[1].active = 1;
        obstacles[1].leftbound = 700;
        obstacles[1].rightbound = 750;

        obstacles[2].rectangle = (Rectangle){900, 150, 120, 50};
        obstacles[2].velocity = (Vector2){-160, 0};
        obstacles[2].type = 1;
        obstacles[2].active = 1;
        obstacles[2].leftbound = 750;
        obstacles[2].rightbound = 1100;

        obstacles[3].rectangle = (Rectangle){500, 350, 100, 100};
        obstacles[3].velocity = (Vector2){0, 0};
        obstacles[3].type = 0;
        obstacles[3].active = 1;
        obstacles[3].leftbound = 500;
        obstacles[3].rightbound = 600;

        obstacles[4].rectangle = (Rectangle){750, 400, 150, 40};
        obstacles[4].velocity = (Vector2){-100, 0};
        obstacles[4].type = 1;
        obstacles[4].active = 1;
        obstacles[4].leftbound = 650;
        obstacles[4].rightbound = 1000;

        obstacles[5].rectangle = (Rectangle){1000, 400, 60, 60};
        obstacles[5].velocity = (Vector2){0, -130};
        obstacles[5].type = 1;
        obstacles[5].active = 1;
        obstacles[5].leftbound = 1000;
        obstacles[5].rightbound = 1060;
    }
    if (obstacleSet == 5){
        obstacleCount = 5;

        obstacles[0].rectangle = (Rectangle){450, 180, 30, 100};
        obstacles[0].velocity = (Vector2){120, 80};
        obstacles[0].type = 1;
        obstacles[0].active = 1;
        obstacles[0].leftbound = 400;
        obstacles[0].rightbound = 700;

        obstacles[1].rectangle = (Rectangle){700, 150, 100, 25};
        obstacles[1].velocity = (Vector2){-100, 120};
        obstacles[1].type = 1;
        obstacles[1].active = 1;
        obstacles[1].leftbound = 600;
        obstacles[1].rightbound = 900;

        obstacles[2].rectangle = (Rectangle){900, 300, 25, 120};
        obstacles[2].velocity = (Vector2){140, -100};
        obstacles[2].type = 1;
        obstacles[2].active = 1;
        obstacles[2].leftbound = 800;
        obstacles[2].rightbound = 1100;

        obstacles[3].rectangle = (Rectangle){550, 400, 120, 60};
        obstacles[3].velocity = (Vector2){0, 0};
        obstacles[3].type = 0;
        obstacles[3].active = 1;
        obstacles[3].leftbound = 550;
        obstacles[3].rightbound = 670;

        obstacles[4].rectangle = (Rectangle){1000, 400, 80, 30};
        obstacles[4].velocity = (Vector2){-160, -70};
        obstacles[4].type = 1;
        obstacles[4].active = 1;
        obstacles[4].leftbound = 850;
        obstacles[4].rightbound = 1150;
    }

    if (obstacleSet == 6){
        obstacleCount = 6;

        obstacles[0].rectangle = (Rectangle){450, 150, 20, 140};
        obstacles[0].velocity = (Vector2){150, 100};
        obstacles[0].type = 1;
        obstacles[0].active = 1;
        obstacles[0].leftbound = 400;
        obstacles[0].rightbound = 650;

        obstacles[1].rectangle = (Rectangle){650, 350, 120, 25};
        obstacles[1].velocity = (Vector2){-120, -90};
        obstacles[1].type = 1;
        obstacles[1].active = 1;
        obstacles[1].leftbound = 550;
        obstacles[1].rightbound = 800;

        obstacles[2].rectangle = (Rectangle){850, 180, 35, 35};
        obstacles[2].velocity = (Vector2){160, 130};
        obstacles[2].type = 1;
        obstacles[2].active = 1;
        obstacles[2].leftbound = 750;
        obstacles[2].rightbound = 1100;

        obstacles[3].rectangle = (Rectangle){500, 400, 100, 70};
        obstacles[3].velocity = (Vector2){0, 0};
        obstacles[3].type = 0;
        obstacles[3].active = 1;
        obstacles[3].leftbound = 500;
        obstacles[3].rightbound = 600;

        obstacles[4].rectangle = (Rectangle){750, 200, 20, 100};
        obstacles[4].velocity = (Vector2){-140, 110};
        obstacles[4].type = 1;
        obstacles[4].active = 1;
        obstacles[4].leftbound = 650;
        obstacles[4].rightbound = 850;

        obstacles[5].rectangle = (Rectangle){1000, 400, 100, 30};
        obstacles[5].velocity = (Vector2){-180, -130};
        obstacles[5].type = 1;
        obstacles[5].active = 1;
        obstacles[5].leftbound = 850;
        obstacles[5].rightbound = 1150;
    }

    if (obstacleSet == 7){
        obstacleCount = 5;

        obstacles[0].rectangle = (Rectangle){450, 200, 25, 25};
        obstacles[0].velocity = (Vector2){180, 150};
        obstacles[0].type = 1;
        obstacles[0].active = 1;
        obstacles[0].leftbound = 400;
        obstacles[0].rightbound = 700;

        obstacles[1].rectangle = (Rectangle){700, 150, 140, 30};
        obstacles[1].velocity = (Vector2){-130, 100};
        obstacles[1].type = 1;
        obstacles[1].active = 1;
        obstacles[1].leftbound = 600;
        obstacles[1].rightbound = 900;

        obstacles[2].rectangle = (Rectangle){900, 250, 30, 150};
        obstacles[2].velocity = (Vector2){150, -120};
        obstacles[2].type = 1;
        obstacles[2].active = 1;
        obstacles[2].leftbound = 800;
        obstacles[2].rightbound = 1100;

        obstacles[3].rectangle = (Rectangle){550, 400, 100, 50};
        obstacles[3].velocity = (Vector2){0, 0};
        obstacles[3].type = 0;
        obstacles[3].active = 1;
        obstacles[3].leftbound = 550;
        obstacles[3].rightbound = 650;

        obstacles[4].rectangle = (Rectangle){850, 400, 15, 100};
        obstacles[4].velocity = (Vector2){-110, -160};
        obstacles[4].type = 1;
        obstacles[4].active = 1;
        obstacles[4].leftbound = 750;
        obstacles[4].rightbound = 1050;
    }

    if (obstacleSet == 8){
        obstacleCount = 6;

        obstacles[0].rectangle = (Rectangle){450, 150, 100, 25};
        obstacles[0].velocity = (Vector2){100, 140};
        obstacles[0].type = 1;
        obstacles[0].active = 1;
        obstacles[0].leftbound = 400;
        obstacles[0].rightbound = 700;

        obstacles[1].rectangle = (Rectangle){650, 250, 25, 130};
        obstacles[1].velocity = (Vector2){160, -100};
        obstacles[1].type = 1;
        obstacles[1].active = 1;
        obstacles[1].leftbound = 550;
        obstacles[1].rightbound = 850;

        obstacles[2].rectangle = (Rectangle){900, 150, 50, 50};
        obstacles[2].velocity = (Vector2){-180, 160};
        obstacles[2].type = 1;
        obstacles[2].active = 1;
        obstacles[2].leftbound = 750;
        obstacles[2].rightbound = 1100;

        obstacles[3].rectangle = (Rectangle){500, 420, 140, 40};
        obstacles[3].velocity = (Vector2){0, 0};
        obstacles[3].type = 0;
        obstacles[3].active = 1;
        obstacles[3].leftbound = 500;
        obstacles[3].rightbound = 640;

        obstacles[4].rectangle = (Rectangle){800, 400, 20, 80};
        obstacles[4].velocity = (Vector2){130, -150};
        obstacles[4].type = 1;
        obstacles[4].active = 1;
        obstacles[4].leftbound = 700;
        obstacles[4].rightbound = 1000;

        obstacles[5].rectangle = (Rectangle){1000, 300, 120, 25};
        obstacles[5].velocity = (Vector2){-140, 100};
        obstacles[5].type = 1;
        obstacles[5].active = 1;
        obstacles[5].leftbound = 850;
        obstacles[5].rightbound = 1150;
    }

    if (obstacleSet == 9){
        obstacleCount = 6;

        obstacles[0].rectangle = (Rectangle){450, 180, 20, 160};
        obstacles[0].velocity = (Vector2){170, 90};
        obstacles[0].type = 1;
        obstacles[0].active = 1;
        obstacles[0].leftbound = 400;
        obstacles[0].rightbound = 650;

        obstacles[1].rectangle = (Rectangle){650, 150, 150, 20};
        obstacles[1].velocity = (Vector2){-150, 130};
        obstacles[1].type = 1;
        obstacles[1].active = 1;
        obstacles[1].leftbound = 550;
        obstacles[1].rightbound = 850;

        obstacles[2].rectangle = (Rectangle){850, 300, 30, 30};
        obstacles[2].velocity = (Vector2){180, -170};
        obstacles[2].type = 1;
        obstacles[2].active = 1;
        obstacles[2].leftbound = 750;
        obstacles[2].rightbound = 1100;

        obstacles[3].rectangle = (Rectangle){500, 400, 80, 80};
        obstacles[3].velocity = (Vector2){0, 0};
        obstacles[3].type = 0;
        obstacles[3].active = 1;
        obstacles[3].leftbound = 500;
        obstacles[3].rightbound = 580;

        obstacles[4].rectangle = (Rectangle){700, 350, 20, 100};
        obstacles[4].velocity = (Vector2){-160, -140};
        obstacles[4].type = 1;
        obstacles[4].active = 1;
        obstacles[4].leftbound = 600;
        obstacles[4].rightbound = 850;

        obstacles[5].rectangle = (Rectangle){1000, 400, 100, 30};
        obstacles[5].velocity = (Vector2){-200, -100};
        obstacles[5].type = 1;
        obstacles[5].active = 1;
        obstacles[5].leftbound = 850;
        obstacles[5].rightbound = 1150;
    }

    Rectangle ground = {0,SCREEN_HEIGHT*2/3, SCREEN_WIDTH, SCREEN_HEIGHT/3};
	Rectangle cannonBase = {120, (SCREEN_HEIGHT*2/3) - 15, 100, 15};
	Rectangle cannonBody = {120, (SCREEN_HEIGHT*2/3) - 50, 100, 35};
    Rectangle speedBar = {120, (SCREEN_HEIGHT*2/3) - 180, 100, 5};
	Vector2 cannonRotationPoint = (Vector2){0.0f, 0.0f};
    int end_projectile_game = 0;
    int barIncreasing = 1;
	float rotation = 0.0f;
    float launchAngle = 0.0f;
    float launchSpeed = 0.0f;
    projectileOrb Orb;
    Orb.inAir = 0;
    Orb.position = rightEdgeMidpoint(cannonBody, cannonRotationPoint, rotation);
    int score = 0;
    float timer = 0.0f;
    int points = 0;
    int gameFinished = 0;
    int remainingObstacles = obstacleCount;
    int highScore = 0;
    int newHighScore = 0;
    if (FileExists("cannon_highscore.txt")){
        char *highScoreText = LoadFileText("cannon_highscore.txt");
        highScore = TextToInteger(highScoreText);
        UnloadFileText(highScoreText);
    }

	while (!WindowShouldClose() && !IsKeyPressed(KEY_ZERO))
	{
        float deltaTime = GetFrameTime();
        if (!gameFinished)
            timer += deltaTime;


		if (IsKeyDown(KEY_UP) && rotation > -70) rotation -= thetaSpeed*deltaTime;
		if (IsKeyDown(KEY_DOWN) && rotation < -1) rotation += thetaSpeed*deltaTime;
        if (launchSpeed >= maxSpeed) barIncreasing = 0;
        if (launchSpeed <= minSpeed) barIncreasing = 1;
        if (IsKeyDown(KEY_Z) && !Orb.inAir) 
            if (launchSpeed <= maxSpeed && barIncreasing) launchSpeed += speedSpeed*deltaTime;
            else if (launchSpeed >= minSpeed && !barIncreasing) launchSpeed -= speedSpeed*deltaTime;
        if (IsKeyReleased(KEY_Z) && !Orb.inAir) {
            PlaySound(releaseSound);
            Orb.inAir = 1;
            launchAngle = rotation;
            Orb.velocity.x = launchSpeed*cosf(launchAngle*DEG2RAD);
            Orb.velocity.y = launchSpeed*sinf(launchAngle*DEG2RAD) - gravity*deltaTime;
        }
        if (Orb.inAir){
            Orb.velocity.y += gravity*deltaTime;
            Orb.position.x += Orb.velocity.x * deltaTime;
            Orb.position.y += Orb.velocity.y *deltaTime;
            for (int i = 0; i <= obstacleCount-1; i++){
                if (obstacles[i].active && CheckCollisionCircleRec(Orb.position, projectileOrb_size, obstacles[i].rectangle)){
                    PlaySound(hitSound);
                    obstacles[i].active = 0;
                    Orb.inAir = 0;
                    launchSpeed = 0;
                    remainingObstacles--;
                    if (obstacles[i].velocity.x != 0 && obstacles[i].velocity.y != 0)
                        points = 300;
                    else if (obstacles[i].velocity.x != 0 || obstacles[i].velocity.y != 0)
                        points = 200;
                    else points = 100;
                    int timeBonus = 500 - (int)timer * 10;
                    score += points + timeBonus;
                }
            }

        }
        if (Orb.inAir && Orb.position.x > SCREEN_WIDTH + projectileOrb_size || Orb.position.y > SCREEN_HEIGHT*2/3 - projectileOrb_size){
            Orb.inAir = 0;
            launchSpeed = 0;
        }
        if(!Orb.inAir){
            speedBar.width = (100*launchSpeed/maxSpeed);
            Orb.position = rightEdgeMidpoint(cannonBody, cannonRotationPoint, rotation);
        }

        for (int i = 0; i <= obstacleCount-1; i++){
            if (obstacles[i].active && obstacles[i].type == 1){
                obstacles[i].rectangle.y += obstacles[i].velocity.y * deltaTime;
                obstacles[i].rectangle.x += obstacles[i].velocity.x * deltaTime;

                if (obstacles[i].rectangle.y < 100){
                    obstacles[i].rectangle.y = 100;
                    obstacles[i].velocity.y *= -1;
                }

                if (obstacles[i].rectangle.y + obstacles[i].rectangle.height > SCREEN_HEIGHT*2/3){
                    obstacles[i].rectangle.y = SCREEN_HEIGHT*2/3 - obstacles[i].rectangle.height;
                    obstacles[i].velocity.y *= -1;
                }

                if (obstacles[i].rectangle.x < obstacles[i].leftbound){
                    obstacles[i].rectangle.x = obstacles[i].leftbound;
                    obstacles[i].velocity.x *= -1;
                }

                if (obstacles[i].rectangle.x + obstacles[i].rectangle.width > obstacles[i].rightbound){
                    obstacles[i].rectangle.x = obstacles[i].rightbound - obstacles[i].rectangle.width;
                    obstacles[i].velocity.x *= -1;
                }
            }
        }
        if (remainingObstacles == 0){
            gameFinished = 1;
            if (score > highScore){
                highScore = score;
                newHighScore = 1;
                SaveFileText("cannon_highscore.txt", TextFormat("%d", highScore));
            }
        }
        if (!gameFinished){
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText(TextFormat("Obstacle Set: %d", obstacleSet), SCREEN_WIDTH - 180, 20, 20, WHITE);     
            DrawText(TextFormat("Score: %d", score), 20, 20, 20, WHITE);
            DrawText(TextFormat("Time: %d", (int)timer), 20, 45, 20, WHITE);   
            if (score > highScore)
                DrawText("New High Score!", 20, 70, 20, GREEN);
            else
                DrawText(TextFormat("High Score: %d", highScore), 20, 70, 20, WHITE);
            DrawRectangleRec(ground, GRAY);
            DrawCircleV(Orb.position, projectileOrb_size, RED);
            DrawRectangleRec(cannonBase, BROWN);
            for (int i = 0; i <= obstacleCount-1; i++){
                if (obstacles[i].active){
                    DrawRectangleRec(obstacles[i].rectangle, BLUE);
                }
            }
            if(!Orb.inAir)DrawRectangleRec(speedBar, GREEN);
            DrawRectanglePro(cannonBody, cannonRotationPoint, rotation, LIGHTGRAY);
            EndDrawing();
        }
        else{
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("GAME COMPLETE!", 300, 300, 70, WHITE);
            DrawText(TextFormat("Final Score: %d", score), 475, 390, 30, WHITE);
            DrawText(TextFormat("Final Time: %d seconds", (int)timer), 430, 430, 30, WHITE);
            if (newHighScore)
                DrawText("NEW HIGH SCORE!", 475, 470, 30, GREEN);
            else
                DrawText(TextFormat("High Score: %d", highScore), 475, 470, 30, WHITE);
            DrawText("Press R to restart", 485, 530, 25, WHITE);
            DrawText("Press ENTER to exit", 475, 565, 25, WHITE);
            EndDrawing();
            if (IsKeyPressed(KEY_R)){
                UnloadSound(releaseSound);
                UnloadSound(hitSound);
                startCannonThrow();
                return;
            }
            if (IsKeyPressed(KEY_ENTER)){
                break;
            }
        }

	}
    UnloadSound(releaseSound);
    UnloadSound(hitSound);
}
