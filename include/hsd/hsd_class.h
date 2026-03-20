/**
 * @file hsd_class.h
 * @brief HSD class/object system - base class infrastructure.
 *
 * Implements a C-based object-oriented class system with:
 * - Class hierarchy (parent/child relationships)
 * - Virtual function tables (alloc/init/release/destroy/amnesia)
 * - Memory pool management via free lists
 *
 * Adapted from the Melee decompilation (doldecomp/melee).
 */
#ifndef HSD_CLASS_H
#define HSD_CLASS_H

#include "dolphin/types.h"
#include "hsd/hsd_forward.h"

/* ========================================================================= */
/*  Type casting macros                                                      */
/* ========================================================================= */

#define HSD_CLASS_INFO(o) ((HSD_ClassInfo*) (o))
#define HSD_CLASS_METHOD(o) (((HSD_Class*) (o))->class_info)
#define HSD_PARENT_INFO(o) ((o)->parent.head.parent)

#define next_p(p) ((p) != NULL ? (p)->next : NULL)

/* ========================================================================= */
/*  Base class structure                                                     */
/* ========================================================================= */

typedef struct _HSD_Class {
    struct _HSD_ClassInfo* class_info;
} HSD_Class;

/* ========================================================================= */
/*  Class info header - metadata for each class type                         */
/* ========================================================================= */

typedef struct _HSD_ClassInfoHead {
    void (*info_init)(void);
    u32 flags;
    char* library_name;
    char* class_name;
    s16 obj_size;
    s16 info_size;
    struct _HSD_ClassInfo* parent;
    struct _HSD_ClassInfo* next;
    struct _HSD_ClassInfo* child;
    u32 nb_exist;
    u32 nb_peak;
} HSD_ClassInfoHead;

/* ========================================================================= */
/*  Class info - contains virtual function pointers                          */
/* ========================================================================= */

typedef struct _HSD_ClassInfo {
    HSD_ClassInfoHead head;
    HSD_Class* (*alloc)(struct _HSD_ClassInfo* c);
    int (*init)(HSD_Class* c);
    void (*release)(HSD_Class* c);
    void (*destroy)(HSD_Class* c);
    void (*amnesia)(struct _HSD_ClassInfo* c);
} HSD_ClassInfo;

/* ========================================================================= */
/*  Memory management structures                                             */
/* ========================================================================= */

typedef struct _HSD_FreeList {
    struct _HSD_FreeList* next;
} HSD_FreeList;

typedef struct _HSD_MemoryEntry {
    u32 size;
    u32 nb_alloc;
    u32 nb_free;
    HSD_FreeList* free_list;
    struct _HSD_MemoryEntry* next;
} HSD_MemoryEntry;

/* ========================================================================= */
/*  Global class info instance                                               */
/* ========================================================================= */

extern HSD_ClassInfo hsdClass;

/* ========================================================================= */
/*  Function declarations                                                    */
/* ========================================================================= */

void ClassInfoInit(HSD_ClassInfo* info);
void hsdInitClassInfo(HSD_ClassInfo* class_info, HSD_ClassInfo* parent_info,
                      char* base_class_library, char* type, s32 info_size,
                      s32 class_size);

void* hsdAllocMemPiece(s32 size);
void hsdFreeMemPiece(void* mem, s32 size);
void* hsdNew(HSD_ClassInfo* info);
BOOL hsdChangeClass(void* object, void* class_info);
BOOL hsdIsDescendantOf(void* info, void* p);
BOOL hsdObjIsDescendantOf(HSD_Obj* o, HSD_ClassInfo* p);
HSD_ClassInfo* hsdSearchClassInfo(const char* class_name);
void hsdForgetClassLibrary(const char* library_name);

HSD_MemoryEntry* GetMemoryEntry(s32 idx);
HSD_Class* _hsdClassAlloc(HSD_ClassInfo* info);
int _hsdClassInit(HSD_Class* arg0);
void _hsdClassRelease(HSD_Class* cls);
void _hsdClassDestroy(HSD_Class* cls);
void _hsdClassAmnesia(HSD_ClassInfo* info);

/* ========================================================================= */
/*  Inline: hsdDelete                                                        */
/* ========================================================================= */

static inline void hsdDelete(void* object)
{
    if (object == NULL) {
        return;
    }
    HSD_CLASS_METHOD(object)->release((HSD_Class*) object);
    HSD_CLASS_METHOD(object)->destroy((HSD_Class*) object);
}

#endif /* HSD_CLASS_H */
