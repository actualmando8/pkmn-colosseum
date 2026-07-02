/**
 * @file fight_range_80201764.c
 * @brief fight code, 0x80201764 - 0x80202810 (14 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 *
 * Reconciliation (2026-07-02): the 14 definitions below were originally
 * written into src/game/trainer.c (fn_80201764) and
 * src/game/colosseum_event.c (the other 13) -- the wrong translation
 * units for this address range per splits.txt. Moved here, in address
 * order, with no functional changes.
 */
#include "dolphin/types.h"

extern void* fn_8012640C(void* context, u32 slot, u16 tableId, u32 flags);

/* 0x80201764 | size: 0x12C */
#pragma scheduling off
#pragma optimize_for_size on
u16 fn_80201764(void* context, void* item, void* target) {
    extern u16 fn_80119ED0();
    extern u16 fn_8011A280();
    extern u16 fn_801214FC();
    void* partyList;
    void* pokemon;
    if (fn_80119ED0(item) == 0x7C || fn_80119ED0(item) == 0xC8 || fn_80119ED0(item) == 0xCD) {
        partyList = fn_8012640C(context, 0, 0xD6, 0);
        if (fn_80119ED0(item) == 0x7C || fn_80119ED0(item) == 0xC8) {
            if (partyList == NULL) {
                pokemon = NULL;
            } else {
                pokemon = fn_8012640C(partyList, 0, 0xCC, 0);
            }
            return fn_801214FC(pokemon, item, target);
        } else if (fn_80119ED0(item) == 0xCD) {
            return fn_8011A280(partyList, item, target);
        }
    } else if (fn_80119ED0(item) == 0xD8) {
        return fn_8011A280(context, item, target);
    }
}
#pragma optimize_for_size reset
#pragma scheduling reset

/* 0x80201890 | size: 0x12C */
#pragma scheduling off
#pragma optimize_for_size on
u16 fn_80201890(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern u16 fn_8011A3E4();
    extern u16 fn_80121574();
    void* resolved;
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8 || fn_80119ED0(typeObj) == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            return fn_80121574(resolved, typeObj);
        }
        if (fn_80119ED0(typeObj) != 0xCD) {
            return 0;
        }
        return fn_8011A3E4(resolved, typeObj);
    }
    if (fn_80119ED0(typeObj) != 0xD8) {
        return 0;
    }
    return fn_8011A3E4(ctx, typeObj);
}
#pragma optimize_for_size reset
#pragma scheduling reset

/* 0x802019BC | size: 0x170 */
#pragma scheduling off
#pragma optimize_for_size on
u16 fn_802019BC(void* ctx, void* src, void* typeObj) {
    extern u16 fn_80119ED0();
    extern u16 fn_8011A0A8();
    extern u16 fn_80121484();
    void* srcResolved;
    void* ctxResolved;
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8 || fn_80119ED0(typeObj) == 0xCD) {
        srcResolved = fn_8012640C(src, 0, 0xD6, 0);
        ctxResolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
            if (ctxResolved == NULL) {
                ctxResolved = NULL;
            } else {
                ctxResolved = fn_8012640C(ctxResolved, 0, 0xCC, 0);
            }
            if (srcResolved == NULL) {
                srcResolved = NULL;
            } else {
                srcResolved = fn_8012640C(srcResolved, 0, 0xCC, 0);
            }
            return fn_80121484(ctxResolved, srcResolved, typeObj);
        } else if (fn_80119ED0(typeObj) == 0xCD) {
            return fn_8011A0A8(ctxResolved, srcResolved, typeObj);
        }
    } else if (fn_80119ED0(typeObj) == 0xD8) {
        return fn_8011A0A8(ctx, src, typeObj);
    }
}
#pragma optimize_for_size reset
#pragma scheduling reset

/* 0x80201B2C | size: 0x12C */
#pragma scheduling off
#pragma optimize_for_size on
u16 fn_80201B2C(void* ctx, void* typeObj, u32 param) {
    extern u16 fn_80119ED0();
    extern u16 fn_8011A570();
    extern u16 fn_801215E4();
    void* resolved;
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8 || fn_80119ED0(typeObj) == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            return fn_801215E4(resolved, typeObj, param);
        } else if (fn_80119ED0(typeObj) == 0xCD) {
            return fn_8011A570(resolved, typeObj, param);
        }
    } else if (fn_80119ED0(typeObj) == 0xD8) {
        return fn_8011A570(ctx, typeObj, param);
    }
}
#pragma optimize_for_size reset
#pragma scheduling reset

/* 0x80201C58 | size: 0x12C */
#pragma scheduling off
#pragma optimize_for_size on
u16 fn_80201C58(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern u16 fn_8011A6D4();
    extern u16 fn_8012165C();
    void* resolved;
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8 || fn_80119ED0(typeObj) == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            return fn_8012165C(resolved, typeObj);
        }
        if (fn_80119ED0(typeObj) != 0xCD) {
            return 0;
        }
        return fn_8011A6D4(resolved, typeObj);
    }
    if (fn_80119ED0(typeObj) != 0xD8) {
        return 0;
    }
    return fn_8011A6D4(ctx, typeObj);
}
#pragma optimize_for_size reset
#pragma scheduling reset

/* 0x80201D84 | size: 0x12C */
#pragma scheduling off
#pragma optimize_for_size on
u16 fn_80201D84(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern u16 fn_8011A860();
    extern u16 fn_801216CC();
    void* resolved;
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8 || fn_80119ED0(typeObj) == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            return fn_801216CC(resolved, typeObj);
        }
        if (fn_80119ED0(typeObj) != 0xCD) {
            return 0;
        }
        return fn_8011A860(resolved, typeObj);
    }
    if (fn_80119ED0(typeObj) != 0xD8) {
        return 0;
    }
    return fn_8011A860(ctx, typeObj);
}
#pragma optimize_for_size reset
#pragma scheduling reset

/* 0x80201EB0 | size: 0x12C */
#pragma scheduling off
#pragma optimize_for_size on
u16 fn_80201EB0(void* ctx, void* typeObj, u32 param) {
    extern u16 fn_80119ED0();
    extern u16 fn_8011A9EC();
    extern u16 fn_8012173C();
    void* resolved;
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8 || fn_80119ED0(typeObj) == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            return fn_8012173C(resolved, typeObj, param);
        } else if (fn_80119ED0(typeObj) == 0xCD) {
            return fn_8011A9EC(resolved, typeObj, param);
        }
    } else if (fn_80119ED0(typeObj) == 0xD8) {
        return fn_8011A9EC(ctx, typeObj, param);
    }
}
#pragma optimize_for_size reset
#pragma scheduling reset

/* 0x80201FDC | size: 0x12C */
#pragma scheduling off
#pragma optimize_for_size on
u16 fn_80201FDC(void* ctx, void* typeObj, u32 param) {
    extern u16 fn_80119ED0();
    extern u16 fn_8011AB50();
    extern u16 fn_801217B4();
    void* resolved;
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8 || fn_80119ED0(typeObj) == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            return fn_801217B4(resolved, typeObj, param);
        } else if (fn_80119ED0(typeObj) == 0xCD) {
            return fn_8011AB50(resolved, typeObj, param);
        }
    } else if (fn_80119ED0(typeObj) == 0xD8) {
        return fn_8011AB50(ctx, typeObj, param);
    }
}
#pragma optimize_for_size reset
#pragma scheduling reset

/* 0x80202108 | size: 0x12C */
#pragma scheduling off
#pragma optimize_for_size on
u16 fn_80202108(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern u16 fn_8011ACB4();
    extern u16 fn_8012182C();
    void* resolved;
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8 || fn_80119ED0(typeObj) == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            return fn_8012182C(resolved, typeObj);
        }
        if (fn_80119ED0(typeObj) != 0xCD) {
            return 0;
        }
        return fn_8011ACB4(resolved, typeObj);
    }
    if (fn_80119ED0(typeObj) != 0xD8) {
        return 0;
    }
    return fn_8011ACB4(ctx, typeObj);
}
#pragma optimize_for_size reset
#pragma scheduling reset

/* 0x80202234 | size: 0x12C */
#pragma scheduling off
#pragma optimize_for_size on
u16 fn_80202234(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern u16 fn_8011AE40();
    extern u16 fn_8012189C();
    void* resolved;
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8 || fn_80119ED0(typeObj) == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            return fn_8012189C(resolved, typeObj);
        }
        if (fn_80119ED0(typeObj) != 0xCD) {
            return 0;
        }
        return fn_8011AE40(resolved, typeObj);
    }
    if (fn_80119ED0(typeObj) != 0xD8) {
        return 0;
    }
    return fn_8011AE40(ctx, typeObj);
}
#pragma optimize_for_size reset
#pragma scheduling reset

/* 0x80202360 | size: 0x12C */
#pragma scheduling off
#pragma optimize_for_size on
u16 fn_80202360(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern u16 fn_8011B130();
    extern u16 fn_80121984();
    void* resolved;
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8 || fn_80119ED0(typeObj) == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            return fn_80121984(resolved, typeObj);
        }
        if (fn_80119ED0(typeObj) != 0xCD) {
            return 0;
        }
        return fn_8011B130(resolved, typeObj);
    }
    if (fn_80119ED0(typeObj) != 0xD8) {
        return 0;
    }
    return fn_8011B130(ctx, typeObj);
}
#pragma optimize_for_size reset
#pragma scheduling reset

/* 0x8020248C | size: 0x12C */
#pragma scheduling off
#pragma optimize_for_size on
u16 fn_8020248C(void* ctx, void* typeObj, u32 param) {
    extern u16 fn_80119ED0();
    extern u16 fn_8011B2C0();
    extern u16 fn_801219F4();
    void* resolved;
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8 || fn_80119ED0(typeObj) == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            return fn_801219F4(resolved, typeObj, param);
        } else if (fn_80119ED0(typeObj) == 0xCD) {
            return fn_8011B2C0(resolved, typeObj, param);
        }
    } else if (fn_80119ED0(typeObj) == 0xD8) {
        return fn_8011B2C0(ctx, typeObj, param);
    }
}
#pragma optimize_for_size reset
#pragma scheduling reset

/* 0x802025B8 | size: 0x12C */
#pragma scheduling off
#pragma optimize_for_size on
u16 fn_802025B8(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern u16 fn_8011B444();
    extern u16 fn_80121A6C();
    void* resolved;
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8 || fn_80119ED0(typeObj) == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            return fn_80121A6C(resolved, typeObj);
        }
        if (fn_80119ED0(typeObj) != 0xCD) {
            return 0;
        }
        return fn_8011B444(resolved, typeObj);
    }
    if (fn_80119ED0(typeObj) != 0xD8) {
        return 0;
    }
    return fn_8011B444(ctx, typeObj);
}
#pragma optimize_for_size reset
#pragma scheduling reset

/* 0x802026E4 | size: 0x12C */
#pragma scheduling off
#pragma optimize_for_size on
u16 fn_802026E4(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern u16 fn_8011B67C();
    extern u16 fn_80121ADC();
    void* resolved;
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8 || fn_80119ED0(typeObj) == 0xCD) {
        resolved = fn_8012640C(ctx, 0, 0xD6, 0);
        if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
            if (resolved == NULL) {
                resolved = NULL;
            } else {
                resolved = fn_8012640C(resolved, 0, 0xCC, 0);
            }
            return fn_80121ADC(resolved, typeObj);
        }
        if (fn_80119ED0(typeObj) != 0xCD) {
            return 0;
        }
        return fn_8011B67C(resolved, typeObj);
    }
    if (fn_80119ED0(typeObj) != 0xD8) {
        return 0;
    }
    return fn_8011B67C(ctx, typeObj);
}
#pragma optimize_for_size reset
#pragma scheduling reset
