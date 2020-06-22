
#include "render.h"
#include "lightning.h"

light_t light = {
    .direction = { 0, 0, 1}
};

uint32_t    lumos(uint32_t color, float luminosity)
{
    uint32_t a_mask = 0xFF000000;
    uint32_t r_mask = 0x00FF0000;
    uint32_t g_mask = 0x0000FF00;
    uint32_t b_mask = 0x000000FF;

    if (luminosity < 0)
        luminosity = 0;
    else if (luminosity > 1)
        luminosity = 1;

    uint32_t a = (color & a_mask);
    uint32_t r = (color & r_mask) * luminosity;
    uint32_t g = (color & g_mask) * luminosity;
    uint32_t b = (color & b_mask) * luminosity;

    return(a | (r & r_mask) | (g & g_mask) | (b & b_mask));
}

