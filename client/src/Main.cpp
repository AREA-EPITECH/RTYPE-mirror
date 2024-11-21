#include <iostream>

#include "Main.hpp"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    int width = 800;
    int height = 450;
    InitWindow(width, height, "Voxels assets visualizer");
    Camera camera = { 0 };
    camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    std::string path = "assets/voxels";
    std::vector<std::string> vox_files;
    for (const auto & entry : std::filesystem::directory_iterator(path))
    {
        if (std::string file = entry.path().c_str(); file.find(".vox") != std::string::npos)
            vox_files.emplace_back(file);
    }
    size_t nb_vox = vox_files.size();
    Model models[nb_vox] = { 0 };
    for (int i = 0; i < nb_vox; i++)
    {
        double t0 = GetTime() * 1000.0;
        models[i] = LoadModel(vox_files[i].c_str());
        double t1 = GetTime() * 1000.0;
        TraceLog(LOG_WARNING, TextFormat("%s file loaded in %.3f ms", vox_files[i].c_str(),t1 - t0));

        auto [min, max] = GetModelBoundingBox(models[i]);
        Vector3 center = { 0 };
        center.x = min.x + ((max.x - min.x) / 2);
        center.z = min.z + ((max.z - min.z) / 2);

        Matrix matTranslate = MatrixTranslate(-center.x, 0, -center.z);
        models[i].transform = matTranslate;
    }

    size_t currentModel = 0;

    Shader shader = LoadShader("assets/voxels/shaders/voxel_lighting.vs", "assets/voxels/shaders/voxel_lighting.fs");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]) { 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

    for (int i = 0; i < nb_vox; i++)
    {
        Model m = models[i];
        for (int j = 0; j < m.materialCount; j++)
        {
            m.materials[j].shader = shader;
        }
    }

    Light lights[MAX_LIGHTS] = { 0 };
    lights[0] = CreateLight(LIGHT_POINT, (Vector3) { -20, 20, -20 }, Vector3Zero(), GRAY, shader);
    lights[1] = CreateLight(LIGHT_POINT, (Vector3) { 20, -20, 20 }, Vector3Zero(), GRAY, shader);
    lights[2] = CreateLight(LIGHT_POINT, (Vector3) { -20, 20, 20 }, Vector3Zero(), GRAY, shader);
    lights[3] = CreateLight(LIGHT_POINT, (Vector3) { 20, -20, -20 }, Vector3Zero(), GRAY, shader);

    SetTargetFPS(60);
    Vector3 modelpos = { 0 };
    Vector3 camerarot = { 0 };


    bool rotating = true;
    float rotationAngle = 0.01f;
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_UP))
            rotationAngle += 0.01f;
        if (IsKeyPressed(KEY_DOWN))
            rotationAngle -= 0.01f;
        if (IsKeyPressed(KEY_SPACE))
        {
            rotating = !rotating;
        }
        Matrix rotation = MatrixRotate((Vector3){ 0.0f, 1.0f, 0.0f }, rotationAngle);
        if (rotating)
            models[currentModel].transform = MatrixMultiply(models[currentModel].transform, rotation);
        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
        {
            const Vector2 mouseDelta = GetMouseDelta();
            camerarot.x = mouseDelta.x * 0.05f;
            camerarot.y = mouseDelta.y * 0.05f;
        }
        UpdateCameraPro(&camera,
            (Vector3) {
            (IsKeyDown(KEY_W)) * 0.1f -
                (IsKeyDown(KEY_S)) * 0.1f,
                (IsKeyDown(KEY_D)) * 0.1f -
                (IsKeyDown(KEY_A)) * 0.1f,
                0.0f
        },
            camerarot,
            GetMouseWheelMove() * -2.0f);

        if (IsKeyPressed(KEY_RIGHT))
            currentModel = (currentModel + 1) % nb_vox;

        if (IsKeyPressed(KEY_LEFT))
            if (currentModel != 0)
                currentModel = (currentModel - 1) % nb_vox;
            else
                currentModel = nb_vox - 1;
        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        for (int i = 0; i < MAX_LIGHTS; i++)
            UpdateLightValues(shader, lights[i]);
        BeginDrawing();

        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        DrawModel(models[currentModel], modelpos, 1.0f, WHITE);
        DrawGrid(10, 1.0);
        for (int i = 0; i < MAX_LIGHTS; i++)
        {
            if (lights[i].enabled) DrawSphereEx(lights[i].position, 0.2f, 8, 8, lights[i].color);
            else DrawSphereWires(lights[i].position, 0.2f, 8, 8, ColorAlpha(lights[i].color, 0.3f));
        }

        EndMode3D();
        EndDrawing();
    }
    CloseWindow();
    for (int i = 0; i < nb_vox; i++)
        UnloadModel(models[i]);
    return 0;
}
