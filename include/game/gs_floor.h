/**
 * @file gs_floor.h
 * @brief GSfloor -- Genius Sonority floor/scene management system.
 *
 * GSfloor manages the loading and unloading of world areas ("floors") in
 * Pokemon Colosseum. Each floor corresponds to a named area (e.g. D1_garage_1F,
 * M1_out, S1_out) and has associated FSYS archives containing models, scripts,
 * textures, particles, sound, collision data, and a REL module for area code.
 *
 * The floor system uses a cooperative thread (GSthread) that runs as a state
 * machine. Floor transitions follow this sequence:
 *   1. Unload old floor resources (textures, particles, collision, REL)
 *   2. Load new floor FSYS archive
 *   3. Parse and allocate floor resource chunks (map, textures, particles, etc.)
 *   4. Register loaded resources with the engine subsystems
 *   5. Resume gameplay on the new floor
 *
 * The floor data table at lbl_80402518 stores 0x80 entries of 0x48 bytes each,
 * describing the FSYS archive file handle and reference count for each floor.
 *
 * The floor resource handler table at lbl_80404918 stores 0x18 entries of 0x10
 * bytes each, mapping resource type IDs to load/size callbacks.
 *
 * Address range: 0x800FF788 - 0x80110000 (GSfloor core, ~199 functions)
 *                0x8011432C - 0x80114CA8 (Floor resource read pre-functions)
 *
 * Debug strings:
 *   "GSfloorOpen: cannot find floor %d"
 *   "loadParticle(): loading..."
 *   "loadBGM(): loading [%s]..."
 *   "_floorGetFilesize(): can't open file [%s]"
 *   "_floorLoadFile(): can't open file [%s]"
 *   "_floorLoadFile(): error reading file [%s]"
 */
#ifndef GS_FLOOR_H
#define GS_FLOOR_H

#include "dolphin/types.h"

/* ===================================================================
 * Constants
 * =================================================================== */

/** Maximum number of floor data entries in the static table */
#define GSFLOOR_MAX_ENTRIES     0x80

/** Size of one floor data entry in bytes */
#define GSFLOOR_ENTRY_SIZE      0x48

/** Maximum number of resource type handlers */
#define GSFLOOR_MAX_RES_TYPES   0x18

/** Size of one resource handler entry in bytes */
#define GSFLOOR_RES_HANDLER_SIZE 0x10

/** Floor ID encoding: upper 16 bits are floor index, lower 16 bits are 0x5001 */
#define GSFLOOR_ID_BASE         0x5001

/** Invalid floor ID sentinel */
#define GSFLOOR_ID_INVALID      ((u32)-1)

/* ===================================================================
 * Floor state machine phases
 * =================================================================== */
typedef enum GSFloorState {
    GSFLOOR_STATE_IDLE          = 0,  /**< No floor active, waiting for open */
    GSFLOOR_STATE_LOADING       = 1,  /**< Loading new floor resources */
    GSFLOOR_STATE_RUNNING       = 2,  /**< Floor loaded, running resource updates */
    GSFLOOR_STATE_UNLOADING     = 3,  /**< Unloading current floor, preparing transition */
    GSFLOOR_STATE_TRANSITIONING = 4,  /**< Mid-transition, switching floor data */
    GSFLOOR_STATE_FINALIZING    = 5,  /**< Final phase, completing transition */
} GSFloorState;

/* ===================================================================
 * Floor resource chunk states (used by GSFloorResource.status)
 * =================================================================== */
typedef enum GSFloorResStatus {
    GSFLOOR_RES_FREE       = 0,  /**< Resource slot unused */
    GSFLOOR_RES_LOADED     = 1,  /**< Resource loaded into memory */
} GSFloorResStatus;

/* ===================================================================
 * Structures
 * =================================================================== */

/**
 * GSFloorContext -- runtime floor state, accessed via gsFloorCurrent.
 *
 * Size: at least 0x14 bytes. Stored in the GSmem-allocated pool
 * (handle gsFloorPoolHandle, pointer gsFloorPoolPtr).
 *
 * Fields recovered from disassembly of fn_800FF788 and fn_800FF970:
 *   0x00: void*  floorDataEntry  -- pointer to GSFloorDataEntry for this floor
 *   0x04: u32    floorId         -- composite floor ID (floor_index * 0x5001 + base)
 *   0x08: u16    resMemHandle    -- GSmem handle for resource memory block
 *   0x0A: u8     doFadeIn        -- if nonzero, fade in on entry
 *   0x0B: u8     doFadeOut       -- if nonzero, fade out on exit
 *   0x10: u32    isActive        -- 1 if floor is the active scene
 */
typedef struct GSFloorContext {
    /* 0x00 */ void*   floorDataEntry;
    /* 0x04 */ u32     floorId;
    /* 0x08 */ u16     resMemHandle;
    /* 0x0A */ u8      doFadeIn;
    /* 0x0B */ u8      doFadeOut;
    /* 0x0C */ u32     pad0C;
    /* 0x10 */ u32     isActive;
} GSFloorContext;

/**
 * GSFloorResource -- describes one loaded resource chunk.
 *
 * Size: 0x24 bytes (36 bytes).
 * The floor system allocates an array of these: total = maxBase + maxExt1 + maxExt2.
 * Resources are organized into three pools:
 *   Pool 0: base resources (count = maxBase)
 *   Pool 1: extended pool 1 (count = maxExt1, starts at base[maxBase])
 *   Pool 2: extended pool 2 (count = maxExt2, starts at pool1[maxExt1])
 *
 * Fields recovered from iteration patterns in fn_800FF970:
 *   0x00: GSFloorResource* prev  -- linked list: previous resource
 *   0x04: GSFloorResource* next  -- linked list: next resource
 *   0x08: u32  active            -- nonzero if resource is in use
 *   0x0C: u32  status            -- 0 = base, 1 = loaded (GSFloorResStatus)
 *   0x10: u32  floorId           -- owning floor ID
 *   0x14: u8   priority          -- scheduling priority
 *   0x15: u8   pending           -- resource pending flag (cleared on load)
 *   0x16: u8   pad16
 *   0x17: u8   pad17
 *   0x18: void* callback         -- resource load callback function pointer
 *   0x1C: void* modelHandle      -- loaded model/data handle
 *   0x20: u16  textureHandle     -- GSmem handle for associated texture
 *   0x22: u16  pad22
 */
typedef struct GSFloorResource {
    /* 0x00 */ struct GSFloorResource* prev;
    /* 0x04 */ struct GSFloorResource* next;
    /* 0x08 */ u32     active;
    /* 0x0C */ u32     status;
    /* 0x10 */ u32     floorId;
    /* 0x14 */ u8      priority;
    /* 0x15 */ u8      pending;
    /* 0x16 */ u8      pad16;
    /* 0x17 */ u8      pad17;
    /* 0x18 */ void*   callback;
    /* 0x1C */ void*   modelHandle;
    /* 0x20 */ u16     textureHandle;
    /* 0x22 */ u16     pad22;
} GSFloorResource;

/**
 * GSFloorDataEntry -- static floor table entry.
 *
 * Size: 0x48 bytes (72 bytes).
 * Stored in the floor data table at lbl_80402518.
 * 0x80 entries total, describing each floor area the game can load.
 *
 * Fields recovered from fn_8010147C:
 *   0x40: u32  fsysFileHandle   -- FSYS archive file handle for this floor
 *   0x44: u32  refCount         -- reference count (0 = unused)
 */
typedef struct GSFloorDataEntry {
    /* 0x00 */ u8   data[0x40];      /**< Floor-specific data (area config, etc.) */
    /* 0x40 */ u32  fsysFileHandle;  /**< FSYS archive file handle */
    /* 0x44 */ u32  refCount;        /**< Reference count: 0 = free, >0 = in use */
} GSFloorDataEntry;

/**
 * GSFloorResHandler -- resource type handler table entry.
 *
 * Size: 0x10 bytes (16 bytes).
 * Stored at lbl_80404918. Each entry maps a resource type byte to
 * a pair of callbacks: one to compute the size of the resource,
 * and one to read/load it into memory.
 *
 * Fields recovered from fn_800FF970 (state 4):
 *   0x00: u8   typeId        -- resource type identifier (matched against header byte)
 *   0x01-0x03: padding
 *   0x04: u32  reserved
 *   0x08: void* readFunc     -- function to read/load the resource
 *   0x0C: void* sizeFunc     -- function to compute the resource size
 */
typedef struct GSFloorResHandler {
    /* 0x00 */ u8    typeId;
    /* 0x01 */ u8    pad01[3];
    /* 0x04 */ u32   reserved;
    /* 0x08 */ void* readFunc;
    /* 0x0C */ void* sizeFunc;
} GSFloorResHandler;

/* ===================================================================
 * Public API
 * =================================================================== */

/**
 * GSfloorInit -- Initialize the floor system.
 *
 * Allocates resource arrays and the floor context pool from GSmem.
 * Creates the floor management thread using GSthreadCreate.
 *
 * @param maxFloors   Maximum floor index count (for floor data array sizing).
 * @param maxBase     Number of base resource slots.
 * @param maxExt1     Number of extended pool 1 resource slots.
 * @param maxExt2     Number of extended pool 2 resource slots.
 *
 * Corresponds to fn_800FF828.
 * Note: This was previously labeled "GSthreadPoolConfig" in gs_thread.h
 * before the floor system was fully analyzed. The function belongs to
 * the floor subsystem, not the thread subsystem.
 */
void GSfloorInit(u32 maxFloors, u32 maxBase, u32 maxExt1, u32 maxExt2);

/**
 * GSfloorOpen -- Request a floor to be loaded.
 *
 * Searches the floor data table for the given floor ID. If found,
 * stores it as the pending floor and sets the state to LOADING.
 * If not found, prints "GSfloorOpen: cannot find floor %d".
 *
 * @param floorId     Floor identifier (used as index into floor data array).
 *
 * Corresponds to fn_800FF788.
 */
void GSfloorOpen(u32 floorId);

/**
 * GSfloorSetFloorTable -- Set the floor data table pointer and count.
 *
 * @param tablePtr    Pointer to the floor data entry array.
 * @param count       Number of entries in the table.
 *
 * Corresponds to fn_800FF81C.
 */
void GSfloorSetFloorTable(void* tablePtr, u32 count);

/**
 * GSfloorThreadMain -- Main thread entry for the floor state machine.
 *
 * Runs as a cooperative GSthread, cycling through states:
 *   IDLE -> LOADING -> RUNNING -> UNLOADING -> TRANSITIONING -> IDLE
 *
 * This is the thread function passed to GSthreadCreate during init.
 *
 * Corresponds to fn_800FF970.
 */
void GSfloorThreadMain(void);

/**
 * GSfloorUpdate -- Per-frame update for the running floor.
 *
 * Called from GSfloorThreadMain while in RUNNING state.
 * Iterates active floor resources, invokes callbacks, checks for
 * model and texture loading completion.
 *
 * @param ctx         Pointer to the current GSFloorContext.
 * @return            1 if still loading (yield), 0 if all resources ready.
 *
 * Corresponds to fn_80100B24.
 */
u8 GSfloorUpdate(GSFloorContext* ctx);

/**
 * GSfloorLoadParticle -- Load particle data for the current floor.
 *
 * Allocates memory, reads particle data from the FSYS archive, and
 * copies it into the particle buffer.
 *
 * @param dst         Destination buffer for particle data.
 * @param size        Size of the particle data in bytes.
 * @param callback    Completion callback.
 * @param callbackArg Argument to the callback.
 *
 * Corresponds to fn_80101244.
 */
void GSfloorLoadParticle(void* dst, u32 size, void* callback, void* callbackArg);

/**
 * GSfloorFindAndOpen -- Find a floor's FSYS data and request loading.
 *
 * Looks up the floor name in the FSYS TOC, resolves its file handle,
 * and calls the resource loader to begin reading the archive.
 *
 * @param archiveName Name of the FSYS archive (e.g. "s1_out.fsys").
 * @param floorId     Floor ID to associate with the loaded data.
 * @param loadParam   Additional load parameter.
 *
 * Corresponds to fn_801012E8.
 */
void GSfloorFindAndOpen(const char* archiveName, u32 floorId, u32 loadParam);

/**
 * GSfloorLoadData -- Load data from a named FSYS sub-file.
 *
 * Searches the archive's file list for the given name, allocates memory,
 * and begins loading the file data.
 *
 * @param archiveName  FSYS archive name string.
 * @param subFileIndex Index of the sub-file to load.
 * @param floorId      Associated floor ID.
 * @param loadParam    Load parameter / callback data.
 *
 * Corresponds to fn_801013A0.
 */
void GSfloorLoadData(const char* archiveName, u32 subFileIndex,
                     u32 floorId, u32 loadParam);

/**
 * GSfloorLoadMain -- Main floor loading procedure.
 *
 * Called to load or reload a floor's FSYS archive. Manages the floor
 * data entry table, allocates a resource load thread, and registers
 * the floor in the static floor data table.
 *
 * @param fsysHandle    FSYS file handle for the floor's archive.
 * @param archiveName   Name of the archive (for lookup).
 * @param loadParam1    Load parameter 1.
 * @param loadParam2    Load parameter 2.
 *
 * Corresponds to fn_8010147C.
 */
void GSfloorLoadMain(u32 fsysHandle, const char* archiveName,
                     u32 loadParam1, u32 loadParam2);

/**
 * GSfloorGetCurrentId -- Return the current active floor ID.
 *
 * @return  The current floor ID, or GSFLOOR_ID_INVALID if none.
 *
 * Corresponds to lbl_80478B18 read.
 */
u32 GSfloorGetCurrentId(void);

/**
 * GSfloorGetContext -- Return the current floor context pointer.
 *
 * @return  Pointer to GSFloorContext, or NULL if no floor active.
 *
 * Corresponds to lbl_8047ACC8 read (gsFloorCurrent).
 */
GSFloorContext* GSfloorGetContext(void);

/* ===================================================================
 * Floor resource read pre-functions (floorRead*PreFunc)
 * These are registered as resource handlers and called during floor
 * loading to process specific resource types from the FSYS archive.
 * =================================================================== */

/**
 * floorReadGFLPreFunc -- Pre-process GFL (model) resource data.
 * Allocates memory for the model and queues it for loading.
 * String: "floorReadGFLPreFunc(): can't alloc %d bytes of memory"
 * Corresponds to fn_8011432C.
 */
void floorReadGFLPreFunc(void);

/**
 * floorReadSoundPreFunc -- Pre-process sound resource data.
 * Validates sound buffer sizes and registers sound resources.
 * String: "ERROR: Over Sound Buffer! snd_res_id=%d buffer size=%d"
 * Corresponds to fn_8011487C.
 */
void floorReadSoundPreFunc(void);

/**
 * floorReadParticlePreFunc -- Pre-process particle resource data.
 * Allocates particle buffers and queues particle data for loading.
 * String: "floorReadParticlePreFunc(): can't alloc %d bytes of memory"
 * Corresponds to fn_80114AE0.
 */
void floorReadParticlePreFunc(void);

#endif /* GS_FLOOR_H */
