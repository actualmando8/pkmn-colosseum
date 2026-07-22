/**
 * @file fight_out_pokemon_candidate_80202810.c
 * @brief fightOutPokemon + fightPokemon candidate prefix, address range
 *        0x80202810-0x8020355C, 9 functions.
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

void fn_80202C1C(u32 r3,u32 r4)

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
  u8 bVar1;
  u16 uVar2;
  u16 uVar3;
  u32 uVar4;
  u8 cVar9;
  u32 iVar5;
  u16 sVar8;
  u32 iVar6;
  u32 iVar7;
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
          if (iVar5 == 0) {
            bVar1 = 0;
            goto initial_valid_done;
          }
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
              if (iVar6 == 0) {
                bVar1 = 0;
                goto initial_valid_done;
              }
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
                        if ((s32)iVar6 < 0) {
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
              if (!bVar1) {
                bVar1 = 0;
              }
              else {
                bVar1 = 1;
              }
            }
          }
        initial_valid_done:
          if (!bVar1) {
            bVar1 = 0;
            goto initial_stage_done;
          }
          {
            iVar6 = (int)pokemonGetStatus(iVar5,0,0x120,0);
            if ((s32)iVar6 == 1) {
              bVar1 = 0;
            }
            else {
              iVar6 = (int)pokemonGetStatus(iVar5,0,0xd6,0);
              if (iVar6 == 0) {
                bVar1 = 0;
              }
              else {
                if (iVar6 == 0) {
                  bVar1 = 0;
                  goto second_valid_done;
                }
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
                          if ((s32)iVar7 < 0) {
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
              second_valid_done:
                if (!bVar1) {
                  bVar1 = 0;
                  goto second_stage_done;
                }
                {
                  iVar7 = (int)pokemonGetStatus(iVar6,0,0xd2,0);
                  if ((s32)iVar7 == 1) {
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
              second_stage_done:
                ;
              }
              if (!bVar1) {
                bVar1 = 0;
              }
              else {
                bVar1 = 1;
              }
            }
          }
        initial_stage_done:
          ;
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
                if (iVar6 == 0) {
                  bVar1 = 0;
                  goto third_valid_done;
                }
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
                          if ((s32)iVar6 < 0) {
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
                if (!bVar1) {
                  bVar1 = 0;
                }
                else {
                  bVar1 = 1;
                }
              }
            }
          }
        third_valid_done:
          if (bVar1) {
            iVar6 = (int)pokemonGetStatus(r3,0,0x122,0);
            iVar7 = fightOutPokemonEnemySearchAry(iVar6,4,iVar5);
            if (iVar7 == 0) {
              for (uVar10 = 0; uVar10 < 4; uVar10++) {
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
#pragma scheduling on
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
