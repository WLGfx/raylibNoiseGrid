#include "inc/NoiseGrid/NoiseGrid.h"

void NoiseChunk::initialise(NoiseGrid *grid) {
    this->grid = grid;
    int data_total_size = grid->chunk.size.x * grid->chunk.size.y * grid->chunk.size.z;
    blocks.resize(data_total_size);
}

void NoiseChunk::set_block(int x, int y, int z, unsigned char value) {
    int index = x + y * grid->chunk.size.x + z * grid->chunk.size.x * grid->chunk.size.y;
    blocks[index] = value;
}

unsigned char NoiseChunk::get_block(int x, int y, int z) {
    int index = x + y * grid->chunk.size.x + z * grid->chunk.size.x * grid->chunk.size.y;
    return blocks[index];
}

bool NoiseChunk::is_block(int x, int y, int z) {
    if (x < 0 || x >= grid->chunk.size.x || y < 0 || y >= grid->chunk.size.y || z < 0 || z >= grid->chunk.size.z) { return false; }
    int index = x + y * grid->chunk.size.x + z * grid->chunk.size.x * grid->chunk.size.y;
    return blocks[index] > 0;
}

void NoiseChunk::generate_noise_block_data() {
    float grid_min_y = grid->bounds.start.y * grid->chunk.size.y;
    float grid_height = grid->chunk.size.y * grid->grid.size.y;
    for (int z = 0; z < grid->chunk.size.z; z++) {
        for (int y = 0; y < grid->chunk.size.y; y++) {
            for (int x = 0; x < grid->chunk.size.x; x++) 
            {
                float noisex = posi.x * grid->chunk.size.x + x;
                float noisey = posi.y * grid->chunk.size.y + y;
                float noisez = posi.z * grid->chunk.size.z + z;

                float value = grid->noise.GetNoise(noisex, noisey, noisez);
                float gradient = 1.0f - ((0.5f / grid_height) * (noisey - grid_min_y));
                gradient = fmaxf(0.0f, fminf(1.0f, gradient));

                value = value * gradient;

                float adjusted_threshold_start = grid->range.min + (1.0f - gradient) * (grid->range.max - grid->range.min);
                float adjusted_threshold_end = grid->range.max - (1.0f - gradient) * (grid->range.max - grid->range.min);

                if (value > adjusted_threshold_start && value < adjusted_threshold_end) {
                    set_block(x, y, z, 1);
                } else {
                    set_block(x, y, z, 0);
                }
                // This bit was deekseep ai's help
                /*// Get world position
                float worldX = posi.x * grid->chunk.size.x + x;
                float worldY = posi.y * grid->chunk.size.y + y;
                float worldZ = posi.z * grid->chunk.size.z + z;
                
                // 1. Calculate gradient value (1 at center, 0 at bounds)
                float distanceFromCenter = sqrt(
                    worldX * worldX + 
                    worldY * worldY + 
                    worldZ * worldZ
                );
                
                // Linear falloff
                float worldRadius = grid->chunk.size.x * grid->grid.size.x / 1.95f;
                float gradient = 1.0f - (distanceFromCenter / worldRadius);
                gradient = fmaxf(0.0f, fminf(1.0f, gradient));  // Clamp to 0-1
                
                // 2. Get noise value
                float noiseVal = grid->noise.GetNoise(worldX, worldY, worldZ);// / 2.0f + 1.0f;
                
                // 3. Adjust the noise threshold based on gradient
                // This is the key part - make the threshold higher near edges
                float baseThreshold = grid->range.min;  // Your original threshold
                float rangeWidth = grid->range.max - grid->range.min;
                
                // As gradient decreases (further from center), increase the threshold
                // This makes caves less likely at the edges
                float adjustedThreshold = baseThreshold + (1.0f - gradient) * rangeWidth;
                
                // Alternative: Scale the noise value instead
                float scaledNoise = noiseVal * gradient;
                
                // Option 1: Using adjusted threshold
                if (gradient > 0.01f && scaledNoise > grid->range.min && scaledNoise < grid->range.max) {
                    set_block(x, y, z, 1);
                } 
                // Option 2: Using gradient to blend between solid and cave
                else if (gradient < 0.01f) {
                    set_block(x, y, z, 1);  // Force solid near edges
                } 
                else {
                    set_block(x, y, z, 0);
                }*/
            }
        }
    }
}

void NoiseChunk::generate_instance_data() {
    transforms.clear();
    block_count = 0;
    for (int z = 0; z < grid->chunk.size.z; z++) {
        for (int y = 0; y < grid->chunk.size.y; y++) {
            for (int x = 0; x < grid->chunk.size.x; x++) {
                if (is_block(x, y, z)) {
                    block_count++; // include even if obscurred
                    bool obsx = is_block(x - 1, y, z) and is_block(x + 1, y, z);
                    bool obsy = is_block(x, y - 1, z) and is_block(x, y + 1, z);
                    bool obsz = is_block(x, y, z - 1) and is_block(x, y, z + 1);
                    bool obscurred = obsx and obsy and obsz;
                    if (!obscurred) { // only draw blocks that are not obscurred
                        Matrix transform = MatrixTranslate(
                                (float)(posi.x * grid->chunk.size.x + x) * grid->block_size.x,
                                (float)(posi.y * grid->chunk.size.y + y) * grid->block_size.y,
                                (float)(posi.z * grid->chunk.size.z + z) * grid->block_size.z
                            );
                        transforms.push_back(transform);
                    }
                }
            }
        }
    }
}

void NoiseChunk::generate() {
    generate_noise_block_data();
    generate_instance_data();
    processed = true;
}

/*
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

    //std::cout << "Processed: (" << position.x <<
    //    ", " << position.y << ", " << position.z << ") - Blocks: " <<
    //    blocks.size() << " - Transforms: " <<
    //    transforms.size() << "\n" << std::flush;

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
    blocks.resize(data_total_size);
}

void NoiseChunk::set_data(int x, int y, int z, float value)
{
    int index = x +
                y * grid->chunk_size.x +
                z * grid->chunk_size.x * grid->chunk_size.y;
    data[index] = value;
}

float NoiseChunk::get_data(int x, int y, int z)
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

    float noise_value = get_data(x, y, z);

    bool exists = noise_value > grid->noise.at(0).range_min &&
                  noise_value < grid->noise.at(0).range_max;

    return exists;
}

void NoiseChunk::set_block(int x, int y, int z, unsigned char value)
{
    int index = x +
                y * grid->chunk_size.x +
                z * grid->chunk_size.x * grid->chunk_size.y;
    blocks[index] = value;
}

unsigned char NoiseChunk::get_block(int x, int y, int z)
{
    int index = x +
                y * grid->chunk_size.x +
                z * grid->chunk_size.x * grid->chunk_size.y;
    return blocks[index];
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
                set_data(x, y, z, value);
            }
        }
    }
}

void NoiseChunk::generate_block_data()
{
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

                    set_block(x, y, z, !obscurred);
                }
                else {
                    set_block(x, y, z, 0);
                }
            }
        }
    }
}

void NoiseChunk::generate_instance_data()
{
    transforms.clear();

    for (int z = 0; z < grid->chunk_size.z; z++)
    {
        for (int y = 0; y < grid->chunk_size.y; y++)
        {
            for (int x = 0; x < grid->chunk_size.x; x++)
            {
                unsigned char block = get_block(x, y, z);

                if (block)
                {
                    transforms.push_back(
                        MatrixTranslate(
                            (float)(position.x * grid->chunk_size.x + x) * grid->block_size.x,
                            (float)(position.y * grid->chunk_size.y + y) * grid->block_size.y,
                            (float)(position.z * grid->chunk_size.z + z) * grid->block_size.z
                        )
                    );
                }
            }
        }
    }
}
*/