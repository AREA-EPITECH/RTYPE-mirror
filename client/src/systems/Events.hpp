#pragma once

#include "iostream"
#include <vector>
#include <filesystem>
#include "raylib.h"
#include <string>
#include "raymath.h"

//Window

struct WindowOpenEvent {};

struct WindowCloseEvent {
    bool shouldClose = false;
};

struct WindowUpdateEvent {};


//Camera

struct InitCameraEvent {
    Vector3 position;
    Vector3 target;
    Vector3 up;
    float fovy;
    int projection;
};


//Models

struct InitModelEvent {};
