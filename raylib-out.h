/**********************************************************************************************
*
*   raylib v5.0 - A simple and easy-to-use library to enjoy videogames programming (www.raylib.com)
*
*   FEATURES:
*       - NO external dependencies, all required libraries included with raylib
*       - Multiplatform: Windows, Linux, FreeBSD, OpenBSD, NetBSD, DragonFly,
*                        MacOS, Haiku, Android, Raspberry Pi, DRM native, HTML5.
*       - Written in plain C code (C99) in PascalCase/camelCase notation
*       - Hardware accelerated with OpenGL (1.1, 2.1, 3.3, 4.3 or ES2 - choose at compile)
*       - Unique OpenGL abstraction layer (usable as standalone module): [rlgl]
*       - Multiple RlFonts formats supported (TTF, XNA fonts, AngelCode fonts)
*       - Outstanding texture formats support, including compressed formats (DXT, ETC, ASTC)
*       - Full 3d support for 3d Shapes, RlModels, Billboards, Heightmaps and more!
*       - Flexible RlMaterials system, supporting classic maps and PBR maps
*       - Animated 3D models supported (skeletal bones animation) (IQM)
*       - RlShaders support, including RlModel shaders and Postprocessing shaders
*       - Powerful math module for Vector, RlMatrix and RlQuaternion operations: [raymath]
*       - Audio loading and playing with streaming support (WAV, OGG, MP3, FLAC, XM, MOD)
*       - VR stereo rendering with configurable HMD device parameters
*       - Bindings to multiple programming languages available!
*
*   NOTES:
*       - One default RlFont is loaded on (* rl_init_window )()->(* rl_load_font )Default() [core, text]
*       - One default RlTexture2D is loaded on rlglInit(), 1x1 white pixel R8G8B8A8 [rlgl] (OpenGL 3.3 or ES2)
*       - One default RlShader is loaded on rlglInit()->rl(* rl_load_shader )Default() [rlgl] (OpenGL 3.3 or ES2)
*       - One default RenderBatch is loaded on rlglInit()->rlLoadRenderBatch() [rlgl] (OpenGL 3.3 or ES2)
*
*   DEPENDENCIES (included):
*       [rcore] rglfw (Camilla Löwy - github.com/glfw/glfw) for window/context management and input (PLATFORM_DESKTOP)
*       [rlgl] glad (David Herberth - github.com/Dav1dde/glad) for OpenGL 3.3 extensions loading (PLATFORM_DESKTOP)
*       [raudio] miniaudio (David Reid - github.com/mackron/miniaudio) for audio device/context management
*
*   OPTIONAL DEPENDENCIES (included):
*       [rcore] msf_gif (Miles Fogle) for GIF recording
*       [rcore] sinfl (Micha Mettke) for DEFLATE decompression algorithm
*       [rcore] sdefl (Micha Mettke) for DEFLATE compression algorithm
*       [rtextures] stb_image (Sean Barret) for images loading (BMP, TGA, PNG, JPEG, HDR...)
*       [rtextures] stb_image_write (Sean Barret) for image writing (BMP, TGA, PNG, JPG)
*       [rtextures] stb_image_resize (Sean Barret) for image resizing algorithms
*       [rtext] stb_truetype (Sean Barret) for ttf fonts loading
*       [rtext] stb_rect_pack (Sean Barret) for rectangles packing
*       [rmodels] par_shapes (Philip Rideout) for parametric 3d shapes generation
*       [rmodels] tinyobj_loader_c (Syoyo Fujita) for models loading (OBJ, MTL)
*       [rmodels] cgltf (Johannes Kuhlmann) for models loading (glTF)
*       [rmodels] RlModel3D (bzt) for models loading (M3D, https://bztsrc.gitlab.io/model3d)
*       [raudio] dr_wav (David Reid) for WAV audio file loading
*       [raudio] dr_flac (David Reid) for FLAC audio file loading
*       [raudio] dr_mp3 (David Reid) for MP3 audio file loading
*       [raudio] stb_vorbis (Sean Barret) for OGG audio loading
*       [raudio] jar_xm (Joshua Reisenauer) for XM audio module loading
*       [raudio] jar_mod (Joshua Reisenauer) for MOD audio module loading
*
*
*   LICENSE: zlib/libpng
*
*   raylib is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
*
*   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef RAYLIB_H
#define RAYLIB_H

#include <stdarg.h>     // Required for: va_list - Only used by RlTraceLogCallback

#define RAYLIB_VERSION_MAJOR 5
#define RAYLIB_VERSION_MINOR 0
#define RAYLIB_VERSION_PATCH 0
#define RAYLIB_VERSION  "5.0"

// Function specifiers in case library is build/used as a shared library (Windows)
// NOTE: Microsoft specifiers to tell compiler that symbols are imported/exported from a .dll
#if defined(_WIN32)
    #if defined(BUILD_LIBTYPE_SHARED)
        #if defined(__TINYC__)
            #define __declspec(x) __attribute__((x))
        #endif
        #define RLAPI __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
    #elif defined(USE_LIBTYPE_SHARED)
        #define RLAPI __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
    #endif
#endif

#ifndef RLAPI
    #define RLAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

//----------------------------------------------------------------------------------
// Some basic Defines
//----------------------------------------------------------------------------------
#ifndef PI
    #define PI 3.14159265358979323846f
#endif
#ifndef DEG2RAD
    #define DEG2RAD (PI/180.0f)
#endif
#ifndef RAD2DEG
    #define RAD2DEG (180.0f/PI)
#endif

// Allow custom memory allocators
// NOTE: Require recompiling raylib sources
#ifndef RL_MALLOC
    #define RL_MALLOC(sz)       malloc(sz)
#endif
#ifndef RL_CALLOC
    #define RL_CALLOC(n,sz)     calloc(n,sz)
#endif
#ifndef RL_REALLOC
    #define RL_REALLOC(ptr,sz)  realloc(ptr,sz)
#endif
#ifndef RL_FREE
    #define RL_FREE(ptr)        free(ptr)
#endif

// NOTE: MSVC C++ compiler does not support compound literals (C99 feature)
// Plain structures in C++ (without constructors) can be initialized with { }
// This is called aggregate initialization (C++11 feature)
#if defined(__cplusplus)
    #define CLITERAL(type)      type
#else
    #define CLITERAL(type)      (type)
#endif

// Some compilers (mostly macos clang) default to C++98,
// where aggregate initialization can't be used
// So, give a more clear error stating how to fix this
#if !defined(_MSC_VER) && (defined(__cplusplus) && __cplusplus < 201103L)
    #error "C++11 or later is required. Add -std=c++11"
#endif

// NOTE: We set some defines with some data types declared by raylib
// Other modules (raymath, rlgl) also require some of those types, so,
// to be able to use those other modules as standalone (not depending on raylib)
// this defines are very useful for internal check and avoid type (re)definitions
#define RL_COLOR_TYPE
#define RL_RECTANGLE_TYPE
#define RL_VECTOR2_TYPE
#define RL_VECTOR3_TYPE
#define RL_VECTOR4_TYPE
#define RL_QUATERNION_TYPE
#define RL_MATRIX_TYPE

// Some Basic RlColors
// NOTE: Custom raylib color palette for amazing visuals on WHITE background
#define LIGHTGRAY  CLITERAL(RlColor){ 200, 200, 200, 255 }   // Light Gray
#define GRAY       CLITERAL(RlColor){ 130, 130, 130, 255 }   // Gray
#define DARKGRAY   CLITERAL(RlColor){ 80, 80, 80, 255 }      // Dark Gray
#define YELLOW     CLITERAL(RlColor){ 253, 249, 0, 255 }     // Yellow
#define GOLD       CLITERAL(RlColor){ 255, 203, 0, 255 }     // Gold
#define ORANGE     CLITERAL(RlColor){ 255, 161, 0, 255 }     // Orange
#define PINK       CLITERAL(RlColor){ 255, 109, 194, 255 }   // Pink
#define RED        CLITERAL(RlColor){ 230, 41, 55, 255 }     // Red
#define MAROON     CLITERAL(RlColor){ 190, 33, 55, 255 }     // Maroon
#define GREEN      CLITERAL(RlColor){ 0, 228, 48, 255 }      // Green
#define LIME       CLITERAL(RlColor){ 0, 158, 47, 255 }      // Lime
#define DARKGREEN  CLITERAL(RlColor){ 0, 117, 44, 255 }      // Dark Green
#define SKYBLUE    CLITERAL(RlColor){ 102, 191, 255, 255 }   // Sky Blue
#define BLUE       CLITERAL(RlColor){ 0, 121, 241, 255 }     // Blue
#define DARKBLUE   CLITERAL(RlColor){ 0, 82, 172, 255 }      // Dark Blue
#define PURPLE     CLITERAL(RlColor){ 200, 122, 255, 255 }   // Purple
#define VIOLET     CLITERAL(RlColor){ 135, 60, 190, 255 }    // Violet
#define DARKPURPLE CLITERAL(RlColor){ 112, 31, 126, 255 }    // Dark Purple
#define BEIGE      CLITERAL(RlColor){ 211, 176, 131, 255 }   // Beige
#define BROWN      CLITERAL(RlColor){ 127, 106, 79, 255 }    // Brown
#define DARKBROWN  CLITERAL(RlColor){ 76, 63, 47, 255 }      // Dark Brown

#define WHITE      CLITERAL(RlColor){ 255, 255, 255, 255 }   // White
#define BLACK      CLITERAL(RlColor){ 0, 0, 0, 255 }         // Black
#define BLANK      CLITERAL(RlColor){ 0, 0, 0, 0 }           // Blank (Transparent)
#define MAGENTA    CLITERAL(RlColor){ 255, 0, 255, 255 }     // Magenta
#define RAYWHITE   CLITERAL(RlColor){ 245, 245, 245, 255 }   // My own White (raylib logo)

//----------------------------------------------------------------------------------
// Structures Definition
//----------------------------------------------------------------------------------
// Boolean type
#if (defined(__STDC__) && __STDC_VERSION__ >= 199901L) || (defined(_MSC_VER) && _MSC_VER >= 1800)
    #include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool)
    typedef enum bool { false = 0, true = !false } bool;
    #define RL_BOOL_TYPE
#endif

// RlVector2, 2 components
typedef struct RlVector2 {
    float x;                // Vector x component
    float y;                // Vector y component
} RlVector2;

// RlVector3, 3 components
typedef struct RlVector3 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
} RlVector3;

// RlVector4, 4 components
typedef struct RlVector4 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
    float w;                // Vector w component
} RlVector4;

// RlQuaternion, 4 components (RlVector4 alias)
typedef RlVector4 RlQuaternion;

// RlMatrix, 4x4 components, column major, OpenGL style, right-handed
typedef struct RlMatrix {
    float m0, m4, m8, m12;  // RlMatrix first row (4 components)
    float m1, m5, m9, m13;  // RlMatrix second row (4 components)
    float m2, m6, m10, m14; // RlMatrix third row (4 components)
    float m3, m7, m11, m15; // RlMatrix fourth row (4 components)
} RlMatrix;

// RlColor, 4 components, R8G8B8A8 (32bit)
typedef struct RlColor {
    unsigned char r;        // RlColor red value
    unsigned char g;        // RlColor green value
    unsigned char b;        // RlColor blue value
    unsigned char a;        // RlColor alpha value
} RlColor;

// RlRectangle, 4 components
typedef struct RlRectangle {
    float x;                // RlRectangle top-left corner position x
    float y;                // RlRectangle top-left corner position y
    float width;            // RlRectangle width
    float height;           // RlRectangle height
} RlRectangle;

// RlImage, pixel data stored in CPU memory (RAM)
typedef struct RlImage {
    void *data;             // RlImage raw data
    int width;              // RlImage base width
    int height;             // RlImage base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (RlPixelFormat type)
} RlImage;

// RlTexture, tex data stored in GPU memory (VRAM)
typedef struct RlTexture {
    unsigned int id;        // OpenGL texture id
    int width;              // RlTexture base width
    int height;             // RlTexture base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (RlPixelFormat type)
} RlTexture;

// RlTexture2D, same as RlTexture
typedef RlTexture RlTexture2D;

// RlTextureCubemap, same as RlTexture
typedef RlTexture RlTextureCubemap;

// RlRenderTexture, fbo for texture rendering
typedef struct RlRenderTexture {
    unsigned int id;        // OpenGL framebuffer object id
    RlTexture texture;        // RlColor buffer attachment texture
    RlTexture depth;          // Depth buffer attachment texture
} RlRenderTexture;

// RlRenderTexture2D, same as RlRenderTexture
typedef RlRenderTexture RlRenderTexture2D;

// RlNPatchInfo, n-patch layout info
typedef struct RlNPatchInfo {
    RlRectangle source;       // RlTexture source rectangle
    int left;               // Left border offset
    int top;                // Top border offset
    int right;              // Right border offset
    int bottom;             // Bottom border offset
    int layout;             // Layout of the n-patch: 3x3, 1x3 or 3x1
} RlNPatchInfo;

// RlGlyphInfo, font characters glyphs info
typedef struct RlGlyphInfo {
    int value;              // Character value (Unicode)
    int offsetX;            // Character offset X when drawing
    int offsetY;            // Character offset Y when drawing
    int advanceX;           // Character advance position X
    RlImage image;            // Character image data
} RlGlyphInfo;

// RlFont, font texture and RlGlyphInfo array data
typedef struct RlFont {
    int baseSize;           // Base size (default chars height)
    int glyphCount;         // Number of glyph characters
    int glyphPadding;       // Padding around the glyph characters
    RlTexture2D texture;      // RlTexture atlas containing the glyphs
    RlRectangle *recs;        // RlRectangles in texture for the glyphs
    RlGlyphInfo *glyphs;      // Glyphs info data
} RlFont;

// RlCamera, defines position/orientation in 3d space
typedef struct RlCamera3D {
    RlVector3 position;       // RlCamera position
    RlVector3 target;         // RlCamera target it looks-at
    RlVector3 up;             // RlCamera up vector (rotation over its axis)
    float fovy;             // RlCamera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic
    int projection;         // RlCamera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
} RlCamera3D;

typedef RlCamera3D RlCamera;    // RlCamera type fallback, defaults to RlCamera3D

// RlCamera2D, defines position/orientation in 2d space
typedef struct RlCamera2D {
    RlVector2 offset;         // RlCamera offset (displacement from target)
    RlVector2 target;         // RlCamera target (rotation and zoom origin)
    float rotation;         // RlCamera rotation in degrees
    float zoom;             // RlCamera zoom (scaling), should be 1.0f by default
} RlCamera2D;

// RlMesh, vertex data and vao/vbo
typedef struct RlMesh {
    int vertexCount;        // Number of vertices stored in arrays
    int triangleCount;      // Number of triangles stored (indexed or not)

    // Vertex attributes data
    float *vertices;        // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
    float *texcoords;       // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
    float *texcoords2;      // Vertex texture second coordinates (UV - 2 components per vertex) (shader-location = 5)
    float *normals;         // Vertex normals (XYZ - 3 components per vertex) (shader-location = 2)
    float *tangents;        // Vertex tangents (XYZW - 4 components per vertex) (shader-location = 4)
    unsigned char *colors;      // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
    unsigned short *indices;    // Vertex indices (in case vertex data comes indexed)

    // Animation vertex data
    float *animVertices;    // Animated vertex positions (after bones transformations)
    float *animNormals;     // Animated normals (after bones transformations)
    unsigned char *boneIds; // Vertex bone ids, max 255 bone ids, up to 4 bones influence by vertex (skinning)
    float *boneWeights;     // Vertex bone weight, up to 4 bones influence by vertex (skinning)

    // OpenGL identifiers
    unsigned int vaoId;     // OpenGL Vertex Array Object id
    unsigned int *vboId;    // OpenGL Vertex Buffer Objects id (default vertex data)
} RlMesh;

// RlShader
typedef struct RlShader {
    unsigned int id;        // RlShader program id
    int *locs;              // RlShader locations array (RL_MAX_SHADER_LOCATIONS)
} RlShader;

// RlMaterialMap
typedef struct RlMaterialMap {
    RlTexture2D texture;      // RlMaterial map texture
    RlColor color;            // RlMaterial map color
    float value;            // RlMaterial map value
} RlMaterialMap;

// RlMaterial, includes shader and maps
typedef struct RlMaterial {
    RlShader shader;          // RlMaterial shader
    RlMaterialMap *maps;      // RlMaterial maps array (MAX_MATERIAL_MAPS)
    float params[4];        // RlMaterial generic parameters (if required)
} RlMaterial;

// RlTransform, vertex transformation data
typedef struct RlTransform {
    RlVector3 translation;    // Translation
    RlQuaternion rotation;    // Rotation
    RlVector3 scale;          // Scale
} RlTransform;

// Bone, skeletal animation bone
typedef struct RlBoneInfo {
    char name[32];          // Bone name
    int parent;             // Bone parent
} RlBoneInfo;

// RlModel, meshes, materials and animation data
typedef struct RlModel {
    RlMatrix transform;       // Local transform matrix

    int meshCount;          // Number of meshes
    int materialCount;      // Number of materials
    RlMesh *meshes;           // RlMeshes array
    RlMaterial *materials;    // RlMaterials array
    int *meshRlMaterial;      // RlMesh material number

    // Animation data
    int boneCount;          // Number of bones
    RlBoneInfo *bones;        // Bones information (skeleton)
    RlTransform *bindPose;    // Bones base transformation (pose)
} RlModel;

// RlModelAnimation
typedef struct RlModelAnimation {
    int boneCount;          // Number of bones
    int frameCount;         // Number of animation frames
    RlBoneInfo *bones;        // Bones information (skeleton)
    RlTransform **framePoses; // Poses array by frame
    char name[32];          // Animation name
} RlModelAnimation;

// RlRay, ray for raycasting
typedef struct RlRay {
    RlVector3 position;       // RlRay position (origin)
    RlVector3 direction;      // RlRay direction
} RlRay;

// RlRayCollision, ray hit information
typedef struct RlRayCollision {
    bool hit;               // Did the ray hit something?
    float distance;         // Distance to the nearest hit
    RlVector3 point;          // Point of the nearest hit
    RlVector3 normal;         // Surface normal of hit
} RlRayCollision;

// RlBoundingBox
typedef struct RlBoundingBox {
    RlVector3 min;            // Minimum vertex box-corner
    RlVector3 max;            // Maximum vertex box-corner
} RlBoundingBox;

// RlWave, audio wave data
typedef struct RlWave {
    unsigned int frameCount;    // Total number of frames (considering channels)
    unsigned int sampleRate;    // Frequency (samples per second)
    unsigned int sampleSize;    // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    unsigned int channels;      // Number of channels (1-mono, 2-stereo, ...)
    void *data;                 // Buffer data pointer
} RlWave;

// Opaque structs declaration
// NOTE: Actual structs are defined internally in raudio module
typedef struct RlrAudioBuffer RlrAudioBuffer;
typedef struct RlrAudioProcessor RlrAudioProcessor;

// RlAudioStream, custom audio stream
typedef struct RlAudioStream {
    RlrAudioBuffer *buffer;       // Pointer to internal data used by the audio system
    RlrAudioProcessor *processor; // Pointer to internal data processor, useful for audio effects

    unsigned int sampleRate;    // Frequency (samples per second)
    unsigned int sampleSize;    // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    unsigned int channels;      // Number of channels (1-mono, 2-stereo, ...)
} RlAudioStream;

// RlSound
typedef struct RlSound {
    RlAudioStream stream;         // Audio stream
    unsigned int frameCount;    // Total number of frames (considering channels)
} RlSound;

// RlMusic, audio stream, anything longer than ~10 seconds should be streamed
typedef struct RlMusic {
    RlAudioStream stream;         // Audio stream
    unsigned int frameCount;    // Total number of frames (considering channels)
    bool looping;               // RlMusic looping enable

    int ctxType;                // Type of music context (audio filetype)
    void *ctxData;              // Audio context data, depends on type
} RlMusic;

// RlVrDeviceInfo, Head-Mounted-Display device parameters
typedef struct RlVrDeviceInfo {
    int hResolution;                // Horizontal resolution in pixels
    int vResolution;                // Vertical resolution in pixels
    float hScreenSize;              // Horizontal size in meters
    float vScreenSize;              // Vertical size in meters
    float vScreenCenter;            // Screen center in meters
    float eyeToScreenDistance;      // Distance between eye and display in meters
    float lensSeparationDistance;   // Lens separation distance in meters
    float interpupillaryDistance;   // IPD (distance between pupils) in meters
    float lensDistortionValues[4];  // Lens distortion constant parameters
    float chromaAbCorrection[4];    // Chromatic aberration correction parameters
} RlVrDeviceInfo;

// RlVrStereoConfig, VR stereo rendering configuration for simulator
typedef struct RlVrStereoConfig {
    RlMatrix projection[2];           // VR projection matrices (per eye)
    RlMatrix viewOffset[2];           // VR view offset matrices (per eye)
    float leftLensCenter[2];        // VR left lens center
    float rightLensCenter[2];       // VR right lens center
    float leftScreenCenter[2];      // VR left screen center
    float rightScreenCenter[2];     // VR right screen center
    float scale[2];                 // VR distortion scale
    float scaleIn[2];               // VR distortion scale in
} RlVrStereoConfig;

// File path list
typedef struct RlFilePathList {
    unsigned int capacity;          // Filepaths max entries
    unsigned int count;             // Filepaths entries count
    char **paths;                   // Filepaths entries
} RlFilePathList;

// Automation event
typedef struct RlAutomationEvent {
    unsigned int frame;             // Event frame
    unsigned int type;              // Event type (RlAutomationEventType)
    int params[4];                  // Event parameters (if required)
} RlAutomationEvent;

// Automation event list
typedef struct RlAutomationEventList {
    unsigned int capacity;          // Events max entries (MAX_AUTOMATION_EVENTS)
    unsigned int count;             // Events entries count
    RlAutomationEvent *events;        // Events entries
} RlAutomationEventList;

//----------------------------------------------------------------------------------
// Enumerators Definition
//----------------------------------------------------------------------------------
// System/Window config flags
// NOTE: Every bit registers one state (use it with bit masks)
// By default all flags are set to 0
typedef enum {
    FLAG_VSYNC_HINT         = 0x00000040,   // Set to try enabling V-Sync on GPU
    FLAG_FULLSCREEN_MODE    = 0x00000002,   // Set to run program in fullscreen
    FLAG_WINDOW_RESIZABLE   = 0x00000004,   // Set to allow resizable window
    FLAG_WINDOW_UNDECORATED = 0x00000008,   // Set to disable window decoration (frame and buttons)
    FLAG_WINDOW_HIDDEN      = 0x00000080,   // Set to hide window
    FLAG_WINDOW_MINIMIZED   = 0x00000200,   // Set to minimize window (iconify)
    FLAG_WINDOW_MAXIMIZED   = 0x00000400,   // Set to maximize window (expanded to monitor)
    FLAG_WINDOW_UNFOCUSED   = 0x00000800,   // Set to window non focused
    FLAG_WINDOW_TOPMOST     = 0x00001000,   // Set to window always on top
    FLAG_WINDOW_ALWAYS_RUN  = 0x00000100,   // Set to allow windows running while minimized
    FLAG_WINDOW_TRANSPARENT = 0x00000010,   // Set to allow transparent framebuffer
    FLAG_WINDOW_HIGHDPI     = 0x00002000,   // Set to support HighDPI
    FLAG_WINDOW_MOUSE_PASSTHROUGH = 0x00004000, // Set to support mouse passthrough, only supported when FLAG_WINDOW_UNDECORATED
    FLAG_BORDERLESS_WINDOWED_MODE = 0x00008000, // Set to run program in borderless windowed mode
    FLAG_MSAA_4X_HINT       = 0x00000020,   // Set to try enabling MSAA 4X
    FLAG_INTERLACED_HINT    = 0x00010000    // Set to try enabling interlaced video format (for V3D)
} RlConfigFlags;

// Trace log level
// NOTE: Organized by priority level
typedef enum {
    LOG_ALL = 0,        // Display all logs
    LOG_TRACE,          // Trace logging, intended for internal use only
    LOG_DEBUG,          // Debug logging, used for internal debugging, it should be disabled on release builds
    LOG_INFO,           // Info logging, used for program execution info
    LOG_WARNING,        // Warning logging, used on recoverable failures
    LOG_ERROR,          // Error logging, used on unrecoverable failures
    LOG_FATAL,          // Fatal logging, used to abort program: exit(EXIT_FAILURE)
    LOG_NONE            // Disable logging
} RlTraceLogLevel;

// Keyboard keys (US keyboard layout)
// NOTE: Use (* rl_get_key_pressed )() to allow redefining
// required keys for alternative layouts
typedef enum {
    KEY_NULL            = 0,        // Key: NULL, used for no key pressed
    // Alphanumeric keys
    KEY_APOSTROPHE      = 39,       // Key: '
    KEY_COMMA           = 44,       // Key: ,
    KEY_MINUS           = 45,       // Key: -
    KEY_PERIOD          = 46,       // Key: .
    KEY_SLASH           = 47,       // Key: /
    KEY_ZERO            = 48,       // Key: 0
    KEY_ONE             = 49,       // Key: 1
    KEY_TWO             = 50,       // Key: 2
    KEY_THREE           = 51,       // Key: 3
    KEY_FOUR            = 52,       // Key: 4
    KEY_FIVE            = 53,       // Key: 5
    KEY_SIX             = 54,       // Key: 6
    KEY_SEVEN           = 55,       // Key: 7
    KEY_EIGHT           = 56,       // Key: 8
    KEY_NINE            = 57,       // Key: 9
    KEY_SEMICOLON       = 59,       // Key: ;
    KEY_EQUAL           = 61,       // Key: =
    KEY_A               = 65,       // Key: A | a
    KEY_B               = 66,       // Key: B | b
    KEY_C               = 67,       // Key: C | c
    KEY_D               = 68,       // Key: D | d
    KEY_E               = 69,       // Key: E | e
    KEY_F               = 70,       // Key: F | f
    KEY_G               = 71,       // Key: G | g
    KEY_H               = 72,       // Key: H | h
    KEY_I               = 73,       // Key: I | i
    KEY_J               = 74,       // Key: J | j
    KEY_K               = 75,       // Key: K | k
    KEY_L               = 76,       // Key: L | l
    KEY_M               = 77,       // Key: M | m
    KEY_N               = 78,       // Key: N | n
    KEY_O               = 79,       // Key: O | o
    KEY_P               = 80,       // Key: P | p
    KEY_Q               = 81,       // Key: Q | q
    KEY_R               = 82,       // Key: R | r
    KEY_S               = 83,       // Key: S | s
    KEY_T               = 84,       // Key: T | t
    KEY_U               = 85,       // Key: U | u
    KEY_V               = 86,       // Key: V | v
    KEY_W               = 87,       // Key: W | w
    KEY_X               = 88,       // Key: X | x
    KEY_Y               = 89,       // Key: Y | y
    KEY_Z               = 90,       // Key: Z | z
    KEY_LEFT_BRACKET    = 91,       // Key: [
    KEY_BACKSLASH       = 92,       // Key: '\'
    KEY_RIGHT_BRACKET   = 93,       // Key: ]
    KEY_GRAVE           = 96,       // Key: `
    // Function keys
    KEY_SPACE           = 32,       // Key: Space
    KEY_ESCAPE          = 256,      // Key: Esc
    KEY_ENTER           = 257,      // Key: Enter
    KEY_TAB             = 258,      // Key: Tab
    KEY_BACKSPACE       = 259,      // Key: Backspace
    KEY_INSERT          = 260,      // Key: Ins
    KEY_DELETE          = 261,      // Key: Del
    KEY_RIGHT           = 262,      // Key: Cursor right
    KEY_LEFT            = 263,      // Key: Cursor left
    KEY_DOWN            = 264,      // Key: Cursor down
    KEY_UP              = 265,      // Key: Cursor up
    KEY_PAGE_UP         = 266,      // Key: Page up
    KEY_PAGE_DOWN       = 267,      // Key: Page down
    KEY_HOME            = 268,      // Key: Home
    KEY_END             = 269,      // Key: End
    KEY_CAPS_LOCK       = 280,      // Key: Caps lock
    KEY_SCROLL_LOCK     = 281,      // Key: Scroll down
    KEY_NUM_LOCK        = 282,      // Key: Num lock
    KEY_PRINT_SCREEN    = 283,      // Key: Print screen
    KEY_PAUSE           = 284,      // Key: Pause
    KEY_F1              = 290,      // Key: F1
    KEY_F2              = 291,      // Key: F2
    KEY_F3              = 292,      // Key: F3
    KEY_F4              = 293,      // Key: F4
    KEY_F5              = 294,      // Key: F5
    KEY_F6              = 295,      // Key: F6
    KEY_F7              = 296,      // Key: F7
    KEY_F8              = 297,      // Key: F8
    KEY_F9              = 298,      // Key: F9
    KEY_F10             = 299,      // Key: F10
    KEY_F11             = 300,      // Key: F11
    KEY_F12             = 301,      // Key: F12
    KEY_LEFT_SHIFT      = 340,      // Key: Shift left
    KEY_LEFT_CONTROL    = 341,      // Key: Control left
    KEY_LEFT_ALT        = 342,      // Key: Alt left
    KEY_LEFT_SUPER      = 343,      // Key: Super left
    KEY_RIGHT_SHIFT     = 344,      // Key: Shift right
    KEY_RIGHT_CONTROL   = 345,      // Key: Control right
    KEY_RIGHT_ALT       = 346,      // Key: Alt right
    KEY_RIGHT_SUPER     = 347,      // Key: Super right
    KEY_KB_MENU         = 348,      // Key: KB menu
    // Keypad keys
    KEY_KP_0            = 320,      // Key: Keypad 0
    KEY_KP_1            = 321,      // Key: Keypad 1
    KEY_KP_2            = 322,      // Key: Keypad 2
    KEY_KP_3            = 323,      // Key: Keypad 3
    KEY_KP_4            = 324,      // Key: Keypad 4
    KEY_KP_5            = 325,      // Key: Keypad 5
    KEY_KP_6            = 326,      // Key: Keypad 6
    KEY_KP_7            = 327,      // Key: Keypad 7
    KEY_KP_8            = 328,      // Key: Keypad 8
    KEY_KP_9            = 329,      // Key: Keypad 9
    KEY_KP_DECIMAL      = 330,      // Key: Keypad .
    KEY_KP_DIVIDE       = 331,      // Key: Keypad /
    KEY_KP_MULTIPLY     = 332,      // Key: Keypad *
    KEY_KP_SUBTRACT     = 333,      // Key: Keypad -
    KEY_KP_ADD          = 334,      // Key: Keypad +
    KEY_KP_ENTER        = 335,      // Key: Keypad Enter
    KEY_KP_EQUAL        = 336,      // Key: Keypad =
    // Android key buttons
    KEY_BACK            = 4,        // Key: Android back button
    KEY_MENU            = 82,       // Key: Android menu button
    KEY_VOLUME_UP       = 24,       // Key: Android volume up button
    KEY_VOLUME_DOWN     = 25        // Key: Android volume down button
} RlKeyboardKey;

// Add backwards compatibility support for deprecated names
#define MOUSE_LEFT_BUTTON   MOUSE_BUTTON_LEFT
#define MOUSE_RIGHT_BUTTON  MOUSE_BUTTON_RIGHT
#define MOUSE_MIDDLE_BUTTON MOUSE_BUTTON_MIDDLE

// Mouse buttons
typedef enum {
    MOUSE_BUTTON_LEFT    = 0,       // Mouse button left
    MOUSE_BUTTON_RIGHT   = 1,       // Mouse button right
    MOUSE_BUTTON_MIDDLE  = 2,       // Mouse button middle (pressed wheel)
    MOUSE_BUTTON_SIDE    = 3,       // Mouse button side (advanced mouse device)
    MOUSE_BUTTON_EXTRA   = 4,       // Mouse button extra (advanced mouse device)
    MOUSE_BUTTON_FORWARD = 5,       // Mouse button forward (advanced mouse device)
    MOUSE_BUTTON_BACK    = 6,       // Mouse button back (advanced mouse device)
} RlMouseButton;

// Mouse cursor
typedef enum {
    MOUSE_CURSOR_DEFAULT       = 0,     // Default pointer shape
    MOUSE_CURSOR_ARROW         = 1,     // Arrow shape
    MOUSE_CURSOR_IBEAM         = 2,     // Text writing cursor shape
    MOUSE_CURSOR_CROSSHAIR     = 3,     // Cross shape
    MOUSE_CURSOR_POINTING_HAND = 4,     // Pointing hand cursor
    MOUSE_CURSOR_RESIZE_EW     = 5,     // Horizontal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NS     = 6,     // Vertical resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NWSE   = 7,     // Top-left to bottom-right diagonal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NESW   = 8,     // The top-right to bottom-left diagonal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_ALL    = 9,     // The omnidirectional resize/move cursor shape
    MOUSE_CURSOR_NOT_ALLOWED   = 10     // The operation-not-allowed shape
} RlMouseCursor;

// Gamepad buttons
typedef enum {
    GAMEPAD_BUTTON_UNKNOWN = 0,         // Unknown button, just for error checking
    GAMEPAD_BUTTON_LEFT_FACE_UP,        // Gamepad left DPAD up button
    GAMEPAD_BUTTON_LEFT_FACE_RIGHT,     // Gamepad left DPAD right button
    GAMEPAD_BUTTON_LEFT_FACE_DOWN,      // Gamepad left DPAD down button
    GAMEPAD_BUTTON_LEFT_FACE_LEFT,      // Gamepad left DPAD left button
    GAMEPAD_BUTTON_RIGHT_FACE_UP,       // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
    GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,    // Gamepad right button right (i.e. PS3: Square, Xbox: X)
    GAMEPAD_BUTTON_RIGHT_FACE_DOWN,     // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
    GAMEPAD_BUTTON_RIGHT_FACE_LEFT,     // Gamepad right button left (i.e. PS3: Circle, Xbox: B)
    GAMEPAD_BUTTON_LEFT_TRIGGER_1,      // Gamepad top/back trigger left (first), it could be a trailing button
    GAMEPAD_BUTTON_LEFT_TRIGGER_2,      // Gamepad top/back trigger left (second), it could be a trailing button
    GAMEPAD_BUTTON_RIGHT_TRIGGER_1,     // Gamepad top/back trigger right (one), it could be a trailing button
    GAMEPAD_BUTTON_RIGHT_TRIGGER_2,     // Gamepad top/back trigger right (second), it could be a trailing button
    GAMEPAD_BUTTON_MIDDLE_LEFT,         // Gamepad center buttons, left one (i.e. PS3: Select)
    GAMEPAD_BUTTON_MIDDLE,              // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
    GAMEPAD_BUTTON_MIDDLE_RIGHT,        // Gamepad center buttons, right one (i.e. PS3: Start)
    GAMEPAD_BUTTON_LEFT_THUMB,          // Gamepad joystick pressed button left
    GAMEPAD_BUTTON_RIGHT_THUMB          // Gamepad joystick pressed button right
} RlGamepadButton;

// Gamepad axis
typedef enum {
    GAMEPAD_AXIS_LEFT_X        = 0,     // Gamepad left stick X axis
    GAMEPAD_AXIS_LEFT_Y        = 1,     // Gamepad left stick Y axis
    GAMEPAD_AXIS_RIGHT_X       = 2,     // Gamepad right stick X axis
    GAMEPAD_AXIS_RIGHT_Y       = 3,     // Gamepad right stick Y axis
    GAMEPAD_AXIS_LEFT_TRIGGER  = 4,     // Gamepad back trigger left, pressure level: [1..-1]
    GAMEPAD_AXIS_RIGHT_TRIGGER = 5      // Gamepad back trigger right, pressure level: [1..-1]
} RlGamepadAxis;

// RlMaterial map index
typedef enum {
    MATERIAL_MAP_ALBEDO = 0,        // Albedo material (same as: MATERIAL_MAP_DIFFUSE)
    MATERIAL_MAP_METALNESS,         // Metalness material (same as: MATERIAL_MAP_SPECULAR)
    MATERIAL_MAP_NORMAL,            // Normal material
    MATERIAL_MAP_ROUGHNESS,         // Roughness material
    MATERIAL_MAP_OCCLUSION,         // Ambient occlusion material
    MATERIAL_MAP_EMISSION,          // Emission material
    MATERIAL_MAP_HEIGHT,            // Heightmap material
    MATERIAL_MAP_CUBEMAP,           // Cubemap material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    MATERIAL_MAP_IRRADIANCE,        // Irradiance material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    MATERIAL_MAP_PREFILTER,         // Prefilter material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    MATERIAL_MAP_BRDF               // Brdf material
} RlMaterialMapIndex;

#define MATERIAL_MAP_DIFFUSE      MATERIAL_MAP_ALBEDO
#define MATERIAL_MAP_SPECULAR     MATERIAL_MAP_METALNESS

// RlShader location index
typedef enum {
    SHADER_LOC_VERTEX_POSITION = 0, // RlShader location: vertex attribute: position
    SHADER_LOC_VERTEX_TEXCOORD01,   // RlShader location: vertex attribute: texcoord01
    SHADER_LOC_VERTEX_TEXCOORD02,   // RlShader location: vertex attribute: texcoord02
    SHADER_LOC_VERTEX_NORMAL,       // RlShader location: vertex attribute: normal
    SHADER_LOC_VERTEX_TANGENT,      // RlShader location: vertex attribute: tangent
    SHADER_LOC_VERTEX_COLOR,        // RlShader location: vertex attribute: color
    SHADER_LOC_MATRIX_MVP,          // RlShader location: matrix uniform: model-view-projection
    SHADER_LOC_MATRIX_VIEW,         // RlShader location: matrix uniform: view (camera transform)
    SHADER_LOC_MATRIX_PROJECTION,   // RlShader location: matrix uniform: projection
    SHADER_LOC_MATRIX_MODEL,        // RlShader location: matrix uniform: model (transform)
    SHADER_LOC_MATRIX_NORMAL,       // RlShader location: matrix uniform: normal
    SHADER_LOC_VECTOR_VIEW,         // RlShader location: vector uniform: view
    SHADER_LOC_COLOR_DIFFUSE,       // RlShader location: vector uniform: diffuse color
    SHADER_LOC_COLOR_SPECULAR,      // RlShader location: vector uniform: specular color
    SHADER_LOC_COLOR_AMBIENT,       // RlShader location: vector uniform: ambient color
    SHADER_LOC_MAP_ALBEDO,          // RlShader location: sampler2d texture: albedo (same as: SHADER_LOC_MAP_DIFFUSE)
    SHADER_LOC_MAP_METALNESS,       // RlShader location: sampler2d texture: metalness (same as: SHADER_LOC_MAP_SPECULAR)
    SHADER_LOC_MAP_NORMAL,          // RlShader location: sampler2d texture: normal
    SHADER_LOC_MAP_ROUGHNESS,       // RlShader location: sampler2d texture: roughness
    SHADER_LOC_MAP_OCCLUSION,       // RlShader location: sampler2d texture: occlusion
    SHADER_LOC_MAP_EMISSION,        // RlShader location: sampler2d texture: emission
    SHADER_LOC_MAP_HEIGHT,          // RlShader location: sampler2d texture: height
    SHADER_LOC_MAP_CUBEMAP,         // RlShader location: samplerCube texture: cubemap
    SHADER_LOC_MAP_IRRADIANCE,      // RlShader location: samplerCube texture: irradiance
    SHADER_LOC_MAP_PREFILTER,       // RlShader location: samplerCube texture: prefilter
    SHADER_LOC_MAP_BRDF             // RlShader location: sampler2d texture: brdf
} RlShaderLocationIndex;

#define SHADER_LOC_MAP_DIFFUSE      SHADER_LOC_MAP_ALBEDO
#define SHADER_LOC_MAP_SPECULAR     SHADER_LOC_MAP_METALNESS

// RlShader uniform data type
typedef enum {
    SHADER_UNIFORM_FLOAT = 0,       // RlShader uniform type: float
    SHADER_UNIFORM_VEC2,            // RlShader uniform type: vec2 (2 float)
    SHADER_UNIFORM_VEC3,            // RlShader uniform type: vec3 (3 float)
    SHADER_UNIFORM_VEC4,            // RlShader uniform type: vec4 (4 float)
    SHADER_UNIFORM_INT,             // RlShader uniform type: int
    SHADER_UNIFORM_IVEC2,           // RlShader uniform type: ivec2 (2 int)
    SHADER_UNIFORM_IVEC3,           // RlShader uniform type: ivec3 (3 int)
    SHADER_UNIFORM_IVEC4,           // RlShader uniform type: ivec4 (4 int)
    SHADER_UNIFORM_SAMPLER2D        // RlShader uniform type: sampler2d
} RlShaderUniformDataType;

// RlShader attribute data types
typedef enum {
    SHADER_ATTRIB_FLOAT = 0,        // RlShader attribute type: float
    SHADER_ATTRIB_VEC2,             // RlShader attribute type: vec2 (2 float)
    SHADER_ATTRIB_VEC3,             // RlShader attribute type: vec3 (3 float)
    SHADER_ATTRIB_VEC4              // RlShader attribute type: vec4 (4 float)
} RlShaderAttributeDataType;

// Pixel formats
// NOTE: Support depends on OpenGL version and platform
typedef enum {
    PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = 1, // 8 bit per pixel (no alpha)
    PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,    // 8*2 bpp (2 channels)
    PIXELFORMAT_UNCOMPRESSED_R5G6B5,        // 16 bpp
    PIXELFORMAT_UNCOMPRESSED_R8G8B8,        // 24 bpp
    PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,      // 16 bpp (1 bit alpha)
    PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,      // 16 bpp (4 bit alpha)
    PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,      // 32 bpp
    PIXELFORMAT_UNCOMPRESSED_R32,           // 32 bpp (1 channel - float)
    PIXELFORMAT_UNCOMPRESSED_R32G32B32,     // 32*3 bpp (3 channels - float)
    PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,  // 32*4 bpp (4 channels - float)
    PIXELFORMAT_UNCOMPRESSED_R16,           // 16 bpp (1 channel - half float)
    PIXELFORMAT_UNCOMPRESSED_R16G16B16,     // 16*3 bpp (3 channels - half float)
    PIXELFORMAT_UNCOMPRESSED_R16G16B16A16,  // 16*4 bpp (4 channels - half float)
    PIXELFORMAT_COMPRESSED_DXT1_RGB,        // 4 bpp (no alpha)
    PIXELFORMAT_COMPRESSED_DXT1_RGBA,       // 4 bpp (1 bit alpha)
    PIXELFORMAT_COMPRESSED_DXT3_RGBA,       // 8 bpp
    PIXELFORMAT_COMPRESSED_DXT5_RGBA,       // 8 bpp
    PIXELFORMAT_COMPRESSED_ETC1_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_ETC2_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA,   // 8 bpp
    PIXELFORMAT_COMPRESSED_PVRT_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_PVRT_RGBA,       // 4 bpp
    PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA,   // 8 bpp
    PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA    // 2 bpp
} RlPixelFormat;

// RlTexture parameters: filter mode
// NOTE 1: Filtering considers mipmaps if available in the texture
// NOTE 2: Filter is accordingly set for minification and magnification
typedef enum {
    TEXTURE_FILTER_POINT = 0,               // No filter, just pixel approximation
    TEXTURE_FILTER_BILINEAR,                // Linear filtering
    TEXTURE_FILTER_TRILINEAR,               // Trilinear filtering (linear with mipmaps)
    TEXTURE_FILTER_ANISOTROPIC_4X,          // Anisotropic filtering 4x
    TEXTURE_FILTER_ANISOTROPIC_8X,          // Anisotropic filtering 8x
    TEXTURE_FILTER_ANISOTROPIC_16X,         // Anisotropic filtering 16x
} RlTextureFilter;

// RlTexture parameters: wrap mode
typedef enum {
    TEXTURE_WRAP_REPEAT = 0,                // Repeats texture in tiled mode
    TEXTURE_WRAP_CLAMP,                     // Clamps texture to edge pixel in tiled mode
    TEXTURE_WRAP_MIRROR_REPEAT,             // Mirrors and repeats the texture in tiled mode
    TEXTURE_WRAP_MIRROR_CLAMP               // Mirrors and clamps to border the texture in tiled mode
} RlTextureWrap;

// Cubemap layouts
typedef enum {
    CUBEMAP_LAYOUT_AUTO_DETECT = 0,         // Automatically detect layout type
    CUBEMAP_LAYOUT_LINE_VERTICAL,           // Layout is defined by a vertical line with faces
    CUBEMAP_LAYOUT_LINE_HORIZONTAL,         // Layout is defined by a horizontal line with faces
    CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR,     // Layout is defined by a 3x4 cross with cubemap faces
    CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE,     // Layout is defined by a 4x3 cross with cubemap faces
    CUBEMAP_LAYOUT_PANORAMA                 // Layout is defined by a panorama image (equirrectangular map)
} RlCubemapLayout;

// RlFont type, defines generation method
typedef enum {
    FONT_DEFAULT = 0,               // Default font generation, anti-aliased
    FONT_BITMAP,                    // Bitmap font generation, no anti-aliasing
    FONT_SDF                        // SDF font generation, requires external shader
} RlFontType;

// RlColor blending modes (pre-defined)
typedef enum {
    BLEND_ALPHA = 0,                // Blend textures considering alpha (default)
    BLEND_ADDITIVE,                 // Blend textures adding colors
    BLEND_MULTIPLIED,               // Blend textures multiplying colors
    BLEND_ADD_COLORS,               // Blend textures adding colors (alternative)
    BLEND_SUBTRACT_COLORS,          // Blend textures subtracting colors (alternative)
    BLEND_ALPHA_PREMULTIPLY,        // Blend premultiplied textures considering alpha
    BLEND_CUSTOM,                   // Blend textures using custom src/dst factors (use rlSetBlendFactors())
    BLEND_CUSTOM_SEPARATE           // Blend textures using custom rgb/alpha separate src/dst factors (use rlSetBlendFactorsSeparate())
} RlBlendMode;

// RlGesture
// NOTE: Provided as bit-wise flags to enable only desired gestures
typedef enum {
    GESTURE_NONE        = 0,        // No gesture
    GESTURE_TAP         = 1,        // Tap gesture
    GESTURE_DOUBLETAP   = 2,        // Double tap gesture
    GESTURE_HOLD        = 4,        // Hold gesture
    GESTURE_DRAG        = 8,        // Drag gesture
    GESTURE_SWIPE_RIGHT = 16,       // Swipe right gesture
    GESTURE_SWIPE_LEFT  = 32,       // Swipe left gesture
    GESTURE_SWIPE_UP    = 64,       // Swipe up gesture
    GESTURE_SWIPE_DOWN  = 128,      // Swipe down gesture
    GESTURE_PINCH_IN    = 256,      // Pinch in gesture
    GESTURE_PINCH_OUT   = 512       // Pinch out gesture
} RlGesture;

// RlCamera system modes
typedef enum {
    CAMERA_CUSTOM = 0,              // Custom camera
    CAMERA_FREE,                    // Free camera
    CAMERA_ORBITAL,                 // Orbital camera
    CAMERA_FIRST_PERSON,            // First person camera
    CAMERA_THIRD_PERSON             // Third person camera
} RlCameraMode;

// RlCamera projection
typedef enum {
    CAMERA_PERSPECTIVE = 0,         // Perspective projection
    CAMERA_ORTHOGRAPHIC             // Orthographic projection
} RlCameraProjection;

// N-patch layout
typedef enum {
    NPATCH_NINE_PATCH = 0,          // Npatch layout: 3x3 tiles
    NPATCH_THREE_PATCH_VERTICAL,    // Npatch layout: 1x3 tiles
    NPATCH_THREE_PATCH_HORIZONTAL   // Npatch layout: 3x1 tiles
} RlNPatchLayout;

// Callbacks to hook some internal functions
// WARNING: These callbacks are intended for advance users
typedef void (*RlTraceLogCallback)(int logLevel, const char *text, va_list args);  // Logging: Redirect trace log messages
typedef unsigned char *(*RlLoadFileDataCallback)(const char *fileName, int *dataSize);    // FileIO: Load binary data
typedef bool (*RlSaveFileDataCallback)(const char *fileName, void *data, int dataSize);   // FileIO: Save binary data
typedef char *(*RlLoadFileTextCallback)(const char *fileName);            // FileIO: Load text data
typedef bool (*RlSaveFileTextCallback)(const char *fileName, char *text); // FileIO: Save text data

//------------------------------------------------------------------------------------
// Global Variables Definition
//------------------------------------------------------------------------------------
// It's lonely here...

//------------------------------------------------------------------------------------
// Window and Graphics Device Functions (Module: core)
//------------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

// Window-related functions
RLAPI void (* rl_init_window )(int width, int height, const char *title);  // Initialize window and OpenGL context
RLAPI void (* rl_close_window )(void);                                     // Close window and unload OpenGL context
RLAPI bool (* rl_window_should_close )(void);                               // Check if application should close (KEY_ESCAPE pressed or windows close icon clicked)
RLAPI bool (* rl_is_window_ready )(void);                                   // Check if window has been initialized successfully
RLAPI bool (* rl_is_window_fullscreen )(void);                              // Check if window is currently fullscreen
RLAPI bool (* rl_is_window_hidden )(void);                                  // Check if window is currently hidden (only PLATFORM_DESKTOP)
RLAPI bool (* rl_is_window_minimized )(void);                               // Check if window is currently minimized (only PLATFORM_DESKTOP)
RLAPI bool (* rl_is_window_maximized )(void);                               // Check if window is currently maximized (only PLATFORM_DESKTOP)
RLAPI bool (* rl_is_window_focused )(void);                                 // Check if window is currently focused (only PLATFORM_DESKTOP)
RLAPI bool (* rl_is_window_resized )(void);                                 // Check if window has been resized last frame
RLAPI bool (* rl_is_window_state )(unsigned int flag);                      // Check if one specific window flag is enabled
RLAPI void (* rl_set_window_state )(unsigned int flags);                    // Set window configuration state using flags (only PLATFORM_DESKTOP)
RLAPI void (* rl_clear_window_state )(unsigned int flags);                  // Clear window configuration state flags
RLAPI void (* rl_toggle_fullscreen )(void);                                // Toggle window state: fullscreen/windowed (only PLATFORM_DESKTOP)
RLAPI void (* rl_toggle_borderless_windowed )(void);                        // Toggle window state: borderless windowed (only PLATFORM_DESKTOP)
RLAPI void (* rl_maximize_window )(void);                                  // Set window state: maximized, if resizable (only PLATFORM_DESKTOP)
RLAPI void (* rl_minimize_window )(void);                                  // Set window state: minimized, if resizable (only PLATFORM_DESKTOP)
RLAPI void (* rl_restore_window )(void);                                   // Set window state: not minimized/maximized (only PLATFORM_DESKTOP)
RLAPI void (* rl_set_window_icon )(RlImage image);                            // Set icon for window (single image, RGBA 32bit, only PLATFORM_DESKTOP)
RLAPI void (* rl_set_window_icons )(RlImage *images, int count);              // Set icon for window (multiple images, RGBA 32bit, only PLATFORM_DESKTOP)
RLAPI void (* rl_set_window_title )(const char *title);                     // Set title for window (only PLATFORM_DESKTOP and PLATFORM_WEB)
RLAPI void (* rl_set_window_position )(int x, int y);                       // Set window position on screen (only PLATFORM_DESKTOP)
RLAPI void (* rl_set_window_monitor )(int monitor);                         // Set monitor for the current window
RLAPI void (* rl_set_window_min_size )(int width, int height);               // Set window minimum dimensions (for FLAG_WINDOW_RESIZABLE)
RLAPI void (* rl_set_window_max_size )(int width, int height);               // Set window maximum dimensions (for FLAG_WINDOW_RESIZABLE)
RLAPI void (* rl_set_window_size )(int width, int height);                  // Set window dimensions
RLAPI void (* rl_set_window_opacity )(float opacity);                       // Set window opacity [0.0f..1.0f] (only PLATFORM_DESKTOP)
RLAPI void (* rl_set_window_focused )(void);                                // Set window focused (only PLATFORM_DESKTOP)
RLAPI void *(* rl_get_window_handle )(void);                                // Get native window handle
RLAPI int (* rl_get_screen_width )(void);                                   // Get current screen width
RLAPI int (* rl_get_screen_height )(void);                                  // Get current screen height
RLAPI int (* rl_get_render_width )(void);                                   // Get current render width (it considers HiDPI)
RLAPI int (* rl_get_render_height )(void);                                  // Get current render height (it considers HiDPI)
RLAPI int (* rl_get_monitor_count )(void);                                  // Get number of connected monitors
RLAPI int (* rl_get_current_monitor )(void);                                // Get current connected monitor
RLAPI RlVector2 (* rl_get_monitor_position )(int monitor);                    // Get specified monitor position
RLAPI int (* rl_get_monitor_width )(int monitor);                           // Get specified monitor width (current video mode used by monitor)
RLAPI int (* rl_get_monitor_height )(int monitor);                          // Get specified monitor height (current video mode used by monitor)
RLAPI int (* rl_get_monitor_physical_width )(int monitor);                   // Get specified monitor physical width in millimetres
RLAPI int (* rl_get_monitor_physical_height )(int monitor);                  // Get specified monitor physical height in millimetres
RLAPI int (* rl_get_monitor_refresh_rate )(int monitor);                     // Get specified monitor refresh rate
RLAPI RlVector2 (* rl_get_window_position )(void);                            // Get window position XY on monitor
RLAPI RlVector2 (* rl_get_window_scale_d_p_i )(void);                            // Get window scale DPI factor
RLAPI const char *(* rl_get_monitor_name )(int monitor);                    // Get the human-readable, UTF-8 encoded name of the specified monitor
RLAPI void (* rl_set_clipboard_text )(const char *text);                    // Set clipboard text content
RLAPI const char *(* rl_get_clipboard_text )(void);                         // Get clipboard text content
RLAPI void (* rl_enable_event_waiting )(void);                              // Enable waiting for events on (* rl_end_drawing )(), no automatic event polling
RLAPI void (* rl_disable_event_waiting )(void);                             // Disable waiting for events on (* rl_end_drawing )(), automatic events polling

// Cursor-related functions
RLAPI void (* rl_show_cursor )(void);                                      // Shows cursor
RLAPI void (* rl_hide_cursor )(void);                                      // Hides cursor
RLAPI bool (* rl_is_cursor_hidden )(void);                                  // Check if cursor is not visible
RLAPI void (* rl_enable_cursor )(void);                                    // Enables cursor (unlock cursor)
RLAPI void (* rl_disable_cursor )(void);                                   // Disables cursor (lock cursor)
RLAPI bool (* rl_is_cursor_on_screen )(void);                                // Check if cursor is on the screen

// Drawing-related functions
RLAPI void (* rl_clear_background )(RlColor color);                          // Set background color (framebuffer clear color)
RLAPI void (* rl_begin_drawing )(void);                                    // Setup canvas (framebuffer) to start drawing
RLAPI void (* rl_end_drawing )(void);                                      // End canvas drawing and swap buffers (double buffering)
RLAPI void (* rl_begin_mode_2_d )(RlCamera2D camera);                          // Begin 2D mode with custom camera (2D)
RLAPI void (* rl_end_mode_2_d )(void);                                       // Ends 2D mode with custom camera
RLAPI void (* rl_begin_mode_3_d )(RlCamera3D camera);                          // Begin 3D mode with custom camera (3D)
RLAPI void (* rl_end_mode_3_d )(void);                                       // Ends 3D mode and returns to default 2D orthographic mode
RLAPI void (* rl_begin_texture_mode )(RlRenderTexture2D target);              // Begin drawing to render texture
RLAPI void (* rl_end_texture_mode )(void);                                  // Ends drawing to render texture
RLAPI void (* rl_begin_shader_mode )(RlShader shader);                        // Begin custom shader drawing
RLAPI void (* rl_end_shader_mode )(void);                                   // End custom shader drawing (use default shader)
RLAPI void (* rl_begin_blend_mode )(int mode);                              // Begin blending mode (alpha, additive, multiplied, subtract, custom)
RLAPI void (* rl_end_blend_mode )(void);                                    // End blending mode (reset to default: alpha blending)
RLAPI void (* rl_begin_scissor_mode )(int x, int y, int width, int height); // Begin scissor mode (define screen area for following drawing)
RLAPI void (* rl_end_scissor_mode )(void);                                  // End scissor mode
RLAPI void (* rl_begin_vr_stereo_mode )(RlVrStereoConfig config);              // Begin stereo rendering (requires VR simulator)
RLAPI void (* rl_end_vr_stereo_mode )(void);                                 // End stereo rendering (requires VR simulator)

// VR stereo config functions for VR simulator
RLAPI RlVrStereoConfig (* rl_load_vr_stereo_config )(RlVrDeviceInfo device);     // Load VR stereo config for VR simulator device parameters
RLAPI void (* rl_unload_vr_stereo_config )(RlVrStereoConfig config);           // Unload VR stereo config

// RlShader management functions
// NOTE: RlShader functionality is not available on OpenGL 1.1
RLAPI RlShader (* rl_load_shader )(const char *vsFileName, const char *fsFileName);   // Load shader from files and bind default locations
RLAPI RlShader (* rl_load_shader_from_memory )(const char *vsCode, const char *fsCode); // Load shader from code strings and bind default locations
RLAPI bool (* rl_is_shader_ready )(RlShader shader);                                   // Check if a shader is ready
RLAPI int (* rl_get_shader_location )(RlShader shader, const char *uniformName);       // Get shader uniform location
RLAPI int (* rl_get_shader_location_attrib )(RlShader shader, const char *attribName);  // Get shader attribute location
RLAPI void (* rl_set_shader_value )(RlShader shader, int locIndex, const void *value, int uniformType);               // Set shader uniform value
RLAPI void (* rl_set_shader_value_v )(RlShader shader, int locIndex, const void *value, int uniformType, int count);   // Set shader uniform value vector
RLAPI void (* rl_set_shader_value_matrix )(RlShader shader, int locIndex, RlMatrix mat);         // Set shader uniform value (matrix 4x4)
RLAPI void (* rl_set_shader_value_texture )(RlShader shader, int locIndex, RlTexture2D texture); // Set shader uniform value for texture (sampler2d)
RLAPI void (* rl_unload_shader )(RlShader shader);                                    // Unload shader from GPU memory (VRAM)

// Screen-space-related functions
RLAPI RlRay (* rl_get_mouse_ray )(RlVector2 mousePosition, RlCamera camera);      // Get a ray trace from mouse position
RLAPI RlMatrix (* rl_get_camera_matrix )(RlCamera camera);                      // Get camera transform matrix (view matrix)
RLAPI RlMatrix (* rl_get_camera_matrix_2_d )(RlCamera2D camera);                  // Get camera 2d transform matrix
RLAPI RlVector2 (* rl_get_world_to_screen )(RlVector3 position, RlCamera camera);  // Get the screen space position for a 3d world space position
RLAPI RlVector2 (* rl_get_screen_to_world_2_d )(RlVector2 position, RlCamera2D camera); // Get the world space position for a 2d camera screen space position
RLAPI RlVector2 (* rl_get_world_to_screen_ex )(RlVector3 position, RlCamera camera, int width, int height); // Get size position for a 3d world space position
RLAPI RlVector2 (* rl_get_world_to_screen_2_d )(RlVector2 position, RlCamera2D camera); // Get the screen space position for a 2d camera world space position

// Timing-related functions
RLAPI void (* rl_set_target_f_p_s )(int fps);                                 // Set target FPS (maximum)
RLAPI float (* rl_get_frame_time )(void);                                   // Get time in seconds for last frame drawn (delta time)
RLAPI double (* rl_get_time )(void);                                       // Get elapsed time in seconds since (* rl_init_window )()
RLAPI int (* rl_get_f_p_s )(void);                                           // Get current FPS

// Custom frame control functions
// NOTE: Those functions are intended for advance users that want full control over the frame processing
// By default (* rl_end_drawing )() does this job: draws everything + (* rl_swap_screen_buffer )() + manage frame timing + (* rl_poll_input_events )()
// To avoid that behaviour and control frame processes manually, enable in config.h: SUPPORT_CUSTOM_FRAME_CONTROL
RLAPI void (* rl_swap_screen_buffer )(void);                                // Swap back buffer with front buffer (screen drawing)
RLAPI void (* rl_poll_input_events )(void);                                 // Register all input events
RLAPI void (* rl_wait_time )(double seconds);                              // Wait for some time (halt program execution)

// Random values generation functions
RLAPI void (* rl_set_random_seed )(unsigned int seed);                      // Set the seed for the random number generator
RLAPI int (* rl_get_random_value )(int min, int max);                       // Get a random value between min and max (both included)
RLAPI int *(* rl_load_random_sequence )(unsigned int count, int min, int max); // Load random values sequence, no values repeated
RLAPI void (* rl_unload_random_sequence )(int *sequence);                   // Unload random values sequence

// Misc. functions
RLAPI void (* rl_take_screenshot )(const char *fileName);                  // Takes a screenshot of current screen (filename extension defines format)
RLAPI void (* rl_set_config_flags )(unsigned int flags);                    // Setup init configuration flags (view FLAGS)
RLAPI void (* rl_open_u_r_l )(const char *url);                              // Open URL with default system browser (if available)

// NOTE: Following functions implemented in module [utils]
//------------------------------------------------------------------
RLAPI void (* rl_trace_log )(int logLevel, const char *text, ...);         // Show trace log messages (LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR...)
RLAPI void (* rl_set_trace_log_level )(int logLevel);                        // Set the current threshold (minimum) log level
RLAPI void *(* rl_mem_alloc )(unsigned int size);                          // Internal memory allocator
RLAPI void *(* rl_mem_realloc )(void *ptr, unsigned int size);             // Internal memory reallocator
RLAPI void (* rl_mem_free )(void *ptr);                                    // Internal memory free

// Set custom callbacks
// WARNING: Callbacks setup is intended for advance users
RLAPI void (* rl_set_trace_log_callback )(RlTraceLogCallback callback);         // Set custom trace log
RLAPI void (* rl_set_load_file_data_callback )(RlLoadFileDataCallback callback); // Set custom file binary data loader
RLAPI void (* rl_set_save_file_data_callback )(RlSaveFileDataCallback callback); // Set custom file binary data saver
RLAPI void (* rl_set_load_file_text_callback )(RlLoadFileTextCallback callback); // Set custom file text data loader
RLAPI void (* rl_set_save_file_text_callback )(RlSaveFileTextCallback callback); // Set custom file text data saver

// Files management functions
RLAPI unsigned char *(* rl_load_file_data )(const char *fileName, int *dataSize); // Load file data as byte array (read)
RLAPI void (* rl_unload_file_data )(unsigned char *data);                   // Unload file data allocated by (* rl_load_file_data )()
RLAPI bool (* rl_save_file_data )(const char *fileName, void *data, int dataSize); // Save data to file from byte array (write), returns true on success
RLAPI bool (* rl_export_data_as_code )(const unsigned char *data, int dataSize, const char *fileName); // Export data to code (.h), returns true on success
RLAPI char *(* rl_load_file_text )(const char *fileName);                   // Load text data from file (read), returns a '\0' terminated string
RLAPI void (* rl_unload_file_text )(char *text);                            // Unload file text data allocated by (* rl_load_file_text )()
RLAPI bool (* rl_save_file_text )(const char *fileName, char *text);        // Save text data to file (write), string must be '\0' terminated, returns true on success
//------------------------------------------------------------------

// File system functions
RLAPI bool (* rl_file_exists )(const char *fileName);                      // Check if file exists
RLAPI bool (* rl_directory_exists )(const char *dirPath);                  // Check if a directory path exists
RLAPI bool (* rl_is_file_extension )(const char *fileName, const char *ext); // Check file extension (including point: .png, .wav)
RLAPI int (* rl_get_file_length )(const char *fileName);                    // Get file length in bytes (NOTE: GetFileSize() conflicts with windows.h)
RLAPI const char *(* rl_get_file_extension )(const char *fileName);         // Get pointer to extension for a filename string (includes dot: '.png')
RLAPI const char *(* rl_get_file_name )(const char *filePath);              // Get pointer to filename for a path string
RLAPI const char *(* rl_get_file_name_without_ext )(const char *filePath);    // Get filename string without extension (uses static string)
RLAPI const char *(* rl_get_directory_path )(const char *filePath);         // Get full path for a given fileName with path (uses static string)
RLAPI const char *(* rl_get_prev_directory_path )(const char *dirPath);      // Get previous directory path for a given path (uses static string)
RLAPI const char *(* rl_get_working_directory )(void);                      // Get current working directory (uses static string)
RLAPI const char *(* rl_get_application_directory )(void);                  // Get the directory of the running application (uses static string)
RLAPI bool (* rl_change_directory )(const char *dir);                      // Change working directory, return true on success
RLAPI bool (* rl_is_path_file )(const char *path);                          // Check if a given path is a file or a directory
RLAPI RlFilePathList (* rl_load_directory_files )(const char *dirPath);       // Load directory filepaths
RLAPI RlFilePathList (* rl_load_directory_files_ex )(const char *basePath, const char *filter, bool scanSubdirs); // Load directory filepaths with extension filtering and recursive directory scan
RLAPI void (* rl_unload_directory_files )(RlFilePathList files);              // Unload filepaths
RLAPI bool (* rl_is_file_dropped )(void);                                   // Check if a file has been dropped into window
RLAPI RlFilePathList (* rl_load_dropped_files )(void);                        // Load dropped filepaths
RLAPI void (* rl_unload_dropped_files )(RlFilePathList files);                // Unload dropped filepaths
RLAPI long (* rl_get_file_mod_time )(const char *fileName);                  // Get file modification time (last write time)

// Compression/Encoding functionality
RLAPI unsigned char *(* rl_compress_data )(const unsigned char *data, int dataSize, int *compDataSize);        // Compress data (DEFLATE algorithm), memory must be (* rl_mem_free )()
RLAPI unsigned char *(* rl_decompress_data )(const unsigned char *compData, int compDataSize, int *dataSize);  // Decompress data (DEFLATE algorithm), memory must be (* rl_mem_free )()
RLAPI char *(* rl_encode_data_base_6_4 )(const unsigned char *data, int dataSize, int *outputSize);               // Encode data to Base64 string, memory must be (* rl_mem_free )()
RLAPI unsigned char *(* rl_decode_data_base_6_4 )(const unsigned char *data, int *outputSize);                    // Decode Base64 string data, memory must be (* rl_mem_free )()

// Automation events functionality
RLAPI RlAutomationEventList (* rl_load_automation_event_list )(const char *fileName);                // Load automation events list from file, NULL for empty list, capacity = MAX_AUTOMATION_EVENTS
RLAPI void (* rl_unload_automation_event_list )(RlAutomationEventList *list);                        // Unload automation events list from file
RLAPI bool (* rl_export_automation_event_list )(RlAutomationEventList list, const char *fileName);   // Export automation events list as text file
RLAPI void (* rl_set_automation_event_list )(RlAutomationEventList *list);                           // Set automation event list to record to
RLAPI void (* rl_set_automation_event_base_frame )(int frame);                                      // Set automation event internal base frame to start recording
RLAPI void (* rl_start_automation_event_recording )(void);                                         // Start recording automation events (RlAutomationEventList must be set)
RLAPI void (* rl_stop_automation_event_recording )(void);                                          // Stop recording automation events
RLAPI void (* rl_play_automation_event )(RlAutomationEvent event);                                  // Play a recorded automation event

//------------------------------------------------------------------------------------
// Input Handling Functions (Module: core)
//------------------------------------------------------------------------------------

// Input-related functions: keyboard
RLAPI bool (* rl_is_key_pressed )(int key);                             // Check if a key has been pressed once
RLAPI bool (* rl_is_key_pressed_repeat )(int key);                       // Check if a key has been pressed again (Only PLATFORM_DESKTOP)
RLAPI bool (* rl_is_key_down )(int key);                                // Check if a key is being pressed
RLAPI bool (* rl_is_key_released )(int key);                            // Check if a key has been released once
RLAPI bool (* rl_is_key_up )(int key);                                  // Check if a key is NOT being pressed
RLAPI int (* rl_get_key_pressed )(void);                                // Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
RLAPI int (* rl_get_char_pressed )(void);                               // Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty
RLAPI void (* rl_set_exit_key )(int key);                               // Set a custom key to exit program (default is ESC)

// Input-related functions: gamepads
RLAPI bool (* rl_is_gamepad_available )(int gamepad);                   // Check if a gamepad is available
RLAPI const char *(* rl_get_gamepad_name )(int gamepad);                // Get gamepad internal name id
RLAPI bool (* rl_is_gamepad_button_pressed )(int gamepad, int button);   // Check if a gamepad button has been pressed once
RLAPI bool (* rl_is_gamepad_button_down )(int gamepad, int button);      // Check if a gamepad button is being pressed
RLAPI bool (* rl_is_gamepad_button_released )(int gamepad, int button);  // Check if a gamepad button has been released once
RLAPI bool (* rl_is_gamepad_button_up )(int gamepad, int button);        // Check if a gamepad button is NOT being pressed
RLAPI int (* rl_get_gamepad_button_pressed )(void);                      // Get the last gamepad button pressed
RLAPI int (* rl_get_gamepad_axis_count )(int gamepad);                   // Get gamepad axis count for a gamepad
RLAPI float (* rl_get_gamepad_axis_movement )(int gamepad, int axis);    // Get axis movement value for a gamepad axis
RLAPI int (* rl_set_gamepad_mappings )(const char *mappings);           // Set internal gamepad mappings (SDL_GameControllerDB)

// Input-related functions: mouse
RLAPI bool (* rl_is_mouse_button_pressed )(int button);                  // Check if a mouse button has been pressed once
RLAPI bool (* rl_is_mouse_button_down )(int button);                     // Check if a mouse button is being pressed
RLAPI bool (* rl_is_mouse_button_released )(int button);                 // Check if a mouse button has been released once
RLAPI bool (* rl_is_mouse_button_up )(int button);                       // Check if a mouse button is NOT being pressed
RLAPI int (* rl_get_mouse_x )(void);                                    // Get mouse position X
RLAPI int (* rl_get_mouse_y )(void);                                    // Get mouse position Y
RLAPI RlVector2 (* rl_get_mouse_position )(void);                         // Get mouse position XY
RLAPI RlVector2 (* rl_get_mouse_delta )(void);                            // Get mouse delta between frames
RLAPI void (* rl_set_mouse_position )(int x, int y);                    // Set mouse position XY
RLAPI void (* rl_set_mouse_offset )(int offsetX, int offsetY);          // Set mouse offset
RLAPI void (* rl_set_mouse_scale )(float scaleX, float scaleY);         // Set mouse scaling
RLAPI float (* rl_get_mouse_wheel_move )(void);                          // Get mouse wheel movement for X or Y, whichever is larger
RLAPI RlVector2 (* rl_get_mouse_wheel_move_v )(void);                       // Get mouse wheel movement for both X and Y
RLAPI void (* rl_set_mouse_cursor )(int cursor);                        // Set mouse cursor

// Input-related functions: touch
RLAPI int (* rl_get_touch_x )(void);                                    // Get touch position X for touch point 0 (relative to screen size)
RLAPI int (* rl_get_touch_y )(void);                                    // Get touch position Y for touch point 0 (relative to screen size)
RLAPI RlVector2 (* rl_get_touch_position )(int index);                    // Get touch position XY for a touch point index (relative to screen size)
RLAPI int (* rl_get_touch_point_id )(int index);                         // Get touch point identifier for given index
RLAPI int (* rl_get_touch_point_count )(void);                           // Get number of touch points

//------------------------------------------------------------------------------------
// RlGestures and Touch Handling Functions (Module: rgestures)
//------------------------------------------------------------------------------------
RLAPI void (* rl_set_gestures_enabled )(unsigned int flags);      // Enable a set of gestures using flags
RLAPI bool (* rl_is_gesture_detected )(unsigned int gesture);     // Check if a gesture have been detected
RLAPI int (* rl_get_gesture_detected )(void);                     // Get latest detected gesture
RLAPI float (* rl_get_gesture_hold_duration )(void);               // Get gesture hold time in milliseconds
RLAPI RlVector2 (* rl_get_gesture_drag_vector )(void);               // Get gesture drag vector
RLAPI float (* rl_get_gesture_drag_angle )(void);                  // Get gesture drag angle
RLAPI RlVector2 (* rl_get_gesture_pinch_vector )(void);              // Get gesture pinch delta
RLAPI float (* rl_get_gesture_pinch_angle )(void);                 // Get gesture pinch angle

//------------------------------------------------------------------------------------
// RlCamera System Functions (Module: rcamera)
//------------------------------------------------------------------------------------
RLAPI void (* rl_update_camera )(RlCamera *camera, int mode);      // Update camera position for selected mode
RLAPI void (* rl_update_camera_pro )(RlCamera *camera, RlVector3 movement, RlVector3 rotation, float zoom); // Update camera movement/rotation

//------------------------------------------------------------------------------------
// Basic Shapes Drawing Functions (Module: shapes)
//------------------------------------------------------------------------------------
// Set texture and rectangle to be used on shapes drawing
// NOTE: It can be useful when using basic shapes and one single font,
// defining a font char white rectangle would allow drawing everything in a single draw call
RLAPI void (* rl_set_shapes_texture )(RlTexture2D texture, RlRectangle source);       // Set texture and rectangle to be used on shapes drawing

// Basic shapes drawing functions
RLAPI void (* rl_draw_pixel )(int posX, int posY, RlColor color);                                                   // Draw a pixel
RLAPI void (* rl_draw_pixel_v )(RlVector2 position, RlColor color);                                                    // Draw a pixel (Vector version)
RLAPI void (* rl_draw_line )(int startPosX, int startPosY, int endPosX, int endPosY, RlColor color);                // Draw a line
RLAPI void (* rl_draw_line_v )(RlVector2 startPos, RlVector2 endPos, RlColor color);                                     // Draw a line (using gl lines)
RLAPI void (* rl_draw_line_ex )(RlVector2 startPos, RlVector2 endPos, float thick, RlColor color);                       // Draw a line (using triangles/quads)
RLAPI void (* rl_draw_line_strip )(RlVector2 *points, int pointCount, RlColor color);                                  // Draw lines sequence (using gl lines)
RLAPI void (* rl_draw_line_bezier )(RlVector2 startPos, RlVector2 endPos, float thick, RlColor color);                   // Draw line segment cubic-bezier in-out interpolation
RLAPI void (* rl_draw_circle )(int centerX, int centerY, float radius, RlColor color);                              // Draw a color-filled circle
RLAPI void (* rl_draw_circle_sector )(RlVector2 center, float radius, float startAngle, float endAngle, int segments, RlColor color);      // Draw a piece of a circle
RLAPI void (* rl_draw_circle_sector_lines )(RlVector2 center, float radius, float startAngle, float endAngle, int segments, RlColor color); // Draw circle sector outline
RLAPI void (* rl_draw_circle_gradient )(int centerX, int centerY, float radius, RlColor color1, RlColor color2);       // Draw a gradient-filled circle
RLAPI void (* rl_draw_circle_v )(RlVector2 center, float radius, RlColor color);                                       // Draw a color-filled circle (Vector version)
RLAPI void (* rl_draw_circle_lines )(int centerX, int centerY, float radius, RlColor color);                         // Draw circle outline
RLAPI void (* rl_draw_circle_lines_v )(RlVector2 center, float radius, RlColor color);                                  // Draw circle outline (Vector version)
RLAPI void (* rl_draw_ellipse )(int centerX, int centerY, float radiusH, float radiusV, RlColor color);             // Draw ellipse
RLAPI void (* rl_draw_ellipse_lines )(int centerX, int centerY, float radiusH, float radiusV, RlColor color);        // Draw ellipse outline
RLAPI void (* rl_draw_ring )(RlVector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, RlColor color); // Draw ring
RLAPI void (* rl_draw_ring_lines )(RlVector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, RlColor color);    // Draw ring outline
RLAPI void (* rl_draw_rectangle )(int posX, int posY, int width, int height, RlColor color);                        // Draw a color-filled rectangle
RLAPI void (* rl_draw_rectangle_v )(RlVector2 position, RlVector2 size, RlColor color);                                  // Draw a color-filled rectangle (Vector version)
RLAPI void (* rl_draw_rectangle_rec )(RlRectangle rec, RlColor color);                                                 // Draw a color-filled rectangle
RLAPI void (* rl_draw_rectangle_pro )(RlRectangle rec, RlVector2 origin, float rotation, RlColor color);                 // Draw a color-filled rectangle with pro parameters
RLAPI void (* rl_draw_rectangle_gradient_v )(int posX, int posY, int width, int height, RlColor color1, RlColor color2);// Draw a vertical-gradient-filled rectangle
RLAPI void (* rl_draw_rectangle_gradient_h )(int posX, int posY, int width, int height, RlColor color1, RlColor color2);// Draw a horizontal-gradient-filled rectangle
RLAPI void (* rl_draw_rectangle_gradient_ex )(RlRectangle rec, RlColor col1, RlColor col2, RlColor col3, RlColor col4);       // Draw a gradient-filled rectangle with custom vertex colors
RLAPI void (* rl_draw_rectangle_lines )(int posX, int posY, int width, int height, RlColor color);                   // Draw rectangle outline
RLAPI void (* rl_draw_rectangle_lines_ex )(RlRectangle rec, float lineThick, RlColor color);                            // Draw rectangle outline with extended parameters
RLAPI void (* rl_draw_rectangle_rounded )(RlRectangle rec, float roundness, int segments, RlColor color);              // Draw rectangle with rounded edges
RLAPI void (* rl_draw_rectangle_rounded_lines )(RlRectangle rec, float roundness, int segments, float lineThick, RlColor color); // Draw rectangle with rounded edges outline
RLAPI void (* rl_draw_triangle )(RlVector2 v1, RlVector2 v2, RlVector2 v3, RlColor color);                                // Draw a color-filled triangle (vertex in counter-clockwise order!)
RLAPI void (* rl_draw_triangle_lines )(RlVector2 v1, RlVector2 v2, RlVector2 v3, RlColor color);                           // Draw triangle outline (vertex in counter-clockwise order!)
RLAPI void (* rl_draw_triangle_fan )(RlVector2 *points, int pointCount, RlColor color);                                // Draw a triangle fan defined by points (first vertex is the center)
RLAPI void (* rl_draw_triangle_strip )(RlVector2 *points, int pointCount, RlColor color);                              // Draw a triangle strip defined by points
RLAPI void (* rl_draw_poly )(RlVector2 center, int sides, float radius, float rotation, RlColor color);               // Draw a regular polygon (Vector version)
RLAPI void (* rl_draw_poly_lines )(RlVector2 center, int sides, float radius, float rotation, RlColor color);          // Draw a polygon outline of n sides
RLAPI void (* rl_draw_poly_lines_ex )(RlVector2 center, int sides, float radius, float rotation, float lineThick, RlColor color); // Draw a polygon outline of n sides with extended parameters

// Splines drawing functions
RLAPI void (* rl_draw_spline_linear )(RlVector2 *points, int pointCount, float thick, RlColor color);                  // Draw spline: Linear, minimum 2 points
RLAPI void (* rl_draw_spline_basis )(RlVector2 *points, int pointCount, float thick, RlColor color);                   // Draw spline: B-Spline, minimum 4 points
RLAPI void (* rl_draw_spline_catmull_rom )(RlVector2 *points, int pointCount, float thick, RlColor color);              // Draw spline: Catmull-Rom, minimum 4 points
RLAPI void (* rl_draw_spline_bezier_quadratic )(RlVector2 *points, int pointCount, float thick, RlColor color);         // Draw spline: Quadratic Bezier, minimum 3 points (1 control point): [p1, c2, p3, c4...]
RLAPI void (* rl_draw_spline_bezier_cubic )(RlVector2 *points, int pointCount, float thick, RlColor color);             // Draw spline: Cubic Bezier, minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
RLAPI void (* rl_draw_spline_segment_linear )(RlVector2 p1, RlVector2 p2, float thick, RlColor color);                    // Draw spline segment: Linear, 2 points
RLAPI void (* rl_draw_spline_segment_basis )(RlVector2 p1, RlVector2 p2, RlVector2 p3, RlVector2 p4, float thick, RlColor color); // Draw spline segment: B-Spline, 4 points
RLAPI void (* rl_draw_spline_segment_catmull_rom )(RlVector2 p1, RlVector2 p2, RlVector2 p3, RlVector2 p4, float thick, RlColor color); // Draw spline segment: Catmull-Rom, 4 points
RLAPI void (* rl_draw_spline_segment_bezier_quadratic )(RlVector2 p1, RlVector2 c2, RlVector2 p3, float thick, RlColor color); // Draw spline segment: Quadratic Bezier, 2 points, 1 control point
RLAPI void (* rl_draw_spline_segment_bezier_cubic )(RlVector2 p1, RlVector2 c2, RlVector2 c3, RlVector2 p4, float thick, RlColor color); // Draw spline segment: Cubic Bezier, 2 points, 2 control points

// Spline segment point evaluation functions, for a given t [0.0f .. 1.0f]
RLAPI RlVector2 (* rl_get_spline_point_linear )(RlVector2 startPos, RlVector2 endPos, float t);                           // Get (evaluate) spline point: Linear
RLAPI RlVector2 (* rl_get_spline_point_basis )(RlVector2 p1, RlVector2 p2, RlVector2 p3, RlVector2 p4, float t);              // Get (evaluate) spline point: B-Spline
RLAPI RlVector2 (* rl_get_spline_point_catmull_rom )(RlVector2 p1, RlVector2 p2, RlVector2 p3, RlVector2 p4, float t);         // Get (evaluate) spline point: Catmull-Rom
RLAPI RlVector2 (* rl_get_spline_point_bezier_quad )(RlVector2 p1, RlVector2 c2, RlVector2 p3, float t);                     // Get (evaluate) spline point: Quadratic Bezier
RLAPI RlVector2 (* rl_get_spline_point_bezier_cubic )(RlVector2 p1, RlVector2 c2, RlVector2 c3, RlVector2 p4, float t);        // Get (evaluate) spline point: Cubic Bezier

// Basic shapes collision detection functions
RLAPI bool (* rl_check_collision_recs )(RlRectangle rec1, RlRectangle rec2);                                           // Check collision between two rectangles
RLAPI bool (* rl_check_collision_circles )(RlVector2 center1, float radius1, RlVector2 center2, float radius2);        // Check collision between two circles
RLAPI bool (* rl_check_collision_circle_rec )(RlVector2 center, float radius, RlRectangle rec);                         // Check collision between circle and rectangle
RLAPI bool (* rl_check_collision_point_rec )(RlVector2 point, RlRectangle rec);                                         // Check if point is inside rectangle
RLAPI bool (* rl_check_collision_point_circle )(RlVector2 point, RlVector2 center, float radius);                       // Check if point is inside circle
RLAPI bool (* rl_check_collision_point_triangle )(RlVector2 point, RlVector2 p1, RlVector2 p2, RlVector2 p3);               // Check if point is inside a triangle
RLAPI bool (* rl_check_collision_point_poly )(RlVector2 point, RlVector2 *points, int pointCount);                      // Check if point is within a polygon described by array of vertices
RLAPI bool (* rl_check_collision_lines )(RlVector2 startPos1, RlVector2 endPos1, RlVector2 startPos2, RlVector2 endPos2, RlVector2 *collisionPoint); // Check the collision between two lines defined by two points each, returns collision point by reference
RLAPI bool (* rl_check_collision_point_line )(RlVector2 point, RlVector2 p1, RlVector2 p2, int threshold);                // Check if point belongs to line created between two points [p1] and [p2] with defined margin in pixels [threshold]
RLAPI RlRectangle (* rl_get_collision_rec )(RlRectangle rec1, RlRectangle rec2);                                         // Get collision rectangle for two rectangles collision

//------------------------------------------------------------------------------------
// RlTexture Loading and Drawing Functions (Module: textures)
//------------------------------------------------------------------------------------

// RlImage loading functions
// NOTE: These functions do not require GPU access
RLAPI RlImage (* rl_load_image )(const char *fileName);                                                             // Load image from file into CPU memory (RAM)
RLAPI RlImage (* rl_load_image_raw )(const char *fileName, int width, int height, int format, int headerSize);       // Load image from RAW file data
RLAPI RlImage (* rl_load_image_svg )(const char *fileNameOrString, int width, int height);                           // Load image from SVG file data or string with specified size
RLAPI RlImage (* rl_load_image_anim )(const char *fileName, int *frames);                                            // Load image sequence from file (frames appended to image.data)
RLAPI RlImage (* rl_load_image_from_memory )(const char *fileType, const unsigned char *fileData, int dataSize);      // Load image from memory buffer, fileType refers to extension: i.e. '.png'
RLAPI RlImage (* rl_load_image_from_texture )(RlTexture2D texture);                                                     // Load image from GPU texture data
RLAPI RlImage (* rl_load_image_from_screen )(void);                                                                   // Load image from screen buffer and (screenshot)
RLAPI bool (* rl_is_image_ready )(RlImage image);                                                                    // Check if an image is ready
RLAPI void (* rl_unload_image )(RlImage image);                                                                     // Unload image from CPU memory (RAM)
RLAPI bool (* rl_export_image )(RlImage image, const char *fileName);                                               // Export image data to file, returns true on success
RLAPI unsigned char *(* rl_export_image_to_memory )(RlImage image, const char *fileType, int *fileSize);              // Export image to memory buffer
RLAPI bool (* rl_export_image_as_code )(RlImage image, const char *fileName);                                         // Export image as code file defining an array of bytes, returns true on success

// RlImage generation functions
RLAPI RlImage (* rl_gen_image_color )(int width, int height, RlColor color);                                           // Generate image: plain color
RLAPI RlImage (* rl_gen_image_gradient_linear )(int width, int height, int direction, RlColor start, RlColor end);        // Generate image: linear gradient, direction in degrees [0..360], 0=Vertical gradient
RLAPI RlImage (* rl_gen_image_gradient_radial )(int width, int height, float density, RlColor inner, RlColor outer);      // Generate image: radial gradient
RLAPI RlImage (* rl_gen_image_gradient_square )(int width, int height, float density, RlColor inner, RlColor outer);      // Generate image: square gradient
RLAPI RlImage (* rl_gen_image_checked )(int width, int height, int checksX, int checksY, RlColor col1, RlColor col2);    // Generate image: checked
RLAPI RlImage (* rl_gen_image_white_noise )(int width, int height, float factor);                                     // Generate image: white noise
RLAPI RlImage (* rl_gen_image_perlin_noise )(int width, int height, int offsetX, int offsetY, float scale);           // Generate image: perlin noise
RLAPI RlImage (* rl_gen_image_cellular )(int width, int height, int tileSize);                                       // Generate image: cellular algorithm, bigger tileSize means bigger cells
RLAPI RlImage (* rl_gen_image_text )(int width, int height, const char *text);                                       // Generate image: grayscale image from text data

// RlImage manipulation functions
RLAPI RlImage (* rl_image_copy )(RlImage image);                                                                      // Create an image duplicate (useful for transformations)
RLAPI RlImage (* rl_image_from_image )(RlImage image, RlRectangle rec);                                                  // Create an image from another image piece
RLAPI RlImage (* rl_image_text )(const char *text, int fontSize, RlColor color);                                      // Create an image from text (default font)
RLAPI RlImage (* rl_image_text_ex )(RlFont font, const char *text, float fontSize, float spacing, RlColor tint);         // Create an image from text (custom sprite font)
RLAPI void (* rl_image_format )(RlImage *image, int newFormat);                                                     // Convert image data to desired format
RLAPI void (* rl_image_to_p_o_t )(RlImage *image, RlColor fill);                                                         // Convert image to POT (power-of-two)
RLAPI void (* rl_image_crop )(RlImage *image, RlRectangle crop);                                                      // Crop an image to a defined rectangle
RLAPI void (* rl_image_alpha_crop )(RlImage *image, float threshold);                                                // Crop image depending on alpha value
RLAPI void (* rl_image_alpha_clear )(RlImage *image, RlColor color, float threshold);                                  // Clear alpha channel to desired color
RLAPI void (* rl_image_alpha_mask )(RlImage *image, RlImage alphaMask);                                                // Apply alpha mask to image
RLAPI void (* rl_image_alpha_premultiply )(RlImage *image);                                                          // Premultiply alpha channel
RLAPI void (* rl_image_blur_gaussian )(RlImage *image, int blurSize);                                                // Apply Gaussian blur using a box blur approximation
RLAPI void (* rl_image_resize )(RlImage *image, int newWidth, int newHeight);                                       // Resize image (Bicubic scaling algorithm)
RLAPI void (* rl_image_resize_n_n )(RlImage *image, int newWidth,int newHeight);                                      // Resize image (Nearest-Neighbor scaling algorithm)
RLAPI void (* rl_image_resize_canvas )(RlImage *image, int newWidth, int newHeight, int offsetX, int offsetY, RlColor fill);  // Resize canvas and fill with color
RLAPI void (* rl_image_mipmaps )(RlImage *image);                                                                   // Compute all mipmap levels for a provided image
RLAPI void (* rl_image_dither )(RlImage *image, int rBpp, int gBpp, int bBpp, int aBpp);                            // Dither image data to 16bpp or lower (Floyd-Steinberg dithering)
RLAPI void (* rl_image_flip_vertical )(RlImage *image);                                                              // Flip image vertically
RLAPI void (* rl_image_flip_horizontal )(RlImage *image);                                                            // Flip image horizontally
RLAPI void (* rl_image_rotate )(RlImage *image, int degrees);                                                       // Rotate image by input angle in degrees (-359 to 359)
RLAPI void (* rl_image_rotate_c_w )(RlImage *image);                                                                  // Rotate image clockwise 90deg
RLAPI void (* rl_image_rotate_c_c_w )(RlImage *image);                                                                 // Rotate image counter-clockwise 90deg
RLAPI void (* rl_image_color_tint )(RlImage *image, RlColor color);                                                    // Modify image color: tint
RLAPI void (* rl_image_color_invert )(RlImage *image);                                                               // Modify image color: invert
RLAPI void (* rl_image_color_grayscale )(RlImage *image);                                                            // Modify image color: grayscale
RLAPI void (* rl_image_color_contrast )(RlImage *image, float contrast);                                             // Modify image color: contrast (-100 to 100)
RLAPI void (* rl_image_color_brightness )(RlImage *image, int brightness);                                           // Modify image color: brightness (-255 to 255)
RLAPI void (* rl_image_color_replace )(RlImage *image, RlColor color, RlColor replace);                                  // Modify image color: replace color
RLAPI RlColor *(* rl_load_image_colors )(RlImage image);                                                               // Load color data from image as a RlColor array (RGBA - 32bit)
RLAPI RlColor *(* rl_load_image_palette )(RlImage image, int maxPaletteSize, int *colorCount);                         // Load colors palette from image as a RlColor array (RGBA - 32bit)
RLAPI void (* rl_unload_image_colors )(RlColor *colors);                                                             // Unload color data loaded with (* rl_load_image_colors )()
RLAPI void (* rl_unload_image_palette )(RlColor *colors);                                                            // Unload colors palette loaded with (* rl_load_image_palette )()
RLAPI RlRectangle (* rl_get_image_alpha_border )(RlImage image, float threshold);                                       // Get image alpha border rectangle
RLAPI RlColor (* rl_get_image_color )(RlImage image, int x, int y);                                                    // Get image pixel color at (x, y) position

// RlImage drawing functions
// NOTE: RlImage software-rendering functions (CPU)
RLAPI void (* rl_image_clear_background )(RlImage *dst, RlColor color);                                                // Clear image background with given color
RLAPI void (* rl_image_draw_pixel )(RlImage *dst, int posX, int posY, RlColor color);                                  // Draw pixel within an image
RLAPI void (* rl_image_draw_pixel_v )(RlImage *dst, RlVector2 position, RlColor color);                                   // Draw pixel within an image (Vector version)
RLAPI void (* rl_image_draw_line )(RlImage *dst, int startPosX, int startPosY, int endPosX, int endPosY, RlColor color); // Draw line within an image
RLAPI void (* rl_image_draw_line_v )(RlImage *dst, RlVector2 start, RlVector2 end, RlColor color);                          // Draw line within an image (Vector version)
RLAPI void (* rl_image_draw_circle )(RlImage *dst, int centerX, int centerY, int radius, RlColor color);               // Draw a filled circle within an image
RLAPI void (* rl_image_draw_circle_v )(RlImage *dst, RlVector2 center, int radius, RlColor color);                        // Draw a filled circle within an image (Vector version)
RLAPI void (* rl_image_draw_circle_lines )(RlImage *dst, int centerX, int centerY, int radius, RlColor color);          // Draw circle outline within an image
RLAPI void (* rl_image_draw_circle_lines_v )(RlImage *dst, RlVector2 center, int radius, RlColor color);                   // Draw circle outline within an image (Vector version)
RLAPI void (* rl_image_draw_rectangle )(RlImage *dst, int posX, int posY, int width, int height, RlColor color);       // Draw rectangle within an image
RLAPI void (* rl_image_draw_rectangle_v )(RlImage *dst, RlVector2 position, RlVector2 size, RlColor color);                 // Draw rectangle within an image (Vector version)
RLAPI void (* rl_image_draw_rectangle_rec )(RlImage *dst, RlRectangle rec, RlColor color);                                // Draw rectangle within an image
RLAPI void (* rl_image_draw_rectangle_lines )(RlImage *dst, RlRectangle rec, int thick, RlColor color);                   // Draw rectangle lines within an image
RLAPI void (* rl_image_draw )(RlImage *dst, RlImage src, RlRectangle srcRec, RlRectangle dstRec, RlColor tint);             // Draw a source image within a destination image (tint applied to source)
RLAPI void (* rl_image_draw_text )(RlImage *dst, const char *text, int posX, int posY, int fontSize, RlColor color);   // Draw text (using default font) within an image (destination)
RLAPI void (* rl_image_draw_text_ex )(RlImage *dst, RlFont font, const char *text, RlVector2 position, float fontSize, float spacing, RlColor tint); // Draw text (custom sprite font) within an image (destination)

// RlTexture loading functions
// NOTE: These functions require GPU access
RLAPI RlTexture2D (* rl_load_texture )(const char *fileName);                                                       // Load texture from file into GPU memory (VRAM)
RLAPI RlTexture2D (* rl_load_texture_from_image )(RlImage image);                                                       // Load texture from image data
RLAPI RlTextureCubemap (* rl_load_texture_cubemap )(RlImage image, int layout);                                        // Load cubemap from image, multiple image cubemap layouts supported
RLAPI RlRenderTexture2D (* rl_load_render_texture )(int width, int height);                                          // Load texture for rendering (framebuffer)
RLAPI bool (* rl_is_texture_ready )(RlTexture2D texture);                                                            // Check if a texture is ready
RLAPI void (* rl_unload_texture )(RlTexture2D texture);                                                             // Unload texture from GPU memory (VRAM)
RLAPI bool (* rl_is_render_texture_ready )(RlRenderTexture2D target);                                                 // Check if a render texture is ready
RLAPI void (* rl_unload_render_texture )(RlRenderTexture2D target);                                                  // Unload render texture from GPU memory (VRAM)
RLAPI void (* rl_update_texture )(RlTexture2D texture, const void *pixels);                                         // Update GPU texture with new data
RLAPI void (* rl_update_texture_rec )(RlTexture2D texture, RlRectangle rec, const void *pixels);                       // Update GPU texture rectangle with new data

// RlTexture configuration functions
RLAPI void (* rl_gen_texture_mipmaps )(RlTexture2D *texture);                                                        // Generate GPU mipmaps for a texture
RLAPI void (* rl_set_texture_filter )(RlTexture2D texture, int filter);                                              // Set texture scaling filter mode
RLAPI void (* rl_set_texture_wrap )(RlTexture2D texture, int wrap);                                                  // Set texture wrapping mode

// RlTexture drawing functions
RLAPI void (* rl_draw_texture )(RlTexture2D texture, int posX, int posY, RlColor tint);                               // Draw a RlTexture2D
RLAPI void (* rl_draw_texture_v )(RlTexture2D texture, RlVector2 position, RlColor tint);                                // Draw a RlTexture2D with position defined as RlVector2
RLAPI void (* rl_draw_texture_ex )(RlTexture2D texture, RlVector2 position, float rotation, float scale, RlColor tint);  // Draw a RlTexture2D with extended parameters
RLAPI void (* rl_draw_texture_rec )(RlTexture2D texture, RlRectangle source, RlVector2 position, RlColor tint);            // Draw a part of a texture defined by a rectangle
RLAPI void (* rl_draw_texture_pro )(RlTexture2D texture, RlRectangle source, RlRectangle dest, RlVector2 origin, float rotation, RlColor tint); // Draw a part of a texture defined by a rectangle with 'pro' parameters
RLAPI void (* rl_draw_texture_n_patch )(RlTexture2D texture, RlNPatchInfo nPatchInfo, RlRectangle dest, RlVector2 origin, float rotation, RlColor tint); // Draws a texture (or part of it) that stretches or shrinks nicely

// RlColor/pixel related functions
RLAPI RlColor (* rl_fade )(RlColor color, float alpha);                                 // Get color with alpha applied, alpha goes from 0.0f to 1.0f
RLAPI int (* rl_color_to_int )(RlColor color);                                          // Get hexadecimal value for a RlColor
RLAPI RlVector4 (* rl_color_normalize )(RlColor color);                                  // Get RlColor normalized as float [0..1]
RLAPI RlColor (* rl_color_from_normalized )(RlVector4 normalized);                        // Get RlColor from normalized values [0..1]
RLAPI RlVector3 (* rl_color_to_h_s_v )(RlColor color);                                      // Get HSV values for a RlColor, hue [0..360], saturation/value [0..1]
RLAPI RlColor (* rl_color_from_h_s_v )(float hue, float saturation, float value);         // Get a RlColor from HSV values, hue [0..360], saturation/value [0..1]
RLAPI RlColor (* rl_color_tint )(RlColor color, RlColor tint);                             // Get color multiplied with another color
RLAPI RlColor (* rl_color_brightness )(RlColor color, float factor);                     // Get color with brightness correction, brightness factor goes from -1.0f to 1.0f
RLAPI RlColor (* rl_color_contrast )(RlColor color, float contrast);                     // Get color with contrast correction, contrast values between -1.0f and 1.0f
RLAPI RlColor (* rl_color_alpha )(RlColor color, float alpha);                           // Get color with alpha applied, alpha goes from 0.0f to 1.0f
RLAPI RlColor (* rl_color_alpha_blend )(RlColor dst, RlColor src, RlColor tint);              // Get src alpha-blended into dst color with tint
RLAPI RlColor (* rl_get_color )(unsigned int hexValue);                                // Get RlColor structure from hexadecimal value
RLAPI RlColor (* rl_get_pixel_color )(void *srcPtr, int format);                        // Get RlColor from a source pixel pointer of certain format
RLAPI void (* rl_set_pixel_color )(void *dstPtr, RlColor color, int format);            // Set color formatted into destination pixel pointer
RLAPI int (* rl_get_pixel_data_size )(int width, int height, int format);              // Get pixel data size in bytes for certain format

//------------------------------------------------------------------------------------
// RlFont Loading and Text Drawing Functions (Module: text)
//------------------------------------------------------------------------------------

// RlFont loading/unloading functions
RLAPI RlFont (* rl_get_font_default )(void);                                                            // Get the default RlFont
RLAPI RlFont (* rl_load_font )(const char *fileName);                                                  // Load font from file into GPU memory (VRAM)
RLAPI RlFont (* rl_load_font_ex )(const char *fileName, int fontSize, int *codepoints, int codepointCount);  // Load font from file with extended parameters, use NULL for codepoints and 0 for codepointCount to load the default character set
RLAPI RlFont (* rl_load_font_from_image )(RlImage image, RlColor key, int firstChar);                        // Load font from RlImage (XNA style)
RLAPI RlFont (* rl_load_font_from_memory )(const char *fileType, const unsigned char *fileData, int dataSize, int fontSize, int *codepoints, int codepointCount); // Load font from memory buffer, fileType refers to extension: i.e. '.ttf'
RLAPI bool (* rl_is_font_ready )(RlFont font);                                                          // Check if a font is ready
RLAPI RlGlyphInfo *(* rl_load_font_data )(const unsigned char *fileData, int dataSize, int fontSize, int *codepoints, int codepointCount, int type); // Load font data for further use
RLAPI RlImage (* rl_gen_image_font_atlas )(const RlGlyphInfo *glyphs, RlRectangle **glyphRecs, int glyphCount, int fontSize, int padding, int packMethod); // Generate image font atlas using chars info
RLAPI void (* rl_unload_font_data )(RlGlyphInfo *glyphs, int glyphCount);                               // Unload font chars info data (RAM)
RLAPI void (* rl_unload_font )(RlFont font);                                                           // Unload font from GPU memory (VRAM)
RLAPI bool (* rl_export_font_as_code )(RlFont font, const char *fileName);                               // Export font as code file, returns true on success

// Text drawing functions
RLAPI void (* rl_draw_f_p_s )(int posX, int posY);                                                     // Draw current FPS
RLAPI void (* rl_draw_text )(const char *text, int posX, int posY, int fontSize, RlColor color);       // Draw text (using default font)
RLAPI void (* rl_draw_text_ex )(RlFont font, const char *text, RlVector2 position, float fontSize, float spacing, RlColor tint); // Draw text using font and additional parameters
RLAPI void (* rl_draw_text_pro )(RlFont font, const char *text, RlVector2 position, RlVector2 origin, float rotation, float fontSize, float spacing, RlColor tint); // Draw text using RlFont and pro parameters (rotation)
RLAPI void (* rl_draw_text_codepoint )(RlFont font, int codepoint, RlVector2 position, float fontSize, RlColor tint); // Draw one character (codepoint)
RLAPI void (* rl_draw_text_codepoints )(RlFont font, const int *codepoints, int codepointCount, RlVector2 position, float fontSize, float spacing, RlColor tint); // Draw multiple character (codepoint)

// Text font info functions
RLAPI void (* rl_set_text_line_spacing )(int spacing);                                                 // Set vertical line spacing when drawing with line-breaks
RLAPI int (* rl_measure_text )(const char *text, int fontSize);                                      // Measure string width for default font
RLAPI RlVector2 (* rl_measure_text_ex )(RlFont font, const char *text, float fontSize, float spacing);    // Measure string size for RlFont
RLAPI int (* rl_get_glyph_index )(RlFont font, int codepoint);                                          // Get glyph index position in font for a codepoint (unicode character), fallback to '?' if not found
RLAPI RlGlyphInfo (* rl_get_glyph_info )(RlFont font, int codepoint);                                     // Get glyph font info data for a codepoint (unicode character), fallback to '?' if not found
RLAPI RlRectangle (* rl_get_glyph_atlas_rec )(RlFont font, int codepoint);                                 // Get glyph rectangle in font atlas for a codepoint (unicode character), fallback to '?' if not found

// Text codepoints management functions (unicode characters)
RLAPI char *(* rl_load_u_t_f_8 )(const int *codepoints, int length);                // Load UTF-8 text encoded from codepoints array
RLAPI void (* rl_unload_u_t_f_8 )(char *text);                                      // Unload UTF-8 text encoded from codepoints array
RLAPI int *(* rl_load_codepoints )(const char *text, int *count);                // Load all codepoints from a UTF-8 text string, codepoints count returned by parameter
RLAPI void (* rl_unload_codepoints )(int *codepoints);                           // Unload codepoints data from memory
RLAPI int (* rl_get_codepoint_count )(const char *text);                          // Get total number of codepoints in a UTF-8 encoded string
RLAPI int (* rl_get_codepoint )(const char *text, int *codepointSize);           // Get next codepoint in a UTF-8 encoded string, 0x3f('?') is returned on failure
RLAPI int (* rl_get_codepoint_next )(const char *text, int *codepointSize);       // Get next codepoint in a UTF-8 encoded string, 0x3f('?') is returned on failure
RLAPI int (* rl_get_codepoint_previous )(const char *text, int *codepointSize);   // Get previous codepoint in a UTF-8 encoded string, 0x3f('?') is returned on failure
RLAPI const char *(* rl_codepoint_to_u_t_f_8 )(int codepoint, int *utf8Size);        // Encode one codepoint into UTF-8 byte array (array length returned as parameter)

// Text strings management functions (no UTF-8 strings, only byte chars)
// NOTE: Some strings allocate memory internally for returned strings, just be careful!
RLAPI int (* rl_text_copy )(char *dst, const char *src);                                             // Copy one string to another, returns bytes copied
RLAPI bool (* rl_text_is_equal )(const char *text1, const char *text2);                               // Check if two text string are equal
RLAPI unsigned int (* rl_text_length )(const char *text);                                            // Get text length, checks for '\0' ending
RLAPI const char *(* rl_text_format )(const char *text, ...);                                        // Text formatting with variables (sprintf() style)
RLAPI const char *(* rl_text_subtext )(const char *text, int position, int length);                  // Get a piece of a text string
RLAPI char *(* rl_text_replace )(char *text, const char *replace, const char *by);                   // Replace text string (WARNING: memory must be freed!)
RLAPI char *(* rl_text_insert )(const char *text, const char *insert, int position);                 // Insert text in a position (WARNING: memory must be freed!)
RLAPI const char *(* rl_text_join )(const char **textList, int count, const char *delimiter);        // Join text strings with delimiter
RLAPI const char **(* rl_text_split )(const char *text, char delimiter, int *count);                 // Split text into multiple strings
RLAPI void (* rl_text_append )(char *text, const char *append, int *position);                       // Append text at specific position and move cursor!
RLAPI int (* rl_text_find_index )(const char *text, const char *find);                                // Find first text occurrence within a string
RLAPI const char *(* rl_text_to_upper )(const char *text);                      // Get upper case version of provided string
RLAPI const char *(* rl_text_to_lower )(const char *text);                      // Get lower case version of provided string
RLAPI const char *(* rl_text_to_pascal )(const char *text);                     // Get Pascal case notation version of provided string
RLAPI int (* rl_text_to_integer )(const char *text);                            // Get integer value from text (negative values not supported)

//------------------------------------------------------------------------------------
// Basic 3d Shapes Drawing Functions (Module: models)
//------------------------------------------------------------------------------------

// Basic geometric 3D shapes drawing functions
RLAPI void (* rl_draw_line_3_d )(RlVector3 startPos, RlVector3 endPos, RlColor color);                                    // Draw a line in 3D world space
RLAPI void (* rl_draw_point_3_d )(RlVector3 position, RlColor color);                                                   // Draw a point in 3D space, actually a small line
RLAPI void (* rl_draw_circle_3_d )(RlVector3 center, float radius, RlVector3 rotationAxis, float rotationAngle, RlColor color); // Draw a circle in 3D world space
RLAPI void (* rl_draw_triangle_3_d )(RlVector3 v1, RlVector3 v2, RlVector3 v3, RlColor color);                              // Draw a color-filled triangle (vertex in counter-clockwise order!)
RLAPI void (* rl_draw_triangle_strip_3_d )(RlVector3 *points, int pointCount, RlColor color);                            // Draw a triangle strip defined by points
RLAPI void (* rl_draw_cube )(RlVector3 position, float width, float height, float length, RlColor color);             // Draw cube
RLAPI void (* rl_draw_cube_v )(RlVector3 position, RlVector3 size, RlColor color);                                       // Draw cube (Vector version)
RLAPI void (* rl_draw_cube_wires )(RlVector3 position, float width, float height, float length, RlColor color);        // Draw cube wires
RLAPI void (* rl_draw_cube_wires_v )(RlVector3 position, RlVector3 size, RlColor color);                                  // Draw cube wires (Vector version)
RLAPI void (* rl_draw_sphere )(RlVector3 centerPos, float radius, RlColor color);                                     // Draw sphere
RLAPI void (* rl_draw_sphere_ex )(RlVector3 centerPos, float radius, int rings, int slices, RlColor color);            // Draw sphere with extended parameters
RLAPI void (* rl_draw_sphere_wires )(RlVector3 centerPos, float radius, int rings, int slices, RlColor color);         // Draw sphere wires
RLAPI void (* rl_draw_cylinder )(RlVector3 position, float radiusTop, float radiusBottom, float height, int slices, RlColor color); // Draw a cylinder/cone
RLAPI void (* rl_draw_cylinder_ex )(RlVector3 startPos, RlVector3 endPos, float startRadius, float endRadius, int sides, RlColor color); // Draw a cylinder with base at startPos and top at endPos
RLAPI void (* rl_draw_cylinder_wires )(RlVector3 position, float radiusTop, float radiusBottom, float height, int slices, RlColor color); // Draw a cylinder/cone wires
RLAPI void (* rl_draw_cylinder_wires_ex )(RlVector3 startPos, RlVector3 endPos, float startRadius, float endRadius, int sides, RlColor color); // Draw a cylinder wires with base at startPos and top at endPos
RLAPI void (* rl_draw_capsule )(RlVector3 startPos, RlVector3 endPos, float radius, int slices, int rings, RlColor color); // Draw a capsule with the center of its sphere caps at startPos and endPos
RLAPI void (* rl_draw_capsule_wires )(RlVector3 startPos, RlVector3 endPos, float radius, int slices, int rings, RlColor color); // Draw capsule wireframe with the center of its sphere caps at startPos and endPos
RLAPI void (* rl_draw_plane )(RlVector3 centerPos, RlVector2 size, RlColor color);                                      // Draw a plane XZ
RLAPI void (* rl_draw_ray )(RlRay ray, RlColor color);                                                                // Draw a ray line
RLAPI void (* rl_draw_grid )(int slices, float spacing);                                                          // Draw a grid (centered at (0, 0, 0))

//------------------------------------------------------------------------------------
// RlModel 3d Loading and Drawing Functions (Module: models)
//------------------------------------------------------------------------------------

// RlModel management functions
RLAPI RlModel (* rl_load_model )(const char *fileName);                                                // Load model from files (meshes and materials)
RLAPI RlModel (* rl_load_model_from_mesh )(RlMesh mesh);                                                   // Load model from generated mesh (default material)
RLAPI bool (* rl_is_model_ready )(RlModel model);                                                       // Check if a model is ready
RLAPI void (* rl_unload_model )(RlModel model);                                                        // Unload model (including meshes) from memory (RAM and/or VRAM)
RLAPI RlBoundingBox (* rl_get_model_bounding_box )(RlModel model);                                         // Compute model bounding box limits (considers all meshes)

// RlModel drawing functions
RLAPI void (* rl_draw_model )(RlModel model, RlVector3 position, float scale, RlColor tint);               // Draw a model (with texture if set)
RLAPI void (* rl_draw_model_ex )(RlModel model, RlVector3 position, RlVector3 rotationAxis, float rotationAngle, RlVector3 scale, RlColor tint); // Draw a model with extended parameters
RLAPI void (* rl_draw_model_wires )(RlModel model, RlVector3 position, float scale, RlColor tint);          // Draw a model wires (with texture if set)
RLAPI void (* rl_draw_model_wires_ex )(RlModel model, RlVector3 position, RlVector3 rotationAxis, float rotationAngle, RlVector3 scale, RlColor tint); // Draw a model wires (with texture if set) with extended parameters
RLAPI void (* rl_draw_bounding_box )(RlBoundingBox box, RlColor color);                                   // Draw bounding box (wires)
RLAPI void (* rl_draw_billboard )(RlCamera camera, RlTexture2D texture, RlVector3 position, float size, RlColor tint);   // Draw a billboard texture
RLAPI void (* rl_draw_billboard_rec )(RlCamera camera, RlTexture2D texture, RlRectangle source, RlVector3 position, RlVector2 size, RlColor tint); // Draw a billboard texture defined by source
RLAPI void (* rl_draw_billboard_pro )(RlCamera camera, RlTexture2D texture, RlRectangle source, RlVector3 position, RlVector3 up, RlVector2 size, RlVector2 origin, float rotation, RlColor tint); // Draw a billboard texture defined by source and rotation

// RlMesh management functions
RLAPI void (* rl_upload_mesh )(RlMesh *mesh, bool dynamic);                                            // Upload mesh vertex data in GPU and provide VAO/VBO ids
RLAPI void (* rl_update_mesh_buffer )(RlMesh mesh, int index, const void *data, int dataSize, int offset); // Update mesh vertex data in GPU for a specific buffer index
RLAPI void (* rl_unload_mesh )(RlMesh mesh);                                                           // Unload mesh data from CPU and GPU
RLAPI void (* rl_draw_mesh )(RlMesh mesh, RlMaterial material, RlMatrix transform);                        // Draw a 3d mesh with material and transform
RLAPI void (* rl_draw_mesh_instanced )(RlMesh mesh, RlMaterial material, const RlMatrix *transforms, int instances); // Draw multiple mesh instances with material and different transforms
RLAPI bool (* rl_export_mesh )(RlMesh mesh, const char *fileName);                                     // Export mesh data to file, returns true on success
RLAPI RlBoundingBox (* rl_get_mesh_bounding_box )(RlMesh mesh);                                            // Compute mesh bounding box limits
RLAPI void (* rl_gen_mesh_tangents )(RlMesh *mesh);                                                     // Compute mesh tangents

// RlMesh generation functions
RLAPI RlMesh (* rl_gen_mesh_poly )(int sides, float radius);                                            // Generate polygonal mesh
RLAPI RlMesh (* rl_gen_mesh_plane )(float width, float length, int resX, int resZ);                     // Generate plane mesh (with subdivisions)
RLAPI RlMesh (* rl_gen_mesh_cube )(float width, float height, float length);                            // Generate cuboid mesh
RLAPI RlMesh (* rl_gen_mesh_sphere )(float radius, int rings, int slices);                              // Generate sphere mesh (standard sphere)
RLAPI RlMesh (* rl_gen_mesh_hemi_sphere )(float radius, int rings, int slices);                          // Generate half-sphere mesh (no bottom cap)
RLAPI RlMesh (* rl_gen_mesh_cylinder )(float radius, float height, int slices);                         // Generate cylinder mesh
RLAPI RlMesh (* rl_gen_mesh_cone )(float radius, float height, int slices);                             // Generate cone/pyramid mesh
RLAPI RlMesh (* rl_gen_mesh_torus )(float radius, float size, int radSeg, int sides);                   // Generate torus mesh
RLAPI RlMesh (* rl_gen_mesh_knot )(float radius, float size, int radSeg, int sides);                    // Generate trefoil knot mesh
RLAPI RlMesh (* rl_gen_mesh_heightmap )(RlImage heightmap, RlVector3 size);                                 // Generate heightmap mesh from image data
RLAPI RlMesh (* rl_gen_mesh_cubicmap )(RlImage cubicmap, RlVector3 cubeSize);                               // Generate cubes-based map mesh from image data

// RlMaterial loading/unloading functions
RLAPI RlMaterial *(* rl_load_materials )(const char *fileName, int *materialCount);                    // Load materials from model file
RLAPI RlMaterial (* rl_load_material_default )(void);                                                   // Load default material (Supports: DIFFUSE, SPECULAR, NORMAL maps)
RLAPI bool (* rl_is_material_ready )(RlMaterial material);                                              // Check if a material is ready
RLAPI void (* rl_unload_material )(RlMaterial material);                                               // Unload material from GPU memory (VRAM)
RLAPI void (* rl_set_material_texture )(RlMaterial *material, int mapType, RlTexture2D texture);          // Set texture for a material map type (MATERIAL_MAP_DIFFUSE, MATERIAL_MAP_SPECULAR...)
RLAPI void (* rl_set_model_mesh_material )(RlModel *model, int meshId, int materialId);                  // Set material for a mesh

// RlModel animations loading/unloading functions
RLAPI RlModelAnimation *(* rl_load_model_animations )(const char *fileName, int *animCount);            // Load model animations from file
RLAPI void (* rl_update_model_animation )(RlModel model, RlModelAnimation anim, int frame);               // Update model animation pose
RLAPI void (* rl_unload_model_animation )(RlModelAnimation anim);                                       // Unload animation data
RLAPI void (* rl_unload_model_animations )(RlModelAnimation *animations, int animCount);                // Unload animation array data
RLAPI bool (* rl_is_model_animation_valid )(RlModel model, RlModelAnimation anim);                         // Check model animation skeleton match

// Collision detection functions
RLAPI bool (* rl_check_collision_spheres )(RlVector3 center1, float radius1, RlVector3 center2, float radius2);   // Check collision between two spheres
RLAPI bool (* rl_check_collision_boxes )(RlBoundingBox box1, RlBoundingBox box2);                                 // Check collision between two bounding boxes
RLAPI bool (* rl_check_collision_box_sphere )(RlBoundingBox box, RlVector3 center, float radius);                  // Check collision between box and sphere
RLAPI RlRayCollision (* rl_get_ray_collision_sphere )(RlRay ray, RlVector3 center, float radius);                    // Get collision info between ray and sphere
RLAPI RlRayCollision (* rl_get_ray_collision_box )(RlRay ray, RlBoundingBox box);                                    // Get collision info between ray and box
RLAPI RlRayCollision (* rl_get_ray_collision_mesh )(RlRay ray, RlMesh mesh, RlMatrix transform);                       // Get collision info between ray and mesh
RLAPI RlRayCollision (* rl_get_ray_collision_triangle )(RlRay ray, RlVector3 p1, RlVector3 p2, RlVector3 p3);            // Get collision info between ray and triangle
RLAPI RlRayCollision (* rl_get_ray_collision_quad )(RlRay ray, RlVector3 p1, RlVector3 p2, RlVector3 p3, RlVector3 p4);    // Get collision info between ray and quad

//------------------------------------------------------------------------------------
// Audio Loading and Playing Functions (Module: audio)
//------------------------------------------------------------------------------------
typedef void (*RlAudioCallback)(void *bufferData, unsigned int frames);

// Audio device management functions
RLAPI void (* rl_init_audio_device )(void);                                     // Initialize audio device and context
RLAPI void (* rl_close_audio_device )(void);                                    // Close the audio device and context
RLAPI bool (* rl_is_audio_device_ready )(void);                                  // Check if audio device has been initialized successfully
RLAPI void (* rl_set_master_volume )(float volume);                             // Set master volume (listener)
RLAPI float (* rl_get_master_volume )(void);                                    // Get master volume (listener)

// RlWave/RlSound loading/unloading functions
RLAPI RlWave (* rl_load_wave )(const char *fileName);                            // Load wave data from file
RLAPI RlWave (* rl_load_wave_from_memory )(const char *fileType, const unsigned char *fileData, int dataSize); // Load wave from memory buffer, fileType refers to extension: i.e. '.wav'
RLAPI bool (* rl_is_wave_ready )(RlWave wave);                                    // Checks if wave data is ready
RLAPI RlSound (* rl_load_sound )(const char *fileName);                          // Load sound from file
RLAPI RlSound (* rl_load_sound_from_wave )(RlWave wave);                             // Load sound from wave data
RLAPI RlSound (* rl_load_sound_alias )(RlSound source);                             // Create a new sound that shares the same sample data as the source sound, does not own the sound data
RLAPI bool (* rl_is_sound_ready )(RlSound sound);                                 // Checks if a sound is ready
RLAPI void (* rl_update_sound )(RlSound sound, const void *data, int sampleCount); // Update sound buffer with new data
RLAPI void (* rl_unload_wave )(RlWave wave);                                     // Unload wave data
RLAPI void (* rl_unload_sound )(RlSound sound);                                  // Unload sound
RLAPI void (* rl_unload_sound_alias )(RlSound alias);                             // Unload a sound alias (does not deallocate sample data)
RLAPI bool (* rl_export_wave )(RlWave wave, const char *fileName);               // Export wave data to file, returns true on success
RLAPI bool (* rl_export_wave_as_code )(RlWave wave, const char *fileName);         // Export wave sample data to code (.h), returns true on success

// RlWave/RlSound management functions
RLAPI void (* rl_play_sound )(RlSound sound);                                    // Play a sound
RLAPI void (* rl_stop_sound )(RlSound sound);                                    // Stop playing a sound
RLAPI void (* rl_pause_sound )(RlSound sound);                                   // Pause a sound
RLAPI void (* rl_resume_sound )(RlSound sound);                                  // Resume a paused sound
RLAPI bool (* rl_is_sound_playing )(RlSound sound);                               // Check if a sound is currently playing
RLAPI void (* rl_set_sound_volume )(RlSound sound, float volume);                 // Set volume for a sound (1.0 is max level)
RLAPI void (* rl_set_sound_pitch )(RlSound sound, float pitch);                   // Set pitch for a sound (1.0 is base level)
RLAPI void (* rl_set_sound_pan )(RlSound sound, float pan);                       // Set pan for a sound (0.5 is center)
RLAPI RlWave (* rl_wave_copy )(RlWave wave);                                       // Copy a wave to a new wave
RLAPI void (* rl_wave_crop )(RlWave *wave, int initSample, int finalSample);     // Crop a wave to defined samples range
RLAPI void (* rl_wave_format )(RlWave *wave, int sampleRate, int sampleSize, int channels); // Convert wave data to desired format
RLAPI float *(* rl_load_wave_samples )(RlWave wave);                              // Load samples data from wave as a 32bit float data array
RLAPI void (* rl_unload_wave_samples )(float *samples);                         // Unload samples data loaded with (* rl_load_wave_samples )()

// RlMusic management functions
RLAPI RlMusic (* rl_load_music_stream )(const char *fileName);                    // Load music stream from file
RLAPI RlMusic (* rl_load_music_stream_from_memory )(const char *fileType, const unsigned char *data, int dataSize); // Load music stream from data
RLAPI bool (* rl_is_music_ready )(RlMusic music);                                 // Checks if a music stream is ready
RLAPI void (* rl_unload_music_stream )(RlMusic music);                            // Unload music stream
RLAPI void (* rl_play_music_stream )(RlMusic music);                              // Start music playing
RLAPI bool (* rl_is_music_stream_playing )(RlMusic music);                         // Check if music is playing
RLAPI void (* rl_update_music_stream )(RlMusic music);                            // Updates buffers for music streaming
RLAPI void (* rl_stop_music_stream )(RlMusic music);                              // Stop music playing
RLAPI void (* rl_pause_music_stream )(RlMusic music);                             // Pause music playing
RLAPI void (* rl_resume_music_stream )(RlMusic music);                            // Resume playing paused music
RLAPI void (* rl_seek_music_stream )(RlMusic music, float position);              // Seek music to a position (in seconds)
RLAPI void (* rl_set_music_volume )(RlMusic music, float volume);                 // Set volume for music (1.0 is max level)
RLAPI void (* rl_set_music_pitch )(RlMusic music, float pitch);                   // Set pitch for a music (1.0 is base level)
RLAPI void (* rl_set_music_pan )(RlMusic music, float pan);                       // Set pan for a music (0.5 is center)
RLAPI float (* rl_get_music_time_length )(RlMusic music);                          // Get music time length (in seconds)
RLAPI float (* rl_get_music_time_played )(RlMusic music);                          // Get current music time played (in seconds)

// RlAudioStream management functions
RLAPI RlAudioStream (* rl_load_audio_stream )(unsigned int sampleRate, unsigned int sampleSize, unsigned int channels); // Load audio stream (to stream raw audio pcm data)
RLAPI bool (* rl_is_audio_stream_ready )(RlAudioStream stream);                    // Checks if an audio stream is ready
RLAPI void (* rl_unload_audio_stream )(RlAudioStream stream);                     // Unload audio stream and free memory
RLAPI void (* rl_update_audio_stream )(RlAudioStream stream, const void *data, int frameCount); // Update audio stream buffers with data
RLAPI bool (* rl_is_audio_stream_processed )(RlAudioStream stream);                // Check if any audio stream buffers requires refill
RLAPI void (* rl_play_audio_stream )(RlAudioStream stream);                       // Play audio stream
RLAPI void (* rl_pause_audio_stream )(RlAudioStream stream);                      // Pause audio stream
RLAPI void (* rl_resume_audio_stream )(RlAudioStream stream);                     // Resume audio stream
RLAPI bool (* rl_is_audio_stream_playing )(RlAudioStream stream);                  // Check if audio stream is playing
RLAPI void (* rl_stop_audio_stream )(RlAudioStream stream);                       // Stop audio stream
RLAPI void (* rl_set_audio_stream_volume )(RlAudioStream stream, float volume);    // Set volume for audio stream (1.0 is max level)
RLAPI void (* rl_set_audio_stream_pitch )(RlAudioStream stream, float pitch);      // Set pitch for audio stream (1.0 is base level)
RLAPI void (* rl_set_audio_stream_pan )(RlAudioStream stream, float pan);          // Set pan for audio stream (0.5 is centered)
RLAPI void (* rl_set_audio_stream_buffer_size_default )(int size);                 // Default size for new audio streams
RLAPI void (* rl_set_audio_stream_callback )(RlAudioStream stream, RlAudioCallback callback); // Audio thread callback to request new data

RLAPI void (* rl_attach_audio_stream_processor )(RlAudioStream stream, RlAudioCallback processor); // Attach audio stream processor to stream, receives the samples as <float>s
RLAPI void (* rl_detach_audio_stream_processor )(RlAudioStream stream, RlAudioCallback processor); // Detach audio stream processor from stream

RLAPI void (* rl_attach_audio_mixed_processor )(RlAudioCallback processor); // Attach audio stream processor to the entire audio pipeline, receives the samples as <float>s
RLAPI void (* rl_detach_audio_mixed_processor )(RlAudioCallback processor); // Detach audio stream processor from the entire audio pipeline

#if defined(__cplusplus)
}
#endif

#endif // RAYLIB_H


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
HANDLE rl_lib_handle = NULL;
void rl_init_lib(const char* raylib_name){
  rl_lib_handle = LoadLibrary(raylib_name);
  rl_detach_audio_mixed_processor = (RLAPI void  (*) (RlAudioCallback processor)) GetProcAddress(rl_lib_handle,"DetachAudioMixedProcessor");
  rl_attach_audio_mixed_processor = (RLAPI void  (*) (RlAudioCallback processor)) GetProcAddress(rl_lib_handle,"AttachAudioMixedProcessor");
  rl_detach_audio_stream_processor = (RLAPI void  (*) (RlAudioStream stream, RlAudioCallback processor)) GetProcAddress(rl_lib_handle,"DetachAudioStreamProcessor");
  rl_attach_audio_stream_processor = (RLAPI void  (*) (RlAudioStream stream, RlAudioCallback processor)) GetProcAddress(rl_lib_handle,"AttachAudioStreamProcessor");
  rl_set_audio_stream_callback = (RLAPI void  (*) (RlAudioStream stream, RlAudioCallback callback)) GetProcAddress(rl_lib_handle,"SetAudioStreamCallback");
  rl_set_audio_stream_buffer_size_default = (RLAPI void  (*) (int size)) GetProcAddress(rl_lib_handle,"SetAudioStreamBufferSizeDefault");
  rl_set_audio_stream_pan = (RLAPI void  (*) (RlAudioStream stream, float pan)) GetProcAddress(rl_lib_handle,"SetAudioStreamPan");
  rl_set_audio_stream_pitch = (RLAPI void  (*) (RlAudioStream stream, float pitch)) GetProcAddress(rl_lib_handle,"SetAudioStreamPitch");
  rl_set_audio_stream_volume = (RLAPI void  (*) (RlAudioStream stream, float volume)) GetProcAddress(rl_lib_handle,"SetAudioStreamVolume");
  rl_stop_audio_stream = (RLAPI void  (*) (RlAudioStream stream)) GetProcAddress(rl_lib_handle,"StopAudioStream");
  rl_is_audio_stream_playing = (RLAPI bool  (*) (RlAudioStream stream)) GetProcAddress(rl_lib_handle,"IsAudioStreamPlaying");
  rl_resume_audio_stream = (RLAPI void  (*) (RlAudioStream stream)) GetProcAddress(rl_lib_handle,"ResumeAudioStream");
  rl_pause_audio_stream = (RLAPI void  (*) (RlAudioStream stream)) GetProcAddress(rl_lib_handle,"PauseAudioStream");
  rl_play_audio_stream = (RLAPI void  (*) (RlAudioStream stream)) GetProcAddress(rl_lib_handle,"PlayAudioStream");
  rl_is_audio_stream_processed = (RLAPI bool  (*) (RlAudioStream stream)) GetProcAddress(rl_lib_handle,"IsAudioStreamProcessed");
  rl_update_audio_stream = (RLAPI void  (*) (RlAudioStream stream, const void *data, int frameCount)) GetProcAddress(rl_lib_handle,"UpdateAudioStream");
  rl_unload_audio_stream = (RLAPI void  (*) (RlAudioStream stream)) GetProcAddress(rl_lib_handle,"UnloadAudioStream");
  rl_is_audio_stream_ready = (RLAPI bool  (*) (RlAudioStream stream)) GetProcAddress(rl_lib_handle,"IsAudioStreamReady");
  rl_load_audio_stream = (RLAPI RlAudioStream  (*) (unsigned int sampleRate, unsigned int sampleSize, unsigned int channels)) GetProcAddress(rl_lib_handle,"LoadAudioStream");
  rl_get_music_time_played = (RLAPI float  (*) (RlMusic music)) GetProcAddress(rl_lib_handle,"GetMusicTimePlayed");
  rl_get_music_time_length = (RLAPI float  (*) (RlMusic music)) GetProcAddress(rl_lib_handle,"GetMusicTimeLength");
  rl_set_music_pan = (RLAPI void  (*) (RlMusic music, float pan)) GetProcAddress(rl_lib_handle,"SetMusicPan");
  rl_set_music_pitch = (RLAPI void  (*) (RlMusic music, float pitch)) GetProcAddress(rl_lib_handle,"SetMusicPitch");
  rl_set_music_volume = (RLAPI void  (*) (RlMusic music, float volume)) GetProcAddress(rl_lib_handle,"SetMusicVolume");
  rl_seek_music_stream = (RLAPI void  (*) (RlMusic music, float position)) GetProcAddress(rl_lib_handle,"SeekMusicStream");
  rl_resume_music_stream = (RLAPI void  (*) (RlMusic music)) GetProcAddress(rl_lib_handle,"ResumeMusicStream");
  rl_pause_music_stream = (RLAPI void  (*) (RlMusic music)) GetProcAddress(rl_lib_handle,"PauseMusicStream");
  rl_stop_music_stream = (RLAPI void  (*) (RlMusic music)) GetProcAddress(rl_lib_handle,"StopMusicStream");
  rl_update_music_stream = (RLAPI void  (*) (RlMusic music)) GetProcAddress(rl_lib_handle,"UpdateMusicStream");
  rl_is_music_stream_playing = (RLAPI bool  (*) (RlMusic music)) GetProcAddress(rl_lib_handle,"IsMusicStreamPlaying");
  rl_play_music_stream = (RLAPI void  (*) (RlMusic music)) GetProcAddress(rl_lib_handle,"PlayMusicStream");
  rl_unload_music_stream = (RLAPI void  (*) (RlMusic music)) GetProcAddress(rl_lib_handle,"UnloadMusicStream");
  rl_is_music_ready = (RLAPI bool  (*) (RlMusic music)) GetProcAddress(rl_lib_handle,"IsMusicReady");
  rl_load_music_stream_from_memory = (RLAPI RlMusic  (*) (const char *fileType, const unsigned char *data, int dataSize)) GetProcAddress(rl_lib_handle,"LoadMusicStreamFromMemory");
  rl_load_music_stream = (RLAPI RlMusic  (*) (const char *fileName)) GetProcAddress(rl_lib_handle,"LoadMusicStream");
  rl_unload_wave_samples = (RLAPI void  (*) (float *samples)) GetProcAddress(rl_lib_handle,"UnloadWaveSamples");
  rl_load_wave_samples = (RLAPI float * (*) (RlWave wave)) GetProcAddress(rl_lib_handle,"LoadWaveSamples");
  rl_wave_format = (RLAPI void  (*) (RlWave *wave, int sampleRate, int sampleSize, int channels)) GetProcAddress(rl_lib_handle,"WaveFormat");
  rl_wave_crop = (RLAPI void  (*) (RlWave *wave, int initSample, int finalSample)) GetProcAddress(rl_lib_handle,"WaveCrop");
  rl_wave_copy = (RLAPI RlWave  (*) (RlWave wave)) GetProcAddress(rl_lib_handle,"WaveCopy");
  rl_set_sound_pan = (RLAPI void  (*) (RlSound sound, float pan)) GetProcAddress(rl_lib_handle,"SetSoundPan");
  rl_set_sound_pitch = (RLAPI void  (*) (RlSound sound, float pitch)) GetProcAddress(rl_lib_handle,"SetSoundPitch");
  rl_set_sound_volume = (RLAPI void  (*) (RlSound sound, float volume)) GetProcAddress(rl_lib_handle,"SetSoundVolume");
  rl_is_sound_playing = (RLAPI bool  (*) (RlSound sound)) GetProcAddress(rl_lib_handle,"IsSoundPlaying");
  rl_resume_sound = (RLAPI void  (*) (RlSound sound)) GetProcAddress(rl_lib_handle,"ResumeSound");
  rl_pause_sound = (RLAPI void  (*) (RlSound sound)) GetProcAddress(rl_lib_handle,"PauseSound");
  rl_stop_sound = (RLAPI void  (*) (RlSound sound)) GetProcAddress(rl_lib_handle,"StopSound");
  rl_play_sound = (RLAPI void  (*) (RlSound sound)) GetProcAddress(rl_lib_handle,"PlaySound");
  rl_export_wave_as_code = (RLAPI bool  (*) (RlWave wave, const char *fileName)) GetProcAddress(rl_lib_handle,"ExportWaveAsCode");
  rl_export_wave = (RLAPI bool  (*) (RlWave wave, const char *fileName)) GetProcAddress(rl_lib_handle,"ExportWave");
  rl_unload_sound_alias = (RLAPI void  (*) (RlSound alias)) GetProcAddress(rl_lib_handle,"UnloadSoundAlias");
  rl_unload_sound = (RLAPI void  (*) (RlSound sound)) GetProcAddress(rl_lib_handle,"UnloadSound");
  rl_unload_wave = (RLAPI void  (*) (RlWave wave)) GetProcAddress(rl_lib_handle,"UnloadWave");
  rl_update_sound = (RLAPI void  (*) (RlSound sound, const void *data, int sampleCount)) GetProcAddress(rl_lib_handle,"UpdateSound");
  rl_is_sound_ready = (RLAPI bool  (*) (RlSound sound)) GetProcAddress(rl_lib_handle,"IsSoundReady");
  rl_load_sound_alias = (RLAPI RlSound  (*) (RlSound source)) GetProcAddress(rl_lib_handle,"LoadSoundAlias");
  rl_load_sound_from_wave = (RLAPI RlSound  (*) (RlWave wave)) GetProcAddress(rl_lib_handle,"LoadSoundFromWave");
  rl_load_sound = (RLAPI RlSound  (*) (const char *fileName)) GetProcAddress(rl_lib_handle,"LoadSound");
  rl_is_wave_ready = (RLAPI bool  (*) (RlWave wave)) GetProcAddress(rl_lib_handle,"IsWaveReady");
  rl_load_wave_from_memory = (RLAPI RlWave  (*) (const char *fileType, const unsigned char *fileData, int dataSize)) GetProcAddress(rl_lib_handle,"LoadWaveFromMemory");
  rl_load_wave = (RLAPI RlWave  (*) (const char *fileName)) GetProcAddress(rl_lib_handle,"LoadWave");
  rl_get_master_volume = (RLAPI float  (*) (void)) GetProcAddress(rl_lib_handle,"GetMasterVolume");
  rl_set_master_volume = (RLAPI void  (*) (float volume)) GetProcAddress(rl_lib_handle,"SetMasterVolume");
  rl_is_audio_device_ready = (RLAPI bool  (*) (void)) GetProcAddress(rl_lib_handle,"IsAudioDeviceReady");
  rl_close_audio_device = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"CloseAudioDevice");
  rl_init_audio_device = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"InitAudioDevice");
  rl_get_ray_collision_quad = (RLAPI RlRayCollision  (*) (RlRay ray, RlVector3 p1, RlVector3 p2, RlVector3 p3, RlVector3 p4)) GetProcAddress(rl_lib_handle,"GetRayCollisionQuad");
  rl_get_ray_collision_triangle = (RLAPI RlRayCollision  (*) (RlRay ray, RlVector3 p1, RlVector3 p2, RlVector3 p3)) GetProcAddress(rl_lib_handle,"GetRayCollisionTriangle");
  rl_get_ray_collision_mesh = (RLAPI RlRayCollision  (*) (RlRay ray, RlMesh mesh, RlMatrix transform)) GetProcAddress(rl_lib_handle,"GetRayCollisionMesh");
  rl_get_ray_collision_box = (RLAPI RlRayCollision  (*) (RlRay ray, RlBoundingBox box)) GetProcAddress(rl_lib_handle,"GetRayCollisionBox");
  rl_get_ray_collision_sphere = (RLAPI RlRayCollision  (*) (RlRay ray, RlVector3 center, float radius)) GetProcAddress(rl_lib_handle,"GetRayCollisionSphere");
  rl_check_collision_box_sphere = (RLAPI bool  (*) (RlBoundingBox box, RlVector3 center, float radius)) GetProcAddress(rl_lib_handle,"CheckCollisionBoxSphere");
  rl_check_collision_boxes = (RLAPI bool  (*) (RlBoundingBox box1, RlBoundingBox box2)) GetProcAddress(rl_lib_handle,"CheckCollisionBoxes");
  rl_check_collision_spheres = (RLAPI bool  (*) (RlVector3 center1, float radius1, RlVector3 center2, float radius2)) GetProcAddress(rl_lib_handle,"CheckCollisionSpheres");
  rl_is_model_animation_valid = (RLAPI bool  (*) (RlModel model, RlModelAnimation anim)) GetProcAddress(rl_lib_handle,"IsModelAnimationValid");
  rl_unload_model_animations = (RLAPI void  (*) (RlModelAnimation *animations, int animCount)) GetProcAddress(rl_lib_handle,"UnloadModelAnimations");
  rl_unload_model_animation = (RLAPI void  (*) (RlModelAnimation anim)) GetProcAddress(rl_lib_handle,"UnloadModelAnimation");
  rl_update_model_animation = (RLAPI void  (*) (RlModel model, RlModelAnimation anim, int frame)) GetProcAddress(rl_lib_handle,"UpdateModelAnimation");
  rl_load_model_animations = (RLAPI RlModelAnimation * (*) (const char *fileName, int *animCount)) GetProcAddress(rl_lib_handle,"LoadModelAnimations");
  rl_set_model_mesh_material = (RLAPI void  (*) (RlModel *model, int meshId, int materialId)) GetProcAddress(rl_lib_handle,"SetModelMeshMaterial");
  rl_set_material_texture = (RLAPI void  (*) (RlMaterial *material, int mapType, RlTexture2D texture)) GetProcAddress(rl_lib_handle,"SetMaterialTexture");
  rl_unload_material = (RLAPI void  (*) (RlMaterial material)) GetProcAddress(rl_lib_handle,"UnloadMaterial");
  rl_is_material_ready = (RLAPI bool  (*) (RlMaterial material)) GetProcAddress(rl_lib_handle,"IsMaterialReady");
  rl_load_material_default = (RLAPI RlMaterial  (*) (void)) GetProcAddress(rl_lib_handle,"LoadMaterialDefault");
  rl_load_materials = (RLAPI RlMaterial * (*) (const char *fileName, int *materialCount)) GetProcAddress(rl_lib_handle,"LoadMaterials");
  rl_gen_mesh_cubicmap = (RLAPI RlMesh  (*) (RlImage cubicmap, RlVector3 cubeSize)) GetProcAddress(rl_lib_handle,"GenMeshCubicmap");
  rl_gen_mesh_heightmap = (RLAPI RlMesh  (*) (RlImage heightmap, RlVector3 size)) GetProcAddress(rl_lib_handle,"GenMeshHeightmap");
  rl_gen_mesh_knot = (RLAPI RlMesh  (*) (float radius, float size, int radSeg, int sides)) GetProcAddress(rl_lib_handle,"GenMeshKnot");
  rl_gen_mesh_torus = (RLAPI RlMesh  (*) (float radius, float size, int radSeg, int sides)) GetProcAddress(rl_lib_handle,"GenMeshTorus");
  rl_gen_mesh_cone = (RLAPI RlMesh  (*) (float radius, float height, int slices)) GetProcAddress(rl_lib_handle,"GenMeshCone");
  rl_gen_mesh_cylinder = (RLAPI RlMesh  (*) (float radius, float height, int slices)) GetProcAddress(rl_lib_handle,"GenMeshCylinder");
  rl_gen_mesh_hemi_sphere = (RLAPI RlMesh  (*) (float radius, int rings, int slices)) GetProcAddress(rl_lib_handle,"GenMeshHemiSphere");
  rl_gen_mesh_sphere = (RLAPI RlMesh  (*) (float radius, int rings, int slices)) GetProcAddress(rl_lib_handle,"GenMeshSphere");
  rl_gen_mesh_cube = (RLAPI RlMesh  (*) (float width, float height, float length)) GetProcAddress(rl_lib_handle,"GenMeshCube");
  rl_gen_mesh_plane = (RLAPI RlMesh  (*) (float width, float length, int resX, int resZ)) GetProcAddress(rl_lib_handle,"GenMeshPlane");
  rl_gen_mesh_poly = (RLAPI RlMesh  (*) (int sides, float radius)) GetProcAddress(rl_lib_handle,"GenMeshPoly");
  rl_gen_mesh_tangents = (RLAPI void  (*) (RlMesh *mesh)) GetProcAddress(rl_lib_handle,"GenMeshTangents");
  rl_get_mesh_bounding_box = (RLAPI RlBoundingBox  (*) (RlMesh mesh)) GetProcAddress(rl_lib_handle,"GetMeshBoundingBox");
  rl_export_mesh = (RLAPI bool  (*) (RlMesh mesh, const char *fileName)) GetProcAddress(rl_lib_handle,"ExportMesh");
  rl_draw_mesh_instanced = (RLAPI void  (*) (RlMesh mesh, RlMaterial material, const RlMatrix *transforms, int instances)) GetProcAddress(rl_lib_handle,"DrawMeshInstanced");
  rl_draw_mesh = (RLAPI void  (*) (RlMesh mesh, RlMaterial material, RlMatrix transform)) GetProcAddress(rl_lib_handle,"DrawMesh");
  rl_unload_mesh = (RLAPI void  (*) (RlMesh mesh)) GetProcAddress(rl_lib_handle,"UnloadMesh");
  rl_update_mesh_buffer = (RLAPI void  (*) (RlMesh mesh, int index, const void *data, int dataSize, int offset)) GetProcAddress(rl_lib_handle,"UpdateMeshBuffer");
  rl_upload_mesh = (RLAPI void  (*) (RlMesh *mesh, bool dynamic)) GetProcAddress(rl_lib_handle,"UploadMesh");
  rl_draw_billboard_pro = (RLAPI void  (*) (RlCamera camera, RlTexture2D texture, RlRectangle source, RlVector3 position, RlVector3 up, RlVector2 size, RlVector2 origin, float rotation, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawBillboardPro");
  rl_draw_billboard_rec = (RLAPI void  (*) (RlCamera camera, RlTexture2D texture, RlRectangle source, RlVector3 position, RlVector2 size, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawBillboardRec");
  rl_draw_billboard = (RLAPI void  (*) (RlCamera camera, RlTexture2D texture, RlVector3 position, float size, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawBillboard");
  rl_draw_bounding_box = (RLAPI void  (*) (RlBoundingBox box, RlColor color)) GetProcAddress(rl_lib_handle,"DrawBoundingBox");
  rl_draw_model_wires_ex = (RLAPI void  (*) (RlModel model, RlVector3 position, RlVector3 rotationAxis, float rotationAngle, RlVector3 scale, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawModelWiresEx");
  rl_draw_model_wires = (RLAPI void  (*) (RlModel model, RlVector3 position, float scale, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawModelWires");
  rl_draw_model_ex = (RLAPI void  (*) (RlModel model, RlVector3 position, RlVector3 rotationAxis, float rotationAngle, RlVector3 scale, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawModelEx");
  rl_draw_model = (RLAPI void  (*) (RlModel model, RlVector3 position, float scale, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawModel");
  rl_get_model_bounding_box = (RLAPI RlBoundingBox  (*) (RlModel model)) GetProcAddress(rl_lib_handle,"GetModelBoundingBox");
  rl_unload_model = (RLAPI void  (*) (RlModel model)) GetProcAddress(rl_lib_handle,"UnloadModel");
  rl_is_model_ready = (RLAPI bool  (*) (RlModel model)) GetProcAddress(rl_lib_handle,"IsModelReady");
  rl_load_model_from_mesh = (RLAPI RlModel  (*) (RlMesh mesh)) GetProcAddress(rl_lib_handle,"LoadModelFromMesh");
  rl_load_model = (RLAPI RlModel  (*) (const char *fileName)) GetProcAddress(rl_lib_handle,"LoadModel");
  rl_draw_grid = (RLAPI void  (*) (int slices, float spacing)) GetProcAddress(rl_lib_handle,"DrawGrid");
  rl_draw_ray = (RLAPI void  (*) (RlRay ray, RlColor color)) GetProcAddress(rl_lib_handle,"DrawRay");
  rl_draw_plane = (RLAPI void  (*) (RlVector3 centerPos, RlVector2 size, RlColor color)) GetProcAddress(rl_lib_handle,"DrawPlane");
  rl_draw_capsule_wires = (RLAPI void  (*) (RlVector3 startPos, RlVector3 endPos, float radius, int slices, int rings, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCapsuleWires");
  rl_draw_capsule = (RLAPI void  (*) (RlVector3 startPos, RlVector3 endPos, float radius, int slices, int rings, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCapsule");
  rl_draw_cylinder_wires_ex = (RLAPI void  (*) (RlVector3 startPos, RlVector3 endPos, float startRadius, float endRadius, int sides, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCylinderWiresEx");
  rl_draw_cylinder_wires = (RLAPI void  (*) (RlVector3 position, float radiusTop, float radiusBottom, float height, int slices, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCylinderWires");
  rl_draw_cylinder_ex = (RLAPI void  (*) (RlVector3 startPos, RlVector3 endPos, float startRadius, float endRadius, int sides, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCylinderEx");
  rl_draw_cylinder = (RLAPI void  (*) (RlVector3 position, float radiusTop, float radiusBottom, float height, int slices, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCylinder");
  rl_draw_sphere_wires = (RLAPI void  (*) (RlVector3 centerPos, float radius, int rings, int slices, RlColor color)) GetProcAddress(rl_lib_handle,"DrawSphereWires");
  rl_draw_sphere_ex = (RLAPI void  (*) (RlVector3 centerPos, float radius, int rings, int slices, RlColor color)) GetProcAddress(rl_lib_handle,"DrawSphereEx");
  rl_draw_sphere = (RLAPI void  (*) (RlVector3 centerPos, float radius, RlColor color)) GetProcAddress(rl_lib_handle,"DrawSphere");
  rl_draw_cube_wires_v = (RLAPI void  (*) (RlVector3 position, RlVector3 size, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCubeWiresV");
  rl_draw_cube_wires = (RLAPI void  (*) (RlVector3 position, float width, float height, float length, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCubeWires");
  rl_draw_cube_v = (RLAPI void  (*) (RlVector3 position, RlVector3 size, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCubeV");
  rl_draw_cube = (RLAPI void  (*) (RlVector3 position, float width, float height, float length, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCube");
  rl_draw_triangle_strip_3_d = (RLAPI void  (*) (RlVector3 *points, int pointCount, RlColor color)) GetProcAddress(rl_lib_handle,"DrawTriangleStrip3D");
  rl_draw_triangle_3_d = (RLAPI void  (*) (RlVector3 v1, RlVector3 v2, RlVector3 v3, RlColor color)) GetProcAddress(rl_lib_handle,"DrawTriangle3D");
  rl_draw_circle_3_d = (RLAPI void  (*) (RlVector3 center, float radius, RlVector3 rotationAxis, float rotationAngle, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCircle3D");
  rl_draw_point_3_d = (RLAPI void  (*) (RlVector3 position, RlColor color)) GetProcAddress(rl_lib_handle,"DrawPoint3D");
  rl_draw_line_3_d = (RLAPI void  (*) (RlVector3 startPos, RlVector3 endPos, RlColor color)) GetProcAddress(rl_lib_handle,"DrawLine3D");
  rl_text_to_integer = (RLAPI int  (*) (const char *text)) GetProcAddress(rl_lib_handle,"TextToInteger");
  rl_text_to_pascal = (RLAPI const char * (*) (const char *text)) GetProcAddress(rl_lib_handle,"TextToPascal");
  rl_text_to_lower = (RLAPI const char * (*) (const char *text)) GetProcAddress(rl_lib_handle,"TextToLower");
  rl_text_to_upper = (RLAPI const char * (*) (const char *text)) GetProcAddress(rl_lib_handle,"TextToUpper");
  rl_text_find_index = (RLAPI int  (*) (const char *text, const char *find)) GetProcAddress(rl_lib_handle,"TextFindIndex");
  rl_text_append = (RLAPI void  (*) (char *text, const char *append, int *position)) GetProcAddress(rl_lib_handle,"TextAppend");
  rl_text_split = (RLAPI const char ** (*) (const char *text, char delimiter, int *count)) GetProcAddress(rl_lib_handle,"TextSplit");
  rl_text_join = (RLAPI const char * (*) (const char **textList, int count, const char *delimiter)) GetProcAddress(rl_lib_handle,"TextJoin");
  rl_text_insert = (RLAPI char * (*) (const char *text, const char *insert, int position)) GetProcAddress(rl_lib_handle,"TextInsert");
  rl_text_replace = (RLAPI char * (*) (char *text, const char *replace, const char *by)) GetProcAddress(rl_lib_handle,"TextReplace");
  rl_text_subtext = (RLAPI const char * (*) (const char *text, int position, int length)) GetProcAddress(rl_lib_handle,"TextSubtext");
  rl_text_format = (RLAPI const char * (*) (const char *text, ...)) GetProcAddress(rl_lib_handle,"TextFormat");
  rl_text_length = (RLAPI unsigned int  (*) (const char *text)) GetProcAddress(rl_lib_handle,"TextLength");
  rl_text_is_equal = (RLAPI bool  (*) (const char *text1, const char *text2)) GetProcAddress(rl_lib_handle,"TextIsEqual");
  rl_text_copy = (RLAPI int  (*) (char *dst, const char *src)) GetProcAddress(rl_lib_handle,"TextCopy");
  rl_codepoint_to_u_t_f_8 = (RLAPI const char * (*) (int codepoint, int *utf8Size)) GetProcAddress(rl_lib_handle,"CodepointToUTF8");
  rl_get_codepoint_previous = (RLAPI int  (*) (const char *text, int *codepointSize)) GetProcAddress(rl_lib_handle,"GetCodepointPrevious");
  rl_get_codepoint_next = (RLAPI int  (*) (const char *text, int *codepointSize)) GetProcAddress(rl_lib_handle,"GetCodepointNext");
  rl_get_codepoint = (RLAPI int  (*) (const char *text, int *codepointSize)) GetProcAddress(rl_lib_handle,"GetCodepoint");
  rl_get_codepoint_count = (RLAPI int  (*) (const char *text)) GetProcAddress(rl_lib_handle,"GetCodepointCount");
  rl_unload_codepoints = (RLAPI void  (*) (int *codepoints)) GetProcAddress(rl_lib_handle,"UnloadCodepoints");
  rl_load_codepoints = (RLAPI int * (*) (const char *text, int *count)) GetProcAddress(rl_lib_handle,"LoadCodepoints");
  rl_unload_u_t_f_8 = (RLAPI void  (*) (char *text)) GetProcAddress(rl_lib_handle,"UnloadUTF8");
  rl_load_u_t_f_8 = (RLAPI char * (*) (const int *codepoints, int length)) GetProcAddress(rl_lib_handle,"LoadUTF8");
  rl_get_glyph_atlas_rec = (RLAPI RlRectangle  (*) (RlFont font, int codepoint)) GetProcAddress(rl_lib_handle,"GetGlyphAtlasRec");
  rl_get_glyph_info = (RLAPI RlGlyphInfo  (*) (RlFont font, int codepoint)) GetProcAddress(rl_lib_handle,"GetGlyphInfo");
  rl_get_glyph_index = (RLAPI int  (*) (RlFont font, int codepoint)) GetProcAddress(rl_lib_handle,"GetGlyphIndex");
  rl_measure_text_ex = (RLAPI RlVector2  (*) (RlFont font, const char *text, float fontSize, float spacing)) GetProcAddress(rl_lib_handle,"MeasureTextEx");
  rl_measure_text = (RLAPI int  (*) (const char *text, int fontSize)) GetProcAddress(rl_lib_handle,"MeasureText");
  rl_set_text_line_spacing = (RLAPI void  (*) (int spacing)) GetProcAddress(rl_lib_handle,"SetTextLineSpacing");
  rl_draw_text_codepoints = (RLAPI void  (*) (RlFont font, const int *codepoints, int codepointCount, RlVector2 position, float fontSize, float spacing, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawTextCodepoints");
  rl_draw_text_codepoint = (RLAPI void  (*) (RlFont font, int codepoint, RlVector2 position, float fontSize, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawTextCodepoint");
  rl_draw_text_pro = (RLAPI void  (*) (RlFont font, const char *text, RlVector2 position, RlVector2 origin, float rotation, float fontSize, float spacing, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawTextPro");
  rl_draw_text_ex = (RLAPI void  (*) (RlFont font, const char *text, RlVector2 position, float fontSize, float spacing, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawTextEx");
  rl_draw_text = (RLAPI void  (*) (const char *text, int posX, int posY, int fontSize, RlColor color)) GetProcAddress(rl_lib_handle,"DrawText");
  rl_draw_f_p_s = (RLAPI void  (*) (int posX, int posY)) GetProcAddress(rl_lib_handle,"DrawFPS");
  rl_export_font_as_code = (RLAPI bool  (*) (RlFont font, const char *fileName)) GetProcAddress(rl_lib_handle,"ExportFontAsCode");
  rl_unload_font = (RLAPI void  (*) (RlFont font)) GetProcAddress(rl_lib_handle,"UnloadFont");
  rl_unload_font_data = (RLAPI void  (*) (RlGlyphInfo *glyphs, int glyphCount)) GetProcAddress(rl_lib_handle,"UnloadFontData");
  rl_gen_image_font_atlas = (RLAPI RlImage  (*) (const RlGlyphInfo *glyphs, RlRectangle **glyphRecs, int glyphCount, int fontSize, int padding, int packMethod)) GetProcAddress(rl_lib_handle,"GenImageFontAtlas");
  rl_load_font_data = (RLAPI RlGlyphInfo * (*) (const unsigned char *fileData, int dataSize, int fontSize, int *codepoints, int codepointCount, int type)) GetProcAddress(rl_lib_handle,"LoadFontData");
  rl_is_font_ready = (RLAPI bool  (*) (RlFont font)) GetProcAddress(rl_lib_handle,"IsFontReady");
  rl_load_font_from_memory = (RLAPI RlFont  (*) (const char *fileType, const unsigned char *fileData, int dataSize, int fontSize, int *codepoints, int codepointCount)) GetProcAddress(rl_lib_handle,"LoadFontFromMemory");
  rl_load_font_from_image = (RLAPI RlFont  (*) (RlImage image, RlColor key, int firstChar)) GetProcAddress(rl_lib_handle,"LoadFontFromImage");
  rl_load_font_ex = (RLAPI RlFont  (*) (const char *fileName, int fontSize, int *codepoints, int codepointCount)) GetProcAddress(rl_lib_handle,"LoadFontEx");
  rl_load_font = (RLAPI RlFont  (*) (const char *fileName)) GetProcAddress(rl_lib_handle,"LoadFont");
  rl_get_font_default = (RLAPI RlFont  (*) (void)) GetProcAddress(rl_lib_handle,"GetFontDefault");
  rl_get_pixel_data_size = (RLAPI int  (*) (int width, int height, int format)) GetProcAddress(rl_lib_handle,"GetPixelDataSize");
  rl_set_pixel_color = (RLAPI void  (*) (void *dstPtr, RlColor color, int format)) GetProcAddress(rl_lib_handle,"SetPixelColor");
  rl_get_pixel_color = (RLAPI RlColor  (*) (void *srcPtr, int format)) GetProcAddress(rl_lib_handle,"GetPixelColor");
  rl_get_color = (RLAPI RlColor  (*) (unsigned int hexValue)) GetProcAddress(rl_lib_handle,"GetColor");
  rl_color_alpha_blend = (RLAPI RlColor  (*) (RlColor dst, RlColor src, RlColor tint)) GetProcAddress(rl_lib_handle,"ColorAlphaBlend");
  rl_color_alpha = (RLAPI RlColor  (*) (RlColor color, float alpha)) GetProcAddress(rl_lib_handle,"ColorAlpha");
  rl_color_contrast = (RLAPI RlColor  (*) (RlColor color, float contrast)) GetProcAddress(rl_lib_handle,"ColorContrast");
  rl_color_brightness = (RLAPI RlColor  (*) (RlColor color, float factor)) GetProcAddress(rl_lib_handle,"ColorBrightness");
  rl_color_tint = (RLAPI RlColor  (*) (RlColor color, RlColor tint)) GetProcAddress(rl_lib_handle,"ColorTint");
  rl_color_from_h_s_v = (RLAPI RlColor  (*) (float hue, float saturation, float value)) GetProcAddress(rl_lib_handle,"ColorFromHSV");
  rl_color_to_h_s_v = (RLAPI RlVector3  (*) (RlColor color)) GetProcAddress(rl_lib_handle,"ColorToHSV");
  rl_color_from_normalized = (RLAPI RlColor  (*) (RlVector4 normalized)) GetProcAddress(rl_lib_handle,"ColorFromNormalized");
  rl_color_normalize = (RLAPI RlVector4  (*) (RlColor color)) GetProcAddress(rl_lib_handle,"ColorNormalize");
  rl_color_to_int = (RLAPI int  (*) (RlColor color)) GetProcAddress(rl_lib_handle,"ColorToInt");
  rl_fade = (RLAPI RlColor  (*) (RlColor color, float alpha)) GetProcAddress(rl_lib_handle,"Fade");
  rl_draw_texture_n_patch = (RLAPI void  (*) (RlTexture2D texture, RlNPatchInfo nPatchInfo, RlRectangle dest, RlVector2 origin, float rotation, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawTextureNPatch");
  rl_draw_texture_pro = (RLAPI void  (*) (RlTexture2D texture, RlRectangle source, RlRectangle dest, RlVector2 origin, float rotation, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawTexturePro");
  rl_draw_texture_rec = (RLAPI void  (*) (RlTexture2D texture, RlRectangle source, RlVector2 position, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawTextureRec");
  rl_draw_texture_ex = (RLAPI void  (*) (RlTexture2D texture, RlVector2 position, float rotation, float scale, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawTextureEx");
  rl_draw_texture_v = (RLAPI void  (*) (RlTexture2D texture, RlVector2 position, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawTextureV");
  rl_draw_texture = (RLAPI void  (*) (RlTexture2D texture, int posX, int posY, RlColor tint)) GetProcAddress(rl_lib_handle,"DrawTexture");
  rl_set_texture_wrap = (RLAPI void  (*) (RlTexture2D texture, int wrap)) GetProcAddress(rl_lib_handle,"SetTextureWrap");
  rl_set_texture_filter = (RLAPI void  (*) (RlTexture2D texture, int filter)) GetProcAddress(rl_lib_handle,"SetTextureFilter");
  rl_gen_texture_mipmaps = (RLAPI void  (*) (RlTexture2D *texture)) GetProcAddress(rl_lib_handle,"GenTextureMipmaps");
  rl_update_texture_rec = (RLAPI void  (*) (RlTexture2D texture, RlRectangle rec, const void *pixels)) GetProcAddress(rl_lib_handle,"UpdateTextureRec");
  rl_update_texture = (RLAPI void  (*) (RlTexture2D texture, const void *pixels)) GetProcAddress(rl_lib_handle,"UpdateTexture");
  rl_unload_render_texture = (RLAPI void  (*) (RlRenderTexture2D target)) GetProcAddress(rl_lib_handle,"UnloadRenderTexture");
  rl_is_render_texture_ready = (RLAPI bool  (*) (RlRenderTexture2D target)) GetProcAddress(rl_lib_handle,"IsRenderTextureReady");
  rl_unload_texture = (RLAPI void  (*) (RlTexture2D texture)) GetProcAddress(rl_lib_handle,"UnloadTexture");
  rl_is_texture_ready = (RLAPI bool  (*) (RlTexture2D texture)) GetProcAddress(rl_lib_handle,"IsTextureReady");
  rl_load_render_texture = (RLAPI RlRenderTexture2D  (*) (int width, int height)) GetProcAddress(rl_lib_handle,"LoadRenderTexture");
  rl_load_texture_cubemap = (RLAPI RlTextureCubemap  (*) (RlImage image, int layout)) GetProcAddress(rl_lib_handle,"LoadTextureCubemap");
  rl_load_texture_from_image = (RLAPI RlTexture2D  (*) (RlImage image)) GetProcAddress(rl_lib_handle,"LoadTextureFromImage");
  rl_load_texture = (RLAPI RlTexture2D  (*) (const char *fileName)) GetProcAddress(rl_lib_handle,"LoadTexture");
  rl_image_draw_text_ex = (RLAPI void  (*) (RlImage *dst, RlFont font, const char *text, RlVector2 position, float fontSize, float spacing, RlColor tint)) GetProcAddress(rl_lib_handle,"ImageDrawTextEx");
  rl_image_draw_text = (RLAPI void  (*) (RlImage *dst, const char *text, int posX, int posY, int fontSize, RlColor color)) GetProcAddress(rl_lib_handle,"ImageDrawText");
  rl_image_draw = (RLAPI void  (*) (RlImage *dst, RlImage src, RlRectangle srcRec, RlRectangle dstRec, RlColor tint)) GetProcAddress(rl_lib_handle,"ImageDraw");
  rl_image_draw_rectangle_lines = (RLAPI void  (*) (RlImage *dst, RlRectangle rec, int thick, RlColor color)) GetProcAddress(rl_lib_handle,"ImageDrawRectangleLines");
  rl_image_draw_rectangle_rec = (RLAPI void  (*) (RlImage *dst, RlRectangle rec, RlColor color)) GetProcAddress(rl_lib_handle,"ImageDrawRectangleRec");
  rl_image_draw_rectangle_v = (RLAPI void  (*) (RlImage *dst, RlVector2 position, RlVector2 size, RlColor color)) GetProcAddress(rl_lib_handle,"ImageDrawRectangleV");
  rl_image_draw_rectangle = (RLAPI void  (*) (RlImage *dst, int posX, int posY, int width, int height, RlColor color)) GetProcAddress(rl_lib_handle,"ImageDrawRectangle");
  rl_image_draw_circle_lines_v = (RLAPI void  (*) (RlImage *dst, RlVector2 center, int radius, RlColor color)) GetProcAddress(rl_lib_handle,"ImageDrawCircleLinesV");
  rl_image_draw_circle_lines = (RLAPI void  (*) (RlImage *dst, int centerX, int centerY, int radius, RlColor color)) GetProcAddress(rl_lib_handle,"ImageDrawCircleLines");
  rl_image_draw_circle_v = (RLAPI void  (*) (RlImage *dst, RlVector2 center, int radius, RlColor color)) GetProcAddress(rl_lib_handle,"ImageDrawCircleV");
  rl_image_draw_circle = (RLAPI void  (*) (RlImage *dst, int centerX, int centerY, int radius, RlColor color)) GetProcAddress(rl_lib_handle,"ImageDrawCircle");
  rl_image_draw_line_v = (RLAPI void  (*) (RlImage *dst, RlVector2 start, RlVector2 end, RlColor color)) GetProcAddress(rl_lib_handle,"ImageDrawLineV");
  rl_image_draw_line = (RLAPI void  (*) (RlImage *dst, int startPosX, int startPosY, int endPosX, int endPosY, RlColor color)) GetProcAddress(rl_lib_handle,"ImageDrawLine");
  rl_image_draw_pixel_v = (RLAPI void  (*) (RlImage *dst, RlVector2 position, RlColor color)) GetProcAddress(rl_lib_handle,"ImageDrawPixelV");
  rl_image_draw_pixel = (RLAPI void  (*) (RlImage *dst, int posX, int posY, RlColor color)) GetProcAddress(rl_lib_handle,"ImageDrawPixel");
  rl_image_clear_background = (RLAPI void  (*) (RlImage *dst, RlColor color)) GetProcAddress(rl_lib_handle,"ImageClearBackground");
  rl_get_image_color = (RLAPI RlColor  (*) (RlImage image, int x, int y)) GetProcAddress(rl_lib_handle,"GetImageColor");
  rl_get_image_alpha_border = (RLAPI RlRectangle  (*) (RlImage image, float threshold)) GetProcAddress(rl_lib_handle,"GetImageAlphaBorder");
  rl_unload_image_palette = (RLAPI void  (*) (RlColor *colors)) GetProcAddress(rl_lib_handle,"UnloadImagePalette");
  rl_unload_image_colors = (RLAPI void  (*) (RlColor *colors)) GetProcAddress(rl_lib_handle,"UnloadImageColors");
  rl_load_image_palette = (RLAPI RlColor * (*) (RlImage image, int maxPaletteSize, int *colorCount)) GetProcAddress(rl_lib_handle,"LoadImagePalette");
  rl_load_image_colors = (RLAPI RlColor * (*) (RlImage image)) GetProcAddress(rl_lib_handle,"LoadImageColors");
  rl_image_color_replace = (RLAPI void  (*) (RlImage *image, RlColor color, RlColor replace)) GetProcAddress(rl_lib_handle,"ImageColorReplace");
  rl_image_color_brightness = (RLAPI void  (*) (RlImage *image, int brightness)) GetProcAddress(rl_lib_handle,"ImageColorBrightness");
  rl_image_color_contrast = (RLAPI void  (*) (RlImage *image, float contrast)) GetProcAddress(rl_lib_handle,"ImageColorContrast");
  rl_image_color_grayscale = (RLAPI void  (*) (RlImage *image)) GetProcAddress(rl_lib_handle,"ImageColorGrayscale");
  rl_image_color_invert = (RLAPI void  (*) (RlImage *image)) GetProcAddress(rl_lib_handle,"ImageColorInvert");
  rl_image_color_tint = (RLAPI void  (*) (RlImage *image, RlColor color)) GetProcAddress(rl_lib_handle,"ImageColorTint");
  rl_image_rotate_c_c_w = (RLAPI void  (*) (RlImage *image)) GetProcAddress(rl_lib_handle,"ImageRotateCCW");
  rl_image_rotate_c_w = (RLAPI void  (*) (RlImage *image)) GetProcAddress(rl_lib_handle,"ImageRotateCW");
  rl_image_rotate = (RLAPI void  (*) (RlImage *image, int degrees)) GetProcAddress(rl_lib_handle,"ImageRotate");
  rl_image_flip_horizontal = (RLAPI void  (*) (RlImage *image)) GetProcAddress(rl_lib_handle,"ImageFlipHorizontal");
  rl_image_flip_vertical = (RLAPI void  (*) (RlImage *image)) GetProcAddress(rl_lib_handle,"ImageFlipVertical");
  rl_image_dither = (RLAPI void  (*) (RlImage *image, int rBpp, int gBpp, int bBpp, int aBpp)) GetProcAddress(rl_lib_handle,"ImageDither");
  rl_image_mipmaps = (RLAPI void  (*) (RlImage *image)) GetProcAddress(rl_lib_handle,"ImageMipmaps");
  rl_image_resize_canvas = (RLAPI void  (*) (RlImage *image, int newWidth, int newHeight, int offsetX, int offsetY, RlColor fill)) GetProcAddress(rl_lib_handle,"ImageResizeCanvas");
  rl_image_resize_n_n = (RLAPI void  (*) (RlImage *image, int newWidth,int newHeight)) GetProcAddress(rl_lib_handle,"ImageResizeNN");
  rl_image_resize = (RLAPI void  (*) (RlImage *image, int newWidth, int newHeight)) GetProcAddress(rl_lib_handle,"ImageResize");
  rl_image_blur_gaussian = (RLAPI void  (*) (RlImage *image, int blurSize)) GetProcAddress(rl_lib_handle,"ImageBlurGaussian");
  rl_image_alpha_premultiply = (RLAPI void  (*) (RlImage *image)) GetProcAddress(rl_lib_handle,"ImageAlphaPremultiply");
  rl_image_alpha_mask = (RLAPI void  (*) (RlImage *image, RlImage alphaMask)) GetProcAddress(rl_lib_handle,"ImageAlphaMask");
  rl_image_alpha_clear = (RLAPI void  (*) (RlImage *image, RlColor color, float threshold)) GetProcAddress(rl_lib_handle,"ImageAlphaClear");
  rl_image_alpha_crop = (RLAPI void  (*) (RlImage *image, float threshold)) GetProcAddress(rl_lib_handle,"ImageAlphaCrop");
  rl_image_crop = (RLAPI void  (*) (RlImage *image, RlRectangle crop)) GetProcAddress(rl_lib_handle,"ImageCrop");
  rl_image_to_p_o_t = (RLAPI void  (*) (RlImage *image, RlColor fill)) GetProcAddress(rl_lib_handle,"ImageToPOT");
  rl_image_format = (RLAPI void  (*) (RlImage *image, int newFormat)) GetProcAddress(rl_lib_handle,"ImageFormat");
  rl_image_text_ex = (RLAPI RlImage  (*) (RlFont font, const char *text, float fontSize, float spacing, RlColor tint)) GetProcAddress(rl_lib_handle,"ImageTextEx");
  rl_image_text = (RLAPI RlImage  (*) (const char *text, int fontSize, RlColor color)) GetProcAddress(rl_lib_handle,"ImageText");
  rl_image_from_image = (RLAPI RlImage  (*) (RlImage image, RlRectangle rec)) GetProcAddress(rl_lib_handle,"ImageFromImage");
  rl_image_copy = (RLAPI RlImage  (*) (RlImage image)) GetProcAddress(rl_lib_handle,"ImageCopy");
  rl_gen_image_text = (RLAPI RlImage  (*) (int width, int height, const char *text)) GetProcAddress(rl_lib_handle,"GenImageText");
  rl_gen_image_cellular = (RLAPI RlImage  (*) (int width, int height, int tileSize)) GetProcAddress(rl_lib_handle,"GenImageCellular");
  rl_gen_image_perlin_noise = (RLAPI RlImage  (*) (int width, int height, int offsetX, int offsetY, float scale)) GetProcAddress(rl_lib_handle,"GenImagePerlinNoise");
  rl_gen_image_white_noise = (RLAPI RlImage  (*) (int width, int height, float factor)) GetProcAddress(rl_lib_handle,"GenImageWhiteNoise");
  rl_gen_image_checked = (RLAPI RlImage  (*) (int width, int height, int checksX, int checksY, RlColor col1, RlColor col2)) GetProcAddress(rl_lib_handle,"GenImageChecked");
  rl_gen_image_gradient_square = (RLAPI RlImage  (*) (int width, int height, float density, RlColor inner, RlColor outer)) GetProcAddress(rl_lib_handle,"GenImageGradientSquare");
  rl_gen_image_gradient_radial = (RLAPI RlImage  (*) (int width, int height, float density, RlColor inner, RlColor outer)) GetProcAddress(rl_lib_handle,"GenImageGradientRadial");
  rl_gen_image_gradient_linear = (RLAPI RlImage  (*) (int width, int height, int direction, RlColor start, RlColor end)) GetProcAddress(rl_lib_handle,"GenImageGradientLinear");
  rl_gen_image_color = (RLAPI RlImage  (*) (int width, int height, RlColor color)) GetProcAddress(rl_lib_handle,"GenImageColor");
  rl_export_image_as_code = (RLAPI bool  (*) (RlImage image, const char *fileName)) GetProcAddress(rl_lib_handle,"ExportImageAsCode");
  rl_export_image_to_memory = (RLAPI unsigned char * (*) (RlImage image, const char *fileType, int *fileSize)) GetProcAddress(rl_lib_handle,"ExportImageToMemory");
  rl_export_image = (RLAPI bool  (*) (RlImage image, const char *fileName)) GetProcAddress(rl_lib_handle,"ExportImage");
  rl_unload_image = (RLAPI void  (*) (RlImage image)) GetProcAddress(rl_lib_handle,"UnloadImage");
  rl_is_image_ready = (RLAPI bool  (*) (RlImage image)) GetProcAddress(rl_lib_handle,"IsImageReady");
  rl_load_image_from_screen = (RLAPI RlImage  (*) (void)) GetProcAddress(rl_lib_handle,"LoadImageFromScreen");
  rl_load_image_from_texture = (RLAPI RlImage  (*) (RlTexture2D texture)) GetProcAddress(rl_lib_handle,"LoadImageFromTexture");
  rl_load_image_from_memory = (RLAPI RlImage  (*) (const char *fileType, const unsigned char *fileData, int dataSize)) GetProcAddress(rl_lib_handle,"LoadImageFromMemory");
  rl_load_image_anim = (RLAPI RlImage  (*) (const char *fileName, int *frames)) GetProcAddress(rl_lib_handle,"LoadImageAnim");
  rl_load_image_svg = (RLAPI RlImage  (*) (const char *fileNameOrString, int width, int height)) GetProcAddress(rl_lib_handle,"LoadImageSvg");
  rl_load_image_raw = (RLAPI RlImage  (*) (const char *fileName, int width, int height, int format, int headerSize)) GetProcAddress(rl_lib_handle,"LoadImageRaw");
  rl_load_image = (RLAPI RlImage  (*) (const char *fileName)) GetProcAddress(rl_lib_handle,"LoadImage");
  rl_get_collision_rec = (RLAPI RlRectangle  (*) (RlRectangle rec1, RlRectangle rec2)) GetProcAddress(rl_lib_handle,"GetCollisionRec");
  rl_check_collision_point_line = (RLAPI bool  (*) (RlVector2 point, RlVector2 p1, RlVector2 p2, int threshold)) GetProcAddress(rl_lib_handle,"CheckCollisionPointLine");
  rl_check_collision_lines = (RLAPI bool  (*) (RlVector2 startPos1, RlVector2 endPos1, RlVector2 startPos2, RlVector2 endPos2, RlVector2 *collisionPoint)) GetProcAddress(rl_lib_handle,"CheckCollisionLines");
  rl_check_collision_point_poly = (RLAPI bool  (*) (RlVector2 point, RlVector2 *points, int pointCount)) GetProcAddress(rl_lib_handle,"CheckCollisionPointPoly");
  rl_check_collision_point_triangle = (RLAPI bool  (*) (RlVector2 point, RlVector2 p1, RlVector2 p2, RlVector2 p3)) GetProcAddress(rl_lib_handle,"CheckCollisionPointTriangle");
  rl_check_collision_point_circle = (RLAPI bool  (*) (RlVector2 point, RlVector2 center, float radius)) GetProcAddress(rl_lib_handle,"CheckCollisionPointCircle");
  rl_check_collision_point_rec = (RLAPI bool  (*) (RlVector2 point, RlRectangle rec)) GetProcAddress(rl_lib_handle,"CheckCollisionPointRec");
  rl_check_collision_circle_rec = (RLAPI bool  (*) (RlVector2 center, float radius, RlRectangle rec)) GetProcAddress(rl_lib_handle,"CheckCollisionCircleRec");
  rl_check_collision_circles = (RLAPI bool  (*) (RlVector2 center1, float radius1, RlVector2 center2, float radius2)) GetProcAddress(rl_lib_handle,"CheckCollisionCircles");
  rl_check_collision_recs = (RLAPI bool  (*) (RlRectangle rec1, RlRectangle rec2)) GetProcAddress(rl_lib_handle,"CheckCollisionRecs");
  rl_get_spline_point_bezier_cubic = (RLAPI RlVector2  (*) (RlVector2 p1, RlVector2 c2, RlVector2 c3, RlVector2 p4, float t)) GetProcAddress(rl_lib_handle,"GetSplinePointBezierCubic");
  rl_get_spline_point_bezier_quad = (RLAPI RlVector2  (*) (RlVector2 p1, RlVector2 c2, RlVector2 p3, float t)) GetProcAddress(rl_lib_handle,"GetSplinePointBezierQuad");
  rl_get_spline_point_catmull_rom = (RLAPI RlVector2  (*) (RlVector2 p1, RlVector2 p2, RlVector2 p3, RlVector2 p4, float t)) GetProcAddress(rl_lib_handle,"GetSplinePointCatmullRom");
  rl_get_spline_point_basis = (RLAPI RlVector2  (*) (RlVector2 p1, RlVector2 p2, RlVector2 p3, RlVector2 p4, float t)) GetProcAddress(rl_lib_handle,"GetSplinePointBasis");
  rl_get_spline_point_linear = (RLAPI RlVector2  (*) (RlVector2 startPos, RlVector2 endPos, float t)) GetProcAddress(rl_lib_handle,"GetSplinePointLinear");
  rl_draw_spline_segment_bezier_cubic = (RLAPI void  (*) (RlVector2 p1, RlVector2 c2, RlVector2 c3, RlVector2 p4, float thick, RlColor color)) GetProcAddress(rl_lib_handle,"DrawSplineSegmentBezierCubic");
  rl_draw_spline_segment_bezier_quadratic = (RLAPI void  (*) (RlVector2 p1, RlVector2 c2, RlVector2 p3, float thick, RlColor color)) GetProcAddress(rl_lib_handle,"DrawSplineSegmentBezierQuadratic");
  rl_draw_spline_segment_catmull_rom = (RLAPI void  (*) (RlVector2 p1, RlVector2 p2, RlVector2 p3, RlVector2 p4, float thick, RlColor color)) GetProcAddress(rl_lib_handle,"DrawSplineSegmentCatmullRom");
  rl_draw_spline_segment_basis = (RLAPI void  (*) (RlVector2 p1, RlVector2 p2, RlVector2 p3, RlVector2 p4, float thick, RlColor color)) GetProcAddress(rl_lib_handle,"DrawSplineSegmentBasis");
  rl_draw_spline_segment_linear = (RLAPI void  (*) (RlVector2 p1, RlVector2 p2, float thick, RlColor color)) GetProcAddress(rl_lib_handle,"DrawSplineSegmentLinear");
  rl_draw_spline_bezier_cubic = (RLAPI void  (*) (RlVector2 *points, int pointCount, float thick, RlColor color)) GetProcAddress(rl_lib_handle,"DrawSplineBezierCubic");
  rl_draw_spline_bezier_quadratic = (RLAPI void  (*) (RlVector2 *points, int pointCount, float thick, RlColor color)) GetProcAddress(rl_lib_handle,"DrawSplineBezierQuadratic");
  rl_draw_spline_catmull_rom = (RLAPI void  (*) (RlVector2 *points, int pointCount, float thick, RlColor color)) GetProcAddress(rl_lib_handle,"DrawSplineCatmullRom");
  rl_draw_spline_basis = (RLAPI void  (*) (RlVector2 *points, int pointCount, float thick, RlColor color)) GetProcAddress(rl_lib_handle,"DrawSplineBasis");
  rl_draw_spline_linear = (RLAPI void  (*) (RlVector2 *points, int pointCount, float thick, RlColor color)) GetProcAddress(rl_lib_handle,"DrawSplineLinear");
  rl_draw_poly_lines_ex = (RLAPI void  (*) (RlVector2 center, int sides, float radius, float rotation, float lineThick, RlColor color)) GetProcAddress(rl_lib_handle,"DrawPolyLinesEx");
  rl_draw_poly_lines = (RLAPI void  (*) (RlVector2 center, int sides, float radius, float rotation, RlColor color)) GetProcAddress(rl_lib_handle,"DrawPolyLines");
  rl_draw_poly = (RLAPI void  (*) (RlVector2 center, int sides, float radius, float rotation, RlColor color)) GetProcAddress(rl_lib_handle,"DrawPoly");
  rl_draw_triangle_strip = (RLAPI void  (*) (RlVector2 *points, int pointCount, RlColor color)) GetProcAddress(rl_lib_handle,"DrawTriangleStrip");
  rl_draw_triangle_fan = (RLAPI void  (*) (RlVector2 *points, int pointCount, RlColor color)) GetProcAddress(rl_lib_handle,"DrawTriangleFan");
  rl_draw_triangle_lines = (RLAPI void  (*) (RlVector2 v1, RlVector2 v2, RlVector2 v3, RlColor color)) GetProcAddress(rl_lib_handle,"DrawTriangleLines");
  rl_draw_triangle = (RLAPI void  (*) (RlVector2 v1, RlVector2 v2, RlVector2 v3, RlColor color)) GetProcAddress(rl_lib_handle,"DrawTriangle");
  rl_draw_rectangle_rounded_lines = (RLAPI void  (*) (RlRectangle rec, float roundness, int segments, float lineThick, RlColor color)) GetProcAddress(rl_lib_handle,"DrawRectangleRoundedLines");
  rl_draw_rectangle_rounded = (RLAPI void  (*) (RlRectangle rec, float roundness, int segments, RlColor color)) GetProcAddress(rl_lib_handle,"DrawRectangleRounded");
  rl_draw_rectangle_lines_ex = (RLAPI void  (*) (RlRectangle rec, float lineThick, RlColor color)) GetProcAddress(rl_lib_handle,"DrawRectangleLinesEx");
  rl_draw_rectangle_lines = (RLAPI void  (*) (int posX, int posY, int width, int height, RlColor color)) GetProcAddress(rl_lib_handle,"DrawRectangleLines");
  rl_draw_rectangle_gradient_ex = (RLAPI void  (*) (RlRectangle rec, RlColor col1, RlColor col2, RlColor col3, RlColor col4)) GetProcAddress(rl_lib_handle,"DrawRectangleGradientEx");
  rl_draw_rectangle_gradient_h = (RLAPI void  (*) (int posX, int posY, int width, int height, RlColor color1, RlColor color2)) GetProcAddress(rl_lib_handle,"DrawRectangleGradientH");
  rl_draw_rectangle_gradient_v = (RLAPI void  (*) (int posX, int posY, int width, int height, RlColor color1, RlColor color2)) GetProcAddress(rl_lib_handle,"DrawRectangleGradientV");
  rl_draw_rectangle_pro = (RLAPI void  (*) (RlRectangle rec, RlVector2 origin, float rotation, RlColor color)) GetProcAddress(rl_lib_handle,"DrawRectanglePro");
  rl_draw_rectangle_rec = (RLAPI void  (*) (RlRectangle rec, RlColor color)) GetProcAddress(rl_lib_handle,"DrawRectangleRec");
  rl_draw_rectangle_v = (RLAPI void  (*) (RlVector2 position, RlVector2 size, RlColor color)) GetProcAddress(rl_lib_handle,"DrawRectangleV");
  rl_draw_rectangle = (RLAPI void  (*) (int posX, int posY, int width, int height, RlColor color)) GetProcAddress(rl_lib_handle,"DrawRectangle");
  rl_draw_ring_lines = (RLAPI void  (*) (RlVector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, RlColor color)) GetProcAddress(rl_lib_handle,"DrawRingLines");
  rl_draw_ring = (RLAPI void  (*) (RlVector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, RlColor color)) GetProcAddress(rl_lib_handle,"DrawRing");
  rl_draw_ellipse_lines = (RLAPI void  (*) (int centerX, int centerY, float radiusH, float radiusV, RlColor color)) GetProcAddress(rl_lib_handle,"DrawEllipseLines");
  rl_draw_ellipse = (RLAPI void  (*) (int centerX, int centerY, float radiusH, float radiusV, RlColor color)) GetProcAddress(rl_lib_handle,"DrawEllipse");
  rl_draw_circle_lines_v = (RLAPI void  (*) (RlVector2 center, float radius, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCircleLinesV");
  rl_draw_circle_lines = (RLAPI void  (*) (int centerX, int centerY, float radius, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCircleLines");
  rl_draw_circle_v = (RLAPI void  (*) (RlVector2 center, float radius, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCircleV");
  rl_draw_circle_gradient = (RLAPI void  (*) (int centerX, int centerY, float radius, RlColor color1, RlColor color2)) GetProcAddress(rl_lib_handle,"DrawCircleGradient");
  rl_draw_circle_sector_lines = (RLAPI void  (*) (RlVector2 center, float radius, float startAngle, float endAngle, int segments, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCircleSectorLines");
  rl_draw_circle_sector = (RLAPI void  (*) (RlVector2 center, float radius, float startAngle, float endAngle, int segments, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCircleSector");
  rl_draw_circle = (RLAPI void  (*) (int centerX, int centerY, float radius, RlColor color)) GetProcAddress(rl_lib_handle,"DrawCircle");
  rl_draw_line_bezier = (RLAPI void  (*) (RlVector2 startPos, RlVector2 endPos, float thick, RlColor color)) GetProcAddress(rl_lib_handle,"DrawLineBezier");
  rl_draw_line_strip = (RLAPI void  (*) (RlVector2 *points, int pointCount, RlColor color)) GetProcAddress(rl_lib_handle,"DrawLineStrip");
  rl_draw_line_ex = (RLAPI void  (*) (RlVector2 startPos, RlVector2 endPos, float thick, RlColor color)) GetProcAddress(rl_lib_handle,"DrawLineEx");
  rl_draw_line_v = (RLAPI void  (*) (RlVector2 startPos, RlVector2 endPos, RlColor color)) GetProcAddress(rl_lib_handle,"DrawLineV");
  rl_draw_line = (RLAPI void  (*) (int startPosX, int startPosY, int endPosX, int endPosY, RlColor color)) GetProcAddress(rl_lib_handle,"DrawLine");
  rl_draw_pixel_v = (RLAPI void  (*) (RlVector2 position, RlColor color)) GetProcAddress(rl_lib_handle,"DrawPixelV");
  rl_draw_pixel = (RLAPI void  (*) (int posX, int posY, RlColor color)) GetProcAddress(rl_lib_handle,"DrawPixel");
  rl_set_shapes_texture = (RLAPI void  (*) (RlTexture2D texture, RlRectangle source)) GetProcAddress(rl_lib_handle,"SetShapesTexture");
  rl_update_camera_pro = (RLAPI void  (*) (RlCamera *camera, RlVector3 movement, RlVector3 rotation, float zoom)) GetProcAddress(rl_lib_handle,"UpdateCameraPro");
  rl_update_camera = (RLAPI void  (*) (RlCamera *camera, int mode)) GetProcAddress(rl_lib_handle,"UpdateCamera");
  rl_get_gesture_pinch_angle = (RLAPI float  (*) (void)) GetProcAddress(rl_lib_handle,"GetGesturePinchAngle");
  rl_get_gesture_pinch_vector = (RLAPI RlVector2  (*) (void)) GetProcAddress(rl_lib_handle,"GetGesturePinchVector");
  rl_get_gesture_drag_angle = (RLAPI float  (*) (void)) GetProcAddress(rl_lib_handle,"GetGestureDragAngle");
  rl_get_gesture_drag_vector = (RLAPI RlVector2  (*) (void)) GetProcAddress(rl_lib_handle,"GetGestureDragVector");
  rl_get_gesture_hold_duration = (RLAPI float  (*) (void)) GetProcAddress(rl_lib_handle,"GetGestureHoldDuration");
  rl_get_gesture_detected = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetGestureDetected");
  rl_is_gesture_detected = (RLAPI bool  (*) (unsigned int gesture)) GetProcAddress(rl_lib_handle,"IsGestureDetected");
  rl_set_gestures_enabled = (RLAPI void  (*) (unsigned int flags)) GetProcAddress(rl_lib_handle,"SetGesturesEnabled");
  rl_get_touch_point_count = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetTouchPointCount");
  rl_get_touch_point_id = (RLAPI int  (*) (int index)) GetProcAddress(rl_lib_handle,"GetTouchPointId");
  rl_get_touch_position = (RLAPI RlVector2  (*) (int index)) GetProcAddress(rl_lib_handle,"GetTouchPosition");
  rl_get_touch_y = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetTouchY");
  rl_get_touch_x = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetTouchX");
  rl_set_mouse_cursor = (RLAPI void  (*) (int cursor)) GetProcAddress(rl_lib_handle,"SetMouseCursor");
  rl_get_mouse_wheel_move_v = (RLAPI RlVector2  (*) (void)) GetProcAddress(rl_lib_handle,"GetMouseWheelMoveV");
  rl_get_mouse_wheel_move = (RLAPI float  (*) (void)) GetProcAddress(rl_lib_handle,"GetMouseWheelMove");
  rl_set_mouse_scale = (RLAPI void  (*) (float scaleX, float scaleY)) GetProcAddress(rl_lib_handle,"SetMouseScale");
  rl_set_mouse_offset = (RLAPI void  (*) (int offsetX, int offsetY)) GetProcAddress(rl_lib_handle,"SetMouseOffset");
  rl_set_mouse_position = (RLAPI void  (*) (int x, int y)) GetProcAddress(rl_lib_handle,"SetMousePosition");
  rl_get_mouse_delta = (RLAPI RlVector2  (*) (void)) GetProcAddress(rl_lib_handle,"GetMouseDelta");
  rl_get_mouse_position = (RLAPI RlVector2  (*) (void)) GetProcAddress(rl_lib_handle,"GetMousePosition");
  rl_get_mouse_y = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetMouseY");
  rl_get_mouse_x = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetMouseX");
  rl_is_mouse_button_up = (RLAPI bool  (*) (int button)) GetProcAddress(rl_lib_handle,"IsMouseButtonUp");
  rl_is_mouse_button_released = (RLAPI bool  (*) (int button)) GetProcAddress(rl_lib_handle,"IsMouseButtonReleased");
  rl_is_mouse_button_down = (RLAPI bool  (*) (int button)) GetProcAddress(rl_lib_handle,"IsMouseButtonDown");
  rl_is_mouse_button_pressed = (RLAPI bool  (*) (int button)) GetProcAddress(rl_lib_handle,"IsMouseButtonPressed");
  rl_set_gamepad_mappings = (RLAPI int  (*) (const char *mappings)) GetProcAddress(rl_lib_handle,"SetGamepadMappings");
  rl_get_gamepad_axis_movement = (RLAPI float  (*) (int gamepad, int axis)) GetProcAddress(rl_lib_handle,"GetGamepadAxisMovement");
  rl_get_gamepad_axis_count = (RLAPI int  (*) (int gamepad)) GetProcAddress(rl_lib_handle,"GetGamepadAxisCount");
  rl_get_gamepad_button_pressed = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetGamepadButtonPressed");
  rl_is_gamepad_button_up = (RLAPI bool  (*) (int gamepad, int button)) GetProcAddress(rl_lib_handle,"IsGamepadButtonUp");
  rl_is_gamepad_button_released = (RLAPI bool  (*) (int gamepad, int button)) GetProcAddress(rl_lib_handle,"IsGamepadButtonReleased");
  rl_is_gamepad_button_down = (RLAPI bool  (*) (int gamepad, int button)) GetProcAddress(rl_lib_handle,"IsGamepadButtonDown");
  rl_is_gamepad_button_pressed = (RLAPI bool  (*) (int gamepad, int button)) GetProcAddress(rl_lib_handle,"IsGamepadButtonPressed");
  rl_get_gamepad_name = (RLAPI const char * (*) (int gamepad)) GetProcAddress(rl_lib_handle,"GetGamepadName");
  rl_is_gamepad_available = (RLAPI bool  (*) (int gamepad)) GetProcAddress(rl_lib_handle,"IsGamepadAvailable");
  rl_set_exit_key = (RLAPI void  (*) (int key)) GetProcAddress(rl_lib_handle,"SetExitKey");
  rl_get_char_pressed = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetCharPressed");
  rl_get_key_pressed = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetKeyPressed");
  rl_is_key_up = (RLAPI bool  (*) (int key)) GetProcAddress(rl_lib_handle,"IsKeyUp");
  rl_is_key_released = (RLAPI bool  (*) (int key)) GetProcAddress(rl_lib_handle,"IsKeyReleased");
  rl_is_key_down = (RLAPI bool  (*) (int key)) GetProcAddress(rl_lib_handle,"IsKeyDown");
  rl_is_key_pressed_repeat = (RLAPI bool  (*) (int key)) GetProcAddress(rl_lib_handle,"IsKeyPressedRepeat");
  rl_is_key_pressed = (RLAPI bool  (*) (int key)) GetProcAddress(rl_lib_handle,"IsKeyPressed");
  rl_play_automation_event = (RLAPI void  (*) (RlAutomationEvent event)) GetProcAddress(rl_lib_handle,"PlayAutomationEvent");
  rl_stop_automation_event_recording = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"StopAutomationEventRecording");
  rl_start_automation_event_recording = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"StartAutomationEventRecording");
  rl_set_automation_event_base_frame = (RLAPI void  (*) (int frame)) GetProcAddress(rl_lib_handle,"SetAutomationEventBaseFrame");
  rl_set_automation_event_list = (RLAPI void  (*) (RlAutomationEventList *list)) GetProcAddress(rl_lib_handle,"SetAutomationEventList");
  rl_export_automation_event_list = (RLAPI bool  (*) (RlAutomationEventList list, const char *fileName)) GetProcAddress(rl_lib_handle,"ExportAutomationEventList");
  rl_unload_automation_event_list = (RLAPI void  (*) (RlAutomationEventList *list)) GetProcAddress(rl_lib_handle,"UnloadAutomationEventList");
  rl_load_automation_event_list = (RLAPI RlAutomationEventList  (*) (const char *fileName)) GetProcAddress(rl_lib_handle,"LoadAutomationEventList");
  rl_decode_data_base_6_4 = (RLAPI unsigned char * (*) (const unsigned char *data, int *outputSize)) GetProcAddress(rl_lib_handle,"DecodeDataBase64");
  rl_encode_data_base_6_4 = (RLAPI char * (*) (const unsigned char *data, int dataSize, int *outputSize)) GetProcAddress(rl_lib_handle,"EncodeDataBase64");
  rl_decompress_data = (RLAPI unsigned char * (*) (const unsigned char *compData, int compDataSize, int *dataSize)) GetProcAddress(rl_lib_handle,"DecompressData");
  rl_compress_data = (RLAPI unsigned char * (*) (const unsigned char *data, int dataSize, int *compDataSize)) GetProcAddress(rl_lib_handle,"CompressData");
  rl_get_file_mod_time = (RLAPI long  (*) (const char *fileName)) GetProcAddress(rl_lib_handle,"GetFileModTime");
  rl_unload_dropped_files = (RLAPI void  (*) (RlFilePathList files)) GetProcAddress(rl_lib_handle,"UnloadDroppedFiles");
  rl_load_dropped_files = (RLAPI RlFilePathList  (*) (void)) GetProcAddress(rl_lib_handle,"LoadDroppedFiles");
  rl_is_file_dropped = (RLAPI bool  (*) (void)) GetProcAddress(rl_lib_handle,"IsFileDropped");
  rl_unload_directory_files = (RLAPI void  (*) (RlFilePathList files)) GetProcAddress(rl_lib_handle,"UnloadDirectoryFiles");
  rl_load_directory_files_ex = (RLAPI RlFilePathList  (*) (const char *basePath, const char *filter, bool scanSubdirs)) GetProcAddress(rl_lib_handle,"LoadDirectoryFilesEx");
  rl_load_directory_files = (RLAPI RlFilePathList  (*) (const char *dirPath)) GetProcAddress(rl_lib_handle,"LoadDirectoryFiles");
  rl_is_path_file = (RLAPI bool  (*) (const char *path)) GetProcAddress(rl_lib_handle,"IsPathFile");
  rl_change_directory = (RLAPI bool  (*) (const char *dir)) GetProcAddress(rl_lib_handle,"ChangeDirectory");
  rl_get_application_directory = (RLAPI const char * (*) (void)) GetProcAddress(rl_lib_handle,"GetApplicationDirectory");
  rl_get_working_directory = (RLAPI const char * (*) (void)) GetProcAddress(rl_lib_handle,"GetWorkingDirectory");
  rl_get_prev_directory_path = (RLAPI const char * (*) (const char *dirPath)) GetProcAddress(rl_lib_handle,"GetPrevDirectoryPath");
  rl_get_directory_path = (RLAPI const char * (*) (const char *filePath)) GetProcAddress(rl_lib_handle,"GetDirectoryPath");
  rl_get_file_name_without_ext = (RLAPI const char * (*) (const char *filePath)) GetProcAddress(rl_lib_handle,"GetFileNameWithoutExt");
  rl_get_file_name = (RLAPI const char * (*) (const char *filePath)) GetProcAddress(rl_lib_handle,"GetFileName");
  rl_get_file_extension = (RLAPI const char * (*) (const char *fileName)) GetProcAddress(rl_lib_handle,"GetFileExtension");
  rl_get_file_length = (RLAPI int  (*) (const char *fileName)) GetProcAddress(rl_lib_handle,"GetFileLength");
  rl_is_file_extension = (RLAPI bool  (*) (const char *fileName, const char *ext)) GetProcAddress(rl_lib_handle,"IsFileExtension");
  rl_directory_exists = (RLAPI bool  (*) (const char *dirPath)) GetProcAddress(rl_lib_handle,"DirectoryExists");
  rl_file_exists = (RLAPI bool  (*) (const char *fileName)) GetProcAddress(rl_lib_handle,"FileExists");
  rl_save_file_text = (RLAPI bool  (*) (const char *fileName, char *text)) GetProcAddress(rl_lib_handle,"SaveFileText");
  rl_unload_file_text = (RLAPI void  (*) (char *text)) GetProcAddress(rl_lib_handle,"UnloadFileText");
  rl_load_file_text = (RLAPI char * (*) (const char *fileName)) GetProcAddress(rl_lib_handle,"LoadFileText");
  rl_export_data_as_code = (RLAPI bool  (*) (const unsigned char *data, int dataSize, const char *fileName)) GetProcAddress(rl_lib_handle,"ExportDataAsCode");
  rl_save_file_data = (RLAPI bool  (*) (const char *fileName, void *data, int dataSize)) GetProcAddress(rl_lib_handle,"SaveFileData");
  rl_unload_file_data = (RLAPI void  (*) (unsigned char *data)) GetProcAddress(rl_lib_handle,"UnloadFileData");
  rl_load_file_data = (RLAPI unsigned char * (*) (const char *fileName, int *dataSize)) GetProcAddress(rl_lib_handle,"LoadFileData");
  rl_set_save_file_text_callback = (RLAPI void  (*) (RlSaveFileTextCallback callback)) GetProcAddress(rl_lib_handle,"SetSaveFileTextCallback");
  rl_set_load_file_text_callback = (RLAPI void  (*) (RlLoadFileTextCallback callback)) GetProcAddress(rl_lib_handle,"SetLoadFileTextCallback");
  rl_set_save_file_data_callback = (RLAPI void  (*) (RlSaveFileDataCallback callback)) GetProcAddress(rl_lib_handle,"SetSaveFileDataCallback");
  rl_set_load_file_data_callback = (RLAPI void  (*) (RlLoadFileDataCallback callback)) GetProcAddress(rl_lib_handle,"SetLoadFileDataCallback");
  rl_set_trace_log_callback = (RLAPI void  (*) (RlTraceLogCallback callback)) GetProcAddress(rl_lib_handle,"SetTraceLogCallback");
  rl_mem_free = (RLAPI void  (*) (void *ptr)) GetProcAddress(rl_lib_handle,"MemFree");
  rl_mem_realloc = (RLAPI void * (*) (void *ptr, unsigned int size)) GetProcAddress(rl_lib_handle,"MemRealloc");
  rl_mem_alloc = (RLAPI void * (*) (unsigned int size)) GetProcAddress(rl_lib_handle,"MemAlloc");
  rl_set_trace_log_level = (RLAPI void  (*) (int logLevel)) GetProcAddress(rl_lib_handle,"SetTraceLogLevel");
  rl_trace_log = (RLAPI void  (*) (int logLevel, const char *text, ...)) GetProcAddress(rl_lib_handle,"TraceLog");
  rl_open_u_r_l = (RLAPI void  (*) (const char *url)) GetProcAddress(rl_lib_handle,"OpenURL");
  rl_set_config_flags = (RLAPI void  (*) (unsigned int flags)) GetProcAddress(rl_lib_handle,"SetConfigFlags");
  rl_take_screenshot = (RLAPI void  (*) (const char *fileName)) GetProcAddress(rl_lib_handle,"TakeScreenshot");
  rl_unload_random_sequence = (RLAPI void  (*) (int *sequence)) GetProcAddress(rl_lib_handle,"UnloadRandomSequence");
  rl_load_random_sequence = (RLAPI int * (*) (unsigned int count, int min, int max)) GetProcAddress(rl_lib_handle,"LoadRandomSequence");
  rl_get_random_value = (RLAPI int  (*) (int min, int max)) GetProcAddress(rl_lib_handle,"GetRandomValue");
  rl_set_random_seed = (RLAPI void  (*) (unsigned int seed)) GetProcAddress(rl_lib_handle,"SetRandomSeed");
  rl_wait_time = (RLAPI void  (*) (double seconds)) GetProcAddress(rl_lib_handle,"WaitTime");
  rl_poll_input_events = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"PollInputEvents");
  rl_swap_screen_buffer = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"SwapScreenBuffer");
  rl_get_f_p_s = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetFPS");
  rl_get_time = (RLAPI double  (*) (void)) GetProcAddress(rl_lib_handle,"GetTime");
  rl_get_frame_time = (RLAPI float  (*) (void)) GetProcAddress(rl_lib_handle,"GetFrameTime");
  rl_set_target_f_p_s = (RLAPI void  (*) (int fps)) GetProcAddress(rl_lib_handle,"SetTargetFPS");
  rl_get_world_to_screen_2_d = (RLAPI RlVector2  (*) (RlVector2 position, RlCamera2D camera)) GetProcAddress(rl_lib_handle,"GetWorldToScreen2D");
  rl_get_world_to_screen_ex = (RLAPI RlVector2  (*) (RlVector3 position, RlCamera camera, int width, int height)) GetProcAddress(rl_lib_handle,"GetWorldToScreenEx");
  rl_get_screen_to_world_2_d = (RLAPI RlVector2  (*) (RlVector2 position, RlCamera2D camera)) GetProcAddress(rl_lib_handle,"GetScreenToWorld2D");
  rl_get_world_to_screen = (RLAPI RlVector2  (*) (RlVector3 position, RlCamera camera)) GetProcAddress(rl_lib_handle,"GetWorldToScreen");
  rl_get_camera_matrix_2_d = (RLAPI RlMatrix  (*) (RlCamera2D camera)) GetProcAddress(rl_lib_handle,"GetCameraMatrix2D");
  rl_get_camera_matrix = (RLAPI RlMatrix  (*) (RlCamera camera)) GetProcAddress(rl_lib_handle,"GetCameraMatrix");
  rl_get_mouse_ray = (RLAPI RlRay  (*) (RlVector2 mousePosition, RlCamera camera)) GetProcAddress(rl_lib_handle,"GetMouseRay");
  rl_unload_shader = (RLAPI void  (*) (RlShader shader)) GetProcAddress(rl_lib_handle,"UnloadShader");
  rl_set_shader_value_texture = (RLAPI void  (*) (RlShader shader, int locIndex, RlTexture2D texture)) GetProcAddress(rl_lib_handle,"SetShaderValueTexture");
  rl_set_shader_value_matrix = (RLAPI void  (*) (RlShader shader, int locIndex, RlMatrix mat)) GetProcAddress(rl_lib_handle,"SetShaderValueMatrix");
  rl_set_shader_value_v = (RLAPI void  (*) (RlShader shader, int locIndex, const void *value, int uniformType, int count)) GetProcAddress(rl_lib_handle,"SetShaderValueV");
  rl_set_shader_value = (RLAPI void  (*) (RlShader shader, int locIndex, const void *value, int uniformType)) GetProcAddress(rl_lib_handle,"SetShaderValue");
  rl_get_shader_location_attrib = (RLAPI int  (*) (RlShader shader, const char *attribName)) GetProcAddress(rl_lib_handle,"GetShaderLocationAttrib");
  rl_get_shader_location = (RLAPI int  (*) (RlShader shader, const char *uniformName)) GetProcAddress(rl_lib_handle,"GetShaderLocation");
  rl_is_shader_ready = (RLAPI bool  (*) (RlShader shader)) GetProcAddress(rl_lib_handle,"IsShaderReady");
  rl_load_shader_from_memory = (RLAPI RlShader  (*) (const char *vsCode, const char *fsCode)) GetProcAddress(rl_lib_handle,"LoadShaderFromMemory");
  rl_load_shader = (RLAPI RlShader  (*) (const char *vsFileName, const char *fsFileName)) GetProcAddress(rl_lib_handle,"LoadShader");
  rl_unload_vr_stereo_config = (RLAPI void  (*) (RlVrStereoConfig config)) GetProcAddress(rl_lib_handle,"UnloadVrStereoConfig");
  rl_load_vr_stereo_config = (RLAPI RlVrStereoConfig  (*) (RlVrDeviceInfo device)) GetProcAddress(rl_lib_handle,"LoadVrStereoConfig");
  rl_end_vr_stereo_mode = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"EndVrStereoMode");
  rl_begin_vr_stereo_mode = (RLAPI void  (*) (RlVrStereoConfig config)) GetProcAddress(rl_lib_handle,"BeginVrStereoMode");
  rl_end_scissor_mode = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"EndScissorMode");
  rl_begin_scissor_mode = (RLAPI void  (*) (int x, int y, int width, int height)) GetProcAddress(rl_lib_handle,"BeginScissorMode");
  rl_end_blend_mode = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"EndBlendMode");
  rl_begin_blend_mode = (RLAPI void  (*) (int mode)) GetProcAddress(rl_lib_handle,"BeginBlendMode");
  rl_end_shader_mode = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"EndShaderMode");
  rl_begin_shader_mode = (RLAPI void  (*) (RlShader shader)) GetProcAddress(rl_lib_handle,"BeginShaderMode");
  rl_end_texture_mode = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"EndTextureMode");
  rl_begin_texture_mode = (RLAPI void  (*) (RlRenderTexture2D target)) GetProcAddress(rl_lib_handle,"BeginTextureMode");
  rl_end_mode_3_d = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"EndMode3D");
  rl_begin_mode_3_d = (RLAPI void  (*) (RlCamera3D camera)) GetProcAddress(rl_lib_handle,"BeginMode3D");
  rl_end_mode_2_d = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"EndMode2D");
  rl_begin_mode_2_d = (RLAPI void  (*) (RlCamera2D camera)) GetProcAddress(rl_lib_handle,"BeginMode2D");
  rl_end_drawing = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"EndDrawing");
  rl_begin_drawing = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"BeginDrawing");
  rl_clear_background = (RLAPI void  (*) (RlColor color)) GetProcAddress(rl_lib_handle,"ClearBackground");
  rl_is_cursor_on_screen = (RLAPI bool  (*) (void)) GetProcAddress(rl_lib_handle,"IsCursorOnScreen");
  rl_disable_cursor = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"DisableCursor");
  rl_enable_cursor = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"EnableCursor");
  rl_is_cursor_hidden = (RLAPI bool  (*) (void)) GetProcAddress(rl_lib_handle,"IsCursorHidden");
  rl_hide_cursor = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"HideCursor");
  rl_show_cursor = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"ShowCursor");
  rl_disable_event_waiting = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"DisableEventWaiting");
  rl_enable_event_waiting = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"EnableEventWaiting");
  rl_get_clipboard_text = (RLAPI const char * (*) (void)) GetProcAddress(rl_lib_handle,"GetClipboardText");
  rl_set_clipboard_text = (RLAPI void  (*) (const char *text)) GetProcAddress(rl_lib_handle,"SetClipboardText");
  rl_get_monitor_name = (RLAPI const char * (*) (int monitor)) GetProcAddress(rl_lib_handle,"GetMonitorName");
  rl_get_window_scale_d_p_i = (RLAPI RlVector2  (*) (void)) GetProcAddress(rl_lib_handle,"GetWindowScaleDPI");
  rl_get_window_position = (RLAPI RlVector2  (*) (void)) GetProcAddress(rl_lib_handle,"GetWindowPosition");
  rl_get_monitor_refresh_rate = (RLAPI int  (*) (int monitor)) GetProcAddress(rl_lib_handle,"GetMonitorRefreshRate");
  rl_get_monitor_physical_height = (RLAPI int  (*) (int monitor)) GetProcAddress(rl_lib_handle,"GetMonitorPhysicalHeight");
  rl_get_monitor_physical_width = (RLAPI int  (*) (int monitor)) GetProcAddress(rl_lib_handle,"GetMonitorPhysicalWidth");
  rl_get_monitor_height = (RLAPI int  (*) (int monitor)) GetProcAddress(rl_lib_handle,"GetMonitorHeight");
  rl_get_monitor_width = (RLAPI int  (*) (int monitor)) GetProcAddress(rl_lib_handle,"GetMonitorWidth");
  rl_get_monitor_position = (RLAPI RlVector2  (*) (int monitor)) GetProcAddress(rl_lib_handle,"GetMonitorPosition");
  rl_get_current_monitor = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetCurrentMonitor");
  rl_get_monitor_count = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetMonitorCount");
  rl_get_render_height = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetRenderHeight");
  rl_get_render_width = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetRenderWidth");
  rl_get_screen_height = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetScreenHeight");
  rl_get_screen_width = (RLAPI int  (*) (void)) GetProcAddress(rl_lib_handle,"GetScreenWidth");
  rl_get_window_handle = (RLAPI void * (*) (void)) GetProcAddress(rl_lib_handle,"GetWindowHandle");
  rl_set_window_focused = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"SetWindowFocused");
  rl_set_window_opacity = (RLAPI void  (*) (float opacity)) GetProcAddress(rl_lib_handle,"SetWindowOpacity");
  rl_set_window_size = (RLAPI void  (*) (int width, int height)) GetProcAddress(rl_lib_handle,"SetWindowSize");
  rl_set_window_max_size = (RLAPI void  (*) (int width, int height)) GetProcAddress(rl_lib_handle,"SetWindowMaxSize");
  rl_set_window_min_size = (RLAPI void  (*) (int width, int height)) GetProcAddress(rl_lib_handle,"SetWindowMinSize");
  rl_set_window_monitor = (RLAPI void  (*) (int monitor)) GetProcAddress(rl_lib_handle,"SetWindowMonitor");
  rl_set_window_position = (RLAPI void  (*) (int x, int y)) GetProcAddress(rl_lib_handle,"SetWindowPosition");
  rl_set_window_title = (RLAPI void  (*) (const char *title)) GetProcAddress(rl_lib_handle,"SetWindowTitle");
  rl_set_window_icons = (RLAPI void  (*) (RlImage *images, int count)) GetProcAddress(rl_lib_handle,"SetWindowIcons");
  rl_set_window_icon = (RLAPI void  (*) (RlImage image)) GetProcAddress(rl_lib_handle,"SetWindowIcon");
  rl_restore_window = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"RestoreWindow");
  rl_minimize_window = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"MinimizeWindow");
  rl_maximize_window = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"MaximizeWindow");
  rl_toggle_borderless_windowed = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"ToggleBorderlessWindowed");
  rl_toggle_fullscreen = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"ToggleFullscreen");
  rl_clear_window_state = (RLAPI void  (*) (unsigned int flags)) GetProcAddress(rl_lib_handle,"ClearWindowState");
  rl_set_window_state = (RLAPI void  (*) (unsigned int flags)) GetProcAddress(rl_lib_handle,"SetWindowState");
  rl_is_window_state = (RLAPI bool  (*) (unsigned int flag)) GetProcAddress(rl_lib_handle,"IsWindowState");
  rl_is_window_resized = (RLAPI bool  (*) (void)) GetProcAddress(rl_lib_handle,"IsWindowResized");
  rl_is_window_focused = (RLAPI bool  (*) (void)) GetProcAddress(rl_lib_handle,"IsWindowFocused");
  rl_is_window_maximized = (RLAPI bool  (*) (void)) GetProcAddress(rl_lib_handle,"IsWindowMaximized");
  rl_is_window_minimized = (RLAPI bool  (*) (void)) GetProcAddress(rl_lib_handle,"IsWindowMinimized");
  rl_is_window_hidden = (RLAPI bool  (*) (void)) GetProcAddress(rl_lib_handle,"IsWindowHidden");
  rl_is_window_fullscreen = (RLAPI bool  (*) (void)) GetProcAddress(rl_lib_handle,"IsWindowFullscreen");
  rl_is_window_ready = (RLAPI bool  (*) (void)) GetProcAddress(rl_lib_handle,"IsWindowReady");
  rl_window_should_close = (RLAPI bool  (*) (void)) GetProcAddress(rl_lib_handle,"WindowShouldClose");
  rl_close_window = (RLAPI void  (*) (void)) GetProcAddress(rl_lib_handle,"CloseWindow");
  rl_init_window = (RLAPI void  (*) (int width, int height, const char *title)) GetProcAddress(rl_lib_handle,"InitWindow");
}

void rl_free_lib(void){
  FreeLibrary(rl_lib_handle);
}

