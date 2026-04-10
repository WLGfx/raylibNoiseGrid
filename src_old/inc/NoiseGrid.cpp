#include "NoiseGrid.h"

#include <raymath.h>

#include <iostream>

NoiseGrid::NoiseGrid() {
    // initialise threads

    unsigned int processor_count = std::thread::hardware_concurrency();
    if (processor_count == 0) processor_count = 1;

    while (processor_count-- > 0)
    {
        proccess_threads.emplace_back(process_thread, this);
    }

    noise.resize(1);
    noise.at(0).noise.SetNoiseType(FastNoise::SimplexFractal);
    noise.at(0).noise.SetFrequency(0.015);

    noise.at(0).noise.SetFractalType(FastNoise::RigidMulti);
    noise.at(0).noise.SetFractalOctaves(2);
    noise.at(0).noise.SetFractalLacunarity(0.5);
    noise.at(0).noise.SetFractalGain(0.5);
}

NoiseGrid::~NoiseGrid()
{
    stop();
}

void NoiseGrid::stop()
{
    chunks_process_mutex.lock();
    chunks_process_queue.clear();
    chunks_process_mutex.unlock();

    exit_threads = true;

    for (std::thread &thread : proccess_threads)
    {
        if (thread.joinable())
        {
            thread.join();
            std::cout << "Joined thread\n" << std::flush;
        }
    }
}

void NoiseGrid::update_noise_range()
{
    while (chunks_used.size())
    {
        NoiseChunk *chunk = chunks_used.back();

        if (chunk->ready)
        {
            chunks_used.pop_back();
            chunks_free.push_back(chunk);
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    for (NoiseChunk *chunk : chunks_free)
    {
        chunk->skip_noise = true;
        chunk->ready = false;

        chunks_process_mutex.lock();
        chunks_process_queue.push_back(chunk);
        chunks_used.push_back(chunk);
        chunks_process_mutex.unlock();
    }
    chunks_free.clear();
}

void NoiseGrid::update_new_noise()
{
    while (chunks_used.size())
    {
        NoiseChunk *chunk = chunks_used.back();

        if (chunk->ready)
        {
            chunks_used.pop_back();
            chunks_free.push_back(chunk);
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    for (NoiseChunk *chunk : chunks_free)
    {
        chunk->skip_noise = false;
        chunk->ready = false;

        chunks_process_mutex.lock();
        chunks_process_queue.push_back(chunk);
        chunks_used.push_back(chunk);
        chunks_process_mutex.unlock();
    }
    chunks_free.clear();
}

// check for dimension changes and handle cleanup and rebuild data
void NoiseGrid::update(Vector3 &follow)
{
    // convert world follow coords to grid position
    grid_origin =
    {
        (int) (follow.x / block_size.x / chunk_size.x),
        (int) (follow.y / block_size.y / chunk_size.y),
        (int) (follow.z / block_size.z / chunk_size.z)
    };


    if (grid_changed())
    {
        rebuild_chunks();
    }
    else if (chunks_changed())
    {
        rebuild_chunks();
    }

    if (check_bounds())
    {
        fill_bounds();
    }
}

bool NoiseGrid::grid_changed()
{
    if (grid_size.x != grid_size_old.x ||
        grid_size.y != grid_size_old.y ||
        grid_size.z != grid_size_old.z)
    {
        grid_size_old = grid_size;

        return true;
    }
    return false;
}

bool NoiseGrid::chunks_changed()
{
    if (chunk_size.x != chunk_size_old.x ||
        chunk_size.y != chunk_size_old.y ||
        chunk_size.z != chunk_size_old.z)
    {
        chunk_size_old = chunk_size;

        return true;
    }
    return false;
}

void NoiseGrid::rebuild_chunks()
{
    std::cout << "Rebuilding NoiseChunks data\n" << std::flush;

    //chunks_process_queue.clear();
    
    chunks_process_mutex.lock();
    if (chunks_process_queue.size()) abandon_flag = true;
    chunks_process_mutex.unlock();
    
    //while (abandon_flag) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }

    chunks_free.clear();

    while (chunks_used.size()) // wait for old threads to complete
    {
        NoiseChunk *chunk = chunks_used.back();
        if (chunk->ready)
        {
            chunks_used.pop_back();
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    int total = grid_size.x * grid_size.y * grid_size.z;

    chunks.clear();
    chunks.resize(total);

    for (NoiseChunk &chunk : chunks)
    {
        chunks_free.push_back(&chunk);
        chunk.initialise(this);
    }

    grid_origin_old.x = grid_origin.x - 1; // cause refresh
}

bool NoiseGrid::check_bounds()
{
    bool result = false;

    if (grid_origin.x != grid_origin_old.x ||
        grid_origin.y != grid_origin_old.y ||
        grid_origin.z != grid_origin_old.z)
    {
        bounds_start =
            {
                grid_origin.x - grid_size.x / 2,
                grid_origin.y - grid_size.y / 2,
                grid_origin.z - grid_size.z / 2
            };

        bounds_end =
            {
                bounds_start.x + grid_size.x - 1,
                bounds_start.y + grid_size.y - 1,
                bounds_start.z + grid_size.z - 1
            };

        std::cout << "New grid origin: (" << grid_origin.x << ", " <<
            grid_origin.y << ", " <<
            grid_origin.z << ") Bounds [ ("
                  << bounds_start.x << ", " <<
            bounds_start.y << ", " <<
            bounds_start.z << ") - ("
                  << bounds_end.x << ", " <<
            bounds_end.y << ", " <<
            bounds_end.z << ") ]\n" <<
            std::flush;

        result = true;
    }

    grid_origin_old = grid_origin;

    return result;
}

void NoiseGrid::fill_bounds()
{
    for (int z = bounds_start.z; z <= bounds_end.z; z++)
    {
        for (int y = bounds_start.y; y <= bounds_end.y; y++)
        {
            for (int x = bounds_start.x; x < bounds_end.x; x++)
            {
                // find if chunk doesn't exist in used_chunks
                NoiseChunk *curr = nullptr;

                int used_index = 0;
                int size = chunks_used.size();

                // gotta explain this bit a hell of a lot better
                while (used_index < size)
                {
                    curr = chunks_used[used_index];

                    if (curr->position.x == x &&
                        curr->position.y == y &&
                        curr->position.z == z)
                    {
                        break; // from the while loop
                    }

                    used_index++;
                }

                if (used_index == size && chunks_free.size())
                {
                    NoiseChunk *chunk = chunks_free.back();
                    chunks_free.pop_back();

                    chunk->position = {x, y, z};
                    chunk->ready = false;
                    chunks_used.push_back(chunk);

                    chunks_process_mutex.lock();
                    chunks_process_queue.push_back(chunk);
                    chunks_process_mutex.unlock();
                }
            }
        }
    }
}

// NB: raymath.h
// TODO: check if semi-colon should be in this piece on code
// Function: RMAPI void MatrixDecompose(...)
// As of version: Unknown (but belongs to raylib version 5.5)
// line no. 2563


//#define USE_CRAPPY_RENDERERERERER
//
void NoiseGrid::render()
{
    for (NoiseChunk *chunk : chunks_used)
    {
        if(chunk->ready && chunk->transforms.size())
        {
#ifdef USE_CRAPPY_RENDERERERERER
            for (Matrix &matrix : chunk->transforms)
            {
                DrawMesh(mesh, material, matrix);
            }
#else
            DrawMeshInstanced(mesh, material,
                chunk->transforms.data(),
                chunk->transforms.size());
#endif
        }
    }
}

//
void NoiseGrid::process_thread(NoiseGrid *grid)
{
    std::cout << "Process thread started...\n" << std::flush;

    while (!grid->exit_threads)
    {
        // is queue occupied?
        NoiseChunk *chunk = nullptr;
        grid->chunks_process_mutex.lock();

        if (grid->abandon_flag)
        {
            grid->chunks_process_queue.clear();
            grid->chunks_process_mutex.unlock();
            grid->abandon_flag = false;
        } 
        else if (grid->chunks_process_queue.size())
        {
            chunk = grid->chunks_process_queue.back();
            grid->chunks_process_queue.pop_back();

            grid->chunks_process_mutex.unlock();

            chunk->process();
        }
        else
        {
            grid->chunks_process_mutex.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    grid->chunks_process_queue.clear();
}
