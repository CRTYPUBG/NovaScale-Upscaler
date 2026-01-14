#ifndef NOVASCALE_API_H
#define NOVASCALE_API_H

#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

typedef enum {
    MODE_SPATIAL_FAST = 0,
    MODE_SPATIAL_BALANCED = 1,
    MODE_CNN_ULTRA = 2
} UpscaleMode;

typedef struct {
    UpscaleMode mode;
    float scale_factor;
    float sharpness;
    bool enable_temporal;
    bool show_fps;
} Config;

typedef struct {
    float frame_time_ms;
    float capture_time_ms;
    float upscale_time_ms;
    float present_time_ms;
    uint32_t fps;
} Stats;

EXPORT bool NovaScale_Initialize();
EXPORT void NovaScale_Shutdown();
EXPORT bool NovaScale_Start(Config config);
EXPORT void NovaScale_Stop();
EXPORT void NovaScale_UpdateConfig(Config config);
EXPORT Stats NovaScale_GetStats();

#endif // NOVASCALE_API_H
