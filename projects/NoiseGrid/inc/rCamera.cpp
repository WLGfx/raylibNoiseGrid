#include "rCamera.h"
#include "raylib.h"
#include "raymath.h"

Vector3 rlCamera::get_forward() {
    return Vector3Normalize(Vector3Subtract(position, target));
}

Vector3 rlCamera::get_up() {
    return Vector3Normalize(up);
}

Vector3 rlCamera::get_right() {
    return Vector3Normalize(Vector3CrossProduct(get_forward(), get_up()));
}

float rlCamera::get_distance() {
    return Vector3Distance(position, target);
}

void rlCamera::set_distance(float distance) {
    float current_distance = get_distance();
    float scale = distance / current_distance;
    Vector3 view = Vector3Subtract(position, target);
    view = Vector3Scale(view, scale);
    position = Vector3Add(target, view);
}

void rlCamera::orbit(float speed) {
    orbit_speed = speed;
    Matrix rotation = MatrixRotate(get_up(), orbit_speed * GetFrameTime());
    Vector3 view = Vector3Subtract(position, target);
    view = Vector3Transform(view, rotation);
    position = Vector3Add(target, view);
}

void rlCamera::move_forward(float distance, bool world_plane) {
    Vector3 forward = get_forward();
    if (world_plane) {
        if      (fabsf(up.z) > 0.7071f) forward.z = 0;
        else if (fabsf(up.x) > 0.7071f) forward.x = 0;
        else    forward.y = 0;
        forward = Vector3Normalize(forward);
    }
    forward = Vector3Scale(position, distance);
    position = Vector3Add(position, forward);
    target = Vector3Add(target, forward);
}

void rlCamera::move_up(float distance) {
    Vector3 _up = Vector3Scale(up, distance);
    position = Vector3Add(position, _up);
    target = Vector3Add(target, _up);
}

void rlCamera::move_right(float distance, bool world_plane) {
    Vector3 right = get_right();
    if (world_plane) {
        if      (fabsf(up.z) > 0.7071f) right.z = 0;
        else if (fabsf(up.x) > 0.7071f) right.x = 0;
        else    right.y = 0;
        right = Vector3Normalize(right);
    }
    right = Vector3Scale(right, distance);
    position = Vector3Add(position, right);
    target = Vector3Add(target, right);
}

