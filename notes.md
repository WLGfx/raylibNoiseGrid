# rlNoiseGrid notes

## 28/01/26 - 15:12(GMT from now on)

- Add flight controls
- Add camera controls
- Add gradient filters for sphere and box shaping

I happened to remove the flight controls from V0.1 when I published V0.2 changes. I should add them back.

The aim is to get V0.3 with at least those listed above.

Hmm. I've just had an idea. Using three steps, generate, fill and drop I can creat a terrain that is more like a mountain range but, with caves and valleys.

1 Generate the noise data using a gradient from ground to empty sky
2 Fill in each section of data and select the smaller chunks.
3 Drop the chunks to create valleys and caves.

I should try this out.

Marching cubes is a must to implement.

