#include "NoiseGrid.h"
#include <iostream>

NoiseChunk::NoiseChunk() {}

NoiseChunk::~NoiseChunk()
{
    data.clear();       // totally unnecessary
    blocks.clear();
    transforms.clear();
}

void NoiseChunk::process()
{
    if (!skip_noise) generate_noise_data();

    generate_block_data();
    generate_instance_data();

    std::cout << "Processed: (" << position.x <<
        ", " << position.y << ", " << position.z << ") - Blocks: " <<
        blocks.size() << " - Transforms: " <<
        transforms.size() << "\n" << std::flush;

    skip_noise = false; // reset noise generation
    ready = true;
}

void NoiseChunk::initialise(NoiseGrid *grid)
{
    this->grid = grid;
    int data_total_size = grid->chunk_size.x *
                          grid->chunk_size.y *
                          grid->chunk_size.z;
    data.resize(data_total_size);
}

void NoiseChunk::set_block(int x, int y, int z, float value)
{
    int index = x +
                y * grid->chunk_size.x +
                z * grid->chunk_size.x * grid->chunk_size.y;
    data[index] = value;
}

float NoiseChunk::get_block(int x, int y, int z)
{
    int index = x +
                y * grid->chunk_size.x +
                z * grid->chunk_size.x * grid->chunk_size.y;
    return data[index];
}

bool NoiseChunk::is_block(int x, int y, int z)
{
    if (x < 0 || x >= grid->chunk_size.x ||
        y < 0 || y >= grid->chunk_size.y ||
        z < 0 || z >= grid->chunk_size.z)
    {
        return false;
    }

    float noise_value = get_block(x, y, z);

    bool exists = noise_value > grid->noise.at(0).range_min &&
                  noise_value < grid->noise.at(0).range_max;

    return exists;
}

void NoiseChunk::generate_noise_data()
{
    for (int z = 0; z < grid->chunk_size.z; z++)
    {
        for (int y = 0; y < grid->chunk_size.y; y++)
        {
            for (int x = 0; x < grid->chunk_size.x; x++)
            {
                double noisex = (double)(position.x * grid->chunk_size.x + x);
                double noisey = (double)(position.y * grid->chunk_size.y + y);
                double noisez = (double)(position.z * grid->chunk_size.z + z);

                float value = grid->noise.at(0).noise.GetNoise(noisex, noisey, noisez);
                set_block(x, y, z, value);
            }
        }
    }
}

void NoiseChunk::generate_block_data()
{
    blocks.clear();

    for (int z = 0; z < grid->chunk_size.z; z++)
    {
        for (int y = 0; y < grid->chunk_size.y; y++)
        {
            for (int x = 0; x < grid->chunk_size.x; x++)
            {
                if (is_block(x, y, z))
                {
                    bool blkx = is_block(x - 1, y, z) && is_block(x + 1, y, z);
                    bool blky = is_block(x, y - 1, z) && is_block(x, y + 1, z);
                    bool blkz = is_block(x, y, z - 1) && is_block(x, y, z + 1);

                    bool obscurred = blkx && blky && blkz;

                    if (!obscurred)
                    {
                        blocks.push_back({x, y, z});
                    }
                }
            }
        }
    }
}

void NoiseChunk::generate_instance_data()
{
    transforms.clear();

    for (vec3i pos: blocks)
    {
        transforms.push_back(
            MatrixTranslate(
            (float)(position.x * grid->chunk_size.x + pos.x) * grid->block_size.x,
            (float)(position.y * grid->chunk_size.y + pos.y) * grid->block_size.y,
            (float)(position.z * grid->chunk_size.z + pos.z) * grid->block_size.z
        ));
    }
}
