/**
 * @file people_field.c
 * @brief People/NPC field-level behavior -- spawn, movement, animation,
 *        camera integration, and rendering.
 *
 * This file contains the field-level NPC logic that handles NPC spawning
 * from floor data, per-frame movement updates (walking, running, pathfinding),
 * animation playback and blending, camera integration for cutscenes, and
 * model rendering submission.
 *
 * It is the largest module in the 0x80138000-0x80165000 gap, spanning
 * approximately 0x1F000 bytes of code (350+ functions).
 *
 * Address range: 0x80144574 - 0x801652DC (approximately 0x20D68 bytes)
 * Function count: ~340 functions
 *
 * ============================================================
 * MODULE MAP
 * ============================================================
 *
 * 0x80144574 - 0x80146E88: People field spawn system (0x2914 bytes)
 *   fn_80144574 (0x1DE8 bytes) -- peopleFieldSpawnMain
 *     Massive spawn/open function. Processes NPC spawn data records from
 *     floor resources, creates models, sets up animations and motions,
 *     configures collision and interaction parameters.
 *   fn_8014635C (0x68 bytes)   -- peopleFieldSpawnHelper
 *   fn_801463C4 (0xAC4 bytes)  -- peopleFieldSpawnProcess
 *     Secondary spawn processing. Handles additional NPC setup tasks
 *     like walk path configuration and interaction zones.
 *   fn_80146E88 (0x64 bytes)   -- peopleFieldSpawnFinalize
 *   fn_80146EEC (0x27C bytes)  -- peopleFieldSpawnMotion
 *     Configure motion/animation for a newly spawned NPC.
 *
 * 0x80147168 - 0x8014ABE8: People movement & camera (0x3A80 bytes)
 *   fn_80147168 (0x2B0 bytes)  -- peopleFieldMoveToTarget
 *     Move an NPC toward a target position with interpolation.
 *   fn_80147418 (0xE4 bytes)   -- peopleFieldCalcDirection
 *     Calculate facing direction toward target.
 *   fn_801474FC (0x108 bytes)  -- peopleFieldUpdateFacing
 *   fn_80147604 (0xBC bytes)   -- peopleFieldCheckCollision
 *   fn_801476C0 (0x1CC bytes)  -- peopleFieldMoveWalkPath
 *   fn_8014788C (0xB58 bytes)  -- peopleFieldCameraCutscene
 *     Large camera control function for cutscene NPC movement.
 *     Manages camera tracking of NPCs during scripted sequences.
 *   fn_801483E4 (0x218 bytes)  -- peopleFieldCameraHelper
 *   fn_801485FC (0xA94 bytes)  -- peopleFieldCameraMain
 *     Main camera positioning logic for field NPCs.
 *   fn_80149090 (0x174 bytes)  -- peopleFieldCameraBlend
 *   fn_80149204 (0x49C bytes)  -- peopleFieldCameraTarget
 *     Camera target tracking -- follows an NPC as the camera target.
 *   fn_801496A0 (0x9F4 bytes)  -- peopleFieldCameraUpdate
 *     Per-frame camera update when tracking NPCs.
 *   fn_8014A094 (0x1A8 bytes)  -- peopleFieldCameraReset
 *   fn_8014A23C (0x44 bytes)   -- peopleFieldCameraGetState
 *   fn_8014A280 (0x34 bytes)   -- peopleFieldCameraSetState
 *   fn_8014A2B4 (0x78 bytes)   -- peopleFieldCameraInit
 *   fn_8014A32C (0x2A0 bytes)  -- peopleFieldCameraSetup
 *   fn_8014A5CC (0x3C0 bytes)  -- peopleFieldCameraAnimate
 *   fn_8014A98C (0x25C bytes)  -- peopleFieldCameraInterp
 *   fn_8014ABE8 (0x45C bytes)  -- peopleFieldCameraFinalize
 *
 * 0x8014B044 - 0x8014CF30: People animation system (0x1EEC bytes)
 *   fn_8014B044 (0x744 bytes)  -- peopleFieldAnimUpdate
 *     Per-frame animation update. Blends walk/run/idle animations
 *     based on NPC movement state.
 *   fn_8014B788 (0x6FC bytes)  -- peopleFieldAnimBlend
 *     Animation blending between two motion sequences.
 *   fn_8014BE84 (0x150 bytes)  -- peopleFieldAnimSetRate
 *   fn_8014BFD4 (0x64 bytes)   -- peopleFieldAnimGetFrame
 *   fn_8014C038 (0x44 bytes)   -- peopleFieldAnimSetFrame
 *   fn_8014C07C (0x28 bytes)   -- peopleFieldAnimGetLoop
 *   fn_8014C0A4 (0x510 bytes)  -- peopleFieldAnimApply
 *     Apply animation data to model joints.
 *   fn_8014C5B4 (0x34 bytes)   -- peopleFieldAnimHelper1
 *   fn_8014C5E8 (0xC8 bytes)   -- peopleFieldAnimHelper2
 *   fn_8014C6B0 (0xE4 bytes)   -- peopleFieldAnimHelper3
 *   fn_8014C794 (0xE4 bytes)   -- peopleFieldAnimHelper4
 *   fn_8014C878 (0x84 bytes)   -- peopleFieldAnimHelper5
 *   fn_8014C8FC (0x88 bytes)   -- peopleFieldAnimHelper6
 *
 *   fn_8014C984 (0x530 bytes)  -- peopleFieldScriptCommand
 *     Script command handler for NPC control. Processes script bytecodes
 *     (0xFA=walk, 0xFB=stop, 0xFC=run, 0xFD=face, 0xFE=wait, 0xFF=anim).
 *     Called from fn_801621BC (script dispatch). Iterates over all NPC
 *     slots in lbl_80434E64 (gPeopleFieldWork), checking field 0x2D for
 *     command state and field 0x2C for command type.
 *
 *   fn_8014CEB4 (0x54 bytes)   -- peopleFieldScriptHelper
 *   fn_8014CF08 (0x28 bytes)   -- peopleFieldScriptGetState
 *   fn_8014CF30 (0xD0 bytes)   -- peopleFieldScriptSetup
 *
 * 0x8014D000 - 0x8014E7CC: People field init & state (0x17CC bytes)
 *   fn_8014D000 (0x574 bytes)  -- peopleFieldSystemInit
 *     Initialize the entire field people subsystem. Allocates the NPC
 *     work array (struct size 0x404 per slot, count passed as parameter).
 *     Sets up global state in sbss (lbl_8047AF44-lbl_8047AF5C).
 *     Zeroes all slots and initializes per-slot fields at offsets:
 *       0xF4 (set to -1), 0x110, 0x114, 0x118 (flags cleared),
 *       0x121-0x123 (animation bank), 0x150, 0x154, 0x16C, 0x170,
 *       0x174, 0x180, 0x184, 0x190-0x193, 0x1A0, 0x1A4, 0x1B8.
 *     Calls fn_801643D8 to allocate the NPC state buffer.
 *
 *   fn_8014D574 (0x24 bytes)   -- getFieldWorkPtr
 *   fn_8014D598 (0x30 bytes)   -- getFieldWorkSlot
 *   fn_8014D5C8 (0x38 bytes)   -- getFieldWorkCount
 *   fn_8014D600 (0x48 bytes)   -- getFieldWorkEntry
 *   fn_8014D648 (0x38 bytes)   -- getFieldWorkFlags
 *   fn_8014D680 (0x58 bytes)   -- setFieldWorkFlags
 *   fn_8014D6D8 (0x68 bytes)   -- getFieldWorkMotionState
 *   fn_8014D740 (0x1C bytes)   -- getFieldWorkAnimBank
 *   fn_8014D75C (0x60 bytes)   -- setFieldWorkAnimBank
 *   fn_8014D7BC (0x40 bytes)   -- getFieldWorkPosition
 *   fn_8014D7FC (0x84 bytes)   -- setFieldWorkPosition
 *   fn_8014D880 (0x40 bytes)   -- getFieldWorkRotation
 *   fn_8014D8C0 (0x8 bytes)    -- getFieldWorkScale (returns immediately)
 *   fn_8014D8C8 (0x60 bytes)   -- setFieldWorkScale
 *   fn_8014D928 (0x94 bytes)   -- setFieldWorkMotion
 *   fn_8014D9BC (0xEC bytes)   -- setFieldWorkMotionBlend
 *   fn_8014DAA8 (0x158 bytes)  -- setFieldWorkMotionFull
 *   fn_8014DC00 (0xA8 bytes)   -- getFieldWorkMotionFrame
 *   fn_8014DCA8 (0xF0 bytes)   -- setFieldWorkMotionFrame
 *   fn_8014DD98 (0x20 bytes)   -- getFieldWorkState1
 *   fn_8014DDB8 (0x20 bytes)   -- getFieldWorkState2
 *   fn_8014DDD8 (0xBC bytes)   -- setFieldWorkState
 *   fn_8014DE94 (0x8C bytes)   -- clearFieldWorkState
 *   fn_8014DF20 (0x8AC bytes)  -- peopleFieldUpdateMain
 *     Large per-frame update that processes all active NPC slots.
 *   fn_8014E7CC (0x4 bytes)    -- nop / placeholder
 *   fn_8014E7D0 (0x84 bytes)   -- peopleFieldCheckActive
 *   fn_8014E854 (0x160 bytes)  -- peopleFieldActivate
 *
 * 0x8014E9B4 - 0x80152AF8: People rendering system (0x4144 bytes)
 *   fn_8014E9B4 (0x2E4 bytes)  -- peopleFieldRenderSetup
 *     Prepare NPC model for rendering. Called from script/event system
 *     (3 external callers in the 0x801E2xxx range).
 *   fn_8014EC98 (0x34 bytes)   -- peopleFieldRenderHelper1
 *   fn_8014ECCC (0x44 bytes)   -- peopleFieldRenderHelper2
 *   fn_8014ED10 (0x130 bytes)  -- peopleFieldRenderHelper3
 *   fn_8014EE40 (0x458 bytes)  -- peopleFieldRenderMain
 *     Main NPC rendering function. Submits model draw commands.
 *   fn_8014F298 (0x44 bytes)   -- peopleFieldRenderFinalize
 *   fn_8014F2DC (0x55C bytes)  -- peopleFieldRenderModel
 *     Detailed model rendering with material and texture setup.
 *   fn_8014F838 (0x6D4 bytes)  -- peopleFieldRenderComplex
 *     Complex NPC rendering with shadows and reflections.
 *     Called from script event handlers (0x801E34F0, 0x801E3F54).
 *   fn_8014FF0C (0x658 bytes)  -- peopleFieldRenderShadow
 *   fn_80150564 (0x714 bytes)  -- peopleFieldRenderLighting
 *   fn_80150C78 (0x1F0 bytes)  -- peopleFieldRenderMaterial
 *   fn_80150E68 (0x17C bytes)  -- peopleFieldRenderTexture
 *   fn_80150FE4 (0x248 bytes)  -- peopleFieldRenderMatrix
 *   fn_8015122C (0x1C4 bytes)  -- peopleFieldRenderJoints
 *   fn_801513F0 (0x204 bytes)  -- peopleFieldRenderBones
 *   fn_801515F4 (0x17C bytes)  -- peopleFieldRenderSkin
 *   fn_80151770 (0x188 bytes)  -- peopleFieldRenderFace
 *   fn_801518F8 (0xD8 bytes)   -- peopleFieldRenderExpression
 *   fn_801519D0 (0x98 bytes)   -- peopleFieldRenderEyes
 *   fn_80151A68 (0x11C bytes)  -- peopleFieldRenderMouth
 *   fn_80151B84 (0x304 bytes)  -- peopleFieldRenderCloth
 *   fn_80151E88 (0x284 bytes)  -- peopleFieldRenderHair
 *   fn_8015210C - fn_80152434  -- render sub-helpers (8 small funcs)
 *   fn_80152444 (0x9C bytes)   -- peopleFieldRenderSubmit
 *   fn_801524E0 (0xE4 bytes)   -- peopleFieldRenderFlush
 *   fn_801525C4 (0x20 bytes)   -- peopleFieldRenderGetState
 *   fn_801525E4 (0x2F0 bytes)  -- peopleFieldRenderSetState
 *
 * 0x801528D4 - 0x80155078: People pathfinding & walk (0x27A4 bytes)
 *   fn_801528D4 (0xD0 bytes)   -- peopleFieldPathInit
 *   fn_801529A4 (0x154 bytes)  -- peopleFieldPathSetTarget
 *   fn_80152AF8 (0x18C bytes)  -- peopleFieldPathCalcRoute
 *   fn_80152C84 (0xD8 bytes)   -- peopleFieldPathUpdate
 *   fn_80152D5C (0x1FC bytes)  -- peopleFieldPathFollow
 *   fn_80152F58 (0x17C bytes)  -- peopleFieldPathInterp
 *   fn_801530D4 (0x1A0 bytes)  -- peopleFieldPathSmooth
 *   fn_80153274 (0x258 bytes)  -- peopleFieldPathCollide
 *   fn_801534CC (0x12C bytes)  -- peopleFieldPathAvoid
 *   fn_801535F8 (0x27C bytes)  -- peopleFieldPathWalk
 *   fn_80153874 (0x9C bytes)   -- peopleFieldPathRun
 *   fn_80153910 (0x9C bytes)   -- peopleFieldPathStop
 *   fn_801539AC (0x134 bytes)  -- peopleFieldPathTurn
 *   fn_80153AE0 (0x130 bytes)  -- peopleFieldPathFace
 *   fn_80153C10 (0x13C bytes)  -- peopleFieldPathAlign
 *   fn_80153D4C (0x19C bytes)  -- peopleFieldPathEval
 *   fn_80153EE8 - fn_80154F14  -- Walk path node evaluators
 *     13 functions of identical size 0x104 each (fn_80153EE8 through
 *     fn_80154B18), plus 3 larger variants (fn_80154C1C 0x17C,
 *     fn_80154D98 0x17C, fn_80154F14 0x164).
 *     These appear to be walk path node type handlers, one per node type.
 *     The repetitive 0x104-byte size suggests a vtable-like dispatch.
 *
 * 0x80155078 - 0x80158BB4: People interaction & talk (0x3B3C bytes)
 *   fn_80155078 (0x68 bytes)   -- peopleFieldInteractInit
 *   fn_801550E0 (0x220 bytes)  -- peopleFieldInteractCheck
 *   fn_80155300 (0x14C bytes)  -- peopleFieldInteractStart
 *   fn_8015544C (0x244 bytes)  -- peopleFieldInteractProcess
 *   fn_80155690 (0x80 bytes)   -- peopleFieldInteractEnd
 *   fn_80155710 (0xDC bytes)   -- peopleFieldInteractSetup
 *
 *   fn_801557EC (0xF58 bytes)  -- peopleFieldResetState
 *     Very large function (3,928 bytes). Resets an NPC's full state:
 *     clears flag bits in 0x114/0x118, resets animation state at
 *     0x180/0x170/0x184/0x174, copies initial values from 0x208-0x210
 *     into 0x121-0x123/0x120/0x193/0x104/0x150/0x16C. Calls
 *     fn_80161A9C, fn_801610F8, fn_801610BC, fn_801589A8.
 *     This is the NPC "soft reset" for when an NPC returns to idle.
 *
 *   fn_80156744 (0x150 bytes)  -- peopleFieldTalkSetup
 *   fn_80156894 (0xAC bytes)   -- peopleFieldTalkCheck
 *   fn_80156940 (0xF0 bytes)   -- peopleFieldTalkStart
 *   fn_80156A30 (0x108 bytes)  -- peopleFieldTalkProcess
 *   fn_80156B38 (0xA4 bytes)   -- peopleFieldTalkEnd
 *   fn_80156BDC (0x108 bytes)  -- peopleFieldTalkFacing
 *   fn_80156CE4 (0xFC bytes)   -- peopleFieldTalkDistance
 *   fn_80156DE0 (0x438 bytes)  -- peopleFieldTalkMain
 *     Main talk processing -- checks distance, angle, and initiates
 *     the NPC talk sequence.
 *
 * 0x80157218 - 0x801589FC: People model helpers (0x17E4 bytes)
 *   fn_80157218 (0x68 bytes)   -- peopleFieldModelInit
 *   fn_80157280 (0xE0 bytes)   -- peopleFieldModelLoad
 *   fn_80157360 (0x350 bytes)  -- peopleFieldModelSetup
 *   fn_801576B0 (0x14 bytes)   -- peopleFieldModelGetPtr
 *   fn_801576C4 (0x104 bytes)  -- peopleFieldModelApplyTransform
 *   fn_801577C8 (0x50 bytes)   -- peopleFieldModelSetAnim
 *   fn_80157818 (0x24C bytes)  -- peopleFieldModelAnimUpdate
 *   fn_80157A64 (0x460 bytes)  -- peopleFieldModelAnimBlend
 *   fn_80157EC4 (0x1C4 bytes)  -- peopleFieldModelAnimSmooth
 *   fn_80158088 (0x2A0 bytes)  -- peopleFieldModelRender
 *   fn_80158328 (0xFC bytes)   -- peopleFieldModelShadow
 *   fn_80158424 (0x218 bytes)  -- peopleFieldModelLighting
 *   fn_8015863C (0x234 bytes)  -- peopleFieldModelMaterial
 *   fn_80158870 (0xC4 bytes)   -- peopleFieldModelTexture
 *   fn_80158934 (0x74 bytes)   -- peopleFieldModelGetJoint
 *   fn_801589A8 (0x54 bytes)   -- peopleFieldModelResetAnim
 *   fn_801589FC (0x74 bytes)   -- peopleFieldModelGetBone
 *
 * 0x80158A70 - 0x80159C48: People collision & floor (0x11D8 bytes)
 *   fn_80158A70 (0x144 bytes)  -- peopleFieldCollisionInit
 *   fn_80158BB4 (0x3C bytes)   -- peopleFieldCollisionGetPtr
 *   fn_80158BF0 (0xE4 bytes)   -- peopleFieldCollisionUpdate
 *   fn_80158CD4 (0x58 bytes)   -- peopleFieldCollisionTest
 *   fn_80158D2C (0x26C bytes)  -- peopleFieldCollisionSolve
 *   fn_80158F98 (0x28 bytes)   -- peopleFieldFloorGetHeight
 *   fn_80158FC0 (0x150 bytes)  -- peopleFieldFloorSnap
 *   fn_80159110 (0x168 bytes)  -- peopleFieldFloorUpdate
 *   fn_80159278 (0x198 bytes)  -- peopleFieldFloorCollide
 *   fn_80159410 (0x84 bytes)   -- peopleFieldFloorGetNormal
 *   fn_80159494 (0xBC bytes)   -- peopleFieldFloorSetElevation
 *   fn_80159550 (0x248 bytes)  -- peopleFieldFloorCalcSlope
 *   fn_80159798 (0xA8 bytes)   -- peopleFieldFloorGetRegion
 *   fn_80159840 (0x1F8 bytes)  -- peopleFieldFloorRegionCheck
 *   fn_80159A38 (0x210 bytes)  -- peopleFieldFloorBoundary
 *
 * 0x80159C48 - 0x8015D408: People script integration (0x37C0 bytes)
 *   fn_80159C48 (0xC bytes)    -- peopleFieldScriptNop
 *   fn_80159C54 (0x27C bytes)  -- peopleFieldScriptExec
 *   fn_80159ED0 (0x20 bytes)   -- peopleFieldScriptGetVar
 *   fn_80159EF0 (0x32C bytes)  -- peopleFieldScriptSetVar
 *   fn_8015A21C (0x14C bytes)  -- peopleFieldScriptCommand2
 *   fn_8015A368 (0x11C bytes)  -- peopleFieldScriptTrigger
 *   fn_8015A484 (0x3B4 bytes)  -- peopleFieldScriptProcess
 *   fn_8015A838 (0x38 bytes)   -- peopleFieldScriptHelper1
 *   fn_8015A870 (0xE0 bytes)   -- peopleFieldScriptHelper2
 *   fn_8015A950 (0x150 bytes)  -- peopleFieldScriptHelper3
 *   fn_8015AAA0 (0x20 bytes)   -- peopleFieldScriptHelper4
 *   fn_8015AAC0 (0xF4 bytes)   -- peopleFieldScriptHelper5
 *   fn_8015ABB4 (0x48 bytes)   -- peopleFieldScriptHelper6
 *   fn_8015ABFC (0x74 bytes)   -- peopleFieldScriptHelper7
 *   fn_8015AC70 (0xAC bytes)   -- peopleFieldScriptHelper8
 *   fn_8015AD1C (0x2DC bytes)  -- peopleFieldScriptDispatch
 *   fn_8015AFF8 (0x258 bytes)  -- peopleFieldScriptEval
 *   fn_8015B250 (0x21B8 bytes) -- peopleFieldScriptMain
 *     Enormous function (8,632 bytes -- the second-largest in the range).
 *     Main script processing loop for NPC behavior. Processes a wide
 *     range of NPC script commands.
 *
 * 0x8015D408 - 0x8015FFA0: People rendering pipeline (0x2B98 bytes)
 *   fn_8015D408 (0x44 bytes)   -- peopleFieldRenderPipeInit
 *   fn_8015D44C (0xA0 bytes)   -- peopleFieldRenderPipeSetup
 *   fn_8015D4EC (0x60 bytes)   -- peopleFieldRenderPipeHelper
 *   fn_8015D54C (0xA8 bytes)   -- peopleFieldRenderPipeConfig
 *   fn_8015D5F4 (0x84 bytes)   -- peopleFieldRenderPipeGetState
 *   fn_8015D678 (0x158 bytes)  -- peopleFieldRenderPipeApply
 *   fn_8015D7D0 (0x6F0 bytes)  -- peopleFieldRenderPipeCalc
 *     Path interpolation calculation for NPC movement rendering.
 *   fn_8015DEC0 (0x4B4 bytes)  -- peopleFieldRenderPipeProcess
 *   fn_8015E374 (0x51C bytes)  -- peopleFieldRenderPipeSubmit
 *   fn_8015E890 (0x20 bytes)   -- peopleFieldRenderPipeFlush
 *   fn_8015E8B0 (0x3F8 bytes)  -- peopleFieldRenderPipeAnimate
 *   fn_8015ECA8 (0x58 bytes)   -- peopleFieldRenderPipeHelper2
 *   fn_8015ED00 (0x204 bytes)  -- peopleFieldRenderPipeHelper3
 *   fn_8015EF04 (0x220 bytes)  -- peopleFieldRenderPipeHelper4
 *   fn_8015F124 (0x14C bytes)  -- peopleFieldRenderPipeHelper5
 *   fn_8015F270 (0x3B0 bytes)  -- peopleFieldRenderPipeHelper6
 *   fn_8015F620 (0x82C bytes)  -- peopleFieldRenderPipeMain
 *     Large rendering pipeline function.
 *   fn_8015FE4C (0x38 bytes)   -- peopleFieldRenderPipeEnd
 *   fn_8015FE84 (0x4 bytes)    -- nop
 *   fn_8015FE88 (0x118 bytes)  -- peopleFieldRenderPipeFinal
 *   fn_8015FFA0 (0x34 bytes)   -- peopleFieldRenderPipeQuery
 *
 * 0x8015FFD4 - 0x80161934: People motion system (0x1960 bytes)
 *   fn_8015FFD4 (0x8 bytes)    -- peopleFieldMotionNop
 *   fn_8015FFDC (0xAC bytes)   -- peopleFieldMotionInit
 *   fn_80160088 (0xB4 bytes)   -- peopleFieldMotionSetup
 *   fn_8016013C (0x64 bytes)   -- peopleFieldMotionGetType
 *   fn_801601A0 (0x1FC bytes)  -- peopleFieldMotionApply
 *   fn_8016039C (0x24 bytes)   -- peopleFieldMotionGetFrame
 *   fn_801603C0 (0x608 bytes)  -- peopleFieldMotionUpdate
 *     Large motion update: processes walk/run/idle state transitions.
 *   fn_801609C8 (0x124 bytes)  -- peopleFieldMotionBlend
 *   fn_80160AEC (0xF0 bytes)   -- peopleFieldMotionSmooth
 *   fn_80160BDC (0x2C4 bytes)  -- peopleFieldMotionInterp
 *   fn_80160EA0 (0x34 bytes)   -- peopleFieldMotionHelper1
 *   fn_80160ED4 (0x3C bytes)   -- peopleFieldMotionHelper2
 *   fn_80160F10 (0x98 bytes)   -- peopleFieldMotionHelper3
 *   fn_80160FA8 (0x114 bytes)  -- peopleFieldMotionHelper4
 *   fn_801610BC (0x3C bytes)   -- peopleFieldMotionLookup
 *   fn_801610F8 (0x3C bytes)   -- peopleFieldMotionLookup2
 *   fn_80161134 (0x4A0 bytes)  -- peopleFieldMotionMain
 *     Large motion processing function.
 *
 *   fn_801615D4 - fn_801618EC  -- Motion type handlers (12 functions)
 *     12 functions of identical size 0x48 each. These are motion type
 *     dispatch handlers -- one per motion type (walk, run, idle, etc.).
 *
 *   fn_80161934 (0xB4 bytes)   -- peopleFieldMotionEval
 *   fn_801619E8 (0xB4 bytes)   -- peopleFieldMotionEval2
 *
 * 0x80161A9C - 0x80162214: People utility functions (0x778 bytes)
 *   fn_80161A9C (0x284 bytes)  -- peopleFieldUtilInit
 *     Utility initialization for NPC field data.
 *   fn_80161D20 (0x70 bytes)   -- peopleFieldUtilHelper1
 *   fn_80161D90 (0xFC bytes)   -- peopleFieldUtilHelper2
 *   fn_80161E8C (0x1E4 bytes)  -- peopleFieldUtilProcess
 *   fn_80162070 (0x1C bytes)   -- peopleFieldUtilGetPtr
 *   fn_8016208C (0x8C bytes)   -- peopleFieldUtilSetup
 *   fn_80162118 (0xA4 bytes)   -- peopleFieldUtilConfig
 *   fn_801621BC (0x10 bytes)   -- peopleFieldUtilDispatch
 *     Script dispatch entry point. Called from fn_8014C984.
 *   fn_801621CC (0x48 bytes)   -- peopleFieldUtilHelper3
 *   fn_80162214 (0x8 bytes)    -- peopleFieldUtilNop
 *
 * 0x8016221C - 0x801652DC: People state & memory management (0x30C0 bytes)
 *   fn_8016221C (0x154 bytes)  -- peopleFieldStateInit
 *   fn_80162370 (0xB8 bytes)   -- peopleFieldStateSetup
 *   fn_80162428 (0x34 bytes)   -- peopleFieldStateGet
 *   fn_8016245C (0x8 bytes)    -- peopleFieldStateNop1
 *   fn_80162464 (0x8 bytes)    -- peopleFieldStateNop2
 *   fn_8016246C (0x20 bytes)   -- peopleFieldStateHelper1
 *   fn_8016248C (0x8 bytes)    -- peopleFieldStateNop3
 *   fn_80162494 (0x14 bytes)   -- peopleFieldStateHelper2
 *   fn_801624A8 (0x1B4 bytes)  -- peopleFieldStateProcess
 *   fn_8016265C (0x50 bytes)   -- peopleFieldStateSwitch
 *   fn_801626AC (0x1AC bytes)  -- peopleFieldStateMain
 *   fn_80162858 (0x20 bytes)   -- peopleFieldMemGetCount
 *   fn_80162878 (0x14 bytes)   -- peopleFieldMemGetSize
 *   fn_8016288C (0x14 bytes)   -- peopleFieldMemGetBase
 *   fn_801628A0 (0x14 bytes)   -- peopleFieldMemGetEnd
 *   fn_801628B4 (0x14 bytes)   -- peopleFieldMemGetFree
 *   fn_801628C8 (0x3C bytes)   -- peopleFieldMemAlloc
 *   fn_80162904 (0x28 bytes)   -- peopleFieldMemFree
 *   fn_8016292C (0x78 bytes)   -- peopleFieldMemInit
 *   fn_801629A4 (0x2C bytes)   -- peopleFieldMemHelper1
 *   fn_801629D0 (0x2C bytes)   -- peopleFieldMemHelper2
 *   fn_801629FC (0x5C bytes)   -- peopleFieldMemSetup
 *   fn_80162A58 (0x2C0 bytes)  -- peopleFieldMoveApply
 *     Applies NPC movement with interpolation. Uses struct size 0xF4,
 *     accesses fields at 0xE5, 0xEF, 0xF0, 0x4C-0x50. Calls into
 *     fn_8015D7D0 for path calculation.
 *   fn_80162D18 (0x2C bytes)   -- peopleFieldMoveHelper1
 *   fn_80162D44 (0x28 bytes)   -- peopleFieldMoveHelper2
 *   fn_80162D6C (0x20 bytes)   -- peopleFieldMoveHelper3
 *   fn_80162D8C (0x20 bytes)   -- peopleFieldMoveHelper4
 *   fn_80162DAC (0x34 bytes)   -- peopleFieldMoveHelper5
 *   fn_80162DE0 (0x34 bytes)   -- peopleFieldMoveHelper6
 *   fn_80162E14 (0xA4 bytes)   -- peopleFieldMoveCalc
 *   fn_80162EB8 (0x90 bytes)   -- peopleFieldMoveInterp
 *   fn_80162F48 (0x20 bytes)   -- peopleFieldMoveGet1
 *   fn_80162F68 (0x20 bytes)   -- peopleFieldMoveGet2
 *   fn_80162F88 (0x24 bytes)   -- peopleFieldMoveGet3
 *   fn_80162FAC (0x4 bytes)    -- nop
 *   fn_80162FB0 (0x5C bytes)   -- peopleFieldMoveSet1
 *   fn_8016300C (0x24 bytes)   -- peopleFieldMoveSet2
 *   fn_80163030 (0x20 bytes)   -- peopleFieldMoveSet3
 *   fn_80163050 (0x94 bytes)   -- peopleFieldMoveProcess
 *   fn_801630E4 (0x20 bytes)   -- peopleFieldMoveGet4
 *   fn_80163104 (0x84 bytes)   -- peopleFieldMoveSet4
 *   fn_80163188 (0x20 bytes)   -- peopleFieldMoveGet5
 *   fn_801631A8 (0x4 bytes)    -- nop
 *   fn_801631AC (0x14 bytes)   -- peopleFieldMoveGet6
 *   fn_801631C0 (0xC bytes)    -- peopleFieldMoveGet7
 *   fn_801631CC (0x28 bytes)   -- peopleFieldMoveSet7
 *   fn_801631F4 (0x20 bytes)   -- peopleFieldMoveGet8
 *   fn_80163214 (0xA0 bytes)   -- peopleFieldMoveEval
 *   fn_801632B4 (0x1DC bytes)  -- peopleFieldMoveMain
 *   fn_80163490 (0x18 bytes)   -- peopleFieldMoveQuery
 *   fn_801634A8 (0x2EC bytes)  -- peopleFieldMoveFinal
 *   fn_80163794 (0x4 bytes)    -- nop
 *   fn_80163798 (0x20 bytes)   -- peopleFieldMoveReset
 *   fn_801637B8 (0x58 bytes)   -- peopleFieldMoveSetTarget
 *   fn_80163810 (0x3BC bytes)  -- peopleFieldMoveUpdate
 *   fn_80163BCC (0x18 bytes)   -- peopleFieldMoveGetState
 *   fn_80163BE4 (0xC4 bytes)   -- peopleFieldMoveSetState
 *   fn_80163CA8 (0x108 bytes)  -- peopleFieldMoveApplyForce
 *   fn_80163DB0 (0x38 bytes)   -- peopleFieldMoveGetForce
 *   fn_80163DE8 (0xF8 bytes)   -- peopleFieldMoveCalcForce
 *   fn_80163EE0 (0xA8 bytes)   -- peopleFieldMoveCalcFriction
 *   fn_80163F88 (0x10 bytes)   -- peopleFieldMoveGetFriction
 *   fn_80163F98 (0x64 bytes)   -- peopleFieldMoveSetFriction
 *   fn_80163FFC (0xC8 bytes)   -- peopleFieldMoveApplyGravity
 *   fn_801640C4 (0x20 bytes)   -- peopleFieldMoveGetGravity
 *   fn_801640E4 (0x34 bytes)   -- peopleFieldMoveSetGravity
 *   fn_80164118 (0x30 bytes)   -- peopleFieldMoveGetVelocity
 *   fn_80164148 (0xBC bytes)   -- peopleFieldMoveSetVelocity
 *   fn_80164204 (0x34 bytes)   -- peopleFieldMoveGetAccel
 *   fn_80164238 (0x74 bytes)   -- peopleFieldMoveSetAccel
 *   fn_801642AC (0x4C bytes)   -- peopleFieldMoveHelper7
 *   fn_801642F8 (0x2C bytes)   -- peopleFieldMoveHelper8
 *   fn_80164324 (0x4 bytes)    -- nop
 *   fn_80164328 (0x38 bytes)   -- peopleFieldMoveHelper9
 *   fn_80164360 (0x38 bytes)   -- peopleFieldMoveHelper10
 *   fn_80164398 (0x20 bytes)   -- peopleFieldMoveHelper11
 *   fn_801643B8 (0x20 bytes)   -- peopleFieldMoveHelper12
 *   fn_801643D8 (0x28 bytes)   -- peopleFieldMemAllocBuf
 *     Allocates the NPC state buffer. Called from fn_8014D000.
 *   fn_80164400 (0x2C bytes)   -- peopleFieldMemFreeBuf
 *   fn_8016442C (0x5C bytes)   -- peopleFieldMemResize
 *   fn_80164488 (0x58 bytes)   -- peopleFieldMemQuery
 *   fn_801644E0 (0x40 bytes)   -- peopleFieldMemValidate
 *   fn_80164520 (0x50C bytes)  -- peopleFieldAnimMain
 *     NPC animation main processing for field rendering.
 *   fn_80164A2C (0x214 bytes)  -- peopleFieldAnimProcess
 *   fn_80164C40 (0x190 bytes)  -- peopleFieldAnimBlendFinal
 *   fn_80164DD0 (0x50C bytes)  -- peopleFieldAnimInterp
 *     Animation interpolation for smooth NPC movement. Accesses struct
 *     fields up to offset 0x1A4. This is the last function in the range.
 *
 * ============================================================
 * FIELD PEOPLE WORK STRUCT (0x404 bytes per slot)
 * ============================================================
 *
 * Recovered from fn_8014D000 (init loop) and fn_801557EC (reset):
 *
 *   0x000-0x0F3: Core NPC state data
 *   0x0F4: s32   entityID (-1 = unassigned)
 *   0x100: u32   reserved
 *   0x104: u8    resetFlag
 *   0x10C: u32   field_10C
 *   0x110: u32   field_110
 *   0x114: u32   flagsLo (64-bit flags, low word)
 *   0x118: u32   flagsHi (64-bit flags, high word)
 *   0x120: u8    defaultAnimBank
 *   0x121: u8    animBankA
 *   0x122: u8    animBankB
 *   0x123: u8    animBankC
 *   0x12F: u8    prevAnimBank
 *   0x130: u8    currentAnimBank
 *   0x150: u16   motionStateA
 *   0x154: u32   motionConfigA
 *   0x158: u32   motionConfigB
 *   0x16C: u16   motionStateB
 *   0x170: u32   blendTargetA
 *   0x174: u32   blendTargetB
 *   0x180: u32   blendSourceA
 *   0x184: u32   blendSourceB
 *   0x190: u8    motionTypeA
 *   0x191: u8    motionTypeB
 *   0x192: u8    motionBlendFlag
 *   0x193: u8    motionSpeed
 *   0x1A0: f32   interpFactorA
 *   0x1A4: u32   interpPointCount
 *   0x1B8: u32   field_1B8
 *   0x208: u8    initAnimBankA
 *   0x209: u8    initAnimSlot
 *   0x20A: u8    initAnimBankB
 *   0x20B: u8    initAnimBankC
 *   0x20C: u8    initAnimBankD
 *   0x20D: u8    initDefaultAnim
 *   0x20E: u8    initMotionType
 *   0x210: u8    initMotionSpeed
 *
 * Global state (sbss):
 *   lbl_8047AF44 -- u32 (cleared by init)
 *   lbl_8047AF48 -- void* gPeopleFieldWorkArray (allocated)
 *   lbl_8047AF4C -- u32 (cleared by init)
 *   lbl_8047AF58 -- u32 (cleared by init)
 *   lbl_8047AF5C -- u32 (cleared by init)
 *   lbl_8047B024 -- void* (used by fn_80162A58)
 */

#include "dolphin/types.h"
#include "game/people/people.h"

/* ===== External SDK / engine functions ===== */
extern void  fn_800DD970(const char* fmt, ...);
extern void* memset(void* dst, int val, u32 size);
extern void* memcpy(void* dst, const void* src, u32 size);
extern void  DCFlushRange(void* ptr, u32 size);
extern u32   OSDisableInterrupts(void);
extern void  OSRestoreInterrupts(u32 level);

/* GSmem allocator */
extern u16   fn_800E3534(u32 size);
extern void* fn_800E27B0(u16 handle);

/* External functions referenced from asm wrappers */
extern u32 fn_80164488(u8* ptr);

/* Model system */
extern void  fn_800EE150(void* model, u32 param);
extern void  fn_800EE828(void* model, u32 param);
extern void  fn_800E24B0(void* model, u32 param);
extern void  fn_800E209C(void* model, u32 param);
extern void  fn_800E01F4(void* dst, void* src);
extern void  fn_800E01D0(void* dst, void* src);
extern void  fn_800E019C(void* model, void* param);
extern void  fn_800E0BA0(void* param);
extern void  fn_800E0BE4(void* param);
extern void  fn_800E013C(void* param);
extern void  fn_800C46B0(void* param1, void* param2);

/* Floor/field system */
extern void* fn_800F9318(u16 group, u16 model, u16 param);

/* GX rendering */
extern void  fn_800E4014(void* param);

/* People data layer (people_data.c) */
extern void* fn_801440A0(u16 index);   /* peopleFieldGetByIndex */
extern void* fn_80142CF4(u32 a, u32 b, u32 c, u32 d);  /* peopleFieldAlloc */
extern void  fn_801429E8(void* entry);  /* peopleFieldGetEntry */
extern void  fn_80142984(u32 id);       /* peopleFieldGetByID */

/* Script system */
extern void fn_801621BC(u32* ptr);  /* peopleFieldUtilDispatch - same-TU asm wrapper */

/* ===================================================================
 * All functions in this module are listed above in the MODULE MAP.
 * The asm files in build/GC6E01/asm/ remain the authoritative
 * implementation until each function is individually decompiled.
 * =================================================================== */

extern void fn_8015210C(void);
extern void fn_80162118(void);
extern u8 lbl_8043D6F8[];
extern u32 lbl_8047AF98;
extern u8 lbl_8047AF90[];
extern u8 lbl_8043DEF8[];
extern u32 lbl_8047AF9C;
extern u32 lbl_8047AF8C;
#if 1
asm void fn_8015211C(void) {
#include "src/game/people/people_field_fn_8015211C.inc"
}
#else
void fn_8015211C(void) {}
#endif
#if 0
asm void fn_801521A8(void) {
#include "src/game/people/people_field_fn_801521A8.inc"
}
#else
s32 fn_801521A8(u16* a, u16* b) {
    return (s32)(a[0]) - (s32)(b[0]);
}
#endif
extern void _savegpr_20(void);
extern void _restgpr_20(void);
extern void _savegpr_23(void);
extern void _restgpr_23(void);
extern void _savegpr_24(void);
extern void _restgpr_24(void);
extern void _savegpr_25(void);
extern void _restgpr_25(void);
extern void _savegpr_27(void);
extern void _restgpr_27(void);
extern u8 lbl_80445EF8[];
extern u8 lbl_8043CCF8[];
extern u32 lbl_8047AF88;
extern u32 lbl_8047AF84;
extern u16 lbl_8047AFAA;
#if 1
asm void fn_801521B8(void) {
#include "src/game/people/people_field_fn_801521B8.inc"
}
#else
void fn_801521B8(void) {}
#endif
#if 0
asm void fn_801522E0(void) {
#include "src/game/people/people_field_fn_801522E0.inc"
}
#else
s32 fn_801522E0(u16* a, u16* b) {
    return (s32)(a[2]) - (s32)(b[2]);
}
#endif
extern u8 lbl_80438CF8[];
extern u8 lbl_8047AF7C[8];
extern u16 lbl_8047AFA8;
extern u32 lbl_8047AF78;
#if 0
asm void fn_801522F0(void) {
#include "src/game/people/people_field_fn_801522F0.inc"
}
#else
u32 fn_801522F0(u16 arg) {
    extern void* fn_80162118(u8* a, u8* b, u16 c, u32 d, void* e);
    void* result;
    *(u16*)(lbl_8047AF7C + 4) = arg;
    result = fn_80162118(lbl_8047AF7C, lbl_80438CF8, lbl_8047AFA8, 8, fn_801522E0);
    lbl_8047AF78 = (u32)result;
    if (result != NULL) { return *(u32*)result; }
    return 0;
}
#endif
extern u8 lbl_804378F8[];
extern u8 lbl_8047AF70[8];
extern u16 lbl_8047AFA6;
extern u32 lbl_8047AF6C;
#if 0
asm void fn_8015234C(void) {
#include "src/game/people/people_field_fn_8015234C.inc"
}
#else
u32 fn_8015234C(u16 arg) {
    extern void* fn_80162118(u8* a, u8* b, u16 c, u32 d, void* e);
    void* result;
    *(u16*)(lbl_8047AF70 + 4) = arg;
    result = fn_80162118(lbl_8047AF70, lbl_804378F8, lbl_8047AFA6, 8, fn_801522E0);
    lbl_8047AF6C = (u32)result;
    if (result != NULL) { return *(u32*)result; }
    return 0;
}
#endif
#if 0
asm void fn_801523A8(void) {
#include "src/game/people/people_field_fn_801523A8.inc"
}
#else
s32 fn_801523A8(u16* a, u16* b) {
    return (s32)(a[2]) - (s32)(b[2]);
}
#endif
extern u8 lbl_80445F18[];
extern u8 lbl_804380F8[];
extern u16 lbl_8047AFA4;
extern u32 lbl_8047AF68;
#if 0
asm void fn_801523B8(void) {
#include "src/game/people/people_field_fn_801523B8.inc"
}
#else
u32 fn_801523B8(u16 arg, u16* out) {
    extern void* fn_80162118(u8* a, u8* b, u16 c, u32 d, void* e);
    void* result;
    *(u16*)(lbl_80445F18 + 4) = arg;
    result = fn_80162118(lbl_80445F18, lbl_804380F8, lbl_8047AFA4, 0xc, fn_801523A8);
    lbl_8047AF68 = (u32)result;
    if (result != NULL) {
        *out = *(u16*)((u8*)result + 6);
        return *(u32*)(u32)lbl_8047AF68;
    }
    return 0;
}
#endif

extern void fn_80160F10(void);
#if 1
asm void fn_80153FEC(void) {
#include "src/game/people/people_field_fn_80153FEC.inc"
}
#else
void fn_80153FEC(void) {}
#endif
#if 1
asm void fn_801540F0(void) {
#include "src/game/people/people_field_fn_801540F0.inc"
}
#else
void fn_801540F0(void) {}
#endif
#if 1
asm void fn_801541F4(void) {
#include "src/game/people/people_field_fn_801541F4.inc"
}
#else
void fn_801541F4(void) {}
#endif
#if 1
asm void fn_801542F8(void) {
#include "src/game/people/people_field_fn_801542F8.inc"
}
#else
void fn_801542F8(void) {}
#endif
#if 1
asm void fn_801543FC(void) {
#include "src/game/people/people_field_fn_801543FC.inc"
}
#else
void fn_801543FC(void) {}
#endif
#if 1
asm void fn_80154500(void) {
#include "src/game/people/people_field_fn_80154500.inc"
}
#else
void fn_80154500(void) {}
#endif
#if 1
asm void fn_80154604(void) {
#include "src/game/people/people_field_fn_80154604.inc"
}
#else
void fn_80154604(void) {}
#endif
#if 1
asm void fn_80154708(void) {
#include "src/game/people/people_field_fn_80154708.inc"
}
#else
void fn_80154708(void) {}
#endif
#if 1
asm void fn_8015480C(void) {
#include "src/game/people/people_field_fn_8015480C.inc"
}
#else
void fn_8015480C(void) {}
#endif
#if 1
asm void fn_80154910(void) {
#include "src/game/people/people_field_fn_80154910.inc"
}
#else
void fn_80154910(void) {}
#endif
#if 1
asm void fn_80154A14(void) {
#include "src/game/people/people_field_fn_80154A14.inc"
}
#else
void fn_80154A14(void) {}
#endif

extern u32 fn_80161134(u8* obj, u8* motionBase, u32 p1, u32 p2);
#if 0
asm void fn_8016161C(void) {
#include "src/game/people/people_field_fn_8016161C.inc"
}
#else
/* If flags bit 30 (0x2) is CLEAR: return halfword at 0x25c.
 * If SET: clear bit 30, call fn_80161134 with motion data, return its result. */
u32 fn_8016161C(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x2)) {
        return *(u16*)(obj + 0x25c);
    }
    *(u32*)(obj + 0x214) = flags & ~0x2u;
    return fn_80161134(obj, obj + 0x23c, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void fn_80161664(void) {
#include "src/game/people/people_field_fn_80161664.inc"
}
#else
/* bit 29 (0x4), offset 0x280, motion at 0x260 */
u32 fn_80161664(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x4)) {
        return *(u16*)(obj + 0x280);
    }
    *(u32*)(obj + 0x214) = flags & ~0x4u;
    return fn_80161134(obj, obj + 0x260, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void fn_801616AC(void) {
#include "src/game/people/people_field_fn_801616AC.inc"
}
#else
/* bit 28 (0x8), offset 0x2a4, motion at 0x284 */
u32 fn_801616AC(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x8)) {
        return *(u16*)(obj + 0x2a4);
    }
    *(u32*)(obj + 0x214) = flags & ~0x8u;
    return fn_80161134(obj, obj + 0x284, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void fn_801616F4(void) {
#include "src/game/people/people_field_fn_801616F4.inc"
}
#else
/* bit 27 (0x10), offset 0x2c8, motion at 0x2a8 */
u32 fn_801616F4(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x10)) {
        return *(u16*)(obj + 0x2c8);
    }
    *(u32*)(obj + 0x214) = flags & ~0x10u;
    return fn_80161134(obj, obj + 0x2a8, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void fn_8016173C(void) {
#include "src/game/people/people_field_fn_8016173C.inc"
}
#else
/* bit 26 (0x20), offset 0x2ec, motion at 0x2cc */
u32 fn_8016173C(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x20)) {
        return *(u16*)(obj + 0x2ec);
    }
    *(u32*)(obj + 0x214) = flags & ~0x20u;
    return fn_80161134(obj, obj + 0x2cc, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void fn_80161784(void) {
#include "src/game/people/people_field_fn_80161784.inc"
}
#else
/* bit 25 (0x40), offset 0x310, motion at 0x2f0 */
u32 fn_80161784(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x40)) {
        return *(u16*)(obj + 0x310);
    }
    *(u32*)(obj + 0x214) = flags & ~0x40u;
    return fn_80161134(obj, obj + 0x2f0, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void fn_801617CC(void) {
#include "src/game/people/people_field_fn_801617CC.inc"
}
#else
/* bit 23 (0x100), offset 0x358, motion at 0x338 */
u32 fn_801617CC(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x100)) {
        return *(u16*)(obj + 0x358);
    }
    *(u32*)(obj + 0x214) = flags & ~0x100u;
    return fn_80161134(obj, obj + 0x338, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void fn_80161814(void) {
#include "src/game/people/people_field_fn_80161814.inc"
}
#else
/* bit 22 (0x200), offset 0x37c, motion at 0x35c */
u32 fn_80161814(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x200)) {
        return *(u16*)(obj + 0x37c);
    }
    *(u32*)(obj + 0x214) = flags & ~0x200u;
    return fn_80161134(obj, obj + 0x35c, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void fn_8016185C(void) {
#include "src/game/people/people_field_fn_8016185C.inc"
}
#else
/* bit 21 (0x400), offset 0x3a0, motion at 0x380 */
u32 fn_8016185C(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x400)) {
        return *(u16*)(obj + 0x3a0);
    }
    *(u32*)(obj + 0x214) = flags & ~0x400u;
    return fn_80161134(obj, obj + 0x380, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void fn_801618A4(void) {
#include "src/game/people/people_field_fn_801618A4.inc"
}
#else
/* bit 20 (0x800), offset 0x3c4, motion at 0x3a4 */
u32 fn_801618A4(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x800)) {
        return *(u16*)(obj + 0x3c4);
    }
    *(u32*)(obj + 0x214) = flags & ~0x800u;
    return fn_80161134(obj, obj + 0x3a4, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#if 0
asm void fn_801618EC(void) {
#include "src/game/people/people_field_fn_801618EC.inc"
}
#else
/* bit 19 (0x1000), offset 0x3e8, motion at 0x3c8 */
u32 fn_801618EC(u8* obj) {
    u32 flags;
    flags = *(u32*)(obj + 0x214);
    if (!(flags & 0x1000)) {
        return *(u16*)(obj + 0x3e8);
    }
    *(u32*)(obj + 0x214) = flags & ~0x1000u;
    return fn_80161134(obj, obj + 0x3c8, *(u8*)(obj + 0x121), *(u8*)(obj + 0x122));
}
#endif
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801619E8(void) {
#include "src/game/people/people_field_fn_801619E8.inc"
}
#else
extern u32 lbl_80449390[];
extern u32 lbl_80369CA0[];
extern u8  lbl_804356F4[];
u32 fn_801619E8(u8 idx, u8 r4, u32 r5, u32 r6) {
    u32* row = lbl_80449390 + (u8)r6 * 16;
    u32 t2 = lbl_80369CA0[r4];
    u32 t1 = row[(u8)r5];
    u32 mask = t2 & t1;
    u32 nonzero = ((-mask | mask) >> 31);
    if (nonzero) {
        row[(u8)r5] = t1 & ~t2;
    }
    if (nonzero) {
        return fn_80161134(NULL, lbl_804356F4 + (u32)idx * 0x90 + (u32)r4 * 0x24, r5, r6);
    } else {
        return *(u16*)(lbl_804356F4 + (u32)idx * 0x90 + (u32)r4 * 0x24 + 0x20);
    }
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80161D20(void) {
#include "src/game/people/people_field_fn_80161D20.inc"
}
#else
void fn_80161D20(void) { /* TODO */ }
#endif
#pragma pop
extern void fn_80160BDC(void);
extern void fn_801603C0(void);
extern u32  fn_8015A484(u32 a, u32 b, u32 c);
extern void fn_8015A870(void);
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80161D90(void) {
#include "src/game/people/people_field_fn_80161D90.inc"
}
#else
void fn_80161D90(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80161E8C(void) {
#include "src/game/people/people_field_fn_80161E8C.inc"
}
#else
void fn_80161E8C(void) { /* TODO */ }
#endif
#pragma pop
extern u32 lbl_80478BF0;
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80162070(void) {
#include "src/game/people/people_field_fn_80162070.inc"
}
#else
u32 fn_80162070(void) {
    u32 temp;

    temp = lbl_80478BF0 * 0xA8351D63;
    lbl_80478BF0 = temp;
    return (temp >> 10) & 0xFFFF;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_8016208C(void) {
#include "src/game/people/people_field_fn_8016208C.inc"
}
#else
void fn_8016208C(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80162118(void) {
#include "src/game/people/people_field_fn_80162118.inc"
}
#else
void fn_80162118(void) { /* TODO */ }
#endif
#pragma pop
extern u32  OSEnableInterrupts(void);
extern u32  OSGetTick(void);
extern void fn_80098034(void);
extern void fn_800AC0F8(void);
extern void fn_800AC110(void);
extern void fn_800AE794(void);
extern void fn_800AE7CC(void);
extern void fn_800AE7E0(void);
extern void fn_800AE8A4(void);
extern void fn_800AE8EC(void);
extern u32  fn_800AE92C(void);
extern void fn_800AE93C(u8* ptr);
extern void fn_800CE358(void);
extern void fn_8015B250(void);
extern void fn_80164520(void);
extern void fn_80164A2C(void);
extern void fn_801652DC(u32 a, u32 b, u32 c, u8* d);
extern void fn_8009B300(void);
extern void fn_800AC02C(u32 a);
extern void fn_800AC070(u8* ptr, u32 size);
extern void fn_800ACB44(void);
extern void fn_800ACB4C(void);
extern void fn_800AE630(void);
extern u32  fn_800AE78C(void);
extern void fn_8014A280(void);
extern void fn_80158CD4(void);
extern void fn_8015A950(void);
extern void fn_8015AAA0(void);
extern void fn_8015D44C(u8* ptr);
extern void fn_8015D4EC(void);
extern void fn_8015D54C(u8* ptr);
extern void fn_8015D5F4(u8* ptr);
extern u8 lbl_80447E60[];
extern void fn_8015D7D0(void);
extern void fn_801629A4(void);
extern void fn_801629D0(void);
extern void fn_801629FC(void);
extern void fn_801632B4(void);
extern void fn_80163490(void);
extern void fn_801634A8(void);
extern void fn_80163794(void);
extern void fn_801637B8(u8* ptr, u32 size);
extern void fn_80163810(void);
extern void fn_80163BCC(u8* a, u32 b);
extern void fn_80163BE4(void);
extern void fn_80163CA8(void);
extern void fn_80163DB0(void);
extern void fn_80163DE8(void);
extern u32  fn_80163FFC(u32(*fnptr)(void), u32 d, u32 a);
extern void fn_801640C4(void);
extern u32  fn_801640E4(void);
extern u32  fn_80164148(u32 d);
extern u32  fn_80164204(void);
extern void fn_80164324(void);
extern void fn_80164328(void);
extern void fn_80164360(void);
extern void fn_801642F8(void);
extern u32 fn_801642AC(void);
extern void fn_801643D8(void);
extern void fn_80164400(void);
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801621BC(void) {
#include "src/game/people/people_field_fn_801621BC.inc"
}
#else
void fn_801621BC(u32* ptr) { *ptr <<= 8; }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801621CC(void) {
#include "src/game/people/people_field_fn_801621CC.inc"
}
#else
void fn_801621CC(u32* ptr, u32 divisor) {
    extern u32 fn_8014A280(u32 a);
    u32 result = fn_8014A280(divisor);
    *ptr = ((*ptr << 16) / result * 0x3E8) >> 5;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162370(void) {
#include "src/game/people/people_field_fn_80162370.inc"
}
#else
extern u8  lbl_8047B05E;
extern u8  lbl_8047B05F;
extern u32 fn_8016221C(void);
u32 fn_80162370(u32 a, u8 b, u8 c, u32 d) {
    extern u32 lbl_8047B028;
    fn_801642F8();
    lbl_8047B05F = 0;
    lbl_8047B05E = 0;
    lbl_8047B028 = 0;
    if (fn_80163FFC(fn_8016221C, d, a) != 0
     && fn_8015A484(b, c, d & 1) != 0
     && fn_80164148(d) != 0) {
        fn_80164328();
        fn_801640C4();
        return 0;
    }
    return -1;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80162428(void) {
#include "src/game/people/people_field_fn_80162428.inc"
}
#else
void fn_80162428(void) {
    fn_80164360();
    fn_80164204();
    fn_8015A870();
    fn_801640E4();
    fn_80164328();
    fn_80164324();
}
#endif
extern u8  lbl_8047B050;
extern u32 lbl_8047B028;
extern u32 lbl_8047B024;
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8016245C(void) {
#include "src/game/people/people_field_fn_8016245C.inc"
}
#else
void fn_8016245C(u8 val) { lbl_8047B050 = val; }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80162464(void) {
#include "src/game/people/people_field_fn_80162464.inc"
}
#else
u8 fn_80162464(void) { return lbl_8047B050; }
#endif
#pragma pop
extern u32 lbl_8047B024;
u32 fn_8016246C(u32 index) {
    u8* elem = (u8*)lbl_8047B024 + index * 0xF4;
    return *(u8*)(elem + 0xEC) != 0;
}
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_8016248C(void) {
#include "src/game/people/people_field_fn_8016248C.inc"
}
#else
void fn_8016248C(u32 val) { lbl_8047B028 = val; }
#endif
#pragma pop
#pragma push
#pragma optimization_level 2
#if 0
asm void fn_80162494(void) {
#include "src/game/people/people_field_fn_80162494.inc"
}
#else
void fn_80162494(u32 index, u32 val) {
    extern u32 lbl_8047B024;
    u8* elem = (u8*)lbl_8047B024 + index * 0xF4;
    *(u32*)(elem + 0x1C) = val;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_801624A8(void) {
#include "src/game/people/people_field_fn_801624A8.inc"
}
#else
void fn_801624A8(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_8016265C(void) {
#include "src/game/people/people_field_fn_8016265C.inc"
}
#else
void fn_8016265C(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_801626AC(void) {
#include "src/game/people/people_field_fn_801626AC.inc"
}
#else
void fn_801626AC(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 2
#if 0
asm void fn_80162858(void) {
#include "src/game/people/people_field_fn_80162858.inc"
}
#else
void fn_80162858(u32 index, u32 val1, u32 val2) {
    extern u32 lbl_8047B024;
    u32 offset = index * 0xF4;
    {
        u8* elem1 = (u8*)lbl_8047B024 + offset;
        *(u32*)(elem1 + 0x94) = val1;
    }
    {
        u8* elem2 = (u8*)lbl_8047B024 + offset;
        *(u32*)(elem2 + 0x98) = val2;
    }
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 2
#if 0
asm void fn_80162878(void) {
#include "src/game/people/people_field_fn_80162878.inc"
}
#else
u8 fn_80162878(u32 index) {
    extern u32 lbl_8047B024;
    u8* elem = (u8*)lbl_8047B024 + index * 0xF4;
    return *(u8*)(elem + 0x9C);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 2
#if 0
asm void fn_8016288C(void) {
#include "src/game/people/people_field_fn_8016288C.inc"
}
#else
u8 fn_8016288C(u32 index) {
    extern u32 lbl_8047B024;
    u8* elem = (u8*)lbl_8047B024 + index * 0xF4;
    return *(u8*)(elem + 0x90);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 2
#if 0
asm void fn_801628A0(void) {
#include "src/game/people/people_field_fn_801628A0.inc"
}
#else
u16 fn_801628A0(u32 index) {
    extern u32 lbl_8047B024;
    u8* elem = (u8*)lbl_8047B024 + index * 0xF4;
    return *(u16*)(elem + 0x70);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 2
#if 0
asm void fn_801628B4(void) {
#include "src/game/people/people_field_fn_801628B4.inc"
}
#else
void fn_801628B4(u32 index, u8 val) {
    extern u32 lbl_8047B024;
    u8* elem = (u8*)lbl_8047B024 + index * 0xF4;
    *(u8*)(elem + 0xA0) = val;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801628C8(void) {
#include "src/game/people/people_field_fn_801628C8.inc"
}
#else
void fn_801628C8(u32 index) {
    u32 offset = index * 0xF4;
    ((u8*)lbl_8047B024 + offset)[0xD4] = lbl_8047B050;
    fn_8015D44C((u8*)lbl_8047B024 + offset);
}
#endif
#pragma pop
extern u32 lbl_8047B024;
extern u8 lbl_8047B050;
void fn_80162904(u32 index) {
    u32* p;
    u8* elem = (u8*)lbl_8047B024 + index * 0xF4;
    elem += (u32)lbl_8047B050 << 2;
    p = (u32*)(elem + 0x24);
    *p |= 0x40;
}
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_8016292C(void) {
#include "src/game/people/people_field_fn_8016292C.inc"
}
#else
void fn_8016292C(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_801629A4(void) {
#include "src/game/people/people_field_fn_801629A4.inc"
}
#else
void fn_801629A4(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_801629D0(void) {
#include "src/game/people/people_field_fn_801629D0.inc"
}
#else
void fn_801629D0(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_801629FC(void) {
#include "src/game/people/people_field_fn_801629FC.inc"
}
#else
void fn_801629FC(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80162A58(void) {
#include "src/game/people/people_field_fn_80162A58.inc"
}
#else
void fn_80162A58(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162D18(void) {
#include "src/game/people/people_field_fn_80162D18.inc"
}
#else
void fn_80162D18(u32 index) {
    extern u32 lbl_8047B024;
    extern void fn_8015D4EC(u8* ptr);
    fn_8015D4EC((u8*)lbl_8047B024 + index * 0xF4);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80162D44(void) {
#include "src/game/people/people_field_fn_80162D44.inc"
}
#else
void fn_80162D44(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162D6C(void) {
#include "src/game/people/people_field_fn_80162D6C.inc"
}
#else
void fn_80162D6C(void) { fn_8015A950(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162D8C(void) {
#include "src/game/people/people_field_fn_80162D8C.inc"
}
#else
void fn_80162D8C(void) { fn_8015AAA0(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162DAC(void) {
#include "src/game/people/people_field_fn_80162DAC.inc"
}
#else
void fn_80162DAC(u8 index) {
    fn_8015D54C(&lbl_80447E60[(u8)index * 0xBC]);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162DE0(void) {
#include "src/game/people/people_field_fn_80162DE0.inc"
}
#else
void fn_80162DE0(u8 index) {
    fn_8015D5F4(&lbl_80447E60[(u8)index * 0xBC]);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80162E14(void) {
#include "src/game/people/people_field_fn_80162E14.inc"
}
#else
void fn_80162E14(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80162EB8(void) {
#include "src/game/people/people_field_fn_80162EB8.inc"
}
#else
void fn_80162EB8(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162F48(void) {
#include "src/game/people/people_field_fn_80162F48.inc"
}
#else
void fn_80162F48(void) { fn_80163CA8(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162F68(void) {
#include "src/game/people/people_field_fn_80162F68.inc"
}
#else
void fn_80162F68(void) { fn_80163DE8(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80162F88(void) {
#include "src/game/people/people_field_fn_80162F88.inc"
}
#else
void fn_80162F88(void* a) {
    extern void fn_80163DB0(void* a, u32 b);
    fn_80163DB0(a, 0);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80162FAC(void) {
#include "src/game/people/people_field_fn_80162FAC.inc"
}
#else
void fn_80162FAC(void) {}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80162FB0(void) {
#include "src/game/people/people_field_fn_80162FB0.inc"
}
#else
void fn_80162FB0(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_8016300C(void) {
#include "src/game/people/people_field_fn_8016300C.inc"
}
#else
void fn_8016300C(u32 a, u32 b) {
    extern void fn_801634A8(u32 x);
    fn_801634A8(b);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80163030(void) {
#include "src/game/people/people_field_fn_80163030.inc"
}
#else
void fn_80163030(void) { fn_80163794(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80163050(void) {
#include "src/game/people/people_field_fn_80163050.inc"
}
#else
void fn_80163050(u32** src, u32* out) {
    extern u32 fn_80163810(u32 a, u32 b);
    u32 val = *(u32*)((u8*)*src + 4);
    u32 type = val >> 24;
    u32 payload = val & 0xFFFFFF;
    switch (type) {
        case 0: case 1: case 4: case 5:
            payload = ((payload + 13) / 28) & ~7u;
            break;
        case 2:
            payload = payload << 1;
            break;
    }
    *out = fn_80163810(*out, payload);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801630E4(void) {
#include "src/game/people/people_field_fn_801630E4.inc"
}
#else
void fn_801630E4(u8* ptr, u32 size) { fn_801637B8(ptr, size); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80163104(void) {
#include "src/game/people/people_field_fn_80163104.inc"
}
#else
void fn_80163104(u8* src, u8* dest) {
    extern void fn_80163BCC(u8* a, u32 b);
    u32 val = *(u32*)(src + 4);
    u32 type = val >> 24;
    u32 payload = val & 0xFFFFFF;
    switch (type) {
        case 0: case 1: case 4: case 5:
            payload = ((payload + 13) / 28) & ~7u;
            break;
        case 2:
            payload = payload << 1;
            break;
    }
    fn_80163BCC(dest, payload);
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80163188(void) {
#include "src/game/people/people_field_fn_80163188.inc"
}
#else
void fn_80163188(void) { fn_80163490(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801631A8(void) {
#include "src/game/people/people_field_fn_801631A8.inc"
}
#else
void fn_801631A8(void) {}
#endif
#pragma pop
extern u32 lbl_8047B054;
extern u32 lbl_8047B058;
extern u32 lbl_8047B014;
#pragma push
void fn_801631AC(u32* src) {
    lbl_8047B054 = src[0];
    lbl_8047B058 = src[1];
}
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_801631C0(void) {
#include "src/game/people/people_field_fn_801631C0.inc"
}
#else
void fn_801631C0(void) { lbl_8047B014 = 0; }
#endif
#pragma pop
extern u32 lbl_8047B024;

u32 fn_801631CC(u32 index) {
    u8* elem = (u8*)lbl_8047B024 + index * 0xF4;
    if (*(u8*)(elem + 0xEC) == 0) {
        return -1;
    } else {
        return *(u32*)(elem + 0xE8);
    }
}
#pragma push
#pragma optimization_level 2
#if 0
asm void fn_801631F4(void) {
#include "src/game/people/people_field_fn_801631F4.inc"
}
#else
u32 fn_801631F4(u32 index) {
    extern u32 lbl_8047B024;
    u8* elem = (u8*)lbl_8047B024 + index * 0xF4;
    return *(u8*)(elem + 0xEC) == 1;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80163214(void) {
#include "src/game/people/people_field_fn_80163214.inc"
}
#else
void fn_80163214(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_801632B4(void) {
#include "src/game/people/people_field_fn_801632B4.inc"
}
#else
void fn_801632B4(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
extern u8 lbl_8044FB90[];
#if 0
asm void fn_80163490(void) {
#include "src/game/people/people_field_fn_80163490.inc"
}
#else
void fn_80163490(void) {
    u8* ptr;

    ptr = lbl_8044FB90;
    while (ptr[0x281] != 0) {
    }
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_801634A8(void) {
#include "src/game/people/people_field_fn_801634A8.inc"
}
#else
void fn_801634A8(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80163794(void) {
#include "src/game/people/people_field_fn_80163794.inc"
}
#else
void fn_80163794(void) {}
#endif
#pragma pop
extern void fn_800ACB44(void);
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80163798(void) {
#include "src/game/people/people_field_fn_80163798.inc"
}
#else
void fn_80163798(void) { fn_800ACB44(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
extern u32 lbl_8047B06C;
extern u32 lbl_8047B070;
#if 0
asm void fn_801637B8(void) {
#include "src/game/people/people_field_fn_801637B8.inc"
}
#else
void fn_801637B8(u8* ptr, u32 size) {
    u32 aligned;
    if (ptr) {
        aligned = (size + 0x1f) & ~0x1fu;
        lbl_8047B06C = aligned >= fn_800AE78C() ? aligned : fn_800AE78C();
    }
    lbl_8047B070 = (u32)ptr;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80163810(void) {
#include "src/game/people/people_field_fn_80163810.inc"
}
#else
void fn_80163810(void) { /* TODO */ }
#endif
#pragma pop
extern u32 lbl_8047B078;
void fn_80163BCC(u8* unused, u32 size) {
    lbl_8047B078 -= (size + 0x1F) & ~0x1F;
}
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80163BE4(void) {
#include "src/game/people/people_field_fn_80163BE4.inc"
}
#else
void fn_80163BE4(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80163CA8(void) {
#include "src/game/people/people_field_fn_80163CA8.inc"
}
#else
void fn_80163CA8(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80163DB0(void) {
#include "src/game/people/people_field_fn_80163DB0.inc"
}
#else
void fn_80163DB0(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80163DE8(void) {
#include "src/game/people/people_field_fn_80163DE8.inc"
}
#else
void fn_80163DE8(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80163EE0(void) {
#include "src/game/people/people_field_fn_80163EE0.inc"
}
#else
extern u8  lbl_8047B0A0;
extern u32 lbl_8047B09C;
extern u32 lbl_8047B08C;
extern u32 lbl_8047B098;
extern u32 lbl_8047B094;
extern u32 lbl_8047B090;
extern u32 lbl_8047B0A4;
void fn_80163EE0(void) {
    int counter = ((int)lbl_8047B0A0 + 1) % 4;
    u8* ptr = (u8*)(lbl_8047B09C + 0x80000000u) + (u8)counter * 0x280;
    lbl_8047B0A0 = counter;
    fn_800AC070(ptr, 0x280);
    lbl_8047B08C = OSGetTick();
    if (lbl_8047B098 != 0) {
        if (lbl_8047B090 == 0) {
            lbl_8047B090 = 1;
            OSEnableInterrupts();
            ((void(*)(void))lbl_8047B0A4)();
            OSDisableInterrupts();
            lbl_8047B090 = 0;
        }
    } else {
        lbl_8047B094 = 1;
    }
}
#endif
#pragma pop
extern u32 lbl_8047B098;
extern u32 lbl_8047B088;
void fn_80163F88(void) {
    lbl_8047B098 = 1;
    lbl_8047B088 = 1;
}
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80163F98(void) {
#include "src/game/people/people_field_fn_80163F98.inc"
}
#else
void fn_80163F98(void) {
    extern u32 lbl_8047B094;
    extern u32 lbl_8047B090;
    extern u32 lbl_8047B0A4;
    lbl_8047B098 = 1;
    if (lbl_8047B094 != 0) {
        lbl_8047B094 = 0;
        if (lbl_8047B090 == 0) {
            lbl_8047B090 = 1;
            OSEnableInterrupts();
            ((void(*)(void))lbl_8047B0A4)();
            OSDisableInterrupts();
            lbl_8047B090 = 0;
        }
    }
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm u32 fn_80163FFC(u32(*fnptr)(void), u32 d, u32 a) {
#include "src/game/people/people_field_fn_80163FFC.inc"
}
#else
u32 fn_80163FFC(u32(*fnptr)(void), u32 d, u32 a) { return 0; }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801640C4(void) {
#include "src/game/people/people_field_fn_801640C4.inc"
}
#else
void fn_801640C4(void) { fn_800AC0F8(); }
#endif
#pragma pop
u32 fn_801640E4(void) {
    extern u32 lbl_8047B09C;
    extern void fn_80164400(u32 a);
    fn_800AC02C(0);
    fn_800AC110();
    fn_80164400(lbl_8047B09C);
    return 1;
}
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80164118(void) {
#include "src/game/people/people_field_fn_80164118.inc"
}
#else
void fn_80164118(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80164148(void) {
#include "src/game/people/people_field_fn_80164148.inc"
}
#else
extern u8  lbl_804504A0[];
extern u8  lbl_80450500[];
extern u8  lbl_8036A520[];
extern u16 lbl_80478C08;
u32 fn_80164148(u32 d) {
    u8* s = lbl_804504A0;
    *(u32*)(s + 0x0C) = (u32)lbl_8036A520;
    *(u32*)(s + 0x10) = lbl_80478C08;
    *(u32*)(s + 0x14) = 0;
    *(u32*)(s + 0x18) = (u32)lbl_80450500;
    *(u32*)(s + 0x1C) = 0x2000;
    *(u32*)(s + 0x20) = 0;
    *(u16*)(s + 0x24) = 0x10;
    *(u16*)(s + 0x26) = 0x30;
    *(u32*)(s + 0x28) = (u32)fn_80163F88;
    *(u32*)(s + 0x2C) = (u32)fn_80163F98;
    *(u32*)(s + 0x30) = 0;
    *(u32*)(s + 0x34) = 0;
    *(u32*)(s + 0x04) = 0;
    fn_800AE7E0();
    fn_800AE93C(lbl_804504A0);
    lbl_8047B088 = 0;
    fn_80164328();
    while (lbl_8047B088 == 0) {}
    fn_80164360();
    return 1;
}
#endif
#pragma pop
u32 fn_80164204(void) {
    fn_800AE8EC();
    while (fn_800AE92C() != 0) {}
    fn_800AE8A4();
    return 1;
}
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80164238(void) {
#include "src/game/people/people_field_fn_80164238.inc"
}
#else
void fn_80164238(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801642AC(void) {
#include "src/game/people/people_field_fn_801642AC.inc"
}
#else
u32 fn_801642AC(void) {
    extern u32 OSGetTick(void);
    extern u32 lbl_8047B08C;
    u32 tick = OSGetTick();
    u32 bus_clock = *(volatile u32*)0x800000F8;
    u32 magic = 0x431CDE83u;
    u32 prev = lbl_8047B08C;
    u32 divisor = __mulhwu(magic, bus_clock >> 2) >> 15;
    return ((tick - prev) << 3) / divisor;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801642F8(void) {
#include "src/game/people/people_field_fn_801642F8.inc"
}
#else
void fn_801642F8(void) {
    extern u32 OSDisableInterrupts(void);
    extern u32 lbl_8047B080;
    extern u16 lbl_8047B084;
    lbl_8047B080 = OSDisableInterrupts();
    lbl_8047B084 = 1;
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 0
asm void fn_80164324(void) {
#include "src/game/people/people_field_fn_80164324.inc"
}
#else
void fn_80164324(void) {}
#endif
#pragma pop
void fn_80164328(void) {
    extern u16 lbl_8047B084;
    extern u32 lbl_8047B080;
    lbl_8047B084 = lbl_8047B084 - 1;
    if (lbl_8047B084 == 0) {
        OSRestoreInterrupts(lbl_8047B080);
    }
}
void fn_80164360(void) {
    extern u16 lbl_8047B084;
    extern u32 lbl_8047B080;
    if ((u16)lbl_8047B084++ == 0) {
        lbl_8047B080 = OSDisableInterrupts();
    }
}
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80164398(void) {
#include "src/game/people/people_field_fn_80164398.inc"
}
#else
u32 fn_80164398(void) { return OSDisableInterrupts(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801643B8(void) {
#include "src/game/people/people_field_fn_801643B8.inc"
}
#else
u32 fn_801643B8(void) { return OSEnableInterrupts(); }
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801643D8(void) {
#include "src/game/people/people_field_fn_801643D8.inc"
}
#else
void fn_801643D8(void) {
    extern u32 lbl_8047B054;
    ((void(*)(void))lbl_8047B054)();
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_80164400(void) {
#include "src/game/people/people_field_fn_80164400.inc"
}
#else
void fn_80164400(void) {
    extern u32 lbl_8047B054;
    u32* base = &lbl_8047B054;
    ((void(*)(void))base[1])();
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_8016442C(void) {
#include "src/game/people/people_field_fn_8016442C.inc"
}
#else
void fn_8016442C(u8 type, u32* data, u8* obj) {
    switch (type) {
        case 0:
            if (obj[0x1C4] != 0) { break; }
            fn_801652DC(data[0], data[1], data[2], obj);
            break;
    }
}
#endif
#pragma pop
u32 fn_80164488(u8* ptr) {
    extern void fn_80164A2C(f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6);
    ptr[0x1C4] = 1;
    fn_80164A2C(
        *(f32*)(ptr + 0x1C8),
        *(f32*)(ptr + 0x1D0),
        *(f32*)(ptr + 0x1CC),
        *(f32*)(ptr + 0x1D4),
        *(f32*)(ptr + 0x1D8),
        *(f32*)(ptr + 0x1DC)
    );
    ptr[0x1C4] = 0;
    return 1;
}
#pragma push
#pragma optimization_level 4
#pragma optimizewithasm off
#if 0
asm void fn_801644E0(void) {
#include "src/game/people/people_field_fn_801644E0.inc"
}
#else
void fn_801644E0(u8* ptr) {
    extern void fn_80164520(f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6);
    ptr[0x1C4] = 0;
    fn_80164520(
        *(f32*)(ptr + 0x1C8),
        *(f32*)(ptr + 0x1D0),
        *(f32*)(ptr + 0x1CC),
        *(f32*)(ptr + 0x1D4),
        *(f32*)(ptr + 0x1D8),
        *(f32*)(ptr + 0x1DC)
    );
}
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80164520(void) {
#include "src/game/people/people_field_fn_80164520.inc"
}
#else
void fn_80164520(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80164A2C(void) {
#include "src/game/people/people_field_fn_80164A2C.inc"
}
#else
void fn_80164A2C(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80164C40(void) {
#include "src/game/people/people_field_fn_80164C40.inc"
}
#else
void fn_80164C40(void) { /* TODO */ }
#endif
#pragma pop
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
#if 1
asm void fn_80164DD0(void) {
#include "src/game/people/people_field_fn_80164DD0.inc"
}
#else
void fn_80164DD0(void) { /* TODO */ }
#endif
#pragma pop
