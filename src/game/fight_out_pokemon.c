/**
 * @file fight_out_pokemon.c
 * @brief fightOutPokemon + fightPokemon section -- split from colosseum_event.c (the fight
 *        engine bucket, 0x80202810-0x80211A00), address range
 *        0x80202810-0x8020AE30, 162 fns.
 *
 * OutPokemon/Pokemon field accessors, sequence/status writers, and
 * damage-calc support the seq/waza layers call into (statusGetStatus,
 * fadeEffectGetRandom callers, etc). Corresponds to XD's
 * fight.cpp fightOutPokemon+fightPokemon cluster (0x80200644-0x80208288).
 */

#include "game/colosseum.h"
#include "game/trainer.h"
#include "game/pokemon.h"

typedef struct ColosseumEventRow6 {
    u8 mode;
    u8 field_01;
    u16 eventIndex;
    u16 nextIndex;
} ColosseumEventRow6;

typedef struct ColosseumEventSubRow {
    u8 valueMode;
    u8 scaleMode;
    s16 scaleNumerator;
    s16 scaleDenominator;
    u16 minValue;
    u16 maxValue;
} ColosseumEventSubRow;

typedef struct ColosseumEventPairRow {
    u8 resultFuncId;
    u8 field_01;
    u16 firstLinkIndex;
    ColosseumEventSubRow slots[2];
} ColosseumEventPairRow;

typedef struct StatusIdTable7 {
    u16 id[7];
} StatusIdTable7;

/* =========================================================================
 * External declarations
 * ========================================================================= */

extern void* pokemonGetStatus();
extern u32   pokemonSetStatus();
extern void  pokemonGrowBasisStatus();
extern u32   itemGetStatus();
extern void  fn_80119ED0(void);
extern void  fn_80121ADC(void);
extern void  fn_8011B67C(void);
extern void  pokemonGetSoubiItemDataId(void);
extern void* fightActionGetPri(void* p);
extern void  wazaGetStatus(void);

/* SDA table pointers for event data arrays */
extern u32 lbl_80478D38;   /* Event table count */
extern ColosseumEventRow6 lbl_80478D30[]; /* Event table base (6 bytes per entry) */
extern u32 lbl_80478D28; /* Pair-row table count */
extern ColosseumEventPairRow lbl_80375A08[]; /* 0x18-byte pair rows */

/* 0x80202810 | size: 0x188 | medium */
/* 0x80202810 | size: 0x188 */
void fightOutPokemonWriteJoutaiDataId(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern void fn_8011B788();
    extern void fn_80121B4C();
    extern void fn_801DA36C();
    void* eeData;

    eeData = pokemonGetStatus(ctx, 0, 0xEE, 0);
    if ((u16)(u32)typeObj == 0) {
        if (eeData != NULL) {
            fn_801DA36C(eeData, 1);
            fn_801DA36C(eeData, 2);
        }
    } else {
        if (eeData != NULL) {
            if ((u16)(u32)typeObj == 8) {
                fn_801DA36C(eeData, 1);
            }
            if ((u16)(u32)typeObj == 7) {
                fn_801DA36C(eeData, 2);
            }
        }
    }
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8 || fn_80119ED0(typeObj) == 0xCD) {
        eeData = pokemonGetStatus(ctx, 0, 0xD6, 0);
        if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
            if (eeData == NULL) {
                eeData = NULL;
            } else {
                eeData = pokemonGetStatus(eeData, 0, 0xCC, 0);
            }
            fn_80121B4C(eeData, typeObj);
        } else if (fn_80119ED0(typeObj) == 0xCD) {
            fn_8011B788(eeData, typeObj);
        }
    } else if (fn_80119ED0(typeObj) == 0xD8) {
        fn_8011B788(ctx, typeObj);
    }
}

/* 0x80202998 | size: 0x94 */
void fightOutPokemonResetSeqStatus(void* ctx, u16 mode) {
    extern void fn_801DA36C();
    void* obj;
    u16 modeVal;
    obj = pokemonGetStatus(ctx, 0, 0xEE, 0);
    modeVal = mode;
    if (modeVal == 0) {
        if (obj != NULL) {
            fn_801DA36C(obj, 1);
            fn_801DA36C(obj, 2);
        }
    } else {
        if (obj != NULL) {
            if (modeVal == 8) {
                fn_801DA36C(obj, 1);
            }
            modeVal = mode;
            if (modeVal == 7) {
                fn_801DA36C(obj, 2);
            }
        }
    }
}

/* 0x80202A2C | size: 0xB0 */
void fightPokemonWriteJoutaiDataId(void* ctx, void* typeObj, u32 param) {
    extern u16 fn_80119ED0();
    extern void fn_8011AFCC();
    extern void fn_8012190C();
    void* resolved;
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
        if (ctx == NULL) {
            resolved = NULL;
        } else {
            resolved = pokemonGetStatus(ctx, 0, 0xCC, 0);
        }
        fn_8012190C(resolved, typeObj, param);
    } else if (fn_80119ED0(typeObj) == 0xCD) {
        fn_8011AFCC(ctx, typeObj, param);
    }
}

/* 0x80202ADC | size: 0xAC */
u32 fightPokemonCheckWriteJoutaiDataId(void* ctx, void* typeObj) {
    extern u16 fn_80119ED0();
    extern u32 fn_8011B67C();
    extern u32 fn_80121ADC();
    void* resolved;
    u32 result;
    if (fn_80119ED0(typeObj) == 0x7C || fn_80119ED0(typeObj) == 0xC8) {
        if (ctx == NULL) {
            resolved = NULL;
        } else {
            resolved = pokemonGetStatus(ctx, 0, 0xCC, 0);
        }
        result = fn_80121ADC(resolved, typeObj);
    } else if (fn_80119ED0(typeObj) != 0xCD) {
        result = 0;
    } else {
        result = fn_8011B67C(ctx, typeObj);
    }
    return result;
}

/* 0x80202B88 | size: 0x94 */
u32 fightOutPokemonIsAlly(void* obj1, void* obj2) {
    extern u32 fightTargetGetPtr();
    extern u16 fightFloorGetStatus();
    u16 tableId;
    u32 val1;
    u32 val2;
    tableId = 0xFFFF & fightFloorGetStatus(NULL, 0, 0x14, 0);
    if (obj1 == NULL) {
        return 0;
    }
    if (obj2 == NULL) {
        return 0;
    }
    val1 = fightTargetGetPtr(2, obj1, tableId);
    val2 = fightTargetGetPtr(2, obj2, tableId);
    return (u8)(val1 == val2);
}

/* Address: 0x80202C1C | Size: 0x57c | Ghidra import */

void fn_80202C1C(int r3,u32 r4)

{
    extern s8 pokemonCheckFightOut();
    extern s8 pokemonCheckValid();
    extern u16 fn_801EF634();
    extern u32 fightFloorGetStatus();
    extern u32 fightSideGetStatus();
    extern s8 fightTrainerCheckValid();
    extern int fightTrainerGetStatus();
    extern int fightOutPokemonEnemySearchAry();
    extern s8 fightOutPokemonEnemyCheckValid();
    extern void fightOutPokemonEnemyCreate();
  u32 bVar1;
  u32 uVar2;
  u32 uVar3;
  u32 uVar4;
  u8 cVar9;
  int iVar5;
  u16 sVar8;
  int iVar6;
  int iVar7;
  u16 uVar10;
  u32 uVar11;
  u32 uVar12;
  
  fightFloorGetStatus(0,0,0x14,0);
  uVar2 = fightFloorGetStatus(0,0,0x16,0);
  uVar3 = fightFloorGetStatus(0,0,0x18,0);
  for (uVar12 = 0; (uVar12 & 0xffff) < (uVar2 & 0xffff); uVar12 = uVar12 + 1) {
    uVar4 = fightSideGetStatus(r4,0,7,uVar12);
    cVar9 = fightTrainerCheckValid();
    if (cVar9 != 0) {
      for (uVar11 = 0; (uVar11 & 0xffff) < (uVar3 & 0xffff); uVar11 = uVar11 + 1) {
        iVar5 = fightTrainerGetStatus(uVar4,0,0x46,uVar11);
        if (iVar5 == 0) {
          bVar1 = 0;
        }
        else {
          sVar8 = fn_801EF634();
          if (sVar8 == 1) {
            bVar1 = 0;
          }
          else {
            iVar6 = (int)pokemonGetStatus(iVar5,0,0xd6,0);
            if (iVar6 == 0) {
              bVar1 = 0;
            }
            else {
              sVar8 = fn_801EF634();
              if (sVar8 == 1) {
                bVar1 = 0;
              }
              else {
                iVar7 = (int)pokemonGetStatus(iVar6,0,0xcb,0);
                if (iVar7 == 0) {
                  bVar1 = 0;
                }
                else {
                  cVar9 = pokemonCheckValid();
                  if (cVar9 == 0) {
                    bVar1 = 0;
                  }
                  else {
                    if (iVar6 == 0) {
                      iVar7 = 0;
                    }
                    else {
                      iVar7 = (int)pokemonGetStatus(iVar6,0,0xcc,0);
                    }
                    if (iVar7 == 0) {
                      bVar1 = 0;
                    }
                    else {
                      cVar9 = pokemonCheckValid();
                      if (cVar9 == 0) {
                        bVar1 = 0;
                      }
                      else {
                        iVar6 = (int)pokemonGetStatus(iVar6,0,0xce,0);
                        if (iVar6 < 0) {
                          bVar1 = 0;
                        }
                        else {
                          bVar1 = 1;
                        }
                      }
                    }
                  }
                }
              }
              if (bVar1) {
                bVar1 = 1;
              }
              else {
                bVar1 = 0;
              }
            }
          }
          if (bVar1) {
            iVar6 = (int)pokemonGetStatus(iVar5,0,0x120,0);
            if (iVar6 == 1) {
              bVar1 = 0;
            }
            else {
              iVar6 = (int)pokemonGetStatus(iVar5,0,0xd6,0);
              if (iVar6 == 0) {
                bVar1 = 0;
              }
              else {
                sVar8 = fn_801EF634();
                if (sVar8 == 1) {
                  bVar1 = 0;
                }
                else {
                  iVar7 = (int)pokemonGetStatus(iVar6,0,0xcb,0);
                  if (iVar7 == 0) {
                    bVar1 = 0;
                  }
                  else {
                    cVar9 = pokemonCheckValid();
                    if (cVar9 == 0) {
                      bVar1 = 0;
                    }
                    else {
                      if (iVar6 == 0) {
                        iVar7 = 0;
                      }
                      else {
                        iVar7 = (int)pokemonGetStatus(iVar6,0,0xcc,0);
                      }
                      if (iVar7 == 0) {
                        bVar1 = 0;
                      }
                      else {
                        cVar9 = pokemonCheckValid();
                        if (cVar9 == 0) {
                          bVar1 = 0;
                        }
                        else {
                          iVar7 = (int)pokemonGetStatus(iVar6,0,0xce,0);
                          if (iVar7 < 0) {
                            bVar1 = 0;
                          }
                          else {
                            bVar1 = 1;
                          }
                        }
                      }
                    }
                  }
                }
                if (bVar1) {
                  iVar7 = (int)pokemonGetStatus(iVar6,0,0xd2,0);
                  if (iVar7 == 1) {
                    bVar1 = 0;
                  }
                  else {
                    if (iVar6 == 0) {
                      iVar6 = 0;
                    }
                    else {
                      iVar6 = (int)pokemonGetStatus(iVar6,0,0xcc,0);
                    }
                    if (iVar6 == 0) {
                      bVar1 = 0;
                    }
                    else {
                      cVar9 = pokemonCheckFightOut();
                      if (cVar9 == 0) {
                        bVar1 = 0;
                      }
                      else {
                        bVar1 = 1;
                      }
                    }
                  }
                }
                else {
                  bVar1 = 0;
                }
              }
              if (bVar1) {
                bVar1 = 1;
              }
              else {
                bVar1 = 0;
              }
            }
          }
          else {
            bVar1 = 0;
          }
        }
        if ((bVar1) && (r3 != 0)) {
          if (iVar5 == 0) {
            bVar1 = 0;
          }
          else {
            sVar8 = fn_801EF634();
            if (sVar8 == 1) {
              bVar1 = 0;
            }
            else {
              iVar6 = (int)pokemonGetStatus(iVar5,0,0xd6,0);
              if (iVar6 == 0) {
                bVar1 = 0;
              }
              else {
                sVar8 = fn_801EF634();
                if (sVar8 == 1) {
                  bVar1 = 0;
                }
                else {
                  iVar7 = (int)pokemonGetStatus(iVar6,0,0xcb,0);
                  if (iVar7 == 0) {
                    bVar1 = 0;
                  }
                  else {
                    cVar9 = pokemonCheckValid();
                    if (cVar9 == 0) {
                      bVar1 = 0;
                    }
                    else {
                      if (iVar6 == 0) {
                        iVar7 = 0;
                      }
                      else {
                        iVar7 = (int)pokemonGetStatus(iVar6,0,0xcc,0);
                      }
                      if (iVar7 == 0) {
                        bVar1 = 0;
                      }
                      else {
                        cVar9 = pokemonCheckValid();
                        if (cVar9 == 0) {
                          bVar1 = 0;
                        }
                        else {
                          iVar6 = (int)pokemonGetStatus(iVar6,0,0xce,0);
                          if (iVar6 < 0) {
                            bVar1 = 0;
                          }
                          else {
                            bVar1 = 1;
                          }
                        }
                      }
                    }
                  }
                }
                if (bVar1) {
                  bVar1 = 1;
                }
                else {
                  bVar1 = 0;
                }
              }
            }
          }
          if (bVar1) {
            iVar6 = (int)pokemonGetStatus(r3,0,0x122,0);
            iVar7 = fightOutPokemonEnemySearchAry(iVar6,4,iVar5);
            if (iVar7 == 0) {
              for (uVar10 = 0; uVar10 < 4; uVar10 = uVar10 + 1) {
                iVar7 = iVar6 + (u32)uVar10 * 0xc;
                cVar9 = fightOutPokemonEnemyCheckValid(iVar7);
                if (cVar9 == 0) {
                  fightOutPokemonEnemyCreate(iVar7,iVar5);
                  break;
                }
              }
            }
          }
        }
      }
    }
  }
}

/* 0x80203198 | size: 0x14C | medium */
/* 0x80203198 | size: 0x14C */
void fn_80203198(void* ctx, u32 param) {
    extern u16 fightOutPokemonEnemyBiosGetOumuWazaDataId();
    extern void* fightOutPokemonEnemySearchAry();
    extern u8 fightOutPokemonEnemyCheckValid();
    extern void fightOutPokemonEnemyInit();
    void* tableData;
    void* entry;
    void* entryPtr;
    u16 species;
    u8 count;
    u8 i;

    if (ctx == NULL) { return; }
    tableData = pokemonGetStatus(ctx, 0, 0x122, 0);
    entry = fightOutPokemonEnemySearchAry(tableData, 4, param);
    if (entry == NULL) { return; }
    species = fightOutPokemonEnemyBiosGetOumuWazaDataId(entry);
    fightOutPokemonEnemyInit(entry);
    if (species == 0 || species == 0x165 || species == 0xFFFF) { return; }
    if ((s32)(u32)pokemonGetStatus(ctx, 0, 0xF7, 0) != 0) { return; }
    if (ctx != NULL) {
        entryPtr = pokemonGetStatus(ctx, 0, 0x122, 0);
        for (i = 0; i < 4; i++) {}
        count = 0;
        for (i = 0; i < 4; i++) {
            entry = (void*)((u32)entryPtr + i * 0xC);
            if ((u8)fightOutPokemonEnemyCheckValid(entry) == 4) { continue; }
            species = fightOutPokemonEnemyBiosGetOumuWazaDataId(entry);
            if (species == 4 || species == 0x165) { continue; }
            count++;
        }
    } else {
        count = 0;
    }
    if (count == 4) {
        pokemonSetStatus(ctx, 0, 0xF7, 0, (u32)species);
    }
}

/* 0x802032E4 | size: 0x138 */
#pragma push
#pragma scheduling off
static inline void* fn_802032E4_getCC(void* ctx) {
    return pokemonGetStatus(ctx, 0, 0xCC, 0);
}

void fightPokemonGetFriendFormPokemonFriendFilterId(void* ctx, u32 param) {
    extern u16 fn_80119ED0();
    extern u8 fn_8011B67C();
    extern u8 fn_80121ADC();
    extern void pokemonGetFriendFormPokemonFriendFilterId();
    extern u32 pokemonGetSoubiItemSoubiDataId();
    void* resolvedPtr;
    void* ccData;
    void* ccCtx;
    u8 result;
    u32 value;

    if (ctx == 0) { ccData = 0; } else { ccData = fn_802032E4_getCC(ctx); }
    if (ccData == NULL) { return; }
    if (ctx == 0) { ccCtx = 0; } else { ccCtx = fn_802032E4_getCC(ctx); }
    if (ccCtx == NULL) {
        value = 0;
    } else {
        if (fn_80119ED0(0x3D) == 0x7C || fn_80119ED0(0x3D) == 0xC8) {
            void* tmp;
            if (ctx == 0) { tmp = 0; } else { resolvedPtr = pokemonGetStatus(ctx, 0, 0xCC, 0); tmp = resolvedPtr; }
            result = fn_80121ADC(tmp, 0x3D);
        } else {
            if (fn_80119ED0(0x3D) != 0xCD) {
                result = 0;
            } else {
                result = fn_8011B67C(ctx, 0x3D);
            }
        }
        if (result == 1) {
            value = 0;
        } else {
            value = pokemonGetSoubiItemSoubiDataId(ccCtx);
        }
    }
    pokemonGetFriendFormPokemonFriendFilterId(ccData, value, param);
}
#pragma pop

/* 0x8020341C | size: 0x140 */
#pragma push
#pragma scheduling off
static inline void* fn_8020341C_resolveCcData(void* ctx)
{
    return pokemonGetStatus(ctx, 0, 0xCC, 0);
}

static inline void* fn_8020341C_resolveCcCtx(void* ctx)
{
    return pokemonGetStatus(ctx, 0, 0xCC, 0);
}

void fightPokemonGetEffortFromPokemon(void* ctx, u32 param1, u32 param2) {
    extern u16 fn_80119ED0();
    extern u8 fn_8011B67C();
    extern u8 fn_80121ADC();
    extern void pokemonGetEffortFromPokemon();
    extern u32 pokemonGetSoubiItemSoubiDataId();
    void* ccData;
    void* ccCtx;
    u8 result;
    u32 value;

    if (ctx == 0) { ccData = 0; } else { ccData = fn_8020341C_resolveCcData(ctx); }
    if (ccData == NULL) { return; }
    if (ctx == 0) { ccCtx = 0; } else { ccCtx = fn_8020341C_resolveCcCtx(ctx); }
    if (ccCtx == NULL) {
        value = 0;
    } else {
        if (fn_80119ED0(0x3D) == 0x7C || fn_80119ED0(0x3D) == 0xC8) {
            void* tmp;
            if (ctx == 0) { tmp = 0; } else { tmp = pokemonGetStatus(ctx, 0, 0xCC, 0); }
            result = fn_80121ADC(tmp, 0x3D);
        } else {
            if (fn_80119ED0(0x3D) != 0xCD) {
                result = 0;
            } else {
                result = fn_8011B67C(ctx, 0x3D);
            }
        }
        if (result == 1) {
            value = 0;
        } else {
            value = pokemonGetSoubiItemSoubiDataId(ccCtx);
        }
    }
    pokemonGetEffortFromPokemon(ccData, value, param1, param2);
}
#pragma pop

/* 0x8020355C | size: 0x60 */
u32 fightPokemonGetLevelToExp(u32 obj, u32 param) {
    extern u32 pokemonGetLevelToExp();
    extern u32 pokemonGetStatus();
    u32 result;
    if (obj == 0) {
        result = 0;
    } else {
        result = pokemonGetStatus(obj, 0, 0xCC, 0);
    }
    if (result == 0) {
        return 0;
    }
    return pokemonGetLevelToExp(result, param);
}

/* 0x802035BC | size: 0x64 */
void figthPokemonSetExp(void* obj, u32 value) {
    void* intermediate;
    if (obj == NULL) {
        intermediate = NULL;
    } else {
        intermediate = pokemonGetStatus(obj, 0, 0xCC, 0);
    }
    if (intermediate != NULL) {
        pokemonSetStatus(intermediate, 0, 0x79, 0, value);
    }
}

/* Forward declarations for converted functions */

/* =========================================================================
 * figthPokemonGetExp
 *
 * Navigate from a trainer context through two data table hops to reach
 * extended Pokemon/trainer data. Same pokemonGetStatus(..., 0xCC/0x79, ...)
 * hop pattern as fightPokemonGetLevelToExp/figthPokemonSetExp above.
 *
 * Hop 1: pokemonGetStatus(ctx, 0, 0xCC, 0) -> intermediate pointer
 * Hop 2: pokemonGetStatus(intermediate, 0, 0x79, 0) -> extended data
 *
 * If either hop returns NULL, the function returns NULL.
 *
 * @param context  Trainer/party context
 * @return         Extended data pointer, or NULL
 * ========================================================================= */
void* figthPokemonGetExp(void* context) {
    void* intermediate;
    if (context == NULL) {
        intermediate = NULL;
    } else {
        intermediate = pokemonGetStatus(context, 0, 0xCC, 0);
    }
    if (intermediate == NULL) {
        return NULL;
    }

    return pokemonGetStatus(intermediate, 0, 0x79, 0);
}

/* =========================================================================
 * fightPokemonGrowBasisStatus
 *
 * Similar two-hop navigation, but the second call writes data via
 * pokemonGrowBasisStatus instead of reading it.
 *
 * @param context  Trainer/party context
 * @param value    Value to write
 * ========================================================================= */
void fightPokemonGrowBasisStatus(void* context, u32 value) {
    void* intermediate;
    if (context == NULL) {
        intermediate = NULL;
    } else {
        intermediate = pokemonGetStatus(context, 0, 0xCC, 0);
    }
    if (intermediate != NULL) {
        pokemonGrowBasisStatus(intermediate, value);
    }
}

/* 0x802036D4 | size: 0x84 */
u32 fightOutPokemonGetVoiceSndId(void* ctx) {
    void* resolved;
    u16 species;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
        if (resolved == NULL) {
            resolved = NULL;
        } else {
            resolved = pokemonGetStatus(resolved, 0, 0xCC, 0);
        }
    }
    species = (u16)(u32)pokemonGetStatus(resolved, 0, 0x6E, 0);
    return (u16)(u32)pokemonGetStatus(NULL, species, 0x61, 0);
}

/* 0x80203758 | size: 0x84 */
u32 fightOutPokemonGetNamePtr(void* ctx) {
    extern u32 GSmsgGetGSchar();
    void* resolved;
    u16 species;
    resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
    if (resolved == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(resolved, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return 0;
    }
    species = (u16)(u32)pokemonGetStatus(resolved, 0, 0x6E, 0);
    resolved = pokemonGetStatus(NULL, species, 0x01, 0);
    return GSmsgGetGSchar(resolved);
}

/* 0x802037DC | size: 0x6C */
void* fightOutPokemonGetNicknamePtr(void* ctx) {
    void* resolved;
    resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
    if (resolved == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(resolved, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return NULL;
    }
    return pokemonGetStatus(resolved, 0, 0x77, 0);
}

/* 0x80203848 | size: 0x5C | small */
u32 fightPokemonGetNicknamePtr(void* param_1) {
    void* iVar1;
    u32 uVar2;

    if (param_1 == NULL) {
        iVar1 = NULL;
    } else {
        iVar1 = pokemonGetStatus(param_1, 0, 0xCC, 0);
    }
    if (iVar1 == NULL) {
        uVar2 = 0;
    } else {
        uVar2 = (u32)pokemonGetStatus(iVar1, 0, 0x77, 0);
    }
    return uVar2;
}

/* Address: 0x802038A4 | Size: 0x1c8 | Ghidra import */

u8 fightOutPokemonIsHinsi(int r3)

{
    extern s8 pokemonCheckValid();
    extern short fn_801EF634();
  u8 bVar1;
  u16 sVar4;
  u32 iVar2;
  u8 cVar5;
  u32 iVar3;
  u8 uVar6;
  
  if (r3 == 0) {
    bVar1 = 0;
  }
  else {
    sVar4 = fn_801EF634();
    if (sVar4 == 1) {
      bVar1 = 0;
    }
    else {
      iVar3 = (int)pokemonGetStatus(r3,0,0xd6,0);
      if (iVar3 == 0) {
        bVar1 = 0;
      }
      else {
        if (iVar3 == 0) {
          bVar1 = 0;
        }
        else {
        sVar4 = fn_801EF634();
        if (sVar4 == 1) {
          bVar1 = 0;
        }
        else {
          iVar2 = (int)pokemonGetStatus(iVar3,0,0xcb,0);
          if (iVar2 == 0) {
            bVar1 = 0;
          }
          else {
            cVar5 = pokemonCheckValid();
            if (cVar5 == 0) {
              bVar1 = 0;
            }
            else {
              if (iVar3 == 0) {
                iVar2 = 0;
              }
              else {
                iVar2 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
              }
              if (iVar2 == 0) {
                bVar1 = 0;
              }
              else {
                cVar5 = pokemonCheckValid();
                if (cVar5 == 0) {
                  bVar1 = 0;
                }
                else {
                  iVar3 = (int)pokemonGetStatus(iVar3,0,0xce,0);
                  if ((int)iVar3 < 0) {
                    bVar1 = 0;
                  }
                  else {
                    bVar1 = 1;
                  }
                }
              }
            }
          }
        }
        }
        if (bVar1 == 0) {
          bVar1 = 0;
        }
        else {
          bVar1 = 1;
        }
      }
    }
  }
  if (bVar1 == 0) {
    uVar6 = 1;
  }
  else {
    if ((u32)r3 == 0) {
      iVar3 = 0;
    }
    else {
      iVar3 = (int)pokemonGetStatus(r3,0,0xd6,0);
      if (iVar3 == 0) {
        iVar3 = 0;
      }
      else {
        iVar3 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
      }
    }
    if (iVar3 == 0) {
      uVar6 = 1;
    }
    else {
      uVar6 = (int)pokemonGetStatus(iVar3,0,0x7b,0);
    }
  }
  return uVar6;
}

/* 0x80203A6C | size: 0x70 */
u32 fightOutPokemonGetNowHpPercentage(void* ctx) {
    extern u32 pokemonGetNowHpPercentage();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
        if (resolved == NULL) {
            resolved = NULL;
        } else {
            resolved = pokemonGetStatus(resolved, 0, 0xCC, 0);
        }
    }
    if (resolved == NULL) {
        return 0;
    }
    return pokemonGetNowHpPercentage(resolved);
}

/* 0x80203ADC | size: 0x80 */
u32 fightOutPokemonNowHpWaruValue(void* ctx, u32 param) {
    extern u32 pokemonGetNowHpWaruValue();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
        if (resolved == NULL) {
            resolved = NULL;
        } else {
            resolved = pokemonGetStatus(resolved, 0, 0xCC, 0);
        }
    }
    if (resolved == NULL) {
        return 0;
    }
    return pokemonGetNowHpWaruValue(resolved, param);
}

/* 0x80203B5C | size: 0x80 */
u32 fightOutPokemonMaxHpWaruValue(void* ctx, u32 param) {
    extern u32 pokemonGetMaxHpWaruValue();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
        if (resolved == NULL) {
            resolved = NULL;
        } else {
            resolved = pokemonGetStatus(resolved, 0, 0xCC, 0);
        }
    }
    if (resolved == NULL) {
        return 0;
    }
    return pokemonGetMaxHpWaruValue(resolved, param);
}

/* 0x80203BDC | size: 0x80 */
u32 fightOutPokemonIsNokoriHpFollowing(void* ctx, u32 param) {
    extern u32 pokemonIsNokoriHpFollowing();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
        if (resolved == NULL) {
            resolved = NULL;
        } else {
            resolved = pokemonGetStatus(resolved, 0, 0xCC, 0);
        }
    }
    if (resolved == NULL) {
        return 0;
    }
    return pokemonIsNokoriHpFollowing(resolved, param);
}

/* 0x80203C5C | size: 0x70 */
u32 fightOutPokemonIsJoutaiKaragenki(void* ctx) {
    extern u32 pokemonIsJoutaiKaragenki();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
        if (resolved == NULL) {
            resolved = NULL;
        } else {
            resolved = pokemonGetStatus(resolved, 0, 0xCC, 0);
        }
    }
    if (resolved == NULL) {
        return 0;
    }
    return pokemonIsJoutaiKaragenki(resolved);
}

/* 0x80203CCC | size: 0x70 */
u32 fightOutPokemonIsJoutaiNormal(void* ctx) {
    extern u32 pokemonIsJoutaiNormal();
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
        if (resolved == NULL) {
            resolved = NULL;
        } else {
            resolved = pokemonGetStatus(resolved, 0, 0xCC, 0);
        }
    }
    if (resolved == NULL) {
        return 0;
    }
    return pokemonIsJoutaiNormal(resolved);
}

/* 0x80203D3C | size: 0x70 */
u16 figthOutPokemonGetPokemonDataId(void* ctx) {
    void* resolved;
    resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
    if (resolved == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(resolved, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return 0;
    }
    return (u16)(u32)pokemonGetStatus(resolved, 0, 0x6E, 0);
}

/* 0x80203DAC | size: 0x60 */
u16 figthPokemonGetPokemonDataId(void* ctx) {
    void* resolved;
    if (ctx == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(ctx, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return 0;
    }
    return (u16)(u32)pokemonGetStatus(resolved, 0, 0x6E, 0);
}

/* 0x80203E0C | size: 0x70 */
u8 figthOutPokemonGetLevel(void* ctx) {
    void* resolved;
    resolved = pokemonGetStatus(ctx, 0, 0xD6, 0);
    if (resolved == NULL) {
        resolved = NULL;
    } else {
        resolved = pokemonGetStatus(resolved, 0, 0xCC, 0);
    }
    if (resolved == NULL) {
        return 0;
    }
    return (u8)(u32)pokemonGetStatus(resolved, 0, 0x7A, 0);
}

/* 0x80203E7C | size: 0x60 */
u32 figthPokemonGetLevel(u32 obj) {
    extern u32 pokemonGetStatus();
    u32 result;
    if (obj == 0) {
        result = 0;
    } else {
        result = pokemonGetStatus(obj, 0, 0xCC, 0);
    }
    if (result == 0) {
        return 0;
    }
    return pokemonGetStatus(result, 0, 0x7A, 0) & 0xFF;
}

/* 0x80203EDC | size: 0x108 */
u16 figthOutPokemonGetSoubiItemBuff(void* ctx) {
    extern u16 fn_80119ED0();
    extern u8 fn_8011B67C();
    extern u8 fn_80121ADC();
    extern u16 pokemonGetSoubiItemBuff();
    void* d6Data;
    void* ccData;
    u16 typeId;
    u8 result;

    d6Data = pokemonGetStatus(ctx, 0, 0xD6, 0);
    ccData = !d6Data ? NULL : pokemonGetStatus(d6Data, 0, 0xCC, 0);
    if (ccData == NULL) { return 0; }
    typeId = fn_80119ED0(0x3D);
    if (typeId == 0x7C || typeId == 0xC8) {
        result = fn_80121ADC(!d6Data ? NULL : pokemonGetStatus(d6Data, 0, 0xCC, 0), 0x3D);
    } else if (fn_80119ED0(0x3D) == 0xCD) {
        result = fn_8011B67C(d6Data, 0x3D);
    } else {
        result = 0;
    }
    if (result == 1) { return 0; }
    return pokemonGetSoubiItemBuff(ccData);
}

/* 0x80203FE4 | size: 0x104 */
u32 fightOutPokemonGetSoubiItemSoubiDataId(void* ctx) {
    extern u16 fn_80119ED0();
    extern u8 fn_8011B67C();
    extern u8 fn_80121ADC();
    extern u32 pokemonGetSoubiItemSoubiDataId();
    void* d6Data;
    void* ccData;
    u16 typeId;
    u8 result;

    d6Data = pokemonGetStatus(ctx, 0, 0xD6, 0);
    ccData = !d6Data ? NULL : pokemonGetStatus(d6Data, 0, 0xCC, 0);
    if (ccData == NULL) { return 0; }
    typeId = fn_80119ED0(0x3D);
    if (typeId == 0x7C || typeId == 0xC8) {
        result = fn_80121ADC(!d6Data ? NULL : pokemonGetStatus(d6Data, 0, 0xCC, 0), 0x3D);
    } else if (fn_80119ED0(0x3D) == 0xCD) {
        result = fn_8011B67C(d6Data, 0x3D);
    } else {
        result = 0;
    }
    if (result == 1) { return 0; }
    return pokemonGetSoubiItemSoubiDataId(ccData);
}

#pragma peephole off
void* fightOutPokemonGetSoubiItemDataId(void) {
    extern void* pokemonGetStatus();
    extern u32 fn_80119ED0();
    extern u32 fn_80121ADC();
    extern void* fn_8011B67C();
    extern void* pokemonGetSoubiItemDataId();
    void* alloc2;
    void* alloc1;
    u32 r0;

    if ((alloc1 = pokemonGetStatus(0, 0, 0xD6, 0)) != 0) {
        alloc2 = pokemonGetStatus(0, 0, 0xCC, 0);
    } else {
        alloc2 = 0;
    }
    if (alloc2 == 0) {
        return 0;
    }
    if ((u16)(u32)fn_80119ED0(0x3D) == 0x7C ||
        (u16)(u32)fn_80119ED0(0x3D) == 0xC8) {
        r0 = fn_80121ADC(alloc1 ? pokemonGetStatus(alloc1, 0, 0xCC, 0) : 0, 0x3D);
    } else {
        r0 = fn_80119ED0(0x3D);
        if ((u16)r0 != 0xCD) {
            return 0;
        }
        r0 = (u32)fn_8011B67C(alloc1, 0x3D);
    }
    if ((u8)r0 == 1) {
        return 0;
    }
    return pokemonGetSoubiItemDataId(alloc2);
}

/* 0x802041EC | size: 0xF4 | medium */
u32 fightPokemonGetSoubiItemSoubiDataId(void* param_1) {
    extern s16 fn_80119ED0(u32);
    extern s8 fn_8011B67C(void*, u32);
    extern s8 fn_80121ADC(void*, u32);
    extern u32 pokemonGetSoubiItemSoubiDataId(void*);
    u32 uVar1;
    s16 sVar2;
    s8 cVar3;
    void* iVar4;

    if (param_1 == NULL) {
        iVar4 = NULL;
    } else {
        iVar4 = pokemonGetStatus(param_1, 0, 0xCC, 0);
    }
    if (iVar4 == NULL) {
        uVar1 = 0;
    } else {
        sVar2 = fn_80119ED0(0x3D);
        if ((sVar2 == 0x7C) || (sVar2 = fn_80119ED0(0x3D), sVar2 == 200)) {
            if (param_1 == NULL) {
                uVar1 = 0;
            } else {
                uVar1 = (u32)pokemonGetStatus(param_1, 0, 0xCC, 0);
            }
            cVar3 = fn_80121ADC((void*)uVar1, 0x3D);
        } else {
            sVar2 = fn_80119ED0(0x3D);
            if (sVar2 == 0xCD) {
                cVar3 = fn_8011B67C(param_1, 0x3D);
            } else {
                cVar3 = 0;
            }
        }
        if (cVar3 == 1) {
            uVar1 = 0;
        } else {
            uVar1 = pokemonGetSoubiItemSoubiDataId(iVar4);
        }
    }
    return uVar1;
}

/* 0x802042E0 | size: 0xF4 | medium */
u32 fightPokemonGetSoubiItemDataId(void* param_1) {
    extern s16 fn_80119ED0(u32);
    extern s8 fn_8011B67C(void*, u32);
    extern s8 fn_80121ADC(void*, u32);
    extern u32 pokemonGetSoubiItemDataId(void*);
    u32 uVar1;
    s16 sVar2;
    s8 cVar3;
    void* iVar4;

    if (param_1 == NULL) {
        iVar4 = NULL;
    } else {
        iVar4 = pokemonGetStatus(param_1, 0, 0xCC, 0);
    }
    if (iVar4 == NULL) {
        uVar1 = 0;
    } else {
        sVar2 = fn_80119ED0(0x3D);
        if ((sVar2 == 0x7C) || (sVar2 = fn_80119ED0(0x3D), sVar2 == 200)) {
            if (param_1 == NULL) {
                uVar1 = 0;
            } else {
                uVar1 = (u32)pokemonGetStatus(param_1, 0, 0xCC, 0);
            }
            cVar3 = fn_80121ADC((void*)uVar1, 0x3D);
        } else {
            sVar2 = fn_80119ED0(0x3D);
            if (sVar2 == 0xCD) {
                cVar3 = fn_8011B67C(param_1, 0x3D);
            } else {
                cVar3 = 0;
            }
        }
        if (cVar3 == 1) {
            uVar1 = 0;
        } else {
            uVar1 = pokemonGetSoubiItemDataId(iVar4);
        }
    }
    return uVar1;
}

/* Address: 0x802043D4 | Size: 0x480 | Ghidra import */
u32 fightOutPokemonGetNowNimbleness(void)

{
    int r3;
    char r4;
    char r5;
    u32 r6;
    int r7;

    extern short fn_80119ED0();
    extern s8 fn_8011B67C();
    extern s8 fn_80121ADC();
    extern u32 pokemonGetSoubiItemBuff();
    extern short pokemonGetSoubiItemSoubiDataId();
    extern s8 heroGetStatus();
    extern int fightAbicntDoKakeWaru();
  u32 uVar1;
  short sVar4;
  int iVar2;
  short sVar5;
  short sVar6;
  u8 uVar7;
  s8 cVar8;
  u32 uVar3;
  int iVar9;
  u32 uVar10;
  int iVar11;
  
  if (r3 == 0) {
    iVar11 = 0;
  }
  else {
    iVar11 = (int)pokemonGetStatus(r3,0,0xd6,0);
    if (iVar11 == 0) {
      iVar11 = 0;
    }
    else {
      iVar11 = (int)pokemonGetStatus(iVar11,0,0xcc,0);
    }
  }
  if (iVar11 == 0) {
    uVar1 = 0;
  }
  else {
    sVar4 = (int)pokemonGetStatus(r3,0,0x100,0);
    iVar2 = (int)pokemonGetStatus(r3,0,0xd6,0);
    if (iVar2 == 0) {
      iVar9 = 0;
    }
    else {
      iVar9 = (int)pokemonGetStatus(iVar2,0,0xcc,0);
    }
    if (iVar9 == 0) {
      sVar5 = 0;
    }
    else {
      sVar5 = fn_80119ED0(0x3d);
      if ((sVar5 == 0x7c) || (sVar5 = fn_80119ED0(0x3d), sVar5 == 200)) {
        if (iVar2 == 0) {
          uVar3 = 0;
        }
        else {
          uVar3 = (int)pokemonGetStatus(iVar2,0,0xcc,0);
        }
        cVar8 = fn_80121ADC(uVar3,0x3d);
      }
      else {
        sVar5 = fn_80119ED0(0x3d);
        if (sVar5 == 0xcd) {
          cVar8 = fn_8011B67C(iVar2,0x3d);
        }
        else {
          cVar8 = 0;
        }
      }
      if (cVar8 == 1) {
        sVar5 = 0;
      }
      else {
        sVar5 = pokemonGetSoubiItemSoubiDataId(iVar9);
      }
    }
    iVar2 = (int)pokemonGetStatus(r3,0,0xd6,0);
    if (iVar2 == 0) {
      iVar9 = 0;
    }
    else {
      iVar9 = (int)pokemonGetStatus(iVar2,0,0xcc,0);
    }
    if (iVar9 == 0) {
      uVar10 = 0;
    }
    else {
      sVar6 = fn_80119ED0(0x3d);
      if ((sVar6 == 0x7c) || (sVar6 = fn_80119ED0(0x3d), sVar6 == 200)) {
        if (iVar2 == 0) {
          uVar3 = 0;
        }
        else {
          uVar3 = (int)pokemonGetStatus(iVar2,0,0xcc,0);
        }
        cVar8 = fn_80121ADC(uVar3,0x3d);
      }
      else {
        sVar6 = fn_80119ED0(0x3d);
        if (sVar6 == 0xcd) {
          cVar8 = fn_8011B67C(iVar2,0x3d);
        }
        else {
          cVar8 = 0;
        }
      }
      if (cVar8 == 1) {
        uVar10 = 0;
      }
      else {
        uVar10 = pokemonGetSoubiItemBuff(iVar9);
        uVar10 = uVar10 & 0xffff;
      }
    }
    uVar7 = (int)pokemonGetStatus(r3,0,0xea,0);
    if (r7 == 0) {
      cVar8 = 0;
    }
    else {
      cVar8 = heroGetStatus(r7,0x11,0);
    }
    uVar1 = (int)pokemonGetStatus(iVar11,0,0x8c,0);
    uVar1 = uVar1 & 0xffff;
    if ((sVar4 == 0x21) && (r5 == 2)) {
      uVar1 = uVar1 << 1;
    }
    else if ((sVar4 == 0x22) && (r5 == 1)) {
      uVar1 = uVar1 << 1;
    }
    uVar1 = fightAbicntDoKakeWaru(uVar7,uVar1);
    if ((r4 == 1) && (cVar8 == 1)) {
      uVar1 = (uVar1 * 0x6e) / 100;
    }
    if (sVar5 == 0x18) {
      uVar1 = uVar1 >> 1;
    }
    sVar4 = fn_80119ED0(5);
    if (((sVar4 == 0x7c) || (sVar4 = fn_80119ED0(5), sVar4 == 200)) ||
       (sVar4 = fn_80119ED0(5), sVar4 == 0xcd)) {
      iVar11 = (int)pokemonGetStatus(r3,0,0xd6,0);
      sVar4 = fn_80119ED0(5);
      if ((sVar4 == 0x7c) || (sVar4 = fn_80119ED0(5), sVar4 == 200)) {
        if (iVar11 == 0) {
          uVar3 = 0;
        }
        else {
          uVar3 = (int)pokemonGetStatus(iVar11,0,0xcc,0);
        }
        cVar8 = fn_80121ADC(uVar3,5);
      }
      else {
        sVar4 = fn_80119ED0(5);
        if (sVar4 == 0xcd) {
          cVar8 = fn_8011B67C(iVar11,5);
        }
        else {
          cVar8 = 0;
        }
      }
    }
    else {
      sVar4 = fn_80119ED0(5);
      if (sVar4 == 0xd8) {
        cVar8 = fn_8011B67C(r3,5);
      }
      else {
        cVar8 = 0;
      }
    }
    if (cVar8 == 1) {
      uVar1 = uVar1 >> 2;
    }
    if ((sVar5 == 0x1a) && ((int)(r6 & 0xffff) < (int)(uVar10 * 0xffff) / 100)) {
      uVar1 = 0xffffffff;
    }
  }
  return uVar1;
}

/* 0x80204854 | size: 0xD4 | medium */
#pragma push
#pragma peephole on
#pragma scheduling on
u32 fightOutPokemonCheckIrekaeReserveFightPokemon(void* param_1, void* param_2) {
    extern u8 fightActionCheckValid(void*);
    extern s16 fightActionBiosGetBuffDataId(void*);
    extern u16 fightActionBiosGetKind(void*);
    void* iVar2;
    s16 sVar3;
    u32 uVar1;
    s16 sVar4;
    u8 cVar5;

    sVar3 = (s16)(u32)pokemonGetStatus(param_2, 0, 0xCE, 0);
    if (sVar3 < 0) {
        uVar1 = 0;
    } else {
        sVar4 = (s16)(u32)pokemonGetStatus(param_1, 0, 0x121, 0);
        if (sVar3 == sVar4) {
            uVar1 = 1;
        } else {
            iVar2 = pokemonGetStatus(param_1, 0, 0xFE, 0);
            if ((((iVar2 != NULL) && (cVar5 = fightActionCheckValid(iVar2), cVar5 == 1)) &&
                ((u16)fightActionBiosGetKind(iVar2) == 9)) &&
               (sVar4 = fightActionBiosGetBuffDataId(iVar2), sVar3 == sVar4)) {
                uVar1 = 1;
            } else {
                uVar1 = 0;
            }
        }
    }
    return uVar1;
}
#pragma pop

/* 0x80204928 | size: 0x48 | small */
#pragma push
#pragma peephole on
u8 fightPokemonCheckMotoFightPokemon(u32 expected, void* ctx) {
    u32 result = (u32)pokemonGetStatus(ctx, 0, 0xd5, 0);
    return (result == expected) ? 1 : 0;
}
#pragma pop

/* Address: 0x80204970 | Size: 0xa0 | Ghidra import */
void fn_80204970(void)

{
    int r3;
    int r4;

  u32 *puVar1;
  u32 *puVar2;
  u32 uVar3;
  u32 *puVar4;
  u32 *puVar5;
  int iVar6;
  u32 uStack_15c;
  u32 local_158 [86];
  
  if ((r3 != 0) && (r4 != 0)) {
    iVar6 = 0x2a;
    puVar1 = (u32 *)(r3 + -4);
    puVar2 = &uStack_15c;
    do {
      puVar5 = puVar2;
      puVar4 = puVar1;
      uVar3 = puVar4[2];
      puVar5[1] = puVar4[1];
      puVar5[2] = uVar3;
      iVar6 = iVar6 + -1;
      puVar1 = puVar4 + 2;
      puVar2 = puVar5 + 2;
    } while (iVar6 != 0);
    puVar5[3] = puVar4[3];
    iVar6 = 0x2a;
    puVar1 = (u32 *)(r4 + -4);
    puVar2 = (u32 *)(r3 + -4);
    do {
      puVar5 = puVar2;
      puVar4 = puVar1;
      uVar3 = puVar4[2];
      puVar5[1] = puVar4[1];
      puVar5[2] = uVar3;
      iVar6 = iVar6 + -1;
      puVar1 = puVar4 + 2;
      puVar2 = puVar5 + 2;
    } while (iVar6 != 0);
    puVar5[3] = puVar4[3];
    iVar6 = 0x2a;
    puVar1 = &uStack_15c;
    puVar2 = (u32 *)(r4 + -4);
    do {
      puVar5 = puVar2;
      puVar4 = puVar1;
      uVar3 = puVar4[2];
      puVar5[1] = puVar4[1];
      puVar5[2] = uVar3;
      iVar6 = iVar6 + -1;
      puVar1 = puVar4 + 2;
      puVar2 = puVar5 + 2;
    } while (iVar6 != 0);
    puVar5[3] = puVar4[3];
  }
  return;
}

/* fightOutPokemonIsGcHeroFightOutPokemon | Size: 0x4C | Check if trainer slot is active */
#pragma push
#pragma peephole on
u8 fightOutPokemonIsGcHeroFightOutPokemon(u32 slotId) {
    extern void* fightFloorGetFightOutPokemonPtrToFightTrainerPtr(u32 context, u32 slot);
    extern u8 fightTrainerIsGcHero(void* trainer);
    void* trainer = fightFloorGetFightOutPokemonPtrToFightTrainerPtr(0, slotId);
    if (trainer == NULL) {
        return 0;
    }
    return fightTrainerIsGcHero(trainer) == 1;
}
#pragma pop

/* 0x80204A5C | size: 0x1AC | medium */
/* 0x80204A5C | size: 0x1AC */
#pragma push
#pragma peephole on
u32 fightOutPokemonIsFightActionUseItemKind(void* ctx, u8 targetSlot, u8 mode) {
    extern u32 lbl_80478BD8;
    extern u8 fn_80142984();
    extern u16 fightActionGetKindDataId();
    extern u8 fightActionCheckValid();
    extern void fightFloorGetStatus();
    u16 field1E;
    u16 field1F;
    int e5Data;
    int feData;
    u8 valid;
    u32 i;

    for (i = 0; (u16)i < lbl_80478BD8; i++) {
        if ((u8)fn_80142984(i) == 0) { continue; }
        if (mode == 1) {
            if (targetSlot != (u8)itemGetStatus(0, i, 0x2, 0)) { continue; }
        } else {
            if (targetSlot == (u8)itemGetStatus(0, i, 0x2, 0)) { continue; }
        }
        fightFloorGetStatus(0, 0, 0x14, 0);
        if (ctx == NULL) { valid = 0; }
        else {
            feData = (int)pokemonGetStatus(ctx, 0, 0xFE, 0);
            if (feData == 0) { valid = 0; }
            else if ((u8)fightActionCheckValid(feData) == 0) { valid = 0; }
            else if (fightActionGetKindDataId(feData) != 0x12) { valid = 0; }
            else {
                e5Data = (int)pokemonGetStatus(ctx, 0, 0xE5, 0);
                if (e5Data == 0) { valid = 0; }
                else {
                    field1E = (u16)itemGetStatus(e5Data, 0, 0x1E, 0);
                    field1F = (u16)itemGetStatus(e5Data, 0, 0x1F, 0);
                    if ((u16)i != 0 && field1E != (u16)i) {
                        valid = 0;
                    } else {
                        valid = 1;
                    }
                }
            }
        }
        if (valid == 1) { return 1; }
    }
    return 0;
}
#pragma pop

/* Address: 0x80204C08 | Size: 0xd8 | Ghidra import */
#pragma push
#pragma peephole on
#pragma scheduling on
u16 fightOutPokemonGetFightActionUseItemDataId(void* r3)

{
    extern u16 fightActionGetKindDataId();
    extern u8 fightActionCheckValid();
    extern void fightFloorGetStatus();
  u32 iVar1;
  u8 cVar4;
  u16 sVar2;
  u16 uVar3;

  fightFloorGetStatus(0,0,0x14,0);
  if (r3 == 0) {
    uVar3 = 0;
  }
  else {
    iVar1 = (u32)pokemonGetStatus(r3,0,0xfe,0);
    if (iVar1 == 0) {
      uVar3 = 0;
    }
    else {
      cVar4 = fightActionCheckValid();
      if (cVar4 == 0) {
        uVar3 = 0;
      }
      else {
        sVar2 = fightActionGetKindDataId(iVar1);
        if (sVar2 != 0x12) {
          uVar3 = 0;
        }
        else {
          iVar1 = (u32)pokemonGetStatus(r3,0,0xe5,0);
          if (iVar1 == 0) {
            uVar3 = 0;
          }
          else {
            uVar3 = itemGetStatus(iVar1,0,0x1e,0);
          }
        }
      }
    }
  }
  return uVar3;
}
#pragma pop

/* 0x80204CE0 | size: 0x104 */
void* fightOutPokemonCreateFightActionUseItem(void* ctx, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7, u32 p8, u8 p9) {
    extern void fn_80142B24();
    extern u8 fightActionCreate();
    extern void fightItemCreate();
    extern void fightActionBiosSetBuffDataId();
    void* e5Data;
    void* feData;

    e5Data = pokemonGetStatus(ctx, 0, 0xE5, 0);
    if (e5Data == NULL) { return NULL; }
    fightItemCreate(e5Data, (u16)p6, p7, p8);
    fn_80142B24(e5Data, 0, 0x21, 0, (u32)p9);
    feData = pokemonGetStatus(ctx, 0, 0xFE, 0);
    if (feData == NULL) { feData = NULL; }
    else {
        if ((u8)fightActionCreate(feData, p2, ctx, p3, p4, p5) == 1) {
            fightActionBiosSetBuffDataId(feData, p6);
        } else {
            feData = NULL;
        }
    }
    return feData;
}

/* 0x80204DE4 | size: 0x188 */
#pragma push
#pragma peephole on
u32 fightOutPokemonIsFightActionAttackWazaOut(void* ctx, u16 slotId, void* tablePtr) {
    extern u16 wazaGetStatus();
    extern void* fightTargetGetTragetPtrToRelativeHostSideFightTargetId();
    extern void* fightTargetGetPtrAsNowFightType();
    extern u16 fightActionGetKindDataId();
    extern u8 fightActionCheckValid();
    extern u16 fightFloorGetStatus();
    void* feData;
    void* d9Data;
    u16 partyCount;
    void* savedEntry;
    u16 field27;
    u16 field09;
    u32 field29;

    partyCount = (u16)fightFloorGetStatus(0, 0, 0x14, 0);
    if (ctx == NULL) { return 0; }
    savedEntry = !tablePtr ? NULL : fightTargetGetTragetPtrToRelativeHostSideFightTargetId(tablePtr, partyCount);
    feData = pokemonGetStatus(ctx, 0, 0xFE, 0);
    if (feData == NULL) { return 0; }
    if ((u8)fightActionCheckValid(feData) == 0) { return 0; }
    if (fightActionGetKindDataId(feData) != 0x13) { return 0; }
    d9Data = pokemonGetStatus(ctx, 0, 0xD9, 0);
    if (d9Data == NULL) { return 0; }
    field27 = (u16)wazaGetStatus(d9Data, 0, 0x27, 0);
    field09 = (u16)wazaGetStatus(0, field27, 0x9, 0);
    if (slotId != 0 && field27 != slotId) { return 0; }
    field29 = (u32)wazaGetStatus(d9Data, 0, 0x29, 0);
    if (field09 == 0xB0) {
        field29 = (u32)fightTargetGetTragetPtrToRelativeHostSideFightTargetId(fightTargetGetPtrAsNowFightType(0xE, ctx), partyCount);
    }
    if ((u16)(u32)savedEntry != 0 && (u16)field29 != (u16)(u32)savedEntry) { return 0; }
    return 1;
}
#pragma pop

/* Address: 0x80204F6C | Size: 0xf0 | Ghidra import */
int fightOutPokemonCreateFightActionAttackWaza(void)

{
    u32 r3;
    u32 r4;
    u32 r5;
    u32 r6;
    u32 r7;
    u32 r8;
    u32 r9;
    u32 r10;
    u8 param_9;

    extern s8 fightActionCreate();
    extern void fightWazaCreate();
    extern void fightActionBiosSetBuffDataId();
  int iVar1;
  s8 cVar2;
  
  iVar1 = (int)pokemonGetStatus(r3,0,0xd9,0);
  if (iVar1 == 0) {
    iVar1 = 0;
  }
  else {
    fightWazaCreate(iVar1,r10,r8 & 0xffff,r9,param_9);
    iVar1 = (int)pokemonGetStatus(r3,0,0xfe,0);
    if (iVar1 == 0) {
      iVar1 = 0;
    }
    else {
      cVar2 = fightActionCreate(iVar1,r4,r3,r5,r6,r7);
      if (cVar2 == 1) {
        fightActionBiosSetBuffDataId(iVar1,r8);
      }
      else {
        iVar1 = 0;
      }
    }
    if (iVar1 == 0) {
      iVar1 = 0;
    }
  }
  return iVar1;
}

/* Address: 0x8020505C | Size: 0x98 | Ghidra import */
#pragma push
#pragma peephole on
#pragma scheduling on
int fightOutPokemonCreateFightAction(void* r3, u32 r4, u32 r5, u32 r6, u32 r7, u32 r8)

{
    extern u8 fightActionCreate();
    extern void fightActionBiosSetBuffDataId();
  int iVar1;
  u8 cVar2;

  iVar1 = (int)pokemonGetStatus(r3,0,0xfe,0);
  if (iVar1 == 0) {
    return 0;
  }
  cVar2 = fightActionCreate(iVar1,r4,r3,r5,r6,r7);
  if (cVar2 == 1) {
    fightActionBiosSetBuffDataId(iVar1,r8);
    return iVar1;
  }
  return 0;
}
#pragma pop

#pragma push
#pragma peephole on
#if 0
asm void fightOutPokemonGetFightActionPri(void) {
#include "src/game/colosseum_event_fn_802050F4.inc"
}
#else
void* fightOutPokemonGetFightActionPri(void* ctx) {
    void* p;
    p = pokemonGetStatus(ctx, 0, 0xFE, 0);
    if (p == NULL) {
        p = (void*)-0x80;
    } else {
        p = fightActionGetPri(p);
    }
    return p;
}
#endif
#pragma pop

/* fightOutPokemonGetWazaZokuseiDataId | Size: 0x50 | Get field 0x30 from resolved 0xD9, default 9 */
#pragma push
#pragma peephole on
u16 fightOutPokemonGetWazaZokuseiDataId(void* ctx) {
    extern u32 wazaGetStatus();
    void* resolved = pokemonGetStatus(ctx, 0, 0xD9, 0);
    if (resolved == NULL) {
        return 9;
    }
    return (u16)wazaGetStatus(resolved, 0, 0x30, 0);
}

u16 fightOutPokemonGetUseWazaDataId(void* ctx) {
    extern void* pokemonGetStatus();
    extern u32 wazaGetStatus();
    void* resolved;
    resolved = pokemonGetStatus(ctx, 0, 0xD9, 0);
    if (resolved == 0) {
        return 0;
    }
    return (u16)wazaGetStatus(resolved, 0, 0x28, 0);
}

/* fightOutPokemonGetCmpNimblenessWazaDataId | Size: 0x50 | Get field 0x27 from resolved 0xD9, default 0 */
u16 fightOutPokemonGetCmpNimblenessWazaDataId(void* ctx) {
    extern u32 wazaGetStatus();
    void* resolved = pokemonGetStatus(ctx, 0, 0xD9, 0);
    if (resolved == NULL) {
        return 0;
    }
    return (u16)wazaGetStatus(resolved, 0, 0x27, 0);
}

/* fightOutPokemonGetMotoWazaDataId | Size: 0x50 | Get field 0x27 from resolved 0xD9, default 0 */
u16 fightOutPokemonGetMotoWazaDataId(void* ctx) {
    extern u32 wazaGetStatus();
    void* resolved = pokemonGetStatus(ctx, 0, 0xD9, 0);
    if (resolved == NULL) {
        return 0;
    }
    return (u16)wazaGetStatus(resolved, 0, 0x27, 0);
}
#pragma pop

/* Address: 0x80205274 | Size: 0x690 | Ghidra import */

void fightOutPokemonSetMeetEnemyFightPokemonEnemySideAll(int r3,u32 r4)

{
    extern s8 pokemonCheckFightOut();
    extern s8 pokemonCheckValid();
    extern short fn_801EF634();
    extern u32 fightFloorGetStatus();
    extern u32 fightSideGetStatus();
    extern s8 fightTrainerCheckValid();
    extern int fightTrainerGetStatus();
  u32 bVar1;
  u32 uVar2;
  u32 uVar3;
  u32 uVar4;
  u8 cVar10;
  int iVar5;
  int iVar6;
  int iVar7;
  short sVar8;
  short sVar9;
  u8 bVar11;
  u32 uVar12;
  u32 uVar13;
  
  fightFloorGetStatus(0,0,0x14,0);
  uVar2 = fightFloorGetStatus(0,0,0x16,0);
  uVar3 = fightFloorGetStatus(0,0,0x18,0);
  uVar12 = 0;
  do {
    if ((uVar2 & 0xffff) <= (uVar12 & 0xffff)) {
      return;
    }
    uVar4 = fightSideGetStatus(r4,0,7,uVar12);
    cVar10 = fightTrainerCheckValid();
    if (cVar10 != 0) {
      for (uVar13 = 0; (uVar13 & 0xffff) < (uVar3 & 0xffff); uVar13 = uVar13 + 1) {
        iVar5 = fightTrainerGetStatus(uVar4,0,0x46,uVar13);
        if (iVar5 == 0) {
          bVar1 = 0;
        }
        else {
          sVar8 = fn_801EF634();
          if (sVar8 == 1) {
            bVar1 = 0;
          }
          else {
            iVar7 = (int)pokemonGetStatus(iVar5,0,0xd6,0);
            if (iVar7 == 0) {
              bVar1 = 0;
            }
            else {
              sVar8 = fn_801EF634();
              if (sVar8 == 1) {
                bVar1 = 0;
              }
              else {
                iVar6 = (int)pokemonGetStatus(iVar7,0,0xcb,0);
                if (iVar6 == 0) {
                  bVar1 = 0;
                }
                else {
                  cVar10 = pokemonCheckValid();
                  if (cVar10 == 0) {
                    bVar1 = 0;
                  }
                  else {
                    if (iVar7 == 0) {
                      iVar6 = 0;
                    }
                    else {
                      iVar6 = (int)pokemonGetStatus(iVar7,0,0xcc,0);
                    }
                    if (iVar6 == 0) {
                      bVar1 = 0;
                    }
                    else {
                      cVar10 = pokemonCheckValid();
                      if (cVar10 == 0) {
                        bVar1 = 0;
                      }
                      else {
                        iVar7 = (int)pokemonGetStatus(iVar7,0,0xce,0);
                        if (iVar7 < 0) {
                          bVar1 = 0;
                        }
                        else {
                          bVar1 = 1;
                        }
                      }
                    }
                  }
                }
              }
              if (bVar1) {
                bVar1 = 1;
              }
              else {
                bVar1 = 0;
              }
            }
          }
          if (bVar1) {
            iVar7 = (int)pokemonGetStatus(iVar5,0,0x120,0);
            if (iVar7 == 1) {
              bVar1 = 0;
            }
            else {
              iVar7 = (int)pokemonGetStatus(iVar5,0,0xd6,0);
              if (iVar7 == 0) {
                bVar1 = 0;
              }
              else {
                sVar8 = fn_801EF634();
                if (sVar8 == 1) {
                  bVar1 = 0;
                }
                else {
                  iVar6 = (int)pokemonGetStatus(iVar7,0,0xcb,0);
                  if (iVar6 == 0) {
                    bVar1 = 0;
                  }
                  else {
                    cVar10 = pokemonCheckValid();
                    if (cVar10 == 0) {
                      bVar1 = 0;
                    }
                    else {
                      if (iVar7 == 0) {
                        iVar6 = 0;
                      }
                      else {
                        iVar6 = (int)pokemonGetStatus(iVar7,0,0xcc,0);
                      }
                      if (iVar6 == 0) {
                        bVar1 = 0;
                      }
                      else {
                        cVar10 = pokemonCheckValid();
                        if (cVar10 == 0) {
                          bVar1 = 0;
                        }
                        else {
                          iVar6 = (int)pokemonGetStatus(iVar7,0,0xce,0);
                          if (iVar6 < 0) {
                            bVar1 = 0;
                          }
                          else {
                            bVar1 = 1;
                          }
                        }
                      }
                    }
                  }
                }
                if (bVar1) {
                  iVar6 = (int)pokemonGetStatus(iVar7,0,0xd2,0);
                  if (iVar6 == 1) {
                    bVar1 = 0;
                  }
                  else {
                    if (iVar7 == 0) {
                      iVar7 = 0;
                    }
                    else {
                      iVar7 = (int)pokemonGetStatus(iVar7,0,0xcc,0);
                    }
                    if (iVar7 == 0) {
                      bVar1 = 0;
                    }
                    else {
                      cVar10 = pokemonCheckFightOut();
                      if (cVar10 == 0) {
                        bVar1 = 0;
                      }
                      else {
                        bVar1 = 1;
                      }
                    }
                  }
                }
                else {
                  bVar1 = 0;
                }
              }
              if (bVar1) {
                bVar1 = 1;
              }
              else {
                bVar1 = 0;
              }
            }
          }
          else {
            bVar1 = 0;
          }
        }
        if ((bVar1) && (iVar5 = (int)pokemonGetStatus(iVar5,0,0xd5,0), r3 != 0)) {
          if (iVar5 == 0) {
            bVar1 = 0;
          }
          else {
            sVar8 = fn_801EF634();
            if (sVar8 == 1) {
              bVar1 = 0;
            }
            else {
              iVar7 = (int)pokemonGetStatus(iVar5,0,0xcb,0);
              if (iVar7 == 0) {
                bVar1 = 0;
              }
              else {
                cVar10 = pokemonCheckValid();
                if (cVar10 == 0) {
                  bVar1 = 0;
                }
                else {
                  if (iVar5 == 0) {
                    iVar7 = 0;
                  }
                  else {
                    iVar7 = (int)pokemonGetStatus(iVar5,0,0xcc,0);
                  }
                  if (iVar7 == 0) {
                    bVar1 = 0;
                  }
                  else {
                    cVar10 = pokemonCheckValid();
                    if (cVar10 == 0) {
                      bVar1 = 0;
                    }
                    else {
                      iVar7 = (int)pokemonGetStatus(iVar5,0,0xce,0);
                      if (iVar7 < 0) {
                        bVar1 = 0;
                      }
                      else {
                        bVar1 = 1;
                      }
                    }
                  }
                }
              }
            }
          }
          if (bVar1) {
            if (r3 == 0) {
              bVar1 = 0;
            }
            else {
              if (iVar5 == 0) {
                bVar1 = 0;
              }
              else {
                sVar8 = fn_801EF634();
                if (sVar8 == 1) {
                  bVar1 = 0;
                }
                else {
                  iVar7 = (int)pokemonGetStatus(iVar5,0,0xcb,0);
                  if (iVar7 == 0) {
                    bVar1 = 0;
                  }
                  else {
                    cVar10 = pokemonCheckValid();
                    if (cVar10 == 0) {
                      bVar1 = 0;
                    }
                    else {
                      if (iVar5 == 0) {
                        iVar7 = 0;
                      }
                      else {
                        iVar7 = (int)pokemonGetStatus(iVar5,0,0xcc,0);
                      }
                      if (iVar7 == 0) {
                        bVar1 = 0;
                      }
                      else {
                        cVar10 = pokemonCheckValid();
                        if (cVar10 == 0) {
                          bVar1 = 0;
                        }
                        else {
                          iVar7 = (int)pokemonGetStatus(iVar5,0,0xce,0);
                          if (iVar7 < 0) {
                            bVar1 = 0;
                          }
                          else {
                            bVar1 = 1;
                          }
                        }
                      }
                    }
                  }
                }
              }
              if (bVar1) {
                sVar8 = (int)pokemonGetStatus(iVar5,0,0xce,0);
                for (bVar11 = 0; bVar11 < 0xc; bVar11 = bVar11 + 1) {
                  sVar9 = (int)pokemonGetStatus(r3,0,0xfd,bVar11);
                  if ((-1 < sVar9) && (sVar9 == sVar8)) {
                    bVar1 = 1;
                    goto LAB_00202858;
                  }
                }
                bVar1 = 0;
              }
              else {
                bVar1 = 0;
              }
            }
LAB_00202858:
            if (bVar1 == 0) {
              sVar8 = (int)pokemonGetStatus(iVar5,0,0xce,0);
              for (bVar11 = 0; bVar11 < 0xc; bVar11 = bVar11 + 1) {
                sVar9 = (int)pokemonGetStatus(r3,0,0xfd,bVar11);
                if (sVar9 < 0) {
                  pokemonSetStatus(r3,0,0xfd,bVar11,(int)sVar8);
                  break;
                }
              }
            }
          }
        }
      }
    }
    uVar12 = uVar12 + 1;
  } while (1);
}

/* Address: 0x80205904 | Size: 0x178 | Ghidra import */
u32 fightOutPokemonCheckMeetEnemyFightPokemon(void)

{
    int r3;
    int r4;

    extern s8 pokemonCheckValid();
    extern short fn_801EF634();
  u32 bVar1;
  int iVar2;
  s8 cVar5;
  short sVar3;
  short sVar4;
  u8 bVar6;
  
  if (r3 != 0) {
    if (r4 == 0) {
      bVar1 = 0;
    }
    else {
      sVar3 = fn_801EF634();
      if (sVar3 == 1) {
        bVar1 = 0;
      }
      else {
        iVar2 = (int)pokemonGetStatus(r4,0,0xcb,0);
        if (iVar2 == 0) {
          bVar1 = 0;
        }
        else {
          cVar5 = pokemonCheckValid();
          if (cVar5 == 0) {
            bVar1 = 0;
          }
          else {
            if (r4 == 0) {
              iVar2 = 0;
            }
            else {
              iVar2 = (int)pokemonGetStatus(r4,0,0xcc,0);
            }
            if (iVar2 == 0) {
              bVar1 = 0;
            }
            else {
              cVar5 = pokemonCheckValid();
              if (cVar5 == 0) {
                bVar1 = 0;
              }
              else {
                iVar2 = (int)pokemonGetStatus(r4,0,0xce,0);
                if (iVar2 < 0) {
                  bVar1 = 0;
                }
                else {
                  bVar1 = 1;
                }
              }
            }
          }
        }
      }
    }
    if (bVar1) {
      sVar3 = (int)pokemonGetStatus(r4,0,0xce,0);
      for (bVar6 = 0; bVar6 < 0xc; bVar6 = bVar6 + 1) {
        sVar4 = (int)pokemonGetStatus(r3,0,0xfd,bVar6);
        if ((-1 < sVar4) && (sVar4 == sVar3)) {
          return 1;
        }
      }
    }
  }
  return 0;
}

/* fightOutPokemonSetOnDarkPokemonFlag | Size: 0x58 | Two-hop resolve and call pokemonSetOnDarkPokemonFlag */
#pragma push
#pragma peephole on
void fightOutPokemonSetOnDarkPokemonFlag(void* ctx, u32 param) {
    extern void pokemonSetOnDarkPokemonFlag(void* obj, u32 param);
    if (ctx == NULL) {
        return;
    }
    ctx = pokemonGetStatus(ctx, 0, 0xD5, 0);
    ctx = pokemonGetStatus(ctx, 0, 0xCB, 0);
    pokemonSetOnDarkPokemonFlag(ctx, param);
}
#pragma pop

/* fightOutPokemonSetOnZukanFlag | Size: 0x58 | Two-hop resolve and call pokemonSetOnZukanFlag */
#pragma push
#pragma peephole on
void fightOutPokemonSetOnZukanFlag(void* ctx, u32 param) {
    extern void pokemonSetOnZukanFlag(void* obj, u32 param);
    if (ctx == NULL) {
        return;
    }
    ctx = pokemonGetStatus(ctx, 0, 0xD5, 0);
    ctx = pokemonGetStatus(ctx, 0, 0xCB, 0);
    pokemonSetOnZukanFlag(ctx, param);
}
#pragma pop

/* fightOutPokemonGetFightEntryId | Size: 0x60 | Two-hop resolve (0xD5 -> 0xCE), return s16 or -1 */
#pragma push
#pragma peephole on
s16 fightOutPokemonGetFightEntryId(void* ctx) {
    void* hop1;
    if (ctx == NULL) {
        return -1;
    }
    hop1 = pokemonGetStatus(ctx, 0, 0xD5, 0);
    if (hop1 == NULL) {
        return -1;
    }
    return (s16)(u32)pokemonGetStatus(hop1, 0, 0xCE, 0);
}
#pragma pop

#pragma push
#pragma peephole on
void* fightOutPokemonGetPokemonPtr(void* ctx) {
    extern void* pokemonGetStatus();
    if (ctx == 0) {
        return 0;
    }
    ctx = pokemonGetStatus(ctx, 0, 0xD6, 0);
    if (ctx == 0) {
        return 0;
    }
    return pokemonGetStatus(ctx, 0, 0xCC, 0);
}
#pragma pop

/* 0x80205BE8 | size: 0x3C | small */
#pragma push
#pragma peephole on
void* fightPokemonGetPokemonPtr(void* ctx) {
    if (ctx == 0) return 0;
    return pokemonGetStatus(ctx, 0, 0xcc, 0);
}
#pragma pop

/* Address: 0x80205C24 | Size: 0x684 | Ghidra import */

u32 fightOutPokemonCheckFightActionSelect(int r3,char r4)

{
    extern short fn_80119ED0();
    extern s8 fn_8011B67C();
    extern u32 wazaGetStatus();
    extern s8 fn_80121ADC();
    extern s8 pokemonCheckFightOut();
    extern s8 pokemonCheckValid();
    extern short fn_801EF634();
    extern u32 fightTargetGetTragetPtrToRelativeHostSideFightTargetId();
    extern s8 fightActionCreate();
    extern u32 fightFloorGetStatus();
    extern void fightWazaCreate();
    extern int fightWazaCheckValid();
    extern void fightActionBiosSetBuffDataId();
    extern u32 fn_8022B2CC();
  u32 bVar1;
  u16 uVar5;
  int iVar2;
  short sVar6;
  int iVar3;
  u32 uVar4;
  s8 cVar8;
  u16 uVar7;
  
  uVar5 = fightFloorGetStatus(0,0,0x14,0);
  if (r3 != 0) {
    sVar6 = fn_801EF634();
    if (sVar6 == 1) {
      bVar1 = 0;
    }
    else {
      iVar3 = (int)pokemonGetStatus(r3,0,0xd6,0);
      if (iVar3 == 0) {
        bVar1 = 0;
      }
      else {
        sVar6 = fn_801EF634();
        if (sVar6 == 1) {
          bVar1 = 0;
        }
        else {
          iVar2 = (int)pokemonGetStatus(iVar3,0,0xcb,0);
          if (iVar2 == 0) {
            bVar1 = 0;
          }
          else {
            cVar8 = pokemonCheckValid();
            if (cVar8 == 0) {
              bVar1 = 0;
            }
            else {
              if (iVar3 == 0) {
                iVar2 = 0;
              }
              else {
                iVar2 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
              }
              if (iVar2 == 0) {
                bVar1 = 0;
              }
              else {
                cVar8 = pokemonCheckValid();
                if (cVar8 == 0) {
                  bVar1 = 0;
                }
                else {
                  iVar3 = (int)pokemonGetStatus(iVar3,0,0xce,0);
                  if (iVar3 < 0) {
                    bVar1 = 0;
                  }
                  else {
                    bVar1 = 1;
                  }
                }
              }
            }
          }
        }
        if (bVar1) {
          bVar1 = 1;
        }
        else {
          bVar1 = 0;
        }
      }
    }
    if (bVar1) {
      iVar3 = (int)pokemonGetStatus(r3,0,0x120,0);
      if (iVar3 == 1) {
        bVar1 = 0;
      }
      else {
        iVar3 = (int)pokemonGetStatus(r3,0,0xd6,0);
        if (iVar3 == 0) {
          bVar1 = 0;
        }
        else {
          sVar6 = fn_801EF634();
          if (sVar6 == 1) {
            bVar1 = 0;
          }
          else {
            iVar2 = (int)pokemonGetStatus(iVar3,0,0xcb,0);
            if (iVar2 == 0) {
              bVar1 = 0;
            }
            else {
              cVar8 = pokemonCheckValid();
              if (cVar8 == 0) {
                bVar1 = 0;
              }
              else {
                if (iVar3 == 0) {
                  iVar2 = 0;
                }
                else {
                  iVar2 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
                }
                if (iVar2 == 0) {
                  bVar1 = 0;
                }
                else {
                  cVar8 = pokemonCheckValid();
                  if (cVar8 == 0) {
                    bVar1 = 0;
                  }
                  else {
                    iVar2 = (int)pokemonGetStatus(iVar3,0,0xce,0);
                    if (iVar2 < 0) {
                      bVar1 = 0;
                    }
                    else {
                      bVar1 = 1;
                    }
                  }
                }
              }
            }
          }
          if (bVar1) {
            iVar2 = (int)pokemonGetStatus(iVar3,0,0xd2,0);
            if (iVar2 == 1) {
              bVar1 = 0;
            }
            else {
              if (iVar3 == 0) {
                iVar3 = 0;
              }
              else {
                iVar3 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
              }
              if (iVar3 == 0) {
                bVar1 = 0;
              }
              else {
                cVar8 = pokemonCheckFightOut();
                if (cVar8 == 0) {
                  bVar1 = 0;
                }
                else {
                  bVar1 = 1;
                }
              }
            }
          }
          else {
            bVar1 = 0;
          }
        }
        if (bVar1) {
          bVar1 = 1;
        }
        else {
          bVar1 = 0;
        }
      }
    }
    else {
      bVar1 = 0;
    }
    if (bVar1) {
      sVar6 = fn_80119ED0(0x12);
      if (((sVar6 == 0x7c) || (sVar6 = fn_80119ED0(0x12), sVar6 == 200)) ||
         (sVar6 = fn_80119ED0(0x12), sVar6 == 0xcd)) {
        iVar3 = (int)pokemonGetStatus(r3,0,0xd6,0);
        sVar6 = fn_80119ED0(0x12);
        if ((sVar6 == 0x7c) || (sVar6 = fn_80119ED0(0x12), sVar6 == 200)) {
          if (iVar3 == 0) {
            uVar4 = 0;
          }
          else {
            uVar4 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
          }
          cVar8 = fn_80121ADC(uVar4,0x12);
        }
        else {
          sVar6 = fn_80119ED0(0x12);
          if (sVar6 == 0xcd) {
            cVar8 = fn_8011B67C(iVar3,0x12);
          }
          else {
            cVar8 = 0;
          }
        }
      }
      else {
        sVar6 = fn_80119ED0(0x12);
        if (sVar6 == 0xd8) {
          cVar8 = fn_8011B67C(r3,0x12);
        }
        else {
          cVar8 = 0;
        }
      }
      if (cVar8 != 1) {
        sVar6 = fn_80119ED0(0x22);
        if (((sVar6 == 0x7c) || (sVar6 = fn_80119ED0(0x22), sVar6 == 200)) ||
           (sVar6 = fn_80119ED0(0x22), sVar6 == 0xcd)) {
          iVar3 = (int)pokemonGetStatus(r3,0,0xd6,0);
          sVar6 = fn_80119ED0(0x22);
          if ((sVar6 == 0x7c) || (sVar6 = fn_80119ED0(0x22), sVar6 == 200)) {
            if (iVar3 == 0) {
              uVar4 = 0;
            }
            else {
              uVar4 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
            }
            cVar8 = fn_80121ADC(uVar4,0x22);
          }
          else {
            sVar6 = fn_80119ED0(0x22);
            if (sVar6 == 0xcd) {
              cVar8 = fn_8011B67C(iVar3,0x22);
            }
            else {
              cVar8 = 0;
            }
          }
        }
        else {
          sVar6 = fn_80119ED0(0x22);
          if (sVar6 == 0xd8) {
            cVar8 = fn_8011B67C(r3,0x22);
          }
          else {
            cVar8 = 0;
          }
        }
        if (cVar8 != 1) {
          return 1;
        }
      }
      if (r4 != 0) {
        uVar4 = (int)pokemonGetStatus(r3,0,0xf8,0);
        cVar8 = fightWazaCheckValid();
        if (cVar8 != 0) {
          uVar7 = wazaGetStatus(uVar4,0,0x28,0);
          cVar8 = wazaGetStatus(uVar4,0,0x26,0);
          uVar4 = fn_8022B2CC(r3,uVar7,uVar5,0x802062a8,1,0, (void*)0xffffffff);
          uVar4 = fightTargetGetTragetPtrToRelativeHostSideFightTargetId(uVar4,uVar5);
          iVar3 = (int)pokemonGetStatus(r3,0,0xd9,0);
          if (iVar3 != 0) {
            fightWazaCreate(iVar3,(int)cVar8,uVar7,uVar4,1);
            iVar3 = (int)pokemonGetStatus(r3,0,0xfe,0);
            if ((iVar3 != 0) &&
               (cVar8 = fightActionCreate(iVar3,0,r3,0x13,0,0x80375ca8), cVar8 == 1)) {
              fightActionBiosSetBuffDataId(iVar3,uVar7);
            }
          }
        }
      }
    }
  }
  return 0;
}

/* 0x802062A8 | size: 0x54 | small */
#pragma push
#pragma peephole on
void _fightOutPokemonCheckFightActionSelectSub__FP15FightOutPokemonUsUs(void* param_1, u32 param_2, u32 param_3) {
    extern u32 wazaGetStatus();
    extern void fightTargetGetRelativeHostSideFightTargetIdToTragetPtr();
    void* uVar1;
    u16 uVar2;

    uVar1 = pokemonGetStatus(param_1, 0, 0xF8, 0);
    uVar2 = (u16)wazaGetStatus(uVar1, 0, 0x29, 0);
    fightTargetGetRelativeHostSideFightTargetIdToTragetPtr(uVar2, param_3);
}
#pragma pop

/* Address: 0x802062FC | Size: 0x30c | Ghidra import */

u8 fightOutPokemonCheckFightOut(int r3)

{
    extern s8 pokemonCheckFightOut();
    extern s8 pokemonCheckValid();
    extern short fn_801EF634();
  u32 bVar1;
  int iVar2;
  short sVar4;
  int iVar3;
  s8 cVar5;
  u8 uVar6;
  
  if (r3 == 0) {
    uVar6 = 0;
  }
  else {
    sVar4 = fn_801EF634();
    if (sVar4 == 1) {
      bVar1 = 0;
    }
    else {
      iVar2 = (int)pokemonGetStatus(r3,0,0xd6,0);
      if (iVar2 == 0) {
        bVar1 = 0;
      }
      else {
        sVar4 = fn_801EF634();
        if (sVar4 == 1) {
          bVar1 = 0;
        }
        else {
          iVar3 = (int)pokemonGetStatus(iVar2,0,0xcb,0);
          if (iVar3 == 0) {
            bVar1 = 0;
          }
          else {
            cVar5 = pokemonCheckValid();
            if (cVar5 == 0) {
              bVar1 = 0;
            }
            else {
              if (iVar2 == 0) {
                iVar3 = 0;
              }
              else {
                iVar3 = (int)pokemonGetStatus(iVar2,0,0xcc,0);
              }
              if (iVar3 == 0) {
                bVar1 = 0;
              }
              else {
                cVar5 = pokemonCheckValid();
                if (cVar5 == 0) {
                  bVar1 = 0;
                }
                else {
                  iVar2 = (int)pokemonGetStatus(iVar2,0,0xce,0);
                  if (iVar2 < 0) {
                    bVar1 = 0;
                  }
                  else {
                    bVar1 = 1;
                  }
                }
              }
            }
          }
        }
        if (bVar1) {
          bVar1 = 1;
        }
        else {
          bVar1 = 0;
        }
      }
    }
    if (bVar1) {
      iVar2 = (int)pokemonGetStatus(r3,0,0x120,0);
      if (iVar2 == 1) {
        uVar6 = 0;
      }
      else {
        iVar2 = (int)pokemonGetStatus(r3,0,0xd6,0);
        if (iVar2 == 0) {
          uVar6 = 0;
        }
        else {
          sVar4 = fn_801EF634();
          if (sVar4 == 1) {
            bVar1 = 0;
          }
          else {
            iVar3 = (int)pokemonGetStatus(iVar2,0,0xcb,0);
            if (iVar3 == 0) {
              bVar1 = 0;
            }
            else {
              cVar5 = pokemonCheckValid();
              if (cVar5 == 0) {
                bVar1 = 0;
              }
              else {
                if (iVar2 == 0) {
                  iVar3 = 0;
                }
                else {
                  iVar3 = (int)pokemonGetStatus(iVar2,0,0xcc,0);
                }
                if (iVar3 == 0) {
                  bVar1 = 0;
                }
                else {
                  cVar5 = pokemonCheckValid();
                  if (cVar5 == 0) {
                    bVar1 = 0;
                  }
                  else {
                    iVar3 = (int)pokemonGetStatus(iVar2,0,0xce,0);
                    if (iVar3 < 0) {
                      bVar1 = 0;
                    }
                    else {
                      bVar1 = 1;
                    }
                  }
                }
              }
            }
          }
          if (bVar1) {
            iVar3 = (int)pokemonGetStatus(iVar2,0,0xd2,0);
            if (iVar3 == 1) {
              uVar6 = 0;
            }
            else {
              if (iVar2 == 0) {
                iVar2 = 0;
              }
              else {
                iVar2 = (int)pokemonGetStatus(iVar2,0,0xcc,0);
              }
              if (iVar2 == 0) {
                uVar6 = 0;
              }
              else {
                cVar5 = pokemonCheckFightOut();
                if (cVar5 == 0) {
                  uVar6 = 0;
                }
                else {
                  uVar6 = 1;
                }
              }
            }
          }
          else {
            uVar6 = 0;
          }
        }
      }
    }
    else {
      uVar6 = 0;
    }
  }
  return uVar6;
}

/* Address: 0x80206608 | Size: 0x178 | Ghidra import */
#pragma push
#pragma peephole on
u32 fightPokemonCheckFightOut(void* r3)

{
    extern u8 pokemonCheckFightOut(void*);
    extern u8 pokemonCheckValid();
    extern u16 fn_801EF634();
  u16 sVar2;
  void* iVar1;
  u8 cVar3;
  u8 bVar4;
  
  if (r3 == 0) {
    return 0;
  }
  if (r3 == 0) {
    bVar4 = 0;
  }
  else {
    sVar2 = fn_801EF634();
    if (sVar2 == 1) {
      bVar4 = 0;
    }
    else {
      iVar1 = pokemonGetStatus(r3,0,0xcb,0);
      if (iVar1 == 0) {
        bVar4 = 0;
      }
      else {
        cVar3 = pokemonCheckValid();
        if (cVar3 == 0) {
          bVar4 = 0;
        }
        else {
          if (r3 == 0) {
            iVar1 = 0;
          }
          else {
            iVar1 = pokemonGetStatus(r3,0,0xcc,0);
          }
          if (iVar1 == 0) {
            bVar4 = 0;
          }
          else {
            cVar3 = pokemonCheckValid();
            if (cVar3 == 0) {
              bVar4 = 0;
            }
            else {
              iVar1 = pokemonGetStatus(r3,0,0xce,0);
              if ((s32)iVar1 < 0) {
                bVar4 = 0;
              }
              else {
                bVar4 = 1;
              }
            }
          }
        }
      }
    }
  }
  if (bVar4 == 0) {
    return 0;
  }
  iVar1 = pokemonGetStatus(r3,0,0xd2,0);
  if ((s32)iVar1 == 1) {
    return 0;
  }
  if (r3 == 0) {
    iVar1 = 0;
  }
  else {
    iVar1 = pokemonGetStatus(r3,0,0xcc,0);
  }
  if (iVar1 == 0) {
    return 0;
  }
  return (u8)pokemonCheckFightOut(iVar1) != 0;
}
#pragma pop

/* Address: 0x80206780 | Size: 0x148 | Ghidra import */
#pragma push
#pragma peephole on
u8 fightOutPokemonCheckValid(void* p1) {
    extern u8 pokemonCheckValid();
    extern u16 fn_801EF634();
    u16 sVar3;
    void* iVar1;
    void* iVar2;
    u8 cVar4;
    u8 uVar5;

    if (p1 == 0) {
        uVar5 = 0;
    } else {
        sVar3 = fn_801EF634();
        if (sVar3 == 1) {
            uVar5 = 0;
        } else {
            iVar1 = pokemonGetStatus(p1, 0, 0xd6, 0);
            if (iVar1 == 0) {
                uVar5 = 0;
            } else {
                sVar3 = fn_801EF634();
                if (sVar3 == 1) {
                    uVar5 = 0;
                } else {
                    iVar2 = pokemonGetStatus(iVar1, 0, 0xcb, 0);
                    if (iVar2 == 0) {
                        uVar5 = 0;
                    } else {
                        cVar4 = pokemonCheckValid();
                        if (cVar4 == 0) {
                            uVar5 = 0;
                        } else {
                            if (iVar1 == 0) {
                                iVar2 = 0;
                            } else {
                                iVar2 = pokemonGetStatus(iVar1, 0, 0xcc, 0);
                            }
                            if (iVar2 == 0) {
                                uVar5 = 0;
                            } else {
                                cVar4 = pokemonCheckValid();
                                if (cVar4 == 0) {
                                    uVar5 = 0;
                                } else {
                                    iVar1 = pokemonGetStatus(iVar1, 0, 0xce, 0);
                                    if ((s32)iVar1 < 0) {
                                        uVar5 = 0;
                                    } else {
                                        uVar5 = 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return uVar5;
}
#pragma pop

/* Address: 0x802068C8 | Size: 0x13c | Ghidra import */

void fightOutPokemonCreate(int r3,int r4,int r5)

{
    extern u32 pokemonGetTokuseiDataId();
    extern void fightFloorSetShadow();
    extern void fightOutPokemonInit();
  u32 uVar1;
  u16 uVar2;
  u16 uVar3;
  u32 uVar4;
  
  if ((r3 != 0) && (r4 != 0)) {
    uVar1 = (int)pokemonGetStatus(r4,0,0xcc,0);
    fightOutPokemonInit(r3);
    pokemonSetStatus(r3,0,0xd5,0,r4);
    pokemonSetStatus(r3,0,0xd6,0,r4);
    if (r5 != 0) {
      pokemonSetStatus(r3,0,0xee,0,r5);
      fightFloorSetShadow();
    }
    uVar2 = (int)pokemonGetStatus(uVar1,0,0x6e,0);
    for (uVar4 = 0; (uVar4 & 0xffff) < 2; uVar4 = uVar4 + 1) {
      uVar3 = (int)pokemonGetStatus(0,uVar2,0x16,uVar4);
      pokemonSetStatus(r3,0,0xff,uVar4 & 0xff,uVar3);
    }
    uVar2 = pokemonGetTokuseiDataId(uVar1);
    pokemonSetStatus(r3,0,0x100,0,uVar2);
  }
  return;
}

/* Address: 0x80206A04 | Size: 0xe8 | Ghidra import */
#pragma push
#pragma peephole on
u32 fightPokemonCheckValid(void* ctx) {
    extern u8 pokemonCheckValid();
    extern u16 fn_801EF634();
    u32 uVar1;
    u16 sVar3;
    void* iVar2;
    u8 cVar4;
    if (ctx == 0) { return 0; }
    sVar3 = fn_801EF634();
    if (sVar3 == 1) { return 0; }
    iVar2 = pokemonGetStatus(ctx, 0, 0xcb, 0);
    if (iVar2 == 0) { return 0; }
    cVar4 = pokemonCheckValid();
    if (cVar4 == 0) { return 0; }
    if (ctx == 0) {
        iVar2 = 0;
    } else {
        iVar2 = pokemonGetStatus(ctx, 0, 0xcc, 0);
    }
    if (iVar2 == 0) { return 0; }
    cVar4 = pokemonCheckValid();
    if (cVar4 == 0) { return 0; }
    uVar1 = (u32)pokemonGetStatus(ctx, 0, 0xce, 0);
    return uVar1 >> 0x1f ^ 1;
}
#pragma pop

/* Address: 0x80206AEC | Size: 0x150 | Ghidra import */
#pragma push
#pragma peephole on
void fightPokemonCreate(void* p1, void* p2, s16 p3) {
    extern void fn_8011B950();
    extern void pokemonBiosCopy();
    extern void pokemonInit();
    u32 uVar1;

    if (p1 != 0 && p2 != 0) {
        if (p1 != 0) {
            pokemonSetStatus(p1, 0, 0xcb, 0, 0);
            pokemonGetStatus(p1, 0, 0xcc, 0);
            pokemonInit();
            uVar1 = (u32)pokemonGetStatus(p1, 0, 0xcd, 0);
            fn_8011B950(uVar1, 1);
            pokemonSetStatus(p1, 0, 0xce, 0, (void*)0xffffffff);
            pokemonSetStatus(p1, 0, 0xcf, 0, 0);
            pokemonSetStatus(p1, 0, 0xd0, 0, 0);
            pokemonSetStatus(p1, 0, 0xd1, 0, 0);
            pokemonSetStatus(p1, 0, 0xd2, 0, 0);
        }
        pokemonSetStatus(p1, 0, 0xcb, 0, p2);
        uVar1 = (u32)pokemonGetStatus(p1, 0, 0xcc, 0);
        pokemonBiosCopy(uVar1, p2);
        pokemonSetStatus(p1, 0, 0xce, 0, (s32)p3);
    }
}
#pragma pop

/* 0x80206C3C | size: 0x58 | small */
#pragma push
#pragma peephole on
void fightOutPokemonInitAry(u32 param_1, u16 param_2) {
    extern void fightOutPokemonInit(u32);
    u32 n;
    int iVar1;

    if (param_1 != 0) {
        iVar1 = 0;
        n = param_2;
        for (; (u16)iVar1 < n; iVar1 = iVar1 + 1) {
            fightOutPokemonInit(param_1 + (u32)(u16)iVar1 * 0x6E0);
        }
    }
}
#pragma pop

/* Address: 0x80206C94 | Size: 0x72c | Ghidra import */
#pragma push
#pragma peephole on
#pragma optimization_level 2
void fightOutPokemonInit(int r3)

{
    extern StatusIdTable7 lbl_80279C60;
    extern void fn_8011B950();
    extern void pokemonInit();
    extern void fightActionInit();
    extern void fn_801FD830();
    extern void fightWazaInit();
    extern void fightItemInit();
    extern void fightOutPokemonEnemyInitAry();
  u32 uVar2;
  u32 *puVar3;
  u16 *puVar4;
  u8 bVar4;
  void* iVar1;
  StatusIdTable7 local_28;

  if (r3) {
    pokemonSetStatus(r3,0,0xd5,0,0);
    pokemonSetStatus(r3,0,0xd6,0,0);
    if ((iVar1 = pokemonGetStatus(r3,0,0xd7,0)) != NULL) {
      pokemonSetStatus(iVar1,0,0xcb,0,0);
      pokemonGetStatus(iVar1,0,0xcc,0);
      pokemonInit();
      uVar2 = (u32)pokemonGetStatus(iVar1,0,0xcd,0);
      fn_8011B950(uVar2,1);
      pokemonSetStatus(iVar1,0,0xce,0, (void*)0xffffffff);
      pokemonSetStatus(iVar1,0,0xcf,0,0);
      pokemonSetStatus(iVar1,0,0xd0,0,0);
      pokemonSetStatus(iVar1,0,0xd1,0,0);
      pokemonSetStatus(iVar1,0,0xd2,0,0);
    }
    uVar2 = (int)pokemonGetStatus(r3,0,0xd8,0);
    fn_8011B950(uVar2,0x34);
    local_28 = lbl_80279C60;
    puVar4 = local_28.id;
    for (bVar4 = 0; bVar4 < 7; bVar4++) {
      pokemonSetStatus(r3,0,puVar4[bVar4],0,6);
    }
    fn_801FD830(r3,0);
    pokemonSetStatus(r3,0,0xed,0,2);
    pokemonSetStatus(r3,0,0xee,0,0);
    for (bVar4 = 0; bVar4 < 0xc; bVar4++) {
      pokemonSetStatus(r3,0,0xfd,bVar4, (void*)0xffffffff);
    }
    iVar1 = pokemonGetStatus(r3,0,0xfe,0);
    if (iVar1 != NULL) {
      fightActionInit();
      pokemonGetStatus(r3,0,0xd9,0);
      fightWazaInit();
      pokemonGetStatus(r3,0,0xe5,0);
      fightItemInit();
    }
    pokemonGetStatus(r3,0,0xf8,0);
    fightWazaInit();
    for (bVar4 = 0; bVar4 < 2; bVar4++) {
      pokemonSetStatus(r3,0,0xff,bVar4,9);
    }
    pokemonSetStatus(r3,0,0x100,0,0);
    puVar3 = (u32 *)pokemonGetStatus(r3,0,0x101,0);
    if (puVar3 != (void *)0) {
      *puVar3 = 0;
    }
    pokemonSetStatus(r3,0,0xef,0,0);
    pokemonSetStatus(r3,0,0xf0,0,0);
    pokemonSetStatus(r3,0,0xf1,0,0);
    pokemonSetStatus(r3,0,0xf2,0,0);
    pokemonSetStatus(r3,0,0xf3,0,0);
    pokemonSetStatus(r3,0,0xf4,0,9);
    pokemonSetStatus(r3,0,0xf5,0,0);
    pokemonSetStatus(r3,0,0xf6,0,0);
    pokemonSetStatus(r3,0,0xf7,0,0);
    pokemonSetStatus(r3,0,0xf9,0,0);
    pokemonSetStatus(r3,0,0xfc,0,0);
    pokemonSetStatus(r3,0,0xfb,0,0);
    pokemonSetStatus(r3,0,0x102,0,0);
    pokemonSetStatus(r3,0,0x103,0,0);
    pokemonSetStatus(r3,0,0x104,0,0);
    pokemonSetStatus(r3,0,0x105,0,0);
    pokemonSetStatus(r3,0,0x106,0,0);
    pokemonSetStatus(r3,0,0x107,0,0);
    pokemonSetStatus(r3,0,0x108,0,0);
    pokemonSetStatus(r3,0,0x109,0,0);
    pokemonSetStatus(r3,0,0x10a,0,0);
    pokemonSetStatus(r3,0,0x10b,0,0);
    pokemonSetStatus(r3,0,0x10c,0,0);
    pokemonSetStatus(r3,0,0x10d,0,0);
    pokemonSetStatus(r3,0,0x10e,0,0);
    pokemonSetStatus(r3,0,0x10f,0,0);
    pokemonSetStatus(r3,0,0x110,0,0);
    pokemonSetStatus(r3,0,0x111,0,0);
    pokemonSetStatus(r3,0,0x112,0,0);
    pokemonSetStatus(r3,0,0x113,0,0);
    pokemonSetStatus(r3,0,0x114,0,0);
    pokemonSetStatus(r3,0,0x115,0,0);
    pokemonSetStatus(r3,0,0x116,0,0);
    pokemonSetStatus(r3,0,0x117,0,0);
    pokemonSetStatus(r3,0,0x118,0,0);
    pokemonSetStatus(r3,0,0x119,0,0);
    pokemonSetStatus(r3,0,0x11a,0,0);
    pokemonSetStatus(r3,0,0x11b,0,0);
    pokemonSetStatus(r3,0,0x11c,0,0);
    pokemonSetStatus(r3,0,0x11d,0,0);
    pokemonSetStatus(r3,0,0x11e,0,0);
    pokemonSetStatus(r3,0,0x11f,0,0);
    pokemonSetStatus(r3,0,0x120,0,0);
    pokemonSetStatus(r3,0,0x121,0, (void*)0xffffffff);
    uVar2 = (int)pokemonGetStatus(r3,0,0x122,0);
    fightOutPokemonEnemyInitAry(uVar2,4);
  }
  return;
}
#pragma pop

/* Address: 0x802073C0 | Size: 0x88 | Ghidra import */
#pragma push
#pragma peephole on
#pragma scheduling on
void fightOutPokemonInitAbiCntAll(u32 r3)

{
    extern StatusIdTable7 lbl_80279C60;
  u8 bVar1;
  StatusIdTable7 local_28;

  local_28 = lbl_80279C60;
  for (bVar1 = 0; bVar1 < 7; bVar1++) {
    pokemonSetStatus(r3,0,local_28.id[bVar1],0,6);
  }
  return;
}
#pragma pop

/* 0x80207448 | size: 0x15C | medium */
#pragma push
#pragma peephole on
void fightOutPokemonInitOneSelfTurn(void* param_1) {
    pokemonSetStatus(param_1, 0, 0x113, 0, 0);
    pokemonSetStatus(param_1, 0, 0x114, 0, 0);
    pokemonSetStatus(param_1, 0, 0x115, 0, 0);
    pokemonSetStatus(param_1, 0, 0x116, 0, 0);
    pokemonSetStatus(param_1, 0, 0x117, 0, 0);
    pokemonSetStatus(param_1, 0, 0x118, 0, 0);
    pokemonSetStatus(param_1, 0, 0x119, 0, 0);
    pokemonSetStatus(param_1, 0, 0x11A, 0, 0);
    pokemonSetStatus(param_1, 0, 0x11B, 0, 0);
    pokemonSetStatus(param_1, 0, 0x11C, 0, 0);
    pokemonSetStatus(param_1, 0, 0x11D, 0, 0);
    pokemonSetStatus(param_1, 0, 0x11E, 0, 0);
    pokemonSetStatus(param_1, 0, 0x11F, 0, 0);
}
#pragma pop

/* 0x802075A4 | size: 0x1BC | medium */
#pragma push
#pragma peephole on
void fightOutPokemonInitOneTurn(void* param_1) {
    pokemonSetStatus(param_1, 0, 0x102, 0, 0);
    pokemonSetStatus(param_1, 0, 0x103, 0, 0);
    pokemonSetStatus(param_1, 0, 0x104, 0, 0);
    pokemonSetStatus(param_1, 0, 0x105, 0, 0);
    pokemonSetStatus(param_1, 0, 0x106, 0, 0);
    pokemonSetStatus(param_1, 0, 0x107, 0, 0);
    pokemonSetStatus(param_1, 0, 0x108, 0, 0);
    pokemonSetStatus(param_1, 0, 0x109, 0, 0);
    pokemonSetStatus(param_1, 0, 0x10A, 0, 0);
    pokemonSetStatus(param_1, 0, 0x10B, 0, 0);
    pokemonSetStatus(param_1, 0, 0x10C, 0, 0);
    pokemonSetStatus(param_1, 0, 0x10D, 0, 0);
    pokemonSetStatus(param_1, 0, 0x10E, 0, 0);
    pokemonSetStatus(param_1, 0, 0x10F, 0, 0);
    pokemonSetStatus(param_1, 0, 0x110, 0, 0);
    pokemonSetStatus(param_1, 0, 0x111, 0, 0);
    pokemonSetStatus(param_1, 0, 0x112, 0, 0);
}
#pragma pop

/* 0x80207760 | size: 0x74 | small */
#pragma push
#pragma peephole on
#pragma scheduling on
void fightOutPokemonInitFightActionBuff(void* param_1) {
    extern void fightActionInit(void*);
    extern void fightWazaInit(void*);
    extern void fightItemInit(void*);
    void* iVar1;

    iVar1 = pokemonGetStatus(param_1, 0, 0xFE, 0);
    if (iVar1 != NULL) {
        fightActionInit(iVar1);
        fightWazaInit(pokemonGetStatus(param_1, 0, 0xD9, 0));
        fightItemInit(pokemonGetStatus(param_1, 0, 0xE5, 0));
    }
}
#pragma pop

/* 0x802077D4 | size: 0x11C */
#pragma push
#pragma peephole on
void fightPokemonInitAry(void* basePtr, u16 count) {
    extern void fn_8011B950();
    extern void pokemonInit();
    void* entry;
    u16 i;

    if (basePtr == NULL) { return; }
    for (i = 0; i < count; i++) {
        entry = (void*)((u32)basePtr + i * 0x154);
        if (entry == NULL) { continue; }
        pokemonSetStatus(entry, 0, 0xCB, 0, 0);
        pokemonInit(pokemonGetStatus(entry, 0, 0xCC, 0));
        fn_8011B950(pokemonGetStatus(entry, 0, 0xCD, 0), 1);
        pokemonSetStatus(entry, 0, 0xCE, 0, (u32)-1);
        pokemonSetStatus(entry, 0, 0xCF, 0, 0);
        pokemonSetStatus(entry, 0, 0xD0, 0, 0);
        pokemonSetStatus(entry, 0, 0xD1, 0, 0);
        pokemonSetStatus(entry, 0, 0xD2, 0, 0);
    }
}
#pragma pop

/* Address: 0x802078F0 | Size: 0xec | Ghidra import */
#pragma push
#pragma peephole on
void fightPokemonInit(void* r3)
{
    extern void fn_8011B950();
    extern void pokemonInit();
    void* ctx;
    u32 uVar1;

    if ((ctx = r3) != NULL) {
        pokemonSetStatus(ctx, 0, 0xcb, 0, 0);
        pokemonGetStatus(ctx, 0, 0xcc, 0);
        pokemonInit();
        uVar1 = (u32)pokemonGetStatus(ctx, 0, 0xcd, 0);
        fn_8011B950(uVar1, 1);
        pokemonSetStatus(ctx, 0, 0xce, 0, (void*)0xffffffff);
        pokemonSetStatus(ctx, 0, 0xcf, 0, 0);
        pokemonSetStatus(ctx, 0, 0xd0, 0, 0);
        pokemonSetStatus(ctx, 0, 0xd1, 0, 0);
        pokemonSetStatus(ctx, 0, 0xd2, 0, 0);
    }
}
#pragma pop

/* 0x802079DC | size: 0x104 */
#pragma push
#pragma peephole on
u32 fightOutPokemonGetTeikouZokuseiDataIdAry(void* ctx, void* battleCtx, u32* outSlots) {
    extern u16 zokuseiGetWazaJoutai(void*, u16);
    u16 i;
    int outCount;
    u16 slot0;
    u16 slot1;
    u16 result;
    u8 isPlayerSlot;

    for (i = 0; i < 0x12; i++) {
        outSlots[i] = (u32)-1;
    }
    outCount = 0;
    for (i = 0; i < 0x12; i++) {
        if (i == (u16)(u32)pokemonGetStatus(ctx, 0, 0xFF, 0)) {
            goto _set1;
        }
        if (i == (u16)(u32)pokemonGetStatus(ctx, 0, 0xFF, 1)) {
        _set1:
            isPlayerSlot = 1;
        } else {
            isPlayerSlot = 0;
        }
        if (isPlayerSlot == 1) { continue; }
        result = zokuseiGetWazaJoutai(battleCtx, i);
        if (result == 0x42 || result == 0x43) {
            outSlots[(u16)outCount] = i;
            outCount++;
        }
    }
    return outCount;
}
#pragma pop

/* Address: 0x80207AE0 | Size: 0x7c | Ghidra import */
#pragma push
#pragma peephole on
#pragma scheduling on
u32 fightOutPokemonIsZokuseiDataId(void* r3, u16 r4)

{
  u16 sVar2;
  u32 uVar1;

  sVar2 = (u16)(u32)pokemonGetStatus(r3,0,0xff,0);
  if ((r4 == sVar2) || (sVar2 = (u16)(u32)pokemonGetStatus(r3,0,0xff,1), r4 == sVar2)) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}
#pragma pop

/* 0x80207B5C | size: 0x30 */
#pragma scheduling on
#pragma peephole on
u32 fightOutPokemonSetZokuseiDataId(void* context, u8 flags, u16 value) {
    return pokemonSetStatus(context, 0, 0xFF, flags, value);
}
#pragma peephole reset
#pragma scheduling reset

/* 0x80207B8C | size: 0x34 */
#pragma push
#pragma scheduling on
#pragma peephole on
u16 fightOutPokemonGetZokuseiDataId(void* context, u8 field) {
    return (u16)(u32)pokemonGetStatus(context, 0, 0xFF, field);
}
#pragma pop

/* 0x80207BC0 | size: 0x34 */
#pragma push
#pragma scheduling on
#pragma peephole on
u32 fightOutPokemonSetTokuseiDataId(void* context, u16 value) {
    return pokemonSetStatus(context, 0, 0x100, 0, value);
}
#pragma pop

/* 0x80207C24 | size: 0x48 | small */
#pragma push
#pragma scheduling on
#pragma peephole on
void fightOutPokemonSetWazaEffectDownFlag(void* ctx, u32 param) {
    extern void fn_801DA5AC();
    void* obj = pokemonGetStatus(ctx, 0, 0xee, 0);
    if (obj != 0) {
        fn_801DA5AC(obj, param);
    }
}
#pragma pop

/* Address: 0x80207C6C | Size: 0x2f0 | Ghidra import */
u32 fightOutPokemonCreateSequence(void)

{
    int r3;
    u16 r4;

    extern short fn_80119ED0();
    extern s8 fn_8011B67C();
    extern void pokemonBiosCopy();
    extern s8 fn_80121ADC();
    extern void pokemonSetSequenceStatus();
    extern u32 pokemonCheckRare();
    extern u32 fn_801DE190();
    extern void fightOutPokemonGetRndStatus();
  u32 uVar1;
  short sVar5;
  u32 uVar2;
  int iVar3;
  s8 cVar6;
  u32 uVar4;
  u8 uVar7;
  u32 local_158;
  u32 local_154;
  u8 auStack_150 [320];
  
  if (r3 == 0) {
    uVar1 = 0;
  }
  else {
    iVar3 = (int)pokemonGetStatus(r3,0,0xd6,0);
    if (iVar3 == 0) {
      uVar1 = 0;
    }
    else {
      uVar1 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
    }
  }
  pokemonBiosCopy(auStack_150,uVar1);
  sVar5 = (int)pokemonGetStatus(auStack_150,0,0x6e,0);
  uVar2 = (int)pokemonGetStatus(0,sVar5,0x66,0);
  if (sVar5 != 0x181) goto LAB_00204d50;
  if (r4 != 3) {
    if (r4 < 3) {
      if (r4 == 1) {
        uVar2 = 0x19f;
        goto LAB_00204d50;
      }
      if (r4 != 0) {
        uVar2 = 0x19e;
        goto LAB_00204d50;
      }
    }
    else if (r4 < 5) {
      uVar2 = 0x1a0;
      goto LAB_00204d50;
    }
  }
  uVar2 = 0x181;
LAB_00204d50:
  sVar5 = fn_80119ED0(0x14);
  if (((sVar5 == 0x7c) || (sVar5 = fn_80119ED0(0x14), sVar5 == 200)) ||
     (sVar5 = fn_80119ED0(0x14), sVar5 == 0xcd)) {
    iVar3 = (int)pokemonGetStatus(r3,0,0xd6,0);
    sVar5 = fn_80119ED0(0x14);
    if ((sVar5 == 0x7c) || (sVar5 = fn_80119ED0(0x14), sVar5 == 200)) {
      if (iVar3 == 0) {
        uVar1 = 0;
      }
      else {
        uVar1 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
      }
      cVar6 = fn_80121ADC(uVar1,0x14);
    }
    else {
      sVar5 = fn_80119ED0(0x14);
      if (sVar5 == 0xcd) {
        cVar6 = fn_8011B67C(iVar3,0x14);
      }
      else {
        cVar6 = 0;
      }
    }
  }
  else {
    sVar5 = fn_80119ED0(0x14);
    if (sVar5 == 0xd8) {
      cVar6 = fn_8011B67C(r3,0x14);
    }
    else {
      cVar6 = 0;
    }
  }
  if (cVar6 == 1) {
    uVar2 = 0x19d;
  }
  if (uVar2 == 0) {
    uVar1 = 0;
  }
  else {
    fightOutPokemonGetRndStatus(r3,&local_154,&local_158);
    pokemonSetStatus(auStack_150,0,0x6f,0,local_154);
    pokemonSetStatus(auStack_150,0,0x75,0,local_158);
    uVar1 = pokemonCheckRare(auStack_150);
    uVar1 = fn_801DE190(uVar2 & 0xffff,local_154,uVar1);
    pokemonSetSequenceStatus(auStack_150,uVar1);
    if (r3 == 0) {
      uVar4 = 0;
    }
    else {
      iVar3 = (int)pokemonGetStatus(r3,0,0xd6,0);
      if (iVar3 == 0) {
        uVar4 = 0;
      }
      else {
        uVar4 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
      }
    }
    uVar7 = (int)pokemonGetStatus(uVar4,0,0x73,0);
    itemGetStatus(0,uVar7,0x10,0);
  }
  return uVar1;
}

/* Address: 0x80207F5C | Size: 0xcc | Ghidra import */
u32 _fightOutPokemonRegWzxFreeSub__FPvUsPv(void)

{
    int r3;
    u32 r4;
    int *r5;

  u32 uVar1;
  int iVar2;
  u8 uVar3;
  int iVar4;
  
  iVar4 = *r5;
  if (r3 == 0) {
    uVar1 = 0;
  }
  else {
    iVar2 = (int)pokemonGetStatus(r3,0,0xd6,0);
    if (iVar2 == 0) {
      uVar1 = 0;
    }
    else {
      uVar1 = (int)pokemonGetStatus(iVar2,0,0xcc,0);
    }
  }
  if (r3 != iVar4) {
    uVar3 = (int)pokemonGetStatus(uVar1,0,0x73,0);
    iVar4 = itemGetStatus(0,uVar3,0x10,0);
    if ((iVar4 != 0) && (r5[1] == iVar4)) {
      r5[2] = r5[2] + 1;
    }
  }
  return 1;
}

/* 0x80208028 | size: 0x80 | small */
#pragma push
#pragma peephole on
void fightOutPokemonRegWzxLoad(void* param_1) {
    void* uVar1;
    void* iVar2;
    u8 uVar3;

    if (param_1 == NULL) {
        uVar1 = NULL;
    } else {
        iVar2 = pokemonGetStatus(param_1, 0, 0xD6, 0);
        if (iVar2 == NULL) {
            uVar1 = NULL;
        } else {
            uVar1 = pokemonGetStatus(iVar2, 0, 0xCC, 0);
        }
    }
    uVar3 = (u8)(u32)pokemonGetStatus(uVar1, 0, 0x73, 0);
    itemGetStatus(0, uVar3, 0x10, 0);
}
#pragma pop

/* Address: 0x802080A8 | Size: 0x35c | Ghidra import */
void fn_802080A8(void)

{
    int r3;
    char r4;
    char r5;
    u32 r6;
    char r7;

    extern void _threadSwitch();
    extern void fn_80166A50();
    extern void fn_801DA8C4();
    extern s8 fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DDD28();
    extern u32 fightFloorGetStatus();
    extern void fightMenuOpenMsg();
    extern void fightMenuFightOutPokemonRenewStatusMenu();
    u32 saved_r25 = 0;
  u16 uVar4;
  int iVar1;
  u32 uVar2;
  u16 uVar5;
  s8 cVar6;
  int iVar3;

  uVar4 = fightFloorGetStatus(0,0,0x14,0);
  iVar1 = (int)pokemonGetStatus(r3,0,0xee,0);
  if (iVar1 != 0) {
    if (r7 == 0) {
      if (r4 == 1) {
        fn_801DDD28(iVar1,0xa3,4,0);
      }
      if (r5 == 1) {
        fn_801DDD28(iVar1,0x9f,4,0);
      }
      if ((r4 == 0) && (r5 == 0)) {
        fn_801DDD28(iVar1,0x57,4,0);
      }
    }
    else if (r7 == 1) {
      if (r4 == 1) {
        fn_801DA9E8(iVar1,0xa3,4);
        if (r3 == 0) {
          uVar2 = 0;
        }
        else {
          iVar3 = (int)pokemonGetStatus(r3,0,0xd6,0);
          if (iVar3 == 0) {
            uVar2 = 0;
          }
          else {
            uVar2 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
          }
        }
        uVar5 = (int)pokemonGetStatus(uVar2,0,0x6e,0);
        uVar5 = (int)pokemonGetStatus(0,uVar5,0x61,0);
        fn_80166A50(uVar5,0,0xff,0);
        fightMenuOpenMsg(r6);
        if (r5 == 0) {
          fightMenuFightOutPokemonRenewStatusMenu(r3,uVar4,1);
        }
      }
      if (r5 == 1) {
        if (r4 == 1) {
          while (1) {
            cVar6 = fn_801DA94C(iVar1,0xa3,4);
            if (cVar6 == 0) break;
            _threadSwitch();
          }
        }
        fn_801DA9E8(iVar1,0x9f,4);
        if (r4 == 0) {
          if (r3 == 0) {
            uVar2 = 0;
          }
          else {
            iVar3 = (int)pokemonGetStatus(r3,0,0xd6,0);
            if (iVar3 == 0) {
              uVar2 = 0;
            }
            else {
              uVar2 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
            }
          }
          uVar5 = (int)pokemonGetStatus(uVar2,0,0x6e,0);
          uVar5 = (int)pokemonGetStatus(0,uVar5,0x61,0);
          fn_80166A50(uVar5,0,0xff,0);
          fightMenuOpenMsg(r6);
        }
        fightMenuFightOutPokemonRenewStatusMenu(r3,uVar4,1);
      }
      if ((r4 == 0) && (r5 == 0)) {
        fn_801DA9E8(iVar1,0x57,4);
        fightMenuOpenMsg(r6);
      }
    }
    else if (r7 == 2) {
      if (r4 == 1) {
        saved_r25 = 0xa3;
      }
      if (r5 == 1) {
        saved_r25 = 0x9f;
      }
      if ((r4 == 0) && (r5 == 0)) {
        saved_r25 = 0x57;
      }
      while (1) {
        cVar6 = fn_801DA94C(iVar1,saved_r25,4);
        if (cVar6 == 0) break;
        _threadSwitch();
      }
    }
    else if (r7 == 3) {
      if (r4 == 1) {
        fn_801DA8C4(iVar1,0xa3,4);
      }
      if (r5 == 1) {
        fn_801DA8C4(iVar1,0x9f,4);
      }
      if ((r4 == 0) && (r5 == 0)) {
        fn_801DA8C4(iVar1,0x57,4);
      }
    }
  }
  return;
}

/* Address: 0x80208404 | Size: 0x150 | Ghidra import */
#pragma push
#pragma peephole on
void fightOutPokemonDarkPokemonEffect(void* ctx, u8 p4, u8 p5, u8 p6)
{
    extern void _threadSwitch();
    extern void fn_801DA8C4();
    extern u8 fn_801DA94C();
    extern void fn_801DA9B4();
    extern void fn_801DA9E8();
    extern void fn_801DDD28();
    extern u32 fightFloorGetStatus();
    extern void fn_80265598();
    void* iVar1;
    u16 uVar2;
    u8 cVar3;
    u32 uVar4;

    uVar2 = fightFloorGetStatus(0, 0, 0x14, 0);
    iVar1 = pokemonGetStatus(ctx, 0, 0xee, 0);
    if (iVar1 != 0) {
        if (p5 == 0) {
            uVar4 = 0x3a;
        } else if (p5 == 1) {
            uVar4 = 0x88;
        } else if (p5 == 2) {
            uVar4 = 0x57;
        } else {
            uVar4 = 0xd9;
        }
        if (p6 == 0) {
            fn_801DDD28(iVar1, uVar4, 4, 0);
        } else if (p6 == 1) {
            fn_801DA9E8(iVar1, uVar4, 4);
            if (p4 == 1) {
                fn_80265598(ctx, uVar2, 1);
            }
        } else if (p6 == 2) {
            while (1) {
                cVar3 = fn_801DA94C(iVar1, uVar4, 4);
                if (cVar3 == 0) break;
                _threadSwitch();
            }
        } else if (p6 == 3) {
            fn_801DA8C4(iVar1, uVar4, 4);
        } else if (p6 == 4) {
            fn_801DA9B4(iVar1, uVar4, 4);
        }
    }
}
#pragma pop

/* Address: 0x80208554 | Size: 0x70 | Ghidra import */
#pragma push
#pragma peephole on
#pragma scheduling on
void fn_80208554(void* r3, u32 r4, u32 r5, u32 r6)

{
    extern void _threadSwitch();
    extern u8 fn_801DA698();
  int iVar1;
  u8 cVar2;

  iVar1 = (int)pokemonGetStatus(r3,0,0xee,0);
  if (iVar1 != 0) {
    while (1) {
      cVar2 = fn_801DA698(iVar1,r4,r5,r6);
      if (cVar2 == 1) break;
      _threadSwitch();
    }
  }
  return;
}
#pragma pop

/* Address: 0x802085C4 | Size: 0xec | Ghidra import */
#pragma push
#pragma peephole on
void fightOutPokemonWazaEffect(u32 r3, u32 r4, u32 r5, u32 r6, int r7)

{
    extern void menuCloseCustom();
    extern void menuOpenCustom(int, ...);
    extern u32 fightFloorGetStatus();
    extern void fightOutPokemonToMenuPokemonStatus();
    extern void fightWazaDoEffect();
    extern int fightMenuGetFightOutPokemonPtrToStatusMenuId();
  int iVar1;
  u16 uVar2;
  u8 auStack_58 [44];

  uVar2 = fightFloorGetStatus(0,0,0x14,0);
  iVar1 = (int)pokemonGetStatus(r3,0,0xee,0);
  if (iVar1 != 0) {
    fightOutPokemonToMenuPokemonStatus(r3,auStack_58);
    if (r7 >= 0) {
      r7 = fightMenuGetFightOutPokemonPtrToStatusMenuId(r3,uVar2,1);
      menuOpenCustom(r7,0,0,0,0,1,auStack_58);
    }
    fightWazaDoEffect(iVar1,r4,r5,r6);
    if (((r6 & 0xff) == 1) && (r7 >= 0)) {
      menuCloseCustom(r7,0,0);
    }
  }
  return;
}
#pragma pop

/* 0x802086B0 | size: 0x38 | small */
#pragma push
#pragma peephole on
void fightOutPokemonFreeAllSequenceWaza(void* ctx) {
    extern void fn_801DA83C();
    void* obj = pokemonGetStatus(ctx, 0, 0xee, 0);
    if (obj != 0) {
        fn_801DA83C(obj);
    }
}
#pragma pop

/* Address: 0x802086E8 | Size: 0x68 | Ghidra import */
void fightOutPokemonFreeWazaEffect(void)

{
    u32 r3;
    u32 r4;
    u32 r5;

    extern u32 wazaGetStatus();
    extern void fn_801DA8C4();
  u16 uVar2;
  int iVar1;

  uVar2 = wazaGetStatus(0,r4,0x1f,0);
  iVar1 = (int)pokemonGetStatus(r3,0,0xee,0);
  if (iVar1 != 0) {
    fn_801DA8C4(iVar1,uVar2,r5);
  }
  return;
}

/* 0x80208750 | size: 0x70 | small */
void fightOutPokemonLoadWazaEffect(void* param_1, u32 param_2, u32 param_3, u32 param_4) {
    extern u32 wazaGetStatus(void*, u32, u16, u32);
    extern void fn_801DDD28(void*, u16, u32, u32);
    u32 uVar2;
    void* iVar1;

    uVar2 = wazaGetStatus(NULL, param_2, 0x1F, 0);
    iVar1 = pokemonGetStatus(param_1, 0, 0xEE, 0);
    if (iVar1 != NULL) {
        fn_801DDD28(iVar1, (u16)uVar2, param_3, param_4);
    }
}

/* Address: 0x802087C0 | Size: 0x458 | Ghidra import */
void fightOutPokemonHokakuEffect(void)

{
    int r3;
    u8 r4;
    u32 r5;
    char r6;
    u8 *r7;

    extern void _threadSwitch();
    extern void battleGridRemovePokemon();
    extern void fn_801DA224();
    extern void fn_801DA2C4();
    extern u32 fn_801DA354();
    extern void fn_801DA4E8();
    extern void fn_801DA8C4();
    extern s8 fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DB100();
    extern void fn_801DDD28();
    extern void fightFloorLoopValidFightOutPokemon();
    extern u32 fightFloorGetStatus();
    extern void fn_8026532C();
  int iVar1;
  u16 uVar4;
  u16 uVar5;
  u16 uVar6;
  u16 uVar7;
  u16 uVar8;
  u8 uVar9;
  s8 cVar10;
  u32 uVar2;
  int iVar3;
  u8 bVar11;
  int local_38;
  int local_34;
  u32 local_30;
  
  iVar1 = (int)pokemonGetStatus(r3,0,0xee,0);
  if (iVar1 != 0) {
    uVar4 = itemGetStatus(0,r5,0x17,0);
    uVar5 = itemGetStatus(0,r5,0x13,0);
    uVar6 = itemGetStatus(0,r5,0x16,0);
    uVar7 = itemGetStatus(0,r5,0x14,0);
    uVar8 = itemGetStatus(0,r5,0x15,0);
    if (r6 == 0) {
      fn_801DDD28(iVar1,uVar4,4,0);
      fn_801DDD28(iVar1,uVar5,4,0);
      fn_801DDD28(iVar1,uVar6,4,0);
      fn_801DDD28(iVar1,uVar7,4,0);
      fn_801DDD28(iVar1,uVar8,4,0);
      if (r7 != (void *)0) {
        uVar9 = fn_801DA354(iVar1);
        *r7 = uVar9;
        fn_801DA2C4(iVar1);
      }
    }
    else if (r6 == 1) {
      fn_801DA9E8(iVar1,uVar4,4);
      while (1) {
        cVar10 = fn_801DA94C(iVar1,uVar4,4);
        if (cVar10 == 0) break;
        _threadSwitch();
      }
      fn_801DA9E8(iVar1,uVar5,4);
      while (1) {
        cVar10 = fn_801DA94C(iVar1,uVar5,4);
        if (cVar10 == 0) break;
        _threadSwitch();
      }
      bVar11 = 0;
      do {
        fn_801DA9E8(iVar1,uVar6,4);
        while (1) {
          cVar10 = fn_801DA94C(iVar1,uVar6,4);
          if (cVar10 == 0) break;
          _threadSwitch();
        }
        bVar11 = bVar11 + 1;
      } while ((bVar11 < 3) && (bVar11 < r4));
      if (r4 < 4) {
        fn_801DA9E8(iVar1,uVar7,4);
        while (1) {
          cVar10 = fn_801DA94C(iVar1,uVar7,4);
          if (cVar10 == 0) break;
          _threadSwitch();
        }
      }
    }
    else if (r6 == 2) {
      if (r4 < 4) {
        fn_801DA9E8(iVar1,uVar8,4);
      }
    }
    else if (r6 == 3) {
      if (r4 < 4) {
        if (r7 != (void *)0) {
          fn_801DA224(iVar1,*r7);
        }
        while (1) {
          cVar10 = fn_801DA94C(iVar1,uVar8,4);
          if (cVar10 == 0) break;
          _threadSwitch();
        }
      }
    }
    else if (r6 == 4) {
      fn_801DA8C4(iVar1,uVar4,4);
      fn_801DA8C4(iVar1,uVar5,4);
      fn_801DA8C4(iVar1,uVar6,4);
      fn_801DA8C4(iVar1,uVar7,4);
      fn_801DA8C4(iVar1,uVar8,4);
      if (r4 < 4) {
        if (r7 != (void *)0) {
          fn_801DA224(iVar1,*r7);
        }
      }
      else {
        if (r3 == 0) {
          uVar2 = 0;
        }
        else {
          iVar1 = (int)pokemonGetStatus(r3,0,0xd6,0);
          if (iVar1 == 0) {
            uVar2 = 0;
          }
          else {
            uVar2 = (int)pokemonGetStatus(iVar1,0,0xcc,0);
          }
        }
        uVar9 = (int)pokemonGetStatus(uVar2,0,0x73,0);
        iVar1 = itemGetStatus(0,uVar9,0x10,0);
        if (iVar1 != 0) {
          local_30 = 0;
          local_38 = r3;
          local_34 = iVar1;
          fightFloorLoopValidFightOutPokemon(0,0x80207f5c,&local_38,0);
        }
        iVar1 = (int)pokemonGetStatus(r3,0,0xee,0);
        if (iVar1 != 0) {
          iVar3 = (int)pokemonGetStatus(r3,0,0xee,0);
          if (iVar3 != 0) {
            fn_801DA4E8(iVar3,0);
          }
          pokemonSetStatus(r3,0,0xee,0,0);
          battleGridRemovePokemon(iVar1);
          fn_801DB100(iVar1);
        }
        uVar4 = fightFloorGetStatus(0,0,0x14,0);
        fn_8026532C(r3,uVar4,1);
      }
    }
  }
  return;
}

/* Address: 0x80208C18 | Size: 0x2b8 | Ghidra import */
void fightOutPokemonDasuEffect(void)

{
    int r3;
    char r4;

    extern void _threadSwitch();
    extern s8 pokemonCheckRare();
    extern void fn_80166A50();
    extern s8 fn_801DA5C4();
    extern void fn_801DA8C4();
    extern s8 fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DDD28();
  int iVar1;
  u8 uVar6;
  u16 uVar4;
  u16 uVar5;
  s8 cVar7;
  u32 uVar2;
  int iVar3;
  u32 uVar8;
  
  iVar1 = (int)pokemonGetStatus(r3,0,0xee,0);
  if (iVar1 != 0) {
    if (r3 == 0) {
      uVar8 = 0;
    }
    else {
      iVar3 = (int)pokemonGetStatus(r3,0,0xd6,0);
      if (iVar3 == 0) {
        uVar8 = 0;
      }
      else {
        uVar8 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
      }
    }
    uVar6 = (int)pokemonGetStatus(uVar8,0,0x73,0);
    uVar4 = itemGetStatus(0,uVar6,0xe,0);
    uVar5 = itemGetStatus(0,uVar6,0xf,0);
    if (r4 == 0) {
      fn_801DDD28(iVar1,uVar4,4,0);
      fn_801DDD28(iVar1,uVar5,4,0);
      fn_801DDD28(iVar1,0x67,4,0);
    }
    else if (r4 == 1) {
      fn_801DA9E8(iVar1,uVar4,4);
    }
    else if (r4 == 2) {
      while (cVar7 = fn_801DA94C(iVar1,uVar4,4), cVar7 != 0) {
        _threadSwitch();
      }
    }
    else if (r4 == 3) {
      fn_801DA9E8(iVar1,uVar5,4);
    }
    else if (r4 == 4) {
      while (cVar7 = fn_801DA5C4(0), cVar7 != 1) {
        _threadSwitch();
      }
      if (r3 == 0) {
        uVar2 = 0;
      }
      else {
        iVar3 = (int)pokemonGetStatus(r3,0,0xd6,0);
        if (iVar3 == 0) {
          uVar2 = 0;
        }
        else {
          uVar2 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
        }
      }
      uVar4 = (int)pokemonGetStatus(uVar2,0,0x6e,0);
      uVar4 = (int)pokemonGetStatus(0,uVar4,0x61,0);
      fn_80166A50(uVar4,0,0xff,0);
      while (cVar7 = fn_801DA94C(iVar1,uVar5,4), cVar7 != 0) {
        _threadSwitch();
      }
      cVar7 = pokemonCheckRare(uVar8);
      if (cVar7 == 1) {
        fn_801DA9E8(iVar1,0x67,4);
        while (cVar7 = fn_801DA94C(iVar1,0x67,4), cVar7 != 0) {
          _threadSwitch();
        }
      }
    }
    else if (r4 == 5) {
      fn_801DA8C4(iVar1,uVar4,4);
      fn_801DA8C4(iVar1,uVar5,4);
      fn_801DA8C4(iVar1,0x67,4);
    }
  }
  return;
}

/* Address: 0x80208ED0 | Size: 0x25c | Ghidra import */
void fightOutPokemonModosuEffect(void)

{
    int r3;
    char r4;

    extern void _threadSwitch();
    extern void battleGridRemovePokemon();
    extern void fn_801DA4E8();
    extern void fn_801DA8C4();
    extern s8 fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DB100();
    extern void fn_801DDD28();
    extern void fightFloorLoopValidFightOutPokemon();
    extern u32 fightFloorGetStatus();
    extern void fn_8026532C();
  int iVar1;
  u32 uVar2;
  u8 uVar5;
  u16 uVar4;
  s8 cVar6;
  int iVar3;
  int local_28;
  int local_24;
  u32 local_20;
  
  iVar1 = (int)pokemonGetStatus(r3,0,0xee,0);
  if (iVar1 != 0) {
    if (r3 == 0) {
      uVar2 = 0;
    }
    else {
      iVar3 = (int)pokemonGetStatus(r3,0,0xd6,0);
      if (iVar3 == 0) {
        uVar2 = 0;
      }
      else {
        uVar2 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
      }
    }
    uVar5 = (int)pokemonGetStatus(uVar2,0,0x73,0);
    uVar4 = itemGetStatus(0,uVar5,0xd,0);
    if (r4 == 0) {
      fn_801DDD28(iVar1,uVar4,4,0);
    }
    else if (r4 == 1) {
      fn_801DA9E8(iVar1,uVar4,4);
    }
    else if (r4 == 2) {
      while (1) {
        cVar6 = fn_801DA94C(iVar1,uVar4,4);
        if (cVar6 == 0) break;
        _threadSwitch();
      }
    }
    else if (r4 == 3) {
      fn_801DA8C4(iVar1,uVar4,4);
    }
    else if (r4 == 4) {
      if (r3 == 0) {
        uVar2 = 0;
      }
      else {
        iVar1 = (int)pokemonGetStatus(r3,0,0xd6,0);
        if (iVar1 == 0) {
          uVar2 = 0;
        }
        else {
          uVar2 = (int)pokemonGetStatus(iVar1,0,0xcc,0);
        }
      }
      uVar5 = (int)pokemonGetStatus(uVar2,0,0x73,0);
      iVar1 = itemGetStatus(0,uVar5,0x10,0);
      if (iVar1 != 0) {
        local_20 = 0;
        local_28 = r3;
        local_24 = iVar1;
        fightFloorLoopValidFightOutPokemon(0,0x80207f5c,&local_28,0);
      }
      iVar1 = (int)pokemonGetStatus(r3,0,0xee,0);
      if (iVar1 != 0) {
        iVar3 = (int)pokemonGetStatus(r3,0,0xee,0);
        if (iVar3 != 0) {
          fn_801DA4E8(iVar3,0);
        }
        pokemonSetStatus(r3,0,0xee,0,0);
        battleGridRemovePokemon(iVar1);
        fn_801DB100(iVar1);
      }
      uVar4 = fightFloorGetStatus(0,0,0x14,0);
      fn_8026532C(r3,uVar4,1);
    }
  }
  return;
}

/* Address: 0x8020912C | Size: 0x254 | Ghidra import */
void fightOutPokemonKizetuEffect(void)

{
    int r3;
    char r4;

    extern void _threadSwitch();
    extern void battleGridRemovePokemon();
    extern void fn_801DA4E8();
    extern void fn_801DA8C4();
    extern s8 fn_801DA94C();
    extern void fn_801DA9E8();
    extern void fn_801DB100();
    extern void fn_801DDD28();
    extern void fightFloorLoopValidFightOutPokemon();
    extern u32 fightFloorGetStatus();
    extern void fn_8026532C();
  int iVar1;
  u32 uVar2;
  u8 uVar5;
  u16 uVar4;
  s8 cVar6;
  int iVar3;
  int local_28;
  int local_24;
  u32 local_20;
  
  iVar1 = (int)pokemonGetStatus(r3,0,0xee,0);
  if (iVar1 != 0) {
    if (r3 == 0) {
      uVar2 = 0;
    }
    else {
      iVar3 = (int)pokemonGetStatus(r3,0,0xd6,0);
      if (iVar3 == 0) {
        uVar2 = 0;
      }
      else {
        uVar2 = (int)pokemonGetStatus(iVar3,0,0xcc,0);
      }
    }
    uVar5 = (int)pokemonGetStatus(uVar2,0,0x73,0);
    uVar4 = itemGetStatus(0,uVar5,0x10,0);
    if (r4 == 0) {
      fn_801DDD28(iVar1,uVar4,4,0);
    }
    else if (r4 == 1) {
      fn_801DA9E8(iVar1,uVar4,4);
    }
    else if (r4 == 2) {
      while (1) {
        cVar6 = fn_801DA94C(iVar1,uVar4,4);
        if (cVar6 == 0) break;
        _threadSwitch();
      }
      fn_801DA8C4(iVar1,uVar4,4);
      uVar4 = fightFloorGetStatus(0,0,0x14,0);
      fn_8026532C(r3,uVar4,1);
    }
    else if (r4 == 3) {
      if (r3 == 0) {
        uVar2 = 0;
      }
      else {
        iVar1 = (int)pokemonGetStatus(r3,0,0xd6,0);
        if (iVar1 == 0) {
          uVar2 = 0;
        }
        else {
          uVar2 = (int)pokemonGetStatus(iVar1,0,0xcc,0);
        }
      }
      uVar5 = (int)pokemonGetStatus(uVar2,0,0x73,0);
      iVar1 = itemGetStatus(0,uVar5,0x10,0);
      if (iVar1 != 0) {
        local_20 = 0;
        local_28 = r3;
        local_24 = iVar1;
        fightFloorLoopValidFightOutPokemon(0,0x80207f5c,&local_28,0);
      }
      iVar1 = (int)pokemonGetStatus(r3,0,0xee,0);
      if (iVar1 != 0) {
        iVar3 = (int)pokemonGetStatus(r3,0,0xee,0);
        if (iVar3 != 0) {
          fn_801DA4E8(iVar3,0);
        }
        pokemonSetStatus(r3,0,0xee,0,0);
        battleGridRemovePokemon(iVar1);
        fn_801DB100(iVar1);
      }
    }
  }
  return;
}

/* 0x80209380 | size: 0x104 */
#pragma push
#pragma peephole on
void fightOutPokemonDamageEffect(void* ctx) {
    extern u8 GSmodelGetVisibility();
    extern void GSmodelSetVisibility();
    extern void fn_801DA4E8();
    extern void* fn_801DAC3C();
    extern void fightMainWaitFrame();
    void* eeData;
    void* resolved;
    u8 i;

    eeData = pokemonGetStatus(ctx, 0, 0xEE, 0);
    resolved = !eeData ? NULL : fn_801DAC3C(eeData);
    if (resolved == NULL) { return; }
    if ((u8)GSmodelGetVisibility(resolved) == 0) { return; }
    for (i = 0; i < 8; i++) {
        eeData = pokemonGetStatus(ctx, 0, 0xEE, 0);
        if (eeData != NULL) {
            fn_801DA4E8(eeData, 1);
        }
        GSmodelSetVisibility(resolved, 1);
        fightMainWaitFrame(3);
        eeData = pokemonGetStatus(ctx, 0, 0xEE, 0);
        if (eeData != NULL) {
            fn_801DA4E8(eeData, 0);
        }
        fightMainWaitFrame(2);
    }
    eeData = pokemonGetStatus(ctx, 0, 0xEE, 0);
    if (eeData != NULL) {
        fn_801DA4E8(eeData, 1);
    }
}
#pragma pop

/* 0x80209484 | size: 0x48 | small */
#pragma push
#pragma peephole on
void fightOutPokemonSetVisibility(void* ctx, u32 param) {
    extern void fn_801DA4E8();
    void* obj = pokemonGetStatus(ctx, 0, 0xee, 0);
    if (obj != 0) {
        fn_801DA4E8(obj, param);
    }
}
#pragma pop

/* 0x802094CC | size: 0x90 | medium */
#pragma push
#pragma peephole on
#pragma scheduling on
void fightWazaDoEffect(u32 param_1, u32 param_2, u32 param_3, u8 param_4) {
    extern void _threadSwitch(void);
    extern u32 wazaGetStatus(void*, u32, u16, u32);
    extern void fn_801DA8C4(u32, u16, u32);
    extern u8 fn_801DA94C(u32, u16, u32);
    extern void fn_801DA9E8(u32, u16, u32);
    u32 uVar1;
    u8 cVar2;

    uVar1 = wazaGetStatus(NULL, param_2, 0x1F, 0);
    fn_801DA9E8(param_1, uVar1, param_3);
    if (param_4 == 1) {
        while (1) {
            cVar2 = fn_801DA94C(param_1, uVar1, param_3);
            if (cVar2 == 0) break;
            _threadSwitch();
        }
        fn_801DA8C4(param_1, uVar1, param_3);
    }
}
#pragma pop

/* Address: 0x8020955C | Size: 0xbc | Ghidra import */
#pragma push
#pragma peephole on
void fightWazaDoEffectFunc(u32 p1, u32 p2, u32 p3, u32 p4)
{
    extern int wazaGetStatus();
    extern void fightWazaWzxTypeFuncNull();
    extern u32 fightWazaWzxVariationFuncNull();
    void* pcVar1;
    void* pcVar2;
    u32 uVar3;

    pcVar1 = (void*)wazaGetStatus(0, p1, 0x20, 0);
    if (pcVar1 == NULL) {
        pcVar1 = (void*)&fightWazaWzxTypeFuncNull;
    }
    pcVar2 = (void*)wazaGetStatus(0, p1, 0x21, 0);
    if (pcVar2 == NULL) {
        pcVar2 = (void*)&fightWazaWzxVariationFuncNull;
    }
    uVar3 = ((u32 (*)(u32, u32, u32, u32))pcVar2)(p1, p2, p3, p4);
    ((void (*)(u32, u32, u32, u32, u32))pcVar1)(p1, p2, p3, p4, uVar3);
}
#pragma pop

/* Address: 0x80209618 | Size: 0xd0 | Ghidra import */

char fightWazaIsMix(u32 r3)

{
    extern u32 _DAT_80279d08;
    extern u32 _DAT_80279d0c;
    extern u32 _DAT_80279d10;
    extern u32 _DAT_80279d14;
    extern short fn_80119ED0();
    extern s8 fn_8011B67C();
  u16 uVar1;
  u16 uVar2;
  short sVar3;
  s8 cVar4;
  u16 uVar5;
  u32 local_28;
  u32 local_24;
  u32 local_20;
  u16 local_1c;
  
  uVar2 = 0;
  local_28 = _DAT_80279d08;
  local_24 = _DAT_80279d0c;
  local_20 = _DAT_80279d10;
  local_1c = _DAT_80279d14;
  for (uVar5 = 0; uVar5 < 7; uVar5 = uVar5 + 1) {
    uVar1 = *(u16 *)((int)&local_28 + (u32)uVar5 * 2);
    sVar3 = fn_80119ED0(uVar1);
    if (sVar3 == 0x2a) {
      cVar4 = fn_8011B67C(r3,uVar1);
    }
    else {
      cVar4 = 0;
    }
    if (cVar4 == 1) {
      uVar2 = uVar2 + 1;
    }
  }
  return -((uVar2 < 2) + -1);
}

/* Address: 0x802096E8 | Size: 0xe0 | Ghidra import */
#pragma push
#pragma peephole off
u32 fightWazaIsHit(void* ctx)
{
    extern u16 fn_80119ED0();
    extern u8 fn_8011B67C();
  u16 sVar2;
  u32 uVar1;
  u8 cVar3;

  sVar2 = fn_80119ED0(0x40);
  if (sVar2 != 0x2a) {
    cVar3 = 0;
  }
  else {
    cVar3 = fn_8011B67C(ctx,0x40);
  }
  if (cVar3 == 1) {
    uVar1 = 0;
  }
  else {
    sVar2 = fn_80119ED0(0x43);
    if (sVar2 != 0x2a) {
      cVar3 = 0;
    }
    else {
      cVar3 = fn_8011B67C(ctx,0x43);
    }
    if (cVar3 == 1) {
      uVar1 = 0;
    }
    else {
      sVar2 = fn_80119ED0(0x45);
      if (sVar2 != 0x2a) {
        cVar3 = 0;
      }
      else {
        cVar3 = fn_8011B67C(ctx,0x45);
      }
      if (cVar3 == 1) {
        uVar1 = 0;
      }
      else {
        uVar1 = 1;
      }
    }
  }
  return uVar1;
}
#pragma pop

/* 0x802097C8 | size: 0x54 | small */
#pragma push
#pragma peephole on
void fightWazaWriteJoutaiDataId(u32 param_1, u32 param_2, u32 param_3) {
    extern u32 fn_80119ED0(u32);
    extern void fn_8011B2C0(u32, u32, u32);
    if ((fn_80119ED0(param_2) & 0xFFFF) == 0x2A) {
        fn_8011B2C0(param_1, param_2, param_3);
    }
}
#pragma pop

/* Address: 0x8020981C | Size: 0x54 | Ghidra import */
#pragma push
#pragma scheduling on
#pragma peephole on
u32 fightWazaCheckWriteJoutaiDataId(void* ctx, u32 param)

{
    extern u16 fn_80119ED0();
    extern u32 fn_8011B444();
  u16 sVar2;
  u32 uVar1;

  sVar2 = fn_80119ED0(param);
  if (sVar2 != 0x2a) {
    uVar1 = 0;
  }
  else {
    uVar1 = fn_8011B444(ctx,param);
  }
  return uVar1;
}
#pragma pop

/* Address: 0x80209870 | Size: 0x9c | Ghidra import */
u32 fightWazaIsJoutaiSousai(void* ctx)

{
    extern u16 fn_80119ED0();
    extern u8 fn_8011B67C();
  u16 sVar1;
  u8 cVar2;

  sVar1 = fn_80119ED0(0x41);
  if (sVar1 != 0x2a) {
    cVar2 = 0;
  }
  else {
    cVar2 = fn_8011B67C(ctx,0x41);
  }
  if (cVar2 == 1) {
    sVar1 = fn_80119ED0(0x42);
    if (sVar1 != 0x2a) {
      cVar2 = 0;
    }
    else {
      cVar2 = fn_8011B67C(ctx,0x42);
    }
    if (cVar2 == 1) {
      return 1;
    }
  }
  return 0;
}

/* 0x8020990C | size: 0x54 */
#pragma push
#pragma peephole on
#pragma scheduling on
u32 fightWazaIsJoutaiDataId(void* ctx, u32 param) {
    extern u32 fn_80119ED0();
    extern u32 fn_8011B67C();
    if ((fn_80119ED0(param) & 0xFFFF) != 0x2A) {
        return 0;
    }
    return fn_8011B67C(ctx, param);
}
#pragma pop

/* 0x80209960 | size: 0x4C | small */
#pragma push
#pragma scheduling on
#pragma peephole on
void fightWazaInitJoutaiDataId(void* ctx, u32 param) {
    extern u32 fn_80119ED0();
    extern void fn_8011B788();
    if ((fn_80119ED0(param) & 0xFFFF) == 0x2a) {
        fn_8011B788(ctx, param);
    }
}
#pragma pop

/* Address: 0x802099AC | Size: 0x270 | Ghidra import */
#pragma push
#pragma peephole on
void fightWazaCreate(void* p1, s8 p2, u32 p3, u16 p4, u8 p5) {
    extern u16 fn_80119ED0();
    extern void fn_8011B2C0();
    extern void fn_8011B950();
    extern void wazaSetStatus();
    extern u32 wazaGetStatus();
    u16 sVar2;

    if (p1 != 0) {
        if (p1) {
            wazaSetStatus(p1, 0, 0x26, 0, (void*)0xffffffff);
            wazaSetStatus(p1, 0, 0x27, 0, 0);
            wazaSetStatus(p1, 0, 0x28, 0, 0);
            wazaSetStatus(p1, 0, 0x29, 0, 0);
            fn_8011B950(wazaGetStatus(p1, 0, 0x2a, 0), 9);
            sVar2 = fn_80119ED0(0x3f);
            if (sVar2 == 0x2a) {
                fn_8011B2C0(p1, 0x3f, 0);
            }
            wazaSetStatus(p1, 0, 0x2b, 0, 1);
            wazaSetStatus(p1, 0, 0x2c, 0, 1);
            wazaSetStatus(p1, 0, 0x2d, 0, 0);
            wazaSetStatus(p1, 0, 0x2e, 0, 0);
            wazaSetStatus(p1, 0, 0x2f, 0, 0);
            wazaSetStatus(p1, 0, 0x30, 0, 9);
            wazaSetStatus(p1, 0, 0x31, 0, 0);
            wazaSetStatus(p1, 0, 0x32, 0, 0);
        }
        wazaSetStatus(p1, 0, 0x26, 0, (s32)p2);
        wazaSetStatus(p1, 0, 0x29, 0, p4);
        wazaSetStatus(p1, 0, 0x27, 0, p3 & 0xffff);
        wazaSetStatus(p1, 0, 0x28, 0, p3 & 0xffff);
        wazaSetStatus(p1, 0, 0x2f, 0, (u16)wazaGetStatus(0, p3, 7, 0));
        wazaSetStatus(p1, 0, 0x30, 0, (u16)wazaGetStatus(0, p3, 3, 0));
        wazaSetStatus(p1, 0, 0x32, 0, p5);
    }
}
#pragma pop

/* Address: 0x80209C1C | Size: 0x98 | Ghidra import */
#pragma push
#pragma peephole on
#pragma scheduling on
void fightWazaSetUseWazaStatus(u32 r3, u32 r4)

{
    extern void wazaSetStatus();
    extern u32 wazaGetStatus();
  u16 uVar1;

  wazaSetStatus(r3,0,0x28,0,r4 & 0xffff);
  uVar1 = (u16)wazaGetStatus(0,r4,7,0);
  wazaSetStatus(r3,0,0x2f,0,uVar1);
  uVar1 = (u16)wazaGetStatus(0,r4,3,0);
  wazaSetStatus(r3,0,0x30,0,uVar1);
  return;
}
#pragma pop

/* Address: 0x80209CB4 | Size: 0xdc | Ghidra import */
#pragma push
#pragma peephole on
u32 fightWazaCheckValid(void* ctx) {
    extern s32 wazaGetStatus(void* ctx, u32 p1, u32 p2, u32 p3);
    s32 iVar1;
    if (ctx == 0) {
        return 0;
    }
    iVar1 = wazaGetStatus(ctx, 0, 0x27, 0);
    if (iVar1 == 0) {
        return 0;
    }
    iVar1 = wazaGetStatus(ctx, 0, 0x27, 0);
    if (iVar1 == 0x163) {
        return 0;
    }
    iVar1 = wazaGetStatus(ctx, 0, 0x28, 0);
    if (iVar1 == 0) {
        return 0;
    }
    iVar1 = wazaGetStatus(ctx, 0, 0x28, 0);
    if (iVar1 == 0x163) {
        return 0;
    }
    iVar1 = wazaGetStatus(ctx, 0, 0x29, 0);
    return iVar1 != 0;
}
#pragma pop

/* 0x80209D90 | size: 0x188 */
#pragma push
#pragma peephole on
void fightWazaInit(void* r3) {
    extern u16 fn_80119ED0();
    extern void fn_8011B2C0();
    extern void fn_8011B950();
    extern void wazaSetStatus();
    extern void* wazaGetStatus();
    void* ctx;

    if ((ctx = r3) == NULL) { return; }
    wazaSetStatus(ctx, 0, 0x26, 0, (u32)-1);
    wazaSetStatus(ctx, 0, 0x27, 0, 0);
    wazaSetStatus(ctx, 0, 0x28, 0, 0);
    wazaSetStatus(ctx, 0, 0x29, 0, 0);
    fn_8011B950(wazaGetStatus(ctx, 0, 0x2A, 0), 9);
    if (fn_80119ED0(0x3F) == 0x2A) {
        fn_8011B2C0(ctx, 0x3F, 0);
    }
    wazaSetStatus(ctx, 0, 0x2B, 0, 1);
    wazaSetStatus(ctx, 0, 0x2C, 0, 1);
    wazaSetStatus(ctx, 0, 0x2D, 0, 0);
    wazaSetStatus(ctx, 0, 0x2E, 0, 0);
    wazaSetStatus(ctx, 0, 0x2F, 0, 0);
    wazaSetStatus(ctx, 0, 0x30, 0, 9);
    wazaSetStatus(ctx, 0, 0x31, 0, 0);
    wazaSetStatus(ctx, 0, 0x32, 0, 0);
}
#pragma pop

/* 0x80209F18 | size: 0xa8 */
#pragma push
#pragma peephole on
void fightWazaInitLoop(void* ctx) {
    extern u16 fn_80119ED0();
    extern void fn_8011B2C0();
    extern void fn_8011B950();
    extern void wazaSetStatus();
    extern u32 wazaGetStatus();
    u32 val;
    u16 typeId;
    val = wazaGetStatus(ctx, 0, 0x2a, 0);
    fn_8011B950(val, 9);
    typeId = fn_80119ED0(0x3f);
    if (typeId == 0x2a) {
        fn_8011B2C0(ctx, 0x3f, 0);
    }
    wazaSetStatus(ctx, 0, 0x2b, 0, 1);
    wazaSetStatus(ctx, 0, 0x2c, 0, 1);
}
#pragma pop

/* 0x80209FAC | size: 0x64 */
#pragma push
#pragma scheduling on
#pragma peephole on
void fightWazaInitJoutai(void* ctx) {
    extern u16 fn_80119ED0();
    extern void fn_8011B2C0();
    extern u32 fn_8011B950();
    extern u32 wazaGetStatus();
    u32 val = wazaGetStatus(ctx, 0, 0x2A, 0);
    fn_8011B950(val, 9);
    if (fn_80119ED0(0x3F) == 0x2A) {
        fn_8011B2C0(ctx, 0x3F, 0);
    }
}
#pragma pop

/* 0x8020A010 | size: 0x18 */
u32 fightWazaHitKakurituDataBiosGetWaru(u8* ptr) {
    if (ptr == NULL) { return 1; }
    return ptr[0x1];
}

/* 0x8020A028 | size: 0x18 */
u32 fightWazaHitKakurituDataBiosGetKake(u8* ptr) {
    if (ptr == NULL) { return 1; }
    return ptr[0x0];
}

/* fightWazaHitKakurituDataBiosGetPtr | Size: 0x28 | Look up 2-byte entry in table */
#pragma push
#pragma peephole on
u16* fightWazaHitKakurituDataBiosGetPtr(u16 index) {
    extern u8 lbl_80375DD0[];
    extern u32 lbl_80478D70;
    u16* result = (u16*)&lbl_80375DD0[index * 2];
    if (index < lbl_80478D70) {
        return result;
    }
    return NULL;
}
#pragma pop

/* Address: 0x8020A068 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightWazaCriticalDataBiosGetBunbo(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* fightWazaCriticalDataBiosGetPtr | Size: 0x24 | Look up byte in table with bounds check */
#pragma push
#pragma peephole on
u8* fightWazaCriticalDataBiosGetPtr(u16 index) {
    extern u8 lbl_80478D58[];
    extern u32 lbl_80478D60;
    u8* result = &lbl_80478D58[index];
    if (index < lbl_80478D60) {
        return result;
    }
    return NULL;
}
#pragma pop

/* Address: 0x8020A0A4 | Size: 0x10 | Pattern: nullcheck_setter */
void fightWazaBiosSetAutoMakeFlag(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA9]) = val;
}

/* Address: 0x8020A0B4 | Size: 0x10 | Pattern: nullcheck_setter */
void fightWazaBiosSetKaisuu(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0xA8]) = val;
}

/* Address: 0x8020A0C4 | Size: 0x10 | Pattern: nullcheck_setter */
void fightWazaBiosSetZokusei(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xA6]) = val;
}

/* Address: 0x8020A0D4 | Size: 0x10 | Pattern: nullcheck_setter */
void fightWazaBiosSetIryoku(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0xA4]) = val;
}

/* Address: 0x8020A0E4 | Size: 0x10 | Pattern: nullcheck_setter */
void fightWazaBiosSetHitDamage(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0xA0]) = val;
}

/* Address: 0x8020A0F4 | Size: 0x10 | Pattern: nullcheck_setter */
void fightWazaBiosSetDamage(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x9C]) = val;
}

/* Address: 0x8020A104 | Size: 0x10 | Pattern: nullcheck_setter */
void fightWazaBiosSetDamageValue(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x99]) = val;
}

/* Address: 0x8020A114 | Size: 0x10 | Pattern: nullcheck_setter */
void fightWazaBiosSetCritical(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x98]) = val;
}

/* Address: 0x8020A124 | Size: 0x10 | Pattern: nullcheck_setter */
void fightWazaBiosSetTargetDataId(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x6]) = val;
}

/* Address: 0x8020A134 | Size: 0x10 | Pattern: nullcheck_setter */
void fightWazaBiosSetUseWazaDataId(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x4]) = val;
}

/* Address: 0x8020A144 | Size: 0x10 | Pattern: nullcheck_setter */
void fightWazaBiosSetMotoWazaDataId(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x2]) = val;
}

/* Address: 0x8020A154 | Size: 0x10 | Pattern: nullcheck_setter */
void fightWazaBiosSetWazaBanme(u8* ptr, u8 val) {
    if (ptr == NULL) { return; }
    *(u8*)(&ptr[0x0]) = val;
}

/* Address: 0x8020A164 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightWazaBiosGetAutoMakeFlag(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA9]);
}

/* Address: 0x8020A17C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightWazaBiosGetKaisuu(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0xA8]);
}

/* Address: 0x8020A194 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightWazaBiosGetZokusei(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xA6]);
}

/* Address: 0x8020A1AC | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightWazaBiosGetIryoku(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0xA4]);
}

/* Address: 0x8020A1C4 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightWazaBiosGetHitDamage(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0xA0]);
}

/* Address: 0x8020A1DC | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightWazaBiosGetDamage(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x9C]);
}

/* Address: 0x8020A1F4 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightWazaBiosGetDamageValue(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x99]);
}

/* Address: 0x8020A20C | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightWazaBiosGetCritical(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x98]);
}

/* 0x8020A224 | size: 0x34 | small */
#pragma push
#pragma peephole on
void* fightWazaBiosGetJoutaiPtr(void* base, u16 index) {
    if (base == 0) return 0;
    if (index >= 9) return 0;
    return (u8*)base + 0x8 + index * 16;
}
#pragma pop

/* Address: 0x8020A258 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightWazaBiosGetTargetDataId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x6]);
}

/* Address: 0x8020A270 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightWazaBiosGetUseWazaDataId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x4]);
}

/* Address: 0x8020A288 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightWazaBiosGetMotoWazaDataId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x2]);
}

/* Address: 0x8020A2A0 | Size: 0x18 | Pattern: nullcheck_getter */
u8 fightWazaBiosGetWazaBanme(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u8*)(&ptr[0x0]);
}

/* 0x8020A2B8 | size: 0x40 -- copy 0xAC bytes (43 u32s) */
void fightWazaBiosCopy(u32* dst, u32* src) {
    struct CopyBlk8020A2B8 { u32 data[43]; };
    if (dst == 0) return;
    if (src == 0) return;
    *(struct CopyBlk8020A2B8*)dst = *(struct CopyBlk8020A2B8*)src;
}

/* Address: 0x8020A2F8 | Size: 0x10 | Pattern: nullcheck_setter */
void fightItemBiosSetBuff(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x8]) = val;
}

/* Address: 0x8020A308 | Size: 0x10 | Pattern: nullcheck_setter */
void fightItemBiosSetCount(u8* ptr, u32 val) {
    if (ptr == NULL) { return; }
    *(u32*)(&ptr[0x4]) = val;
}

/* Address: 0x8020A318 | Size: 0x10 | Pattern: nullcheck_setter */
void fightItemBiosSetTargetDataId(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x2]) = val;
}

/* Address: 0x8020A328 | Size: 0x10 | Pattern: nullcheck_setter */
void fightItemBiosSetItemDataId(u8* ptr, u16 val) {
    if (ptr == NULL) { return; }
    *(u16*)(&ptr[0x0]) = val;
}

/* Address: 0x8020A338 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightItemBiosGetBuff(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x8]);
}

/* Address: 0x8020A350 | Size: 0x18 | Pattern: nullcheck_getter */
u32 fightItemBiosGetCount(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u32*)(&ptr[0x4]);
}

/* Address: 0x8020A368 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightItemBiosGetTargetDataId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x2]);
}

/* Address: 0x8020A380 | Size: 0x18 | Pattern: nullcheck_getter */
u16 fightItemBiosGetItemDataId(u8* ptr) {
    if (ptr == NULL) { return 0; }
    return *(u16*)(&ptr[0x0]);
}

/* Address: 0x8020A398 | Size: 0xe0 | Ghidra import */
void fightItemCreate(void)

{
    int r3;
    u16 r4;
    u16 r5;
    u32 r6;

    extern void fn_80142B24();
  if (r3 != 0) {
    fn_80142B24(r3,0,0x1e,0,0);
    fn_80142B24(r3,0,0x1f,0,0);
    fn_80142B24(r3,0,0x20,0, (void*)0xffffffff);
    fn_80142B24(r3,0,0x21,0,0);
    fn_80142B24(r3,0,0x1e,0,r4);
    fn_80142B24(r3,0,0x1f,0,r5);
    fn_80142B24(r3,0,0x20,0,r6);
  }
  return;
}

/* Address: 0x8020A478 | Size: 0x88 | Ghidra import */
#pragma push
#pragma peephole on
void fightItemInit(void* r3)
{
    extern void fn_80142B24();
    void* ctx;
    if ((ctx = r3) != NULL) {
        fn_80142B24(ctx, 0, 0x1e, 0, 0);
        fn_80142B24(ctx, 0, 0x1f, 0, 0);
        fn_80142B24(ctx, 0, 0x20, 0, (void*)0xffffffff);
        fn_80142B24(ctx, 0, 0x21, 0, 0);
    }
}
#pragma pop

/* 0x8020A500 | size: 0x40 */
u32 fn_8020A500(u16 idx) {
    ColosseumEventRow6* entry;
    idx = idx;
    if (idx >= lbl_80478D38) {
        entry = NULL;
    } else {
        entry = &lbl_80478D30[idx];
    }
    if (entry == NULL) { return 0; }
    return entry->nextIndex;
}

/* 0x8020A540 | size: 0x40 */
u32 fn_8020A540(u16 idx) {
    ColosseumEventRow6* entry;
    idx = idx;
    if (idx >= lbl_80478D38) {
        entry = NULL;
    } else {
        entry = &lbl_80478D30[idx];
    }
    if (entry == NULL) { return 0; }
    return entry->eventIndex;
}

/* 0x8020A580 | size: 0x40 */
u32 fn_8020A580(u16 idx) {
    ColosseumEventRow6* entry;
    idx = idx;
    if (idx >= lbl_80478D38) {
        entry = NULL;
    } else {
        entry = &lbl_80478D30[idx];
    }
    if (entry == NULL) { return 0; }
    return entry->mode;
}

/* 0x8020A5C0 | size: 0x70 */
s16 fn_8020A5C0(u16 index, u16 slot) {
    ColosseumEventPairRow* entry;
    ColosseumEventSubRow* sub;
    if (index >= lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index];
    }
    if (entry == NULL) {
        sub = NULL;
    } else if (slot >= 2) {
        sub = NULL;
    } else {
        sub = &entry->slots[slot];
    }
    if (sub == NULL) {
        return 0;
    }
    return sub->scaleDenominator;
}

/* 0x8020A630 | size: 0x70 */
s16 fn_8020A630(u16 index, u16 slot) {
    ColosseumEventPairRow* entry;
    ColosseumEventSubRow* sub;
    if (index >= lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index];
    }
    if (entry == NULL) {
        sub = NULL;
    } else if (slot >= 2) {
        sub = NULL;
    } else {
        sub = &entry->slots[slot];
    }
    if (sub == NULL) {
        return 0;
    }
    return sub->scaleNumerator;
}

/* 0x8020A6A0 | size: 0x70 */
u8 fn_8020A6A0(u16 index, u16 slot) {
    ColosseumEventPairRow* entry;
    ColosseumEventSubRow* sub;
    if (index >= lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index];
    }
    if (entry == NULL) {
        sub = NULL;
    } else if (slot >= 2) {
        sub = NULL;
    } else {
        sub = &entry->slots[slot];
    }
    if (sub == NULL) {
        return 0;
    }
    return sub->scaleMode;
}

/* 0x8020A710 | size: 0x70 */
u16 fn_8020A710(u16 index, u16 slot) {
    ColosseumEventPairRow* entry;
    ColosseumEventSubRow* sub;
    if (index >= lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index];
    }
    if (entry == NULL) {
        sub = NULL;
    } else if (slot >= 2) {
        sub = NULL;
    } else {
        sub = &entry->slots[slot];
    }
    if (sub == NULL) {
        return 0;
    }
    return sub->maxValue;
}

/* 0x8020A780 | size: 0x70 */
u16 fn_8020A780(u16 index, u16 slot) {
    ColosseumEventPairRow* entry;
    ColosseumEventSubRow* sub;
    if (index >= lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index];
    }
    if (entry == NULL) {
        sub = NULL;
    } else if (slot >= 2) {
        sub = NULL;
    } else {
        sub = &entry->slots[slot];
    }
    if (sub == NULL) {
        return 0;
    }
    return sub->minValue;
}

/* 0x8020A7F0 | size: 0x70 */
u8 fn_8020A7F0(u16 index, u16 slot) {
    ColosseumEventPairRow* entry;
    ColosseumEventSubRow* sub;
    if (index >= lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index];
    }
    if (entry == NULL) {
        sub = NULL;
    } else if (slot >= 2) {
        sub = NULL;
    } else {
        sub = &entry->slots[slot];
    }
    if (sub == NULL) {
        return 0;
    }
    return sub->valueMode;
}

/* fn_8020A860 | Size: 0x40 | Look up u16 field at offset 2 in 0x18-byte table */
u16 fn_8020A860(u16 index) {
    ColosseumEventPairRow* entry;
    if (index >= lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index];
    }
    if (entry == NULL) {
        return 0;
    }
    return entry->firstLinkIndex;
}

/* fn_8020A8A0 | Size: 0x40 | Look up u8 field at offset 0 in 0x18-byte table */
u8 fn_8020A8A0(u16 index) {
    ColosseumEventPairRow* entry;
    if (index >= lbl_80478D28) {
        entry = NULL;
    } else {
        entry = &lbl_80375A08[index];
    }
    if (entry == NULL) {
        return 0;
    }
    return entry->resultFuncId;
}

/* Address: 0x8020A8E0 | Size: 0x424 | Ghidra import */

int fn_8020A8E0(u32 r3,u32 r4)

{
    extern int _fadeEffectGetRandom__FUl();
    extern u32 fn_80135E44();
    extern u32 fightTargetDataBiosGetStatusKid();
    extern void fightTargetDataBiosGetPtr();
    extern int fightTargetGetPtr();
    extern u32 fightFloorGetStatus();
    extern u32 fn_8020A500();
    extern u32 fn_8020A540();
    extern u32 fn_8020A580();
    extern s16 fn_8020A5C0();
    extern s16 fn_8020A630();
    extern u8 fn_8020A6A0();
    extern u16 fn_8020A710();
    extern u16 fn_8020A780();
    extern u8 fn_8020A7F0();
    extern u16 fn_8020A860();
    extern u8 fn_8020A8A0();
  u32 uVar1;
  int iVar2;
  u32 uVar3;
  u8 bVar12;
  u32 uVar4;
  u32 uVar5;
  short sVar9;
  short sVar10;
  s8 cVar13;
  u16 uVar11;
  int iVar6;
  u32 uVar7;
  u32 uVar8;
  u8 bVar14;
  u32 local_48 [5];
  
  bVar14 = 0;
  do {
    if (1 < bVar14) {
      uVar1 = fn_8020A8A0(r3);
      if ((uVar1 & 0xff) < 7) {

        iVar2 = ((int (*)(void))**(void ***)((uVar1 & 0xff) * 4 + -0x7fc8a6ac))();
        return iVar2;
      }
      iVar2 = 0;
      uVar1 = fn_8020A860(r3);
      if ((uVar1 & 0xffff) == 0) {
        iVar2 = 0;
      }
      else {
        do {
          uVar3 = fn_8020A540(uVar1);
          for (bVar14 = 0; bVar14 < 2; bVar14 = bVar14 + 1) {
            uVar8 = 0;
            bVar12 = fn_8020A7F0(uVar3,bVar14);
            uVar4 = fn_8020A780(uVar3,bVar14);
            uVar5 = fn_8020A710(uVar3,bVar14);
            sVar9 = fn_8020A630(uVar3,bVar14);
            sVar10 = fn_8020A5C0(uVar3,bVar14);
            cVar13 = fn_8020A6A0(uVar3,bVar14);
            if (bVar12 == 2) {
              iVar6 = _fadeEffectGetRandom__FUl((uVar5 & 0xffff) - (uVar4 & 0xffff));
              uVar8 = (uVar4 & 0xffff) + iVar6;
LAB_00207c9c:
              if ((cVar13 == 1) && (uVar8 = uVar8 * (int)sVar9, sVar10 != 0)) {
                uVar8 = (int)uVar8 / (int)sVar10;
              }
            }
            else {
              if (bVar12 < 2) {
                if (bVar12 != 0) {
                  uVar8 = uVar4 & 0xffff;
                }
                goto LAB_00207c9c;
              }
              if (3 < bVar12) goto LAB_00207c9c;
              uVar11 = fightFloorGetStatus(0,0,0x14,0);
              iVar6 = fightTargetGetPtr(uVar4,r4,uVar11);
              if (iVar6 != 0) {
                fightTargetDataBiosGetPtr(uVar4);
                uVar7 = fightTargetDataBiosGetStatusKid();
                if (cVar13 == 0) {
                  uVar8 = fn_80135E44(uVar7,iVar6,sVar9,uVar5,sVar10);
                }
                else {
                  uVar8 = fn_80135E44(uVar7,iVar6,0,uVar5,0);
                }
                goto LAB_00207c9c;
              }
              uVar8 = 0;
            }
            local_48[bVar14] = uVar8;
          }
          uVar8 = fn_8020A8A0(uVar3);
          if ((uVar8 & 0xff) < 7) {

            iVar2 = ((int (*)(void))**(void ***)((uVar8 & 0xff) * 4 + -0x7fc8a6c8))();
            return iVar2;
          }
          bVar14 = fn_8020A580(uVar3);
          if (bVar14 == 2) {
            iVar2 = 0;
          }
          else if ((bVar14 < 2) && (bVar14 != 0)) {
            if (iVar2 == 0) {
              iVar2 = 0;
            }
            else {
              iVar2 = 1;
            }
          }
          uVar1 = fn_8020A500(uVar1);
        } while ((uVar1 & 0xffff) != 0);
      }
      return iVar2;
    }
    uVar1 = 0;
    bVar12 = fn_8020A7F0(r3,bVar14);
    uVar8 = fn_8020A780(r3,bVar14);
    uVar4 = fn_8020A710(r3,bVar14);
    sVar9 = fn_8020A630(r3,bVar14);
    sVar10 = fn_8020A5C0(r3,bVar14);
    cVar13 = fn_8020A6A0(r3,bVar14);
    if (bVar12 == 2) {
      iVar2 = _fadeEffectGetRandom__FUl((uVar4 & 0xffff) - (uVar8 & 0xffff));
      uVar1 = (uVar8 & 0xffff) + iVar2;
LAB_00207a34:
      if ((cVar13 == 1) && (uVar1 = uVar1 * (int)sVar9, sVar10 != 0)) {
        uVar1 = (int)uVar1 / (int)sVar10;
      }
    }
    else {
      if (bVar12 < 2) {
        if (bVar12 != 0) {
          uVar1 = uVar8 & 0xffff;
        }
        goto LAB_00207a34;
      }
      if (3 < bVar12) goto LAB_00207a34;
      uVar11 = fightFloorGetStatus(0,0,0x14,0);
      iVar2 = fightTargetGetPtr(uVar8,r4,uVar11);
      if (iVar2 != 0) {
        fightTargetDataBiosGetPtr(uVar8);
        uVar3 = fightTargetDataBiosGetStatusKid();
        if (cVar13 == 0) {
          uVar1 = fn_80135E44(uVar3,iVar2,sVar9,uVar4,sVar10);
        }
        else {
          uVar1 = fn_80135E44(uVar3,iVar2,0,uVar4,0);
        }
        goto LAB_00207a34;
      }
      uVar1 = 0;
    }
    uVar8 = (u32)bVar14;
    bVar14 = bVar14 + 1;
    local_48[uVar8 + 2] = uVar1;
  } while (1);
}
