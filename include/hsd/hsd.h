/**
 * @file hsd.h
 * @brief Master include for the HSD (HAL SysDolphin) library.
 *
 * HAL Laboratory's SysDolphin is the rendering and scene graph library
 * used by both Super Smash Bros. Melee and Pokemon Colosseum. This file
 * provides a single include point for all HSD type definitions and
 * function declarations.
 *
 * The library string "sysdolphin_base_library" appears in the Colosseum
 * binary, confirming this is the same library lineage.
 *
 * HSD Object Hierarchy:
 *
 *   HSD_Class (base, with ClassInfo virtual table)
 *     +-- HSD_Obj (adds reference counting)
 *           +-- HSD_JObj  (Joint - skeletal hierarchy node)
 *           +-- HSD_DObj  (Display - visible geometry)
 *           +-- HSD_MObj  (Material - surface properties)
 *           +-- HSD_TObj  (Texture - texture state)
 *           +-- HSD_CObj  (Camera - view/projection)
 *           +-- HSD_LObj  (Light - hardware lights)
 *           +-- HSD_WObj  (World - 3D position target)
 *           +-- HSD_Fog   (Fog - distance fog)
 *           +-- HSD_FogAdj (FogAdj - fog correction)
 *     +-- HSD_PObj  (Primitive - mesh/polygon data, not ref-counted)
 *     +-- HSD_DObj  (Display - links material to polygons)
 *
 *   HSD_GObj (Game Object - top-level scene container)
 *   HSD_AObj (Animation Object - playback state)
 *   HSD_FObj (Function Object - keyframe data)
 *   HSD_RObj (Reference Object - constraints/IK)
 *
 * Colosseum HSD address range: 0x80190E34 - 0x801C0000
 *
 * Reference: doldecomp/melee (https://github.com/doldecomp/melee)
 */
#ifndef HSD_H
#define HSD_H

/* Foundation */
#include "hsd/hsd_forward.h"
#include "hsd/hsd_debug.h"
#include "hsd/hsd_memory.h"
#include "hsd/hsd_class.h"
#include "hsd/hsd_object.h"

/* Animation primitives */
#include "hsd/hsd_fobj.h"
#include "hsd/hsd_aobj.h"
#include "hsd/hsd_robj.h"

/* Scene graph objects */
#include "hsd/hsd_wobj.h"
#include "hsd/hsd_tobj.h"
#include "hsd/hsd_mobj.h"
#include "hsd/hsd_pobj.h"
#include "hsd/hsd_dobj.h"
#include "hsd/hsd_jobj.h"
#include "hsd/hsd_cobj.h"
#include "hsd/hsd_lobj.h"
#include "hsd/hsd_fog.h"

/* Top-level containers */
#include "hsd/hsd_gobj.h"

/* System */
#include "hsd/hsd_initialize.h"

#endif /* HSD_H */
