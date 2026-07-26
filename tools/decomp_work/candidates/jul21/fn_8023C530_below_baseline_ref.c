typedef u32 (*HitFunc)(void* ctx, u32 arg2, u32 moveId, void* poke);

extern u32 fightTrainerGetStatus(u32 a, u32 b, u32 c, u32 d);
extern u32 wazaGetStatus(u32 a, u32 b, u32 c, u32 d);
extern u32 fightFloorGetFightTrainerFightOutPokemonPtrAry(u32 a, void* ctx, u32* ary, u32 b, u32 c);
extern u8 fightOutPokemonCheckFightOut(void* poke);
extern u8 fn_80237F74(void* ctx, void* poke, u32 field);
extern u8 fn_802026E4(void* poke, u32 a);
extern s16 fn_80239500(void* ctx, u32 moveId);
extern u16 fn_802395C8(void* ctx, u32 moveId, u32 arg2);
extern u32 fightTrainerAiWazaHitNull(void* ctx, u32 arg2, u32 moveId, void* poke);

u32 fn_8023C530(void* ctx, u32 arg2, u32 moveId, void* poke)
{
    u32 ary[8];
    HitFunc hitFunc;
    u32 count;
    u32 result;
    s16 range;
    u16 type;
    u8 flag;
    u16 n;
    u16 i;

    if ((u8)fightTrainerGetStatus(0, (u16)fightTrainerGetStatus(0, (u16)fightTrainerGetStatus((u32)ctx, 0, 0x43, 0), 2, 0), 0x34, 0) == 1) {
        hitFunc = (HitFunc)wazaGetStatus(0, moveId, 0x1d, 0);
        if (hitFunc == (HitFunc)0) {
            hitFunc = fightTrainerAiWazaHitNull;
        }
    } else {
        return 1;
    }
    count = fightFloorGetFightTrainerFightOutPokemonPtrAry(0, ctx, ary, 0, 1);
    switch ((u8)wazaGetStatus(0, moveId, 5, 0)) {
    case 0:
        range = fn_80239500(ctx, moveId);
        type = fn_802395C8(ctx, moveId, arg2);
        flag = 1;
        if (fightOutPokemonCheckFightOut(poke) == 0) {
            flag = 0;
        } else if ((u16)moveId != 0 && (u16)moveId != 0x165) {
            if (fn_80237F74(ctx, poke, 10) == 1 && type == 13 && range != 0) {
                flag = 0;
            }
            if (fn_80237F74(ctx, poke, 11) == 1 && type == 11 && range != 0) {
                flag = 0;
            }
            if (fn_80237F74(ctx, poke, 18) == 1 && type == 10 && fn_802026E4(poke, 7) == 0) {
                flag = 0;
            }
            if (fn_80237F74(ctx, poke, 43) == 1 && (u8)wazaGetStatus(0, moveId, 23, 0) == 1) {
                flag = 0;
            }
        }
        if (flag == 0) {
            return 0;
        }
        return hitFunc(ctx, arg2, moveId, poke);
    case 1:
        n = (u16)count;
        for (i = 0; i < n; i++) {
            range = fn_80239500(ctx, moveId);
            type = fn_802395C8(ctx, moveId, arg2);
            flag = 1;
            if (fightOutPokemonCheckFightOut(poke) == 0) {
                flag = 0;
            } else if ((u16)moveId != 0 && (u16)moveId != 0x165) {
                if (fn_80237F74(ctx, poke, 10) == 1 && type == 13 && range != 0) {
                    flag = 0;
                }
                if (fn_80237F74(ctx, poke, 11) == 1 && type == 11 && range != 0) {
                    flag = 0;
                }
                if (fn_80237F74(ctx, poke, 18) == 1 && type == 10 && fn_802026E4(poke, 7) == 0) {
                    flag = 0;
                }
                if (fn_80237F74(ctx, poke, 43) == 1 && (u8)wazaGetStatus(0, moveId, 23, 0) == 1) {
                    flag = 0;
                }
            }
            if (flag != 0) {
                result = hitFunc(ctx, arg2, moveId, (void*)ary[i]);
            } else {
                result = 0;
            }
            if ((u8)result == 1) {
                return result;
            }
        }
        return result;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        range = fn_80239500(ctx, moveId);
        type = fn_802395C8(ctx, moveId, arg2);
        flag = 1;
        if (fightOutPokemonCheckFightOut(poke) == 0) {
            flag = 0;
        } else if ((u16)moveId != 0 && (u16)moveId != 0x165) {
            if (fn_80237F74(ctx, poke, 10) == 1 && type == 13 && range != 0) {
                flag = 0;
            }
            if (fn_80237F74(ctx, poke, 11) == 1 && type == 11 && range != 0) {
                flag = 0;
            }
            if (fn_80237F74(ctx, poke, 18) == 1 && type == 10 && fn_802026E4(poke, 7) == 0) {
                flag = 0;
            }
            if (fn_80237F74(ctx, poke, 43) == 1 && (u8)wazaGetStatus(0, moveId, 23, 0) == 1) {
                flag = 0;
            }
        }
        if (flag == 0) {
            return 0;
        }
        return hitFunc(ctx, arg2, moveId, poke);
    default:
        return 0;
    }
}