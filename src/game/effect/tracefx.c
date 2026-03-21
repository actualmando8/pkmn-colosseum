/**
 * @file tracefx.c
 * @brief TraceFX -- Trail / trace visual effects for Pokemon Colosseum.
 *
 * Decompiled from:
 *   fn_80137114 (tracefxRender)          -- Per-frame trail rendering
 *   fn_8013735C (tracefxInit)            -- Initialise a TraceFXWork structure
 *   fn_8013757C (tracefxStartEffectImpl) -- Internal start implementation
 *   fn_80137780 (tracefxStopEffectImpl)  -- Internal stop / cleanup
 *   fn_8013796C (tracefxStartUpdate)     -- Begin trail update cycle
 *   fn_801379E4 (tracefxSetTrailParam)   -- Set a trail parameter
 *   fn_80137A2C (tracefxSetTrailColor)   -- Set trail RGBA colour
 *   fn_80137AA4 (tracefxStartEffect)     -- Public start API
 *   fn_80137D14 (tracefxAddSegment)      -- Add segments to a running trail
 *   fn_80137F58 (tracefxUpdate)          -- Per-frame trail logic update
 *
 * Debug strings:
 *   "tracefxStartEffect: Could not start trail effect!"
 *       (lbl_80272B08 -- referenced when allocation fails)
 *
 * The trail effect system renders motion trails behind moving objects
 * (e.g., attack animations, Pokemon tails).  Each trail consists of a
 * chain of segments that are generated from model bone positions,
 * interpolated over time, and faded out as they age.
 *
 * A trail effect works by:
 *   1. Loading two model references: a "start bone" and an "end bone".
 *   2. Each frame, sampling the bone positions and creating a quad strip
 *      between consecutive samples.
 *   3. Applying colour fade and width taper over the segment lifetime.
 *   4. Rendering via the GS rendering pipeline (GSpart model system).
 *
 * Address range: 0x80137114 - 0x801380D4 (approx.)
 */

#include "dolphin/types.h"
#include "game/effect/gs_effect.h"

/* ===== External engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);          /* OSReport / GSlog */
extern u32   GSgfxGetFrameCount(void);                   /* fn_800D37CC */
extern void* fn_800F9318(u32 group, u32 model);         /* GSfloor model load */
extern void* fn_800EE150(void* model, u16 partIdx);     /* GSpart get sub-part */
extern void  fn_800EE3BC(void* part, void* outPos,
                          void* a, void* b);             /* GSpart get position */
extern void  fn_800EE828(void* part);                    /* GSpart commit */
extern void  fn_800E01D0(void* dst, void* src);         /* Vec3 copy */
extern void  fn_800E090C(void* dst, void* srcA,
                          void* srcB, f32 t);            /* Vec3 lerp */
extern void  fn_800E4014(void* model, u32 flag);        /* GSpart set visibility */
extern u32   fn_801DB060(void);                          /* Random seed generator */
extern void  fn_8010147C(u32 memOffset, u32 resId,
                          u32 size, u32 handle);         /* GSfloor load resource */
extern void  fn_801013A0(u32 memOffset, u32 size,
                          u32 data, u32 handle);         /* GSfloor load data */
extern void  memset(void* dst, u32 val, u32 size);

/* ===== String constants (rodata) ===== */
extern const char lbl_80272B08[]; /* "tracefxStartEffect: Could not start trail effect!" */

/* ===== SDA21 float constants ===== */
extern f32 lbl_8047D118;   /* 60.0f -- frames-per-second constant */
extern f64 lbl_8047D128;   /* 4503599627370496.0 -- int-to-float magic */
extern f32 lbl_8047D130;   /* lerp denominator constant */
extern f64 lbl_8047D140;   /* int-to-float magic (unsigned) */

/* Forward declarations for converted functions */
u32 fn_801379E4(void);
u32 fn_80137A2C(void);
void fn_80137114(void);
void fn_8013735C(void);
void fn_8013757C(void);
void fn_80137780(void);
void fn_8013796C(void);
void fn_80137AA4(void);
void fn_80137D14(void);
void fn_80137F58(void);


/* =======================================================================
 *  tracefxInit / fn_8013735C
 *  Address: 0x8013735C, Size: 0x220
 *
 *  Initialises a TraceFXWork structure from the parameter block.
 *
 *  The parameter block layout (pointed to by r31 / "params"):
 *    0x00: f32 startPos.x
 *    0x04: f32 startPos.y
 *    0x08: f32 startPos.z
 *    0x0C: u32 packedRGBA  (A in bits 31-24, B in 23-16, G in 15-8, R in 7-0)
 *    0x10: f32 width
 *    0x14: f32 height
 *    0x18: f32 depth
 *    0x1C: u32 segCountA (as lhz -- lower 16 bits)
 *    0x20: u32 segCountB (as lhz -- lower 16 bits)
 *    0x24: f32 scaleX
 *    0x28: f32 scaleY
 *    0x2C: f32 scaleZ
 *    0x30: f32 endScale
 *    0x34: f32 fadeRate
 *    0x38: u32 modelResId
 *    0x3C: u32 trailType (0=standard, 1=reversed, 2=custom)
 *    0x40: u32 extraFlags
 *
 *  Assembly (abbreviated):
 *    mr r28, r3              ; work
 *    mr r31, r4              ; params pointer
 *    mr r27, r5              ; frames
 *    memset(r28, 0, 0xAC)   ; zero the work area
 *    -- Check trail type (params+0x3C) --
 *    lwz r0, 0x3C(r31)
 *    cmpwi r0, 1 -> reversed path
 *    cmpwi r0, 2 -> custom path
 *    -- Standard path: --
 *    stw 0, 0xA8(r28)       ; flags = 0, adjust offset by -4
 *    -- Custom path: --
 *    lwz r0, 0x40(r31)      ; extra flags
 *    stw r0, 0xA8(r28)
 *    -- Calculate lifetime in frames --
 *    bl GSgfxGetFrameCount   ; currentFrame
 *    convert (currentFrame * frames / 60.0f) -> u16
 *    sth result, 0xA6(r28)  ; lifetime
 *    -- Copy start position (3 floats) --
 *    lfs f0, 0x00(r31); stfs f0, 0x48(r28)  ; x
 *    lfs f0, 0x04(r31); stfs f0, 0x4C(r28)  ; y
 *    lfs f0, 0x08(r31); stfs f0, 0x50(r28)  ; z
 *    -- Extract RGBA from packed u32 at params+0x0C --
 *    lwz r0, 0x0C(r31); srawi r0, r0, 24; stb r0, 0x63(r28)  ; A
 *    lwz r0, 0x0C(r31); extrwi 8,8;        stb r0, 0x62(r28)  ; B
 *    lwz r0, 0x0C(r31); extrwi 8,16;       stb r0, 0x61(r28)  ; G
 *    lwz r0, 0x0C(r31);                     stb r0, 0x60(r28)  ; R
 *    -- Copy size/scale parameters --
 *    lfs, stfs for width(0x10->0x64), height(0x14->0x68), depth(0x18->0x6C)
 *    -- Copy segment counts, ensure nonzero --
 *    lhz for segCountA (0x1C->0x70), segCountB (0x20->0x72)
 *    if segCountA == 0: segCountA = 1
 *    if segCountB == 0: segCountB = 1
 *    -- Copy scale factors --
 *    lfs for scaleX(0x24->0x90), scaleY(0x28->0x94), scaleZ(0x2C->0x98),
 *    endScale(0x30->0x9C), fadeRate(0x34->0xA0)
 *    -- Generate random seeds --
 *    stw 20000, 0x74(r28)     ; randomSeed constant
 *    bl fn_801DB060; stw r3, 0x7C(r28)  ; random handle 1
 *    bl fn_801DB060; stw r3, 0x78(r28)  ; random handle 2
 *    -- Load trail model resources --
 *    bl fn_8010147C(memOffset, resId, 20000, handle1)
 *    bl fn_800F9318(20000, handle1)
 *    bl fn_801013A0(memOffset, 20000, 0, handle2)
 *    bl fn_800F9318(20000, handle2)
 *    if model != NULL:
 *      fn_800E4014(model, 0)   ; hide model initially
 *    -- Calculate return value (memory consumed) --
 *    r29 = align32(resIdParam + 0x1F) + align32(memConsumed)
 *    return r29
 * ======================================================================= */
u32 tracefxInit(TraceFXWork* work, void* params, u32 frames) {
    u8* p = (u8*)params;
    u32 packedColor;
    u32 frameCount;
    u32 trailType;
    f32 lifetime;
    s32 memOffset;
    u32 resId;
    void* model;
    u32 consumed;

    /* Zero the work area */
    memset(work, 0, 0xAC);

    /* Determine trail type */
    trailType = *(u32*)(p + 0x3C);
    if (trailType >= 1 && trailType < 2) {
        /* Reversed trail */
        work->flags = 0;
        memOffset = -4;
    } else if (trailType == 2) {
        /* Custom trail: use extra flags */
        work->flags = *(u32*)(p + 0x40);
        memOffset = 0;
    } else {
        /* Standard trail */
        work->flags = *(u32*)(p + 0x40);
        memOffset = 0;
    }

    /* Calculate lifetime in frames */
    frameCount = GSgfxGetFrameCount();
    lifetime = (f32)((s32)frameCount * (s32)frames) / 60.0f;
    work->lifetime = (u16)(s32)lifetime;

    /* Copy starting position */
    work->startPos[0] = *(f32*)(p + 0x00);
    work->startPos[1] = *(f32*)(p + 0x04);
    work->startPos[2] = *(f32*)(p + 0x08);

    /* Extract RGBA colour from packed word */
    packedColor = *(u32*)(p + 0x0C);
    work->colorA = (u8)(packedColor >> 24);
    work->colorB = (u8)((packedColor >> 16) & 0xFF);
    work->colorG = (u8)((packedColor >> 8) & 0xFF);
    work->colorR = (u8)(packedColor & 0xFF);

    /* Copy width/height/depth */
    work->width  = *(f32*)(p + 0x10);
    work->height = *(f32*)(p + 0x14);
    work->depth  = *(f32*)(p + 0x18);

    /* Copy segment counts (ensure at least 1) */
    work->segmentCountA = (u16)(*(u32*)(p + 0x1C));
    work->segmentCountB = (u16)(*(u32*)(p + 0x20));
    if (work->segmentCountA == 0) {
        work->segmentCountA = 1;
    }
    if (work->segmentCountB == 0) {
        work->segmentCountB = 1;
    }

    /* Copy scale factors */
    work->scaleX    = *(f32*)(p + 0x24);
    work->scaleY    = *(f32*)(p + 0x28);
    work->scaleZ    = *(f32*)(p + 0x2C);
    work->endScale  = *(f32*)(p + 0x30);
    work->fadeRate   = *(f32*)(p + 0x34);

    /* Initialise random seeds */
    work->randomSeed = 20000;
    work->memHandle1 = fn_801DB060();
    work->memHandle2 = fn_801DB060();

    /* Load trail model resources */
    resId = *(u32*)(p + 0x38);
    consumed = (resId + 0x1F) & ~0x1F;  /* align to 32 bytes */

    fn_8010147C(consumed + (u32)memOffset, resId, 20000, work->memHandle1);
    model = fn_800F9318(20000, work->memHandle1);

    fn_801013A0(consumed + (u32)memOffset, 20000, 0, work->memHandle2);
    model = fn_800F9318(20000, work->memHandle2);

    /* Hide the model initially */
    if (model != NULL) {
        fn_800E4014(model, 0);
    }

    return consumed;
}

/* =======================================================================
 *  tracefxStartEffect / fn_80137AA4
 *  Address: 0x80137AA4, Size: 0x270
 *
 *  Public entry point for starting a trail effect.
 *
 *  1. Allocates an effect slot via GSEffectAllocSlot.
 *  2. Calls tracefxInit to set up the work area.
 *  3. Loads models via GSpart.
 *  4. Builds the initial trail geometry.
 *  5. If any step fails, prints the error and returns 0.
 *
 *  Assembly (heavily abbreviated):
 *    stmw r27, 0x1C(r1)
 *    -- Allocate effect slot --
 *    bl GSEffectAllocSlot
 *    cmplwi r3, 0 -> goto fail
 *    -- Initialise the TraceFXWork --
 *    bl tracefxInit
 *    -- Load start/end bone models --
 *    lhz r3, group(work); lhz r4, model(work)
 *    bl fn_800F9318         ; load start model
 *    cmplwi r3, 0 -> goto fail
 *    -- Get sub-parts and positions --
 *    bl fn_800EE150 (GSpart get sub-part)
 *    bl fn_800EE3BC (GSpart get position)
 *    bl fn_800EE828 (GSpart commit)
 *    -- Build initial segment list --
 *    -- If trail has interpolation flag set: --
 *      compute per-segment lerp factor
 *      for each segment:
 *        fn_800E090C (Vec3 lerp between start/end positions)
 *    -- else: --
 *      for each segment:
 *        fn_800E01D0 (Vec3 copy)
 *    -- Update segment counts --
 *    update work->segmentCountA, segmentCountB
 *    clamp to work->maxSegments
 *    return 1
 *  fail:
 *    lis r3, lbl_80272B08@ha
 *    addi r3, r3, lbl_80272B08@l
 *    bl OSReport
 *    return 0
 * ======================================================================= */
BOOL tracefxStartEffect(void* work, void* params, u32 frames) {
    TraceFXWork* traceWork = (TraceFXWork*)work;
    u8* p = (u8*)params;
    void* startModel;
    void* endModel;
    void* startPart;
    void* endPart;
    f32 startPos[3];
    f32 endPos[3];
    u32 numInitialSegs;
    u32 i;
    f32 t;
    u16 groupRes;
    u16 modelRes;

    /* Initialise the work area */
    tracefxInit(traceWork, params, frames);

    /* Load the start bone model */
    groupRes = *(u16*)(p + 0x24);
    modelRes = *(u16*)(p + 0x26);
    startModel = fn_800F9318((u32)groupRes, (u32)modelRes);
    if (startModel == NULL) {
        fn_800DD970(lbl_80272B08);
        return FALSE;
    }

    /* Load the trail's source model reference */
    if (traceWork->model == NULL) {
        fn_800DD970(lbl_80272B08);
        return FALSE;
    }

    /* Get sub-part for start position */
    startPart = fn_800EE150(startModel, *(u16*)(p + 0x28));
    if (startPart == NULL) {
        fn_800DD970(lbl_80272B08);
        return FALSE;
    }

    /* Get position from start part */
    fn_800EE3BC(startPart, startPos, NULL, NULL);
    fn_800EE828(startPart);

    /* Get sub-part for end position */
    endPart = fn_800EE150(startModel, *(u16*)(p + 0x2A));
    if (endPart == NULL) {
        fn_800DD970(lbl_80272B08);
        return FALSE;
    }

    /* Get position from end part */
    fn_800EE3BC(endPart, endPos, NULL, NULL);
    fn_800EE828(endPart);

    /* Build initial segment chain */
    numInitialSegs = (u32)traceWork->segmentCountA;

    /* Copy initial positions from model data */
    endModel = traceWork->model;
    fn_800E01D0(endModel, startPos);
    fn_800E01D0((u8*)endModel + 0x0C, endPos);

    return TRUE;
}

/* =======================================================================
 *  tracefxAddSegment / fn_80137D14
 *  Address: 0x80137D14, Size: 0x244
 *
 *  Adds new trail segments between the start and end bone positions.
 *  Called each frame while the trail effect is active.
 *
 *  The function:
 *  1. Checks if the segment count has reached the maximum.
 *  2. Loads the start/end bone models and gets their current positions.
 *  3. If interpolation is enabled (segmentCountA > 0), computes a lerp
 *     factor per segment (1.0 / segmentCountA) and interpolates between
 *     the old and new positions.
 *  4. Otherwise, copies the new positions directly.
 *  5. Updates the segment counts and clamps to the maximum.
 *
 *  Assembly (abbreviated -- 0x244 bytes):
 *    mr r30, r3              ; work
 *    mr r31, r4              ; numSegs
 *    lhz r3, 0x1C(r30)      ; current segment count
 *    lhz r0, 0x20(r30)      ; max segments
 *    cmplw r3, r0 -> full    ; trail is full
 *    -- Load bone models and positions --
 *    lhz r3, 0x24(r30); lhz r4, 0x26(r30)
 *    bl fn_800F9318
 *    bl fn_800EE150          ; get sub-part
 *    bl fn_800EE3BC          ; get position
 *    bl fn_800EE828          ; commit
 *    -- Check interpolation flag --
 *    lhz r0, 0x1C(r30)
 *    cmplwi r0, 0 -> skip interpolation
 *    -- Interpolation loop --
 *    compute f31 = 1.0 / segmentCount  (lerp step)
 *    for i = 0 to numSegs:
 *      f30 = f31 * (i + 1)
 *      fn_800E090C(dst, oldPos, newPos, f30)    ; lerp position
 *      fn_800E090C(dst+0xC, oldEndPos, newEndPos, f30)  ; lerp end
 *    -- Update counts --
 *    lhz r0, 0x1C(r30)
 *    add r0, r0, r31
 *    sth r0, 0x1C(r30)
 *    clamp to max
 *    return 1
 *  full:
 *    return 0
 * ======================================================================= */
BOOL tracefxAddSegment(void* work, u32 numSegs) {
    TraceFXWork* tw = (TraceFXWork*)work;
    void* model;
    void* part;
    f32 newStartPos[3];
    f32 newEndPos[3];
    f32 oldStartPos[3];
    f32 oldEndPos[3];
    u16 currentSegs;
    u16 maxSegs;
    u32 i;
    f32 lerpStep;
    f32 lerpT;
    void* segNode;

    currentSegs = *(u16*)((u8*)tw + 0x1C);
    maxSegs     = *(u16*)((u8*)tw + 0x20);

    /* Check if the trail is already full */
    if (currentSegs >= maxSegs) {
        return FALSE;
    }

    /* Load the bone model */
    model = fn_800F9318(*(u16*)((u8*)tw + 0x24),
                         *(u16*)((u8*)tw + 0x26));
    if (model == NULL) {
        return FALSE;
    }

    /* Get the source model reference */
    if (tw->model == NULL) {
        return FALSE;
    }

    /* Get start sub-part */
    part = fn_800EE150(model, *(u16*)((u8*)tw + 0x28));
    if (part == NULL) {
        return FALSE;
    }

    /* Sample new start position */
    fn_800EE3BC(part, newStartPos, NULL, NULL);
    fn_800EE828(part);

    /* Get end sub-part */
    part = fn_800EE150(model, *(u16*)((u8*)tw + 0x2A));
    if (part == NULL) {
        return FALSE;
    }

    /* Sample new end position */
    fn_800EE3BC(part, newEndPos, NULL, NULL);
    fn_800EE828(part);

    /* Walk to the current tail of the segment chain */
    segNode = tw->model;

    /* Save old positions for interpolation */
    if (currentSegs > 0) {
        fn_800E01D0(oldStartPos, segNode);
        fn_800E01D0(oldEndPos, (u8*)segNode + 0x0C);

        /* Compute per-segment lerp step */
        lerpStep = 1.0f / (f32)(numSegs + 1);

        /* Interpolate new segments */
        for (i = 0; i < numSegs; i++) {
            segNode = *(void**)((u8*)segNode + 0x18); /* advance to next node */
            lerpT = lerpStep * (f32)(i + 1);

            fn_800E090C(segNode, oldStartPos,
                        newStartPos, lerpT);
            fn_800E090C((u8*)segNode + 0x0C, oldEndPos,
                        newEndPos, lerpT);
        }
    } else {
        /* No interpolation -- just copy new positions */
        for (i = 0; i < numSegs; i++) {
            segNode = *(void**)((u8*)segNode + 0x18);
            fn_800E01D0(segNode, newStartPos);
            fn_800E01D0((u8*)segNode + 0x0C, newEndPos);
        }
    }

    /* Store the new tail node */
    tw->model = segNode;

    /* Update segment counts */
    currentSegs += (u16)numSegs;
    *(u16*)((u8*)tw + 0x1C) = currentSegs;

    *(u16*)((u8*)tw + 0x1E) += (u16)numSegs;

    /* Clamp to maximum */
    if (currentSegs > maxSegs) {
        *(u16*)((u8*)tw + 0x1C) = maxSegs;
    }

    {
        u16 segB = *(u16*)((u8*)tw + 0x1E);
        u16 maxB = *(u16*)((u8*)tw + 0x22);
        if (segB > maxB) {
            *(u16*)((u8*)tw + 0x1E) = maxB;
        }
    }

    /* Ensure segmentCountB does not exceed available space */
    {
        u16 segA = *(u16*)((u8*)tw + 0x1C);
        u16 max  = *(u16*)((u8*)tw + 0x20);
        u16 segB = *(u16*)((u8*)tw + 0x1E);
        u16 avail = max - segA;
        if (segB > avail) {
            *(u16*)((u8*)tw + 0x1E) = avail;
        }
    }

    return TRUE;
}

/* =======================================================================
 *  tracefxUpdate / fn_80137F58
 *  Address: 0x80137F58, Size: 0x17C
 *
 *  Per-frame update for a running trail effect.
 *
 *  Walks the segment chain and:
 *  1. Fades the alpha of each segment based on the fade rate.
 *  2. Scales segment widths toward the endScale value.
 *  3. Removes fully-faded segments.
 *  4. Updates the model reference.
 *
 *  Assembly (abbreviated):
 *    mr r27, r3              ; work
 *    lwz r0, 0x14(r3)       ; model reference
 *    cmplwi r0, 0 -> return  ; no model = nothing to do
 *    -- Walk segment chain --
 *    lhz r0, 0x1C(r27)      ; segmentCountA
 *    lhz r4, 0x20(r27)      ; maxSegments
 *    loop:
 *      load RGBA from segment
 *      multiply alpha by fadeRate
 *      if alpha <= 0: mark for removal
 *      scale width toward endScale
 *      advance to next segment
 *    end loop
 * ======================================================================= */
void tracefxUpdate(void* work) {
    TraceFXWork* tw = (TraceFXWork*)work;
    void* model;
    u16 segCount;
    u16 maxSegs;
    u32 i;

    model = tw->model;
    if (model == NULL) {
        return;
    }

    segCount = *(u16*)((u8*)tw + 0x1C);
    maxSegs  = *(u16*)((u8*)tw + 0x20);

    /* Iterate through active segments */
    for (i = 0; i < segCount; i++) {
        void* seg = *(void**)((u8*)model + 0x18);
        if (seg == NULL) {
            break;
        }

        /* Fade segment alpha */
        {
            f32 alpha = *(f32*)((u8*)seg + 0x10);
            alpha *= tw->fadeRate;
            *(f32*)((u8*)seg + 0x10) = alpha;
        }

        /* Scale segment width toward endScale */
        {
            f32 scaleX = *(f32*)((u8*)seg + 0x14);
            f32 diff = tw->endScale - scaleX;
            scaleX += diff * 0.1f;
            *(f32*)((u8*)seg + 0x14) = scaleX;
        }

        model = seg;
    }
}

/* ===================================================================
 * Generated: 0 pattern-matched + 10 stubs
 * Range: 0x80137114 - 0x801380D4
 * =================================================================== */

/* 0x80137114 | 0x248 */
void fn_80137114(void) {
    extern u8 lbl_8047D11C[];
    extern void fn_800D37CC();
    extern void fn_8013DB64();
    u8 sp[0x60];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    r28 = r4;
    r27 = r5;
    r31 = *(u32*)((u8*)r4 + 0x0);
    r29 = r3;
    r4 = 0x0;
    r5 = 0x20;
    memset((void*)r3, (int)r4, (u32)r5);
    tmp = *(u32*)((u8*)r28 + 0x4);
    if ((s32)tmp == 2) goto L_801371FC;
    if ((s32)tmp >= 2) goto L_8013716C;
    if ((s32)tmp >= 1) goto L_80137284;
    goto L_80137334;
L_8013716C:
    if ((s32)tmp >= 4) goto L_80137334;
    tmp = *(u32*)((u8*)r28 + 0x8);
    r30 = r28 + 0xc;
    f31 = *(f64*)&lbl_8047D128;
    r27 = 0x0;
    *(u32*)((u8*)r29 + 0x10) = tmp;
    r28 = 0x43300000;
    f30 = *(f32*)&lbl_8047D118;
    goto L_801371F0;
L_80137194:
    fn_800D37CC();
    tmp = *(u32*)((u8*)r30 + 0x8);
    r3 = r29;
    f1 = *(f32*)((u8*)r30 + 0x0);
    f2 = *(f32*)((u8*)r30 + 0x4);
    *(u32*)(sp + 0x1C) = tmp;
    f3 = f0 - f31;
    f0 = f0 - f31;
    f0 = f0 * f3;
    f0 = f0 / f30;
    f0 = (f64)(s32)f0;
    fn_8013DB64();
    r27 = r27 + 0x1;
    r30 = r30 + 0x10;
L_801371F0:
    if ((s32)r27 < (s32)r31) goto L_80137194;
    goto L_80137334;
L_801371FC:
    tmp = 0x1;
    f30 = *(f64*)&lbl_8047D128;
    *(u32*)((u8*)r29 + 0x10) = tmp;
    r30 = r28 + 0x8;
    f31 = *(f32*)&lbl_8047D118;
    r27 = 0x0;
    r28 = 0x43300000;
    goto L_80137278;
L_8013721C:
    fn_800D37CC();
    tmp = *(u32*)((u8*)r30 + 0x8);
    r3 = r29;
    f1 = *(f32*)((u8*)r30 + 0x0);
    f2 = *(f32*)((u8*)r30 + 0x4);
    *(u32*)(sp + 0x1C) = tmp;
    f3 = f0 - f30;
    f0 = f0 - f30;
    f0 = f0 * f3;
    f0 = f0 / f31;
    f0 = (f64)(s32)f0;
    fn_8013DB64();
    r27 = r27 + 0x1;
    r30 = r30 + 0x10;
L_80137278:
    if ((s32)r27 < (s32)r31) goto L_8013721C;
    goto L_80137334;
L_80137284:
    tmp = 0x1;
    *(u32*)((u8*)r29 + 0x10) = tmp;
    tmp = *(u32*)((u8*)r28 + 0x0);
    *(u32*)(sp + 0x8) = tmp;
    f30 = *(f32*)(sp + 0x8);
    fn_800D37CC();
    r5 = 0x43300000;
    *(u32*)(sp + 0x24) = tmp;
    r3 = 0x55550000;
    f5 = *(f64*)&lbl_8047D128;
    tmp = r3 + 0x5556;
    f0 = *(f32*)&lbl_8047D118;
    f2 = f30;
    r3 = r29;
    f4 = f1 - f5;
    f1 = *(f32*)lbl_8047D11C;
    f3 = f3 - f5;
    f3 = f3 * f4;
    f0 = f3 / f0;
    f0 = (f64)(s32)f0;
    r4 = (s32)((s64)tmp * (s64)r27 >> 32);
    tmp = (u32)r4 >> 31;
    r28 = r4 + tmp;
    r4 = r28;
    fn_8013DB64();
    f1 = f30;
    r3 = r29;
    f2 = f30;
    r4 = r28;
    fn_8013DB64();
    f1 = *(f32*)lbl_8047D11C;
    r3 = r29;
    r4 = r28;
    f2 = f1;
    fn_8013DB64();
L_80137334:
    r3 = r30;
    return;
}

/* 0x8013735C | 0x220 */
void fn_8013735C(void) {
    extern void fn_800D37CC();
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    r29 = r4;
    r27 = r5;
    r28 = r3;
    r30 = 0x0;
    r31 = r29;
    r4 = 0x0;
    r5 = 0xac;
    memset((void*)r3, (int)r4, (u32)r5);
    tmp = *(u32*)((u8*)r31 + 0x3C);
    if ((s32)tmp == 2) goto L_801373B8;
    if ((s32)tmp >= 2) goto L_801373B8;
    if ((s32)tmp >= 1) goto L_801373A8;
    goto L_801373B8;
L_801373A8:
    tmp = 0x0;
    r30 = -0x4;
    *(u32*)((u8*)r28 + 0xA8) = tmp;
    goto L_801373C0;
L_801373B8:
    tmp = *(u32*)((u8*)r31 + 0x40);
    *(u32*)((u8*)r28 + 0xA8) = tmp;
L_801373C0:
    fn_800D37CC();
    r4 = 0x43300000;
    f3 = *(f64*)&lbl_8047D128;
    f0 = *(f32*)&lbl_8047D118;
    *(u32*)(sp + 0x14) = tmp;
    f2 = f1 - f3;
    f1 = f1 - f3;
    f1 = f1 * f2;
    f0 = f1 / f0;
    f0 = (f64)(s32)f0;
    *(u16*)((u8*)r28 + 0xA6) = tmp;
    f0 = *(f32*)((u8*)r31 + 0x0);
    *(f32*)((u8*)r28 + 0x48) = f0;
    f0 = *(f32*)((u8*)r31 + 0x4);
    *(f32*)((u8*)r28 + 0x4C) = f0;
    f0 = *(f32*)((u8*)r31 + 0x8);
    *(f32*)((u8*)r28 + 0x50) = f0;
    tmp = *(u32*)((u8*)r31 + 0xC);
    tmp = (s32)tmp >> 24;
    *(u8*)((u8*)r28 + 0x63) = tmp;
    tmp = *(u32*)((u8*)r31 + 0xC);
    /* extrwi tmp, tmp, 8, 8 */;
    *(u8*)((u8*)r28 + 0x62) = tmp;
    tmp = *(u32*)((u8*)r31 + 0xC);
    /* extrwi tmp, tmp, 8, 16 */;
    *(u8*)((u8*)r28 + 0x61) = tmp;
    tmp = *(u32*)((u8*)r31 + 0xC);
    *(u8*)((u8*)r28 + 0x60) = tmp;
    f0 = *(f32*)((u8*)r31 + 0x10);
    *(f32*)((u8*)r28 + 0x64) = f0;
    f0 = *(f32*)((u8*)r31 + 0x14);
    *(f32*)((u8*)r28 + 0x68) = f0;
    f0 = *(f32*)((u8*)r31 + 0x18);
    *(f32*)((u8*)r28 + 0x6C) = f0;
    tmp = *(u32*)((u8*)r31 + 0x1C);
    *(u16*)((u8*)r28 + 0x70) = tmp;
    tmp = *(u32*)((u8*)r31 + 0x20);
    *(u16*)((u8*)r28 + 0x72) = tmp;
    r4 = *(u16*)((u8*)r28 + 0x70);
    r3 = (u32)r4 >> 31;
    tmp = r4 & 0x1;
    tmp = tmp ^ r3;
    /* subf. tmp, r3, tmp */;
    if ((s32)tmp != 1) goto L_8013749C;
    tmp = r4 + 0x1;
    *(u16*)((u8*)r28 + 0x70) = tmp;
L_8013749C:
    r4 = *(u16*)((u8*)r28 + 0x72);
    r3 = (u32)r4 >> 31;
    tmp = r4 & 0x1;
    tmp = tmp ^ r3;
    /* subf. tmp, r3, tmp */;
    if ((s32)tmp != 1) goto L_801374BC;
    tmp = r4 + 0x1;
    *(u16*)((u8*)r28 + 0x72) = tmp;
L_801374BC:
    f0 = *(f32*)((u8*)r31 + 0x24);
    r29 = r30 + r29;
    r3 = r29 + 0x63;
    tmp = 0x4e20;
    *(f32*)((u8*)r28 + 0x90) = f0;
    /* clrrwi r29, r3, 5 */;
    f0 = *(f32*)((u8*)r31 + 0x28);
    *(f32*)((u8*)r28 + 0x94) = f0;
    f0 = *(f32*)((u8*)r31 + 0x2C);
    *(f32*)((u8*)r28 + 0x98) = f0;
    f0 = *(f32*)((u8*)r31 + 0x30);
    *(f32*)((u8*)r28 + 0x9C) = f0;
    f0 = *(f32*)((u8*)r31 + 0x34);
    *(f32*)((u8*)r28 + 0xA0) = f0;
    *(u32*)((u8*)r28 + 0x74) = tmp;
    ((void(*)(void))fn_801DB060)();
    *(u32*)((u8*)r28 + 0x7C) = r3;
    ((void(*)(void))fn_801DB060)();
    *(u32*)((u8*)r28 + 0x78) = r3;
    r3 = r29;
    r5 = 0x4e20;
    r4 = *(u32*)((u8*)r31 + 0x38);
    r6 = *(u32*)((u8*)r28 + 0x7C);
    ((void(*)(void))fn_8010147C)();
    r4 = *(u32*)((u8*)r28 + 0x7C);
    r3 = 0x4e20;
    ((void(*)(void))fn_800F9318)();
    r6 = *(u32*)((u8*)r28 + 0x78);
    r4 = 0x4e20;
    r5 = 0x0;
    ((void(*)(void))fn_801013A0)();
    r4 = *(u32*)((u8*)r28 + 0x78);
    r3 = 0x4e20;
    ((void(*)(void))fn_800F9318)();
    if (r3 == 0) goto L_80137554;
    r4 = 0x0;
    ((void(*)(void))fn_800E4014)();
L_80137554:
    r3 = *(u32*)((u8*)r31 + 0x38);
    tmp = r3 + 0x1f;
    /* clrrwi tmp, tmp, 5 */;
    r29 = r29 + tmp;
    r3 = r29;
    return;
}

/* 0x8013757C | 0x204 */
void fn_8013757C(void) {
    extern u8 lbl_80314638[];
    extern u8 lbl_80314AE8[];
    extern void fn_800D37CC();
    extern void fn_800E27B0();
    extern void fn_800E2C04();
    extern void fn_800EF590();
    extern void fn_800EFD14();
    extern void fn_800EFD3C();
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    r30 = r4;
    r27 = r5;
    r29 = r3;
    r4 = 0x0;
    r31 = r30;
    r5 = 0x70;
    memset((void*)r3, (int)r4, (u32)r5);
    fn_800D37CC();
    r5 = 0x43300000;
    f3 = *(f64*)&lbl_8047D128;
    r3 = r29 + 0x34;
    r4 = r31 + 0xc;
    f0 = *(f32*)&lbl_8047D118;
    *(u32*)(sp + 0x14) = tmp;
    f2 = f1 - f3;
    f1 = f1 - f3;
    f1 = f1 * f2;
    f0 = f1 / f0;
    f0 = (f64)(s32)f0;
    *(u16*)((u8*)r29 + 0x12) = tmp;
    tmp = *(u32*)((u8*)r31 + 0x0);
    tmp = (u32)tmp >> 24;
    *(u8*)((u8*)r29 + 0x23) = tmp;
    tmp = *(u32*)((u8*)r31 + 0x0);
    /* extrwi tmp, tmp, 8, 8 */;
    *(u8*)((u8*)r29 + 0x22) = tmp;
    tmp = *(u32*)((u8*)r31 + 0x0);
    /* extrwi tmp, tmp, 8, 16 */;
    *(u8*)((u8*)r29 + 0x21) = tmp;
    tmp = *(u32*)((u8*)r31 + 0x0);
    *(u8*)((u8*)r29 + 0x20) = tmp;
    tmp = *(u32*)((u8*)r31 + 0x4);
    tmp = (u32)tmp >> 24;
    *(u8*)((u8*)r29 + 0x27) = tmp;
    tmp = *(u32*)((u8*)r31 + 0x4);
    /* extrwi tmp, tmp, 8, 8 */;
    *(u8*)((u8*)r29 + 0x26) = tmp;
    tmp = *(u32*)((u8*)r31 + 0x4);
    /* extrwi tmp, tmp, 8, 16 */;
    *(u8*)((u8*)r29 + 0x25) = tmp;
    tmp = *(u32*)((u8*)r31 + 0x4);
    *(u8*)((u8*)r29 + 0x24) = tmp;
    ((void(*)(void))fn_800E01D0)();
    r3 = r29 + 0x40;
    r4 = r31 + 0x18;
    ((void(*)(void))fn_800E01D0)();
    r3 = *(u32*)((u8*)r31 + 0x8);
    tmp = r30 + 0x73;
    /* clrrwi r30, tmp, 5 */;
    r4 = 0x20;
    *(u16*)((u8*)r29 + 0x8) = r3;
    f0 = *(f32*)((u8*)r31 + 0x24);
    *(f32*)((u8*)r29 + 0x4C) = f0;
    f0 = *(f32*)((u8*)r31 + 0x28);
    *(f32*)((u8*)r29 + 0x50) = f0;
    f0 = *(f32*)((u8*)r31 + 0x2C);
    *(f32*)((u8*)r29 + 0x54) = f0;
    f0 = *(f32*)((u8*)r31 + 0x30);
    *(f32*)((u8*)r29 + 0x58) = f0;
    f0 = *(f32*)((u8*)r31 + 0x34);
    *(f32*)((u8*)r29 + 0x5C) = f0;
    f0 = *(f32*)((u8*)r31 + 0x38);
    *(f32*)((u8*)r29 + 0x60) = f0;
    f0 = *(f32*)((u8*)r31 + 0x3C);
    *(f32*)((u8*)r29 + 0x64) = f0;
    f0 = *(f32*)((u8*)r31 + 0x40);
    *(f32*)((u8*)r29 + 0x68) = f0;
    f0 = *(f32*)((u8*)r31 + 0x44);
    *(f32*)((u8*)r29 + 0x6C) = f0;
    tmp = *(u32*)((u8*)r31 + 0x48);
    *(u16*)((u8*)r29 + 0xE) = tmp;
    r3 = *(u32*)((u8*)r31 + 0x4C);
    tmp = r3 + 0x1f;
    /* clrrwi r3, tmp, 5 */;
    fn_800E2C04();
    tmp = r3 & 0xFFFF;
    r28 = r3;
    if ((s32)tmp == 0) goto L_80137720;
    fn_800E27B0();
    r5 = *(u32*)((u8*)r31 + 0x4C);
    r27 = r3;
    r4 = r30;
    tmp = r5 + 0x1f;
    /* clrrwi r5, tmp, 5 */;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r3 = r27;
    fn_800EFD3C();
    *(u32*)((u8*)r29 + 0x1C) = r3;
    r4 = r28;
    r3 = *(u32*)((u8*)r29 + 0x1C);
    fn_800EFD14();
    goto L_80137728;
L_80137720:
    tmp = 0x0;
    *(u32*)((u8*)r29 + 0x1C) = tmp;
L_80137728:
    r4 = *(u32*)((u8*)r31 + 0x4C);
    r3 = *(u32*)((u8*)r29 + 0x1C);
    tmp = r4 + 0x1f;
    /* clrrwi tmp, tmp, 5 */;
    r30 = r30 + tmp;
    if (r3 == 0) goto L_80137750;
    r4 = 0x2;
    r5 = 0x2;
    fn_800EF590();
L_80137750:
    r3 = (u32)lbl_80314638;
    r4 = (u32)lbl_80314AE8;
    tmp = (u32)lbl_80314638;
    r3 = r30;
    *(u32*)((u8*)r29 + 0x14) = tmp;
    tmp = (u32)lbl_80314AE8;
    *(u32*)((u8*)r29 + 0x18) = tmp;
    return;
}

/* 0x80137780 | 0x1EC */
void fn_80137780(void) {
    extern void fn_800D37CC();
    extern void fn_8013AB60();
    u8 sp[0x40];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    r5 = 0x5c;
    r30 = r4;
    r27 = r3;
    r26 = 0x0;
    r28 = 0x1;
    r29 = *(u32*)((u8*)r4 + 0x4);
    r4 = 0x0;
    memset((void*)r3, (int)r4, (u32)r5);
    tmp = *(u32*)((u8*)r30 + 0x0);
    *(u8*)((u8*)r27 + 0x4C) = tmp;
    tmp = *(u32*)((u8*)r30 + 0x8);
    if ((s32)tmp == 3) goto L_801377DC;
    if ((s32)tmp >= 3) goto L_801377DC;
    if ((s32)tmp >= 1) goto L_801377D4;
    goto L_801377DC;
L_801377D4:
    r3 = -0x4;
    goto L_80137804;
L_801377DC:
    r3 = *(u32*)((u8*)r30 + 0xC);
    tmp = r3 & 0x00000004;
    if ((s32)tmp != 4) goto L_801377F0;
    r28 = 0x0;
L_801377F0:
    tmp = r3 & 0x1;
    if ((s32)tmp != 1) goto L_80137800;
    r26 = 0x1;
L_80137800:
    r3 = 0x0;
L_80137804:
    tmp = *(u32*)((u8*)r30 + 0x0);
    if ((s32)tmp != 0) goto L_80137818;
    r28 = 0x1;
    r26 = 0x1;
L_80137818:
    r30 = r3 + r30;
    *(u8*)((u8*)r27 + 0x4E) = r26;
    r30 = r30 + 0x10;
    r31 = r28 & 0xFF;
    r28 = r30;
    r26 = r26 & 0xFF;
    r30 = 0x0;
    goto L_8013794C;
L_80137838:
    if (r31 == 0) goto L_80137874;
    r4 = *(u32*)((u8*)r28 + 0x0);
    /* extrwi r3, r4, 8, 8 */;
    /* extrwi tmp, r4, 8, 16 */;
    *(u8*)(sp + 0xE) = r3;
    *(u8*)(sp + 0xD) = tmp;
    *(u8*)(sp + 0xC) = r4;
    r4 = *(u32*)((u8*)r28 + 0x4);
    /* extrwi r3, r4, 8, 8 */;
    /* extrwi tmp, r4, 8, 16 */;
    *(u8*)(sp + 0xA) = r3;
    *(u8*)(sp + 0x9) = tmp;
    *(u8*)(sp + 0x8) = r4;
    goto L_80137890;
L_80137874:
    tmp = 0x7f;
    *(u8*)(sp + 0xE) = tmp;
    *(u8*)(sp + 0xD) = tmp;
    *(u8*)(sp + 0xC) = tmp;
    *(u8*)(sp + 0xA) = tmp;
    *(u8*)(sp + 0x9) = tmp;
    *(u8*)(sp + 0x8) = tmp;
L_80137890:
    if (r26 == 0) goto L_801378B4;
    tmp = *(u32*)((u8*)r28 + 0x0);
    tmp = (u32)tmp >> 24;
    *(u8*)(sp + 0xF) = tmp;
    tmp = *(u32*)((u8*)r28 + 0x4);
    tmp = (u32)tmp >> 24;
    *(u8*)(sp + 0xB) = tmp;
    goto L_801378C0;
L_801378B4:
    tmp = 0xff;
    *(u8*)(sp + 0xF) = tmp;
    *(u8*)(sp + 0xB) = tmp;
L_801378C0:
    tmp = *(u32*)((u8*)r28 + 0x8);
    if ((s32)tmp >= 0) goto L_801378E4;
    r3 = r27;
    r4 = (u32)sp + 0xc;
    r5 = (u32)sp + 0x8;
    r6 = -0x1;
    fn_8013AB60();
    goto L_80137944;
L_801378E4:
    fn_800D37CC();
    r6 = 0x43300000;
    tmp = *(u32*)((u8*)r28 + 0x8);
    f3 = *(f64*)&lbl_8047D128;
    r3 = r27;
    f0 = *(f32*)&lbl_8047D118;
    r4 = (u32)sp + 0xc;
    r5 = (u32)sp + 0x8;
    *(u32*)(sp + 0x1C) = tmp;
    f2 = f1 - f3;
    f1 = f1 - f3;
    f1 = f1 * f2;
    f0 = f1 / f0;
    f0 = (f64)(s32)f0;
    fn_8013AB60();
L_80137944:
    r30 = r30 + 0x1;
    r28 = r28 + 0x10;
L_8013794C:
    if ((s32)r30 < (s32)r29) goto L_80137838;
    r3 = r28;
    return;
}

/* 0x78 | fn_8013796C | multi_call_guarded */
void fn_8013796C(void) {
    if (fn_80131428() == 0) { return; }
    fn_80131200();
    fn_8013139C();
}

/* 0x48 | fn_801379E4 | generic */
u32 fn_801379E4(void) {
    fn_800B8DF4();
    fn_800B856C();
    fn_800EF5A4();
    return 1;
}

/* 0x78 | fn_80137A2C | generic */
u32 fn_80137A2C(void) {
    fn_800B8DF4();
    fn_800B856C();
    fn_800E24B0();
    fn_800E209C();
    fn_800E24B0();
    fn_800E209C();
    return 1;
}

/* 0x80137AA4 | 0x270 */
void fn_80137AA4(void) {
    extern u8 lbl_8047D134[];
    extern u8 lbl_8047D138[];
    extern void fn_800E209C();
    extern void fn_800E24B0();
    extern void fn_800E27B0();
    extern void fn_800E3534();
    u8 sp[0x30];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;

    /* mr. r29, r3 */;
    if ((s32)tmp == 0) goto L_80137CEC;
    r28 = *(u16*)((u8*)r29 + 0x20);
    r27 = *(u16*)((u8*)r29 + 0x22);
    r3 = *(u16*)((u8*)r29 + 0x24);
    r4 = *(u16*)((u8*)r29 + 0x26);
    ((void(*)(void))fn_800F9318)();
    tmp = *(u32*)((u8*)r29 + 0x14);
    r30 = r3;
    if (tmp != 0) goto L_80137AE8;
    r3 = 0x0;
    goto L_80137D00;
L_80137AE8:
    if (r27 == 0) goto L_80137AF8;
    if (r28 != 0) goto L_80137B00;
L_80137AF8:
    r3 = 0x0;
    goto L_80137D00;
L_80137B00:
    if (r30 != 0) goto L_80137B10;
    r3 = 0x0;
    goto L_80137D00;
L_80137B10:
    r4 = *(u16*)((u8*)r29 + 0x28);
    ((void(*)(void))fn_800EE150)();
    if (r3 != 0) goto L_80137B28;
    r3 = 0x0;
    goto L_80137D00;
L_80137B28:
    ((void(*)(void))fn_800EE828)();
    r4 = *(u16*)((u8*)r29 + 0x2A);
    r3 = r30;
    ((void(*)(void))fn_800EE150)();
    if (r3 != 0) goto L_80137B48;
    r3 = 0x0;
    goto L_80137D00;
L_80137B48:
    ((void(*)(void))fn_800EE828)();
    /* extrwi tmp, r28, 15, 16 */;
    if ((s32)r27 <= (s32)tmp) goto L_80137B60;
    *(u16*)((u8*)r29 + 0x22) = tmp;
    r27 = tmp;
L_80137B60:
    r30 = r27 & 0xFFFF;
    r3 = r28;
    fn_800E3534();
    tmp = r3 & 0xFFFF;
    if ((s32)r27 != (s32)tmp) goto L_80137B80;
    r3 = 0x0;
    goto L_80137D00;
L_80137B80:
    *(u16*)((u8*)r29 + 0xC) = r3;
    fn_800E27B0();
    r31 = r3;
    r5 = r28;
    *(u32*)((u8*)r29 + 0x4) = r31;
    r4 = 0x0;
    *(u32*)((u8*)r29 + 0x0) = r31;
    memset((void*)r3, (int)r4, (u32)r5);
    tmp = r30;
    r3 = r6 << 5;
    r7 = 0x0;
    r5 = r31 + r3;
    goto L_80137C20;
L_80137BB8:
    r3 = r7 & 0xFFFF;
    if ((s32)r27 != (s32)tmp) goto L_80137BD0;
    r3 = r3 + 0x1c;
    *(u32*)(r31 + r3) = r5;
    goto L_80137BE8;
L_80137BD0:
    r4 = r4 << 5;
    r4 = r31 + r4;
    r3 = r3 + 0x1c;
    *(u32*)(r31 + r3) = r4;
L_80137BE8:
    r3 = r7 & 0xFFFF;
    if ((s32)r3 != (s32)r6) goto L_80137C04;
    r3 = r3 + 0x18;
    *(u32*)(r31 + r3) = r31;
    goto L_80137C1C;
L_80137C04:
    r4 = r3 + 0x1;
    r4 = r4 << 5;
    r4 = r31 + r4;
    r3 = r3 + 0x18;
    *(u32*)(r31 + r3) = r4;
L_80137C1C:
    r7 = r7 + 0x1;
L_80137C20:
    r3 = r7 & 0xFFFF;
    if (r3 < tmp) goto L_80137BB8;
    r3 = r30 << 4;
    fn_800E3534();
    tmp = r3 & 0xFFFF;
    if (r3 != tmp) goto L_80137C54;
    r3 = *(u16*)((u8*)r29 + 0xC);
    fn_800E24B0();
    r3 = *(u16*)((u8*)r29 + 0xC);
    fn_800E209C();
    r3 = 0x0;
    goto L_80137D00;
L_80137C54:
    *(u16*)((u8*)r29 + 0xE) = r3;
    fn_800E27B0();
    *(u32*)((u8*)r29 + 0x8) = r3;
    r5 = 0x0;
    r3 = 0x43300000;
    f3 = *(f64*)lbl_8047D138;
    *(u16*)((u8*)r29 + 0x1C) = r5;
    f4 = *(f32*)&lbl_8047D130;
    *(u16*)((u8*)r29 + 0x1E) = r5;
    f2 = *(f64*)&lbl_8047D140;
    r4 = *(u16*)((u8*)r29 + 0x22);
    r6 = *(u32*)((u8*)r29 + 0x8);
    f0 = *(f32*)lbl_8047D134;
    *(u32*)(sp + 0xC) = tmp;
    f1 = f1 - f3;
    f3 = f4 / f1;
    goto L_80137CD8;
L_80137CA8:
    tmp = r5 & 0xFFFF;
    r5 = r5 + 0x1;
    *(u32*)(sp + 0xC) = tmp;
    f1 = f1 - f2;
    f1 = f1 * f3;
    *(f32*)((u8*)r6 + 0x0) = f1;
    *(f32*)((u8*)r6 + 0x4) = f0;
    *(f32*)((u8*)r6 + 0x8) = f1;
    *(f32*)((u8*)r6 + 0xC) = f4;
    r6 = r6 + 0x10;
L_80137CD8:
    tmp = r5 & 0xFFFF;
    if (tmp < r4) goto L_80137CA8;
    r3 = 0x1;
    goto L_80137D00;
L_80137CEC:
    r3 = (u32)&lbl_80272B08;
    r3 = (u32)&lbl_80272B08;
    ((void(*)(void))fn_800DD970)();
    r3 = 0x0;
L_80137D00:
    return;
}

/* 0x80137D14 | 0x244 */
void fn_80137D14(void) {
    u8 sp[0x70];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f0 = 0.0f;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f30 = 0.0f;
    f32 f31 = 0.0f;

    r30 = r3;
    r31 = r4;
    r3 = *(u16*)((u8*)r3 + 0x1C);
    tmp = *(u16*)((u8*)r30 + 0x20);
    if (r3 >= tmp) goto L_80137F24;
    r3 = *(u16*)((u8*)r30 + 0x24);
    r4 = *(u16*)((u8*)r30 + 0x26);
    ((void(*)(void))fn_800F9318)();
    /* mr. r29, r3 */;
    if (r3 != tmp) goto L_80137D74;
    r3 = 0x0;
    goto L_80137F28;
L_80137D74:
    tmp = *(u32*)((u8*)r30 + 0x14);
    if (tmp != 0) goto L_80137D88;
    r3 = 0x0;
    goto L_80137F28;
L_80137D88:
    r4 = *(u16*)((u8*)r30 + 0x28);
    ((void(*)(void))fn_800EE150)();
    /* mr. r28, r3 */;
    if (tmp != 0) goto L_80137DA0;
    r3 = 0x0;
    goto L_80137F28;
L_80137DA0:
    r4 = (u32)sp + 0x2c;
    r5 = 0x0;
    r6 = 0x0;
    ((void(*)(void))fn_800EE3BC)();
    r3 = r28;
    ((void(*)(void))fn_800EE828)();
    r4 = *(u16*)((u8*)r30 + 0x2A);
    r3 = r29;
    ((void(*)(void))fn_800EE150)();
    /* mr. r29, r3 */;
    if (tmp != 0) goto L_80137DD4;
    r3 = 0x0;
    goto L_80137F28;
L_80137DD4:
    r4 = (u32)sp + 0x20;
    r5 = 0x0;
    r6 = 0x0;
    ((void(*)(void))fn_800EE3BC)();
    r3 = r29;
    ((void(*)(void))fn_800EE828)();
    tmp = *(u16*)((u8*)r30 + 0x1C);
    r29 = *(u32*)((u8*)r30 + 0x0);
    if (tmp == 0) goto L_80137E34;
    r4 = r29;
    r3 = (u32)sp + 0x14;
    ((void(*)(void))fn_800E01D0)();
    r3 = (u32)sp + 0x8;
    r4 = r29 + 0xc;
    ((void(*)(void))fn_800E01D0)();
    tmp = 0x43300000;
    f1 = *(f64*)&lbl_8047D140;
    *(u32*)(sp + 0x38) = tmp;
    f2 = *(f32*)&lbl_8047D130;
    f0 = f0 - f1;
    f31 = f2 / f0;
L_80137E34:
    r28 = 0x0;
    goto L_80137EB4;
L_80137E3C:
    tmp = *(u16*)((u8*)r30 + 0x1C);
    r29 = *(u32*)((u8*)r29 + 0x18);
    if (tmp == 0) goto L_80137E98;
    r3 = r28 + 0x1;
    tmp = 0x43300000;
    r3 = r29;
    f1 = *(f64*)&lbl_8047D140;
    r4 = (u32)sp + 0x14;
    *(u32*)(sp + 0x38) = tmp;
    r5 = (u32)sp + 0x2c;
    f0 = f0 - f1;
    f30 = f31 * f0;
    f1 = f30;
    ((void(*)(void))fn_800E090C)();
    f1 = f30;
    r3 = r29 + 0xc;
    r4 = (u32)sp + 0x8;
    r5 = (u32)sp + 0x20;
    ((void(*)(void))fn_800E090C)();
    goto L_80137EB0;
L_80137E98:
    r3 = r29;
    r4 = (u32)sp + 0x2c;
    ((void(*)(void))fn_800E01D0)();
    r3 = r29 + 0xc;
    r4 = (u32)sp + 0x20;
    ((void(*)(void))fn_800E01D0)();
L_80137EB0:
    r28 = r28 + 0x1;
L_80137EB4:
    if (r28 < r31) goto L_80137E3C;
    *(u32*)((u8*)r30 + 0x0) = r29;
    tmp = *(u16*)((u8*)r30 + 0x1C);
    tmp = tmp + r31;
    *(u16*)((u8*)r30 + 0x1C) = tmp;
    tmp = *(u16*)((u8*)r30 + 0x1E);
    tmp = tmp + r31;
    *(u16*)((u8*)r30 + 0x1E) = tmp;
    tmp = *(u16*)((u8*)r30 + 0x1C);
    r3 = *(u16*)((u8*)r30 + 0x20);
    if (tmp <= r3) goto L_80137EEC;
    *(u16*)((u8*)r30 + 0x1C) = r3;
L_80137EEC:
    tmp = *(u16*)((u8*)r30 + 0x1E);
    r3 = *(u16*)((u8*)r30 + 0x22);
    if (tmp <= r3) goto L_80137F00;
    *(u16*)((u8*)r30 + 0x1E) = r3;
L_80137F00:
    r3 = *(u16*)((u8*)r30 + 0x1C);
    tmp = *(u16*)((u8*)r30 + 0x20);
    r4 = *(u16*)((u8*)r30 + 0x1E);
    tmp = tmp - r3;
    if ((s32)r4 <= (s32)tmp) goto L_80137F1C;
    *(u16*)((u8*)r30 + 0x1E) = tmp;
L_80137F1C:
    r3 = 0x1;
    goto L_80137F28;
L_80137F24:
    r3 = 0x0;
L_80137F28:
    return;
}

/* 0x80137F58 | 0x17C */
void fn_80137F58(void) {
    extern void fn_800D2248();
    extern void fn_800D59B8();
    extern void fn_800D5CB8();
    extern void fn_800D6680();
    extern void fn_800D6728();
    extern void fn_800D67BC();
    extern void fn_800D6A00();
    extern void fn_800D7820();
    extern void fn_800D85D4();
    extern void fn_800D888C();
    extern void fn_800D88DC();
    extern void fn_800DA028();
    extern void fn_800DA1E8();
    extern void fn_800DA2BC();
    extern void fn_800DA4C4();
    u8 sp[0x20];
    u32 tmp = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;

    r27 = r3;
    tmp = *(u32*)((u8*)r3 + 0x14);
    r3 = *(u16*)((u8*)r3 + 0x1C);
    r31 = *(u16*)((u8*)r27 + 0x1E);
    if (tmp != 0) goto L_80137F88;
    r3 = 0x0;
    goto L_801380C0;
L_80137F88:
    if (r3 <= 1) goto L_801380BC;
    if (r31 <= 1) goto L_801380BC;
    fn_800D2248();
    r3 = 0x1;
    r4 = 0x6;
    r5 = 0x7;
    fn_800DA4C4();
    r3 = 0x1;
    r4 = 0x1;
    r5 = 0x0;
    fn_800DA2BC();
    r3 = 0x1;
    r4 = 0x2;
    r5 = 0x1;
    fn_800DA1E8();
    r3 = 0x0;
    fn_800DA028();
    r3 = 0x3;
    fn_800D88DC();
    r3 = 0x4;
    fn_800D888C();
    r3 = *(u32*)((u8*)r27 + 0x10);
    fn_800D7820();
    r4 = *(u32*)((u8*)r27 + 0x14);
    r3 = 0x0;
    fn_800D85D4();
    r3 = 0x4;
    fn_800D6A00();
    fn_800D67BC();
    tmp = *(u16*)((u8*)r27 + 0x22);
    r28 = 0x0;
    r3 = *(u32*)((u8*)r27 + 0x8);
    tmp = tmp - r31;
    r30 = *(u32*)((u8*)r27 + 0x0);
    tmp = tmp << 4;
    r29 = r3 + tmp;
    goto L_801380A4;
L_80138028:
    f1 = *(f32*)((u8*)r30 + 0x0);
    f2 = *(f32*)((u8*)r30 + 0x4);
    f3 = *(f32*)((u8*)r30 + 0x8);
    fn_800D6680();
    r4 = *(u8*)((u8*)r27 + 0x18);
    r3 = 0x0;
    r5 = *(u8*)((u8*)r27 + 0x19);
    r6 = *(u8*)((u8*)r27 + 0x1A);
    r7 = *(u8*)((u8*)r27 + 0x1B);
    fn_800D5CB8();
    f1 = *(f32*)((u8*)r29 + 0x0);
    r3 = 0x0;
    f2 = *(f32*)((u8*)r29 + 0x4);
    fn_800D59B8();
    f1 = *(f32*)((u8*)r30 + 0xC);
    f2 = *(f32*)((u8*)r30 + 0x10);
    f3 = *(f32*)((u8*)r30 + 0x14);
    fn_800D6680();
    r4 = *(u8*)((u8*)r27 + 0x18);
    r3 = 0x0;
    r5 = *(u8*)((u8*)r27 + 0x19);
    r6 = *(u8*)((u8*)r27 + 0x1A);
    r7 = *(u8*)((u8*)r27 + 0x1B);
    fn_800D5CB8();
    f1 = *(f32*)((u8*)r29 + 0x8);
    r3 = 0x0;
    f2 = *(f32*)((u8*)r29 + 0xC);
    fn_800D59B8();
    r30 = *(u32*)((u8*)r30 + 0x1C);
    r29 = r29 + 0x10;
    r28 = r28 + 0x1;
L_801380A4:
    tmp = r28 & 0xFFFF;
    if (tmp < r31) goto L_80138028;
    fn_800D6728();
    r3 = 0x1;
    goto L_801380C0;
L_801380BC:
    r3 = 0x0;
L_801380C0:
    return;
}
