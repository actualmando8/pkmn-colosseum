#ifndef PCPORT_FIELD_COLLISION_H
#define PCPORT_FIELD_COLLISION_H

#include "dolphin/types.h"

/* Host-side parser + query for the field WZX collision mesh.
 *
 * Field-map .fsys archives bundle a WZX collision member (see
 * include/game/gs_colsys.h + real_content_host.c:WZXLooksValid). The on-disc
 * layout (all offsets relative to the WZX block base):
 *
 *   header  0x00 u32 vertDataOff   -> offset to the group-record array
 *           0x04 u32 groupCount
 *   group   0x40 bytes; 6 submesh-slot offsets at +0x24,+0x28,...,+0x38,
 *           one per surface category (see PCPortColCategory).
 *   submesh 0x00 u32 vtxOff   -> offset to the triangle array
 *           0x04 u32 triCount
 *           0x08 u32 normalOff, 0x0C u32 extraOff
 *   tri     0x34 bytes: 3 x Vec3f verts (0x24) then a face normal (Vec3f) +1 word.
 *
 * We flatten every submesh into one triangle list tagged by category, then
 * answer vertical floor-height queries against the walkable categories. */

typedef enum {
    PCPORT_COLCAT_FLOOR    = 0, /* slot A: primary walkable floor */
    PCPORT_COLCAT_WALL     = 1, /* slot B: walls (block movement) */
    PCPORT_COLCAT_SLOPE    = 2, /* slot C: ramps/slopes (walkable) */
    PCPORT_COLCAT_CEIL     = 3, /* slot D: ceiling/overhead */
    PCPORT_COLCAT_EXTFLOOR = 4, /* slot E: extended/alternate floor (walkable) */
    PCPORT_COLCAT_BOUND    = 5  /* slot F: boundary/trigger (block movement) */
} PCPortColCategory;

/* Parse the WZX collision mesh from the given field-map .fsys archive.
 * Replaces any previously loaded mesh. Returns the triangle count (0 on
 * failure / no WZX member). */
int PCPort_FieldColLoad(const char* fsysPath);

/* Discard the loaded collision mesh. */
void PCPort_FieldColUnload(void);

/* Number of triangles in the loaded mesh. */
int PCPort_FieldColTriCount(void);

/* Fetch triangle `i`: writes 9 floats (v0,v1,v2) into out9 and the category
 * into *outCat. Returns FALSE if `i` is out of range. */
BOOL PCPort_FieldColGetTri(int i, f32 out9[9], int* outCat);

/* Vertical floor query: find the walkable surface (floor/slope/extfloor) under
 * world position (x, queryY, z) and write its height to *outY. Picks the
 * highest walkable triangle whose surface Y is <= queryY + climb (so you stand
 * on the floor beneath you, stepping up small lips). Returns TRUE if a floor
 * was found below the query point. */
BOOL PCPort_FieldColFloorAt(f32 x, f32 z, f32 queryY, f32 climb, f32* outY);

/* World-space axis-aligned bounds of the whole mesh (for camera framing /
 * sanity checks). Returns FALSE if no mesh is loaded. */
BOOL PCPort_FieldColBounds(f32 outMin[3], f32 outMax[3]);

/* Push (*x,*z) out of any blocking (wall/boundary) triangles it is within
 * `radius` of, considering only walls whose vertical span overlaps [yLo, yHi]
 * (the player's body height range). Resolves against triangle edges projected
 * to XZ with a few relaxation passes so corners and multiple walls settle,
 * giving wall sliding. Returns TRUE if any push was applied. */
BOOL PCPort_FieldColResolveXZ(f32* x, f32* z, f32 yLo, f32 yHi, f32 radius);

#endif
