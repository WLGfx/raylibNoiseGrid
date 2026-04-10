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

void rlCamera::move_to_target(float delta) {
    float distance = get_distance();
    distance += delta;
    if (distance <= 0) distance = 0.001f;
    Vector3 forward = get_forward();
    position = Vector3Add(target, Vector3Scale(forward, -distance));
}

void rlCamera::yaw(float angle, bool rotate_around_target) {
    Vector3 target_position = Vector3RotateByAxisAngle(Vector3Subtract(target, position), up, angle);
    if (rotate_around_target) {
        position = Vector3Subtract(target, target_position);
    } else {
        target = Vector3Add(position, target_position);
    }
}

void rlCamera::pitch(float angle, bool lock_view, bool rotate_around_target, bool rotate_up_vector) {
    
    Vector3 targetPosition = Vector3Subtract(target, position);

    if (lock_view)
    {
        // In these camera modes we clamp the Pitch angle
        // to allow only viewing straight up or down.

        // Clamp view up
        float maxAngleUp = Vector3Angle(up, targetPosition);
        maxAngleUp -= 0.001f; // avoid numerical errors
        if (angle > maxAngleUp) angle = maxAngleUp;

        // Clamp view down
        float maxAngleDown = Vector3Angle(Vector3Negate(up), targetPosition);
        maxAngleDown *= -1.0f; // downwards angle is negative
        maxAngleDown += 0.001f; // avoid numerical errors
        if (angle < maxAngleDown) angle = maxAngleDown;
    }

    targetPosition = Vector3RotateByAxisAngle(targetPosition, get_right(), angle);

    if (rotate_around_target)
    {
        // Move position relative to target
        position = Vector3Subtract(target, targetPosition);
    }
    else // Rotate around camera.position
    {
        // Move target relative to position
        target = Vector3Add(position, targetPosition);
    }

    if (rotate_up_vector)
    {
        // Rotate up direction around right axis
        up = Vector3RotateByAxisAngle(up, get_right(), angle);
    }
}

void rlCamera::roll(float angle) {
    up = Vector3RotateByAxisAngle(up, get_forward(), angle);
}

Matrix rlCamera::get_matrix() {
    return MatrixLookAt(position, target, up);
}

Matrix rlCamera::get_projection_matrix(float aspect) {
    if (projection == CAMERA_PERSPECTIVE)
    {
        return MatrixPerspective(fovy*DEG2RAD, aspect, RLCAMERA_CULL_DISTANCE_NEAR, RLCAMERA_CULL_DISTANCE_FAR);
    }
    else if (projection == CAMERA_ORTHOGRAPHIC)
    {
        float top = fovy/2.0f;
        float right = top*aspect;

        return MatrixOrtho(-right, right, -top, top, RLCAMERA_CULL_DISTANCE_NEAR, RLCAMERA_CULL_DISTANCE_FAR);
    }

    return MatrixIdentity();
}

void rlCamera::update_pro(Vector3 movement, Vector3 rotation, float zoom) {
    // Required values
    // movement.x - Move forward/backward
    // movement.y - Move right/left
    // movement.z - Move up/down
    // rotation.x - yaw
    // rotation.y - pitch
    // rotation.z - roll
    // zoom - Move towards target

    bool lockView = true;
    bool rotateAroundTarget = false;
    bool rotateUp = false;
    bool moveInWorldPlane = true;

    // Camera rotation
    pitch(rotation.y, lockView, rotateAroundTarget, rotateUp);
    yaw(rotation.x, rotateAroundTarget);
    roll(rotation.z);

    // Camera movement
    move_forward(movement.x, moveInWorldPlane);
    move_right(movement.y, moveInWorldPlane);
    move_up(movement.z);

    // Zoom target distance
    move_to_target(zoom);
}
// original source for these functions are in rcamera.h (raylib)
