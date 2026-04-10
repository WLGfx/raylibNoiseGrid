#include "NoiseGrid.h"
#include "raylib.h"
#include <iostream>

// 114/63 - 67 (95/68)

NoiseGrid::NoiseGrid() {
    int cpu_thread_count = std::thread::hardware_concurrency();
    if (cpu_thread_count == 0) cpu_thread_count = 1;
    cpu_thread_count = (cpu_thread_count + 1) / 2;
    for (int i = 0; i < cpu_thread_count; i++) {
        thread_busy.push_back(false);
        thread.emplace_back(thread_function, this, i);
    }
    std::cout << "Threads initialise: " << cpu_thread_count << std::endl;
}
NoiseGrid::~NoiseGrid() {
    _exit_thread = true;
    for (std::thread &thread : thread) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
void NoiseGrid::thread_function(NoiseGrid *grid, int index) {
    while (!grid->_exit_thread) {
        grid->thread_mutex.lock();
        if (grid->thread_queue.size()) {
            NoiseChunk *chunk = grid->thread_queue.back();
            grid->thread_busy[index] = true;
            grid->thread_queue.pop_back();
            grid->thread_mutex.unlock();
            chunk->generate();
            grid->thread_busy[index] = false;
        } else {
            grid->thread_mutex.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

void NoiseGrid::clear_thread_queue() {
    thread_mutex.lock();
    thread_queue.clear();
    thread_mutex.unlock();
    for (int i = 0; i < thread_busy.size(); i++) {
        while (thread_busy[i]) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
    }
}

void NoiseGrid::update(Vector3 position) {
    source_position = position;
    origin.pos.x = int(source_position.x / chunk.size.x / block_size.x);
    origin.pos.y = int(source_position.y / chunk.size.y / block_size.y);
    origin.pos.z = int(source_position.z / chunk.size.z / block_size.z);
    bool result = false;
    if (grid.size.x != grid.old.x || grid.size.y != grid.old.y || grid.size.z != grid.old.z) {
        grid.old = grid.size;
        result = true;
    }
    if (chunk.size.x != chunk.old.x || chunk.size.y != chunk.old.y || chunk.size.z != chunk.old.z) {
        chunk.old = chunk.size;
        result = true;
    }
    if (result) initialise_chunks();
    if (origin.pos.x != origin.old.x || origin.pos.y != origin.old.y || origin.pos.z != origin.old.z) {
        origin.old = origin.pos;
        bounds.start.x = origin.pos.x - grid.size.x / 2;
        bounds.start.y = origin.pos.y - grid.size.y / 2;
        bounds.start.z = origin.pos.z - grid.size.z / 2;
        bounds.end.x = bounds.start.x + grid.size.x - 1;
        bounds.end.y = bounds.start.y + grid.size.y - 1;
        bounds.end.z = bounds.start.z + grid.size.z - 1;
        fill_chunks();
    }
}

void NoiseGrid::initialise_chunks() {
    // TODO: initialise chunks
    clear_thread_queue();
    chunks_used.clear();
    chunks_free.clear();
    int chunk_total = grid.size.x * grid.size.y * grid.size.z;
    chunks.resize(chunk_total);
    for (int i = 0; i < chunk_total; i++) {
        chunks[i].initialise(this);
        chunks_free.push_back(&chunks[i]);
    }
    origin.old.x -= 1;
    std::cout << "Chunk count: " << chunk_total << std::endl;
}

void NoiseGrid::fill_chunks() {
    int index = chunks_used.size();
    while (index--) { // remove out of bounds chunks
        NoiseChunk* chunk = chunks_used[index];
        if (chunk->posi.x < bounds.start.x || chunk->posi.x > bounds.end.x ||
            chunk->posi.y < bounds.start.y || chunk->posi.y > bounds.end.y ||
            chunk->posi.z < bounds.start.z || chunk->posi.z > bounds.end.z) {
            chunks_used.erase(chunks_used.begin() + index);
            chunks_free.push_back(chunk);
        }
    }
    for (int z = bounds.start.z; z <= bounds.end.z; z++) {
        for (int y = bounds.start.y; y <= bounds.end.y; y++) {
            for (int x = bounds.start.x; x <= bounds.end.x; x++) {
                vec3i grid_pos = {x, y, z};
                bool result = false;
                for (NoiseChunk* chunk : chunks_used) {
                    if (grid_pos.x == chunk->posi.x && grid_pos.y == chunk->posi.y && grid_pos.z == chunk->posi.z) {
                        result = true;
                        break;
                    }
                }
                if (!result && chunks_free.size()) {
                    NoiseChunk* chunk = chunks_free.back();
                    chunks_free.pop_back();
                    chunks_used.push_back(chunk);
                    chunk->posi = grid_pos;
                    chunk->processed = false;
                    thread_mutex.lock();
                    thread_queue.push_back(chunk);
                    thread_mutex.unlock();
                }
            }
        }
    }
}

//#define USE_CRAPPY_RENDERERERERER
void NoiseGrid::render()
{
    for (NoiseChunk *_chunk : chunks_used)
    {
        if(_chunk->processed && _chunk->transforms.size())
        {
#ifdef USE_CRAPPY_RENDERERERERER
            for (Matrix &matrix : _chunk->transforms)
            {
                DrawMesh(mesh, material, matrix);
            }
#else
            DrawMeshInstanced(mesh, material,
                _chunk->transforms.data(),
                _chunk->transforms.size());
#endif
        }
        // TODO draw debug or selection boxes
        // also add options in the UI
        // ...

        // draw rectangle outline around chunk size and position account for block size

        Vector3 position = { _chunk->posi.x * chunk.size.x * block_size.x, _chunk->posi.y * chunk.size.y * block_size.y, _chunk->posi.z * chunk.size.z * block_size.z};
        Vector3 size = {chunk.size.x * block_size.x, chunk.size.y * block_size.y, chunk.size.z * block_size.z};
        DrawCubeWiresV(position + (size / 2.0f), size, GREEN);
    }
}

/*
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
        chunk->skip_noise = true; // set the flag to SKIP the noise generation
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
        chunk->skip_noise = false; // Allows for new noise generation on this chunk
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


    if (grid_changed() || chunks_changed())
    {
        rebuild_chunks(); // auto sets off the bounds refresh
    }

    if (check_bounds()) // sets the new start and end bounds
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

void NoiseGrid::rebuild_chunks() // NB: just have a look and think cos I've blanked
{
    std::cout << "Rebuilding NoiseChunks data\n" << std::flush;
    
    chunks_process_mutex.lock();
    if (chunks_process_queue.size() && !abandon_flag)
    {
        abandon_flag = true;
        chunks_process_mutex.unlock();
        while (abandon_flag) 
        { 
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); 
        }
    }
    else 
    {
        chunks_process_mutex.unlock();
    }

    chunks_free.clear();
    chunks_used.clear();

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
            grid->abandon_flag = false;
            
            grid->chunks_process_mutex.unlock();
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

void NoiseGrid::effect_drop()
{
    if (chunks_process_queue.size())
    {
        return;
    }
    
    for (int z = bounds_start.z; z <= bounds_end.z; z++)
    {
        for (int x = bounds_start.x; x <= bounds_end.x; x++)
        {
            std::vector<NoiseChunk*> _chunks;

            for (int y = bounds_start.y; y <= bounds_end.y; y++)
            {
                for (auto &chunk : chunks_used)
                {
                    if (chunk->position.x == x && chunk->position.y == y && chunk->position.z == z)
                    {
                        _chunks.push_back(chunk);
                        break;
                    }
                }
            }

            // now drop blocks
                
            for (int cz = 0; cz < chunk_size.z; cz++)
            {
                for (int cx = 0; cx < chunk_size.x; cx++)
                {
                    int y_index = 0;
                    int total_size = _chunks.size() * chunk_size.y;

                    std::cout << "Dropping y axis on [" << cx << ", " << cz << "]\n" << std::flush;

                    int block = get_sblock(_chunks, cx, y_index, cz);

                    while (y_index < total_size - 1 && block == 0) // climb over blocks
                    {
                        int y_pos = y_index + 1;
                        block = get_sblock(_chunks, cx, y_pos, cz);
                        y_index++;
                    }

                    
                }
            }

            for (auto &chunk : _chunks)
            {
                chunk->generate_instance_data();
            }
        }
    }
}

unsigned char NoiseGrid::get_sblock(std::vector<NoiseChunk*> &chunks, int x, int y, int z)
{
    int chunk_index = y / chunk_size.y;
    int block_index = y % chunk_size.y;

    return chunks[chunk_index]->get_block(x, block_index, z);
}

void NoiseGrid::set_sblock(std::vector<NoiseChunk*> &chunks, int x, int y, int z, unsigned char value)
{
    int chunk_index = y / chunk_size.y;
    int block_index = y % chunk_size.y;

    chunks[chunk_index]->set_block(x, block_index, z, value);
}
*/
