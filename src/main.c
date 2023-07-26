#include "raylib.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------

    const int screenWidth = 1200;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");

    // CAMERA
    //--------------------------------------------------------------------------------------

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };  // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    // SHADER
    //--------------------------------------------------------------------------------------

    // Load basic lighting shader
    Shader shader = LoadShader(TextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
                               TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));

    // Get some required shader locations
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    // NOTE: "matModel" location name is automatically assigned on shader loading, 
    // no need to get the location again if using that uniform name
    //shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");

    // Ambient light level (some basic lighting)
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

    // MODELS / TEXTURES
    //--------------------------------------------------------------------------------------

    Texture2D blackTexture = LoadTexture("resources/models/obj/black.png"); // Load model texture
    Texture2D redTexture = LoadTexture("resources/models/obj/red.png"); // Load model texture

    Model board = LoadModel("resources/models/obj/board.obj");                 // Load OBJ model
    board.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = blackTexture;            // Set model diffuse texture
    board.materials[0].shader = shader;
    Vector3 boardPos = { 0.0f, 0.0f, 0.0f };                        // Set model position

    Model piece = LoadModel("resources/models/obj/1.obj");                 // Load OBJ model
    piece.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = redTexture;     // Set model diffuse texture
    piece.materials[0].shader = shader;
    Vector3 piecePos = { 0.0f, 0.0f, 0.0f };                        // Set model position

    SetTargetFPS(60);

    // Create light
    Light lights[MAX_LIGHTS] = { 0 };
    lights[0] = CreateLight(LIGHT_POINT, (Vector3){ 0, 15, -15 }, (Vector3){ 0.0f, 0.0f, 0.0f }, WHITE, shader);

    // MAIN GAME LOOP
    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose()) {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_ORBITAL);

        // Update the shader with the camera view vector (points towards { 0.0f, 0.0f, 0.0f })
        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(shader, lights[i]);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            ClearBackground(BLACK);

            BeginMode3D(camera);
            {
                // Draw spheres to show where the lights are
                for (int i = 0; i < MAX_LIGHTS; i++) {
                    if (lights[i].enabled) DrawSphereEx(lights[i].position, 0.2f, 8, 8, lights[i].color);
                    else DrawSphereWires(lights[i].position, 0.2f, 8, 8, ColorAlpha(lights[i].color, 0.3f));
                }
                DrawModel(board, boardPos, 1.0f, WHITE);
                DrawModel(piece, piecePos, 1.0f, WHITE);
                DrawGrid(40, 1.0f);
            }
            EndMode3D();

            DrawText("Welcome to the third dimension!", 10, 40, 20, DARKGRAY);

            DrawFPS(10, 10);
        }
        EndDrawing();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(blackTexture);
    UnloadTexture(redTexture);
    UnloadModel(piece);
    UnloadModel(board);
    UnloadShader(shader);

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
