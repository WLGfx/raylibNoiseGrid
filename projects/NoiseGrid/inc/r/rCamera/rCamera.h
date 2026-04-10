#ifndef RLCAMERA_H
#define RLCAMERA_H

#include "raylib.h"

/*typedef struct Camera3D {
    Vector3 position;       // Camera position
    Vector3 target;         // Camera target it looks-at
    Vector3 up;             // Camera up vector (rotation over its axis)
    float fovy;             // Camera field-of-view aperture in Y (degrees) in perspective, used as near plane height in world units in orthographic
    int projection;         // Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
} Camera3D;*/

#define RLCAMERA_CULL_DISTANCE_NEAR 0.01f
#define RLCAMERA_CULL_DISTANCE_FAR 10000.0f

struct rlCamera : public Camera {
    rlCamera(Vector3 position = {0, 0, 0}, 
            Vector3 target = {0, 0, 0}, 
            Vector3 up = {0, 1, 0}, 
            float fovy = 45.0f, 
            int projection = CAMERA_PERSPECTIVE) 
        : Camera{position, target, up, fovy, projection} {}
    //rlCamera(Camera camera) : Camera{camera} {}

    float orbit_speed = 1.0f;

    Vector3 get_forward();
    Vector3 get_up();
    Vector3 get_right();
    float get_distance();
    void set_distance(float distance);

    void orbit(float speed = 0.5f);
    void move_forward(float distance, bool world_plane = false);
    void move_up(float distance);
    void move_right(float distance, bool world_plane);
    void move_to_target(float delta);
    void yaw(float angle, bool rotate_around_target = false);
    void pitch(float angle, bool lock_view, bool rotate_around_target = false, bool rotate_up_vector = false);
    void roll(float angle);
    Matrix get_matrix();
    Matrix get_projection_matrix(float aspect);
    void update_pro(Vector3 movement, Vector3 rotation, float zoom);
};

#endif // RLCAMERA_H