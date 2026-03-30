    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    li r5, 0x14
    li r6, 0x0
    stw r0, 0x44(r1)
    stmw r21, 0x14(r1)
    mr r28, r3
    mr r26, r4
    li r3, 0x0
    li r4, 0x0
    bl fn_801F54A4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r24, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x18
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r25, r3, 0, 16, 31
    li r31, 0x0
    b @L_80203178
@L_80202C80:
    mr r3, r26
    mr r6, r31
    li r4, 0x0
    li r5, 0x7
    bl fn_801F76B8
    mr r27, r3
    bl fn_801FA634
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80203174
    li r30, 0x0
    b @L_80203168
@L_80202CAC:
    mr r3, r27
    mr r6, r30
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr. r29, r3
    bne @L_80202CD0
    li r0, 0x0
    b @L_80202FB0
@L_80202CD0:
    bne @L_80202CDC
    li r0, 0x0
    b @L_80202DF8
@L_80202CDC:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80202CF4
    li r0, 0x0
    b @L_80202DF8
@L_80202CF4:
    mr r3, r29
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr. r21, r3
    bne @L_80202D18
    li r0, 0x0
    b @L_80202DF8
@L_80202D18:
    bne @L_80202D24
    li r0, 0x0
    b @L_80202DE4
@L_80202D24:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80202D3C
    li r0, 0x0
    b @L_80202DE4
@L_80202D3C:
    mr r3, r21
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80202D60
    li r0, 0x0
    b @L_80202DE4
@L_80202D60:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80202D74
    li r0, 0x0
    b @L_80202DE4
@L_80202D74:
    cmplwi r21, 0x0
    bne @L_80202D84
    li r3, 0x0
    b @L_80202D98
@L_80202D84:
    mr r3, r21
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80202D98:
    cmplwi r3, 0x0
    bne @L_80202DA8
    li r0, 0x0
    b @L_80202DE4
@L_80202DA8:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80202DBC
    li r0, 0x0
    b @L_80202DE4
@L_80202DBC:
    mr r3, r21
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_80202DE0
    li r0, 0x0
    b @L_80202DE4
@L_80202DE0:
    li r0, 0x1
@L_80202DE4:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80202DF4
    li r0, 0x0
    b @L_80202DF8
@L_80202DF4:
    li r0, 0x1
@L_80202DF8:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80202E08
    li r0, 0x0
    b @L_80202FB0
@L_80202E08:
    mr r3, r29
    li r4, 0x0
    li r5, 0x120
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x1
    bne @L_80202E2C
    li r0, 0x0
    b @L_80202FB0
@L_80202E2C:
    mr r3, r29
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr. r23, r3
    bne @L_80202E50
    li r0, 0x0
    b @L_80202F9C
@L_80202E50:
    bne @L_80202E5C
    li r0, 0x0
    b @L_80202F1C
@L_80202E5C:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80202E74
    li r0, 0x0
    b @L_80202F1C
@L_80202E74:
    mr r3, r23
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80202E98
    li r0, 0x0
    b @L_80202F1C
@L_80202E98:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80202EAC
    li r0, 0x0
    b @L_80202F1C
@L_80202EAC:
    cmplwi r23, 0x0
    bne @L_80202EBC
    li r3, 0x0
    b @L_80202ED0
@L_80202EBC:
    mr r3, r23
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80202ED0:
    cmplwi r3, 0x0
    bne @L_80202EE0
    li r0, 0x0
    b @L_80202F1C
@L_80202EE0:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80202EF4
    li r0, 0x0
    b @L_80202F1C
@L_80202EF4:
    mr r3, r23
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_80202F18
    li r0, 0x0
    b @L_80202F1C
@L_80202F18:
    li r0, 0x1
@L_80202F1C:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80202F2C
    li r0, 0x0
    b @L_80202F9C
@L_80202F2C:
    mr r3, r23
    li r4, 0x0
    li r5, 0xd2
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x1
    bne @L_80202F50
    li r0, 0x0
    b @L_80202F9C
@L_80202F50:
    cmplwi r23, 0x0
    bne @L_80202F60
    li r3, 0x0
    b @L_80202F74
@L_80202F60:
    mr r3, r23
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80202F74:
    cmplwi r3, 0x0
    bne @L_80202F84
    li r0, 0x0
    b @L_80202F9C
@L_80202F84:
    bl fn_801233F4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80202F98
    li r0, 0x0
    b @L_80202F9C
@L_80202F98:
    li r0, 0x1
@L_80202F9C:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80202FAC
    li r0, 0x0
    b @L_80202FB0
@L_80202FAC:
    li r0, 0x1
@L_80202FB0:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_80203164
    cmplwi r28, 0x0
    beq @L_80203164
    cmplwi r29, 0x0
    bne @L_80202FD0
    li r0, 0x0
    b @L_802030EC
@L_80202FD0:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80202FE8
    li r0, 0x0
    b @L_802030EC
@L_80202FE8:
    mr r3, r29
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr. r21, r3
    bne @L_8020300C
    li r0, 0x0
    b @L_802030EC
@L_8020300C:
    bne @L_80203018
    li r0, 0x0
    b @L_802030D8
@L_80203018:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80203030
    li r0, 0x0
    b @L_802030D8
@L_80203030:
    mr r3, r21
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80203054
    li r0, 0x0
    b @L_802030D8
@L_80203054:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80203068
    li r0, 0x0
    b @L_802030D8
@L_80203068:
    cmplwi r21, 0x0
    bne @L_80203078
    li r3, 0x0
    b @L_8020308C
@L_80203078:
    mr r3, r21
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_8020308C:
    cmplwi r3, 0x0
    bne @L_8020309C
    li r0, 0x0
    b @L_802030D8
@L_8020309C:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802030B0
    li r0, 0x0
    b @L_802030D8
@L_802030B0:
    mr r3, r21
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_802030D4
    li r0, 0x0
    b @L_802030D8
@L_802030D4:
    li r0, 0x1
@L_802030D8:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_802030E8
    li r0, 0x0
    b @L_802030EC
@L_802030E8:
    li r0, 0x1
@L_802030EC:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_80203164
    mr r3, r28
    li r4, 0x0
    li r5, 0x122
    li r6, 0x0
    bl fn_8012640C
    mr r23, r3
    mr r5, r29
    li r4, 0x4
    bl fn_8020E57C
    cmplwi r3, 0x0
    bne @L_80203164
    li r22, 0x0
    b @L_80203158
@L_80203128:
    rlwinm r0, r22, 0, 16, 31
    mulli r0, r0, 0xc
    add r21, r23, r0
    mr r3, r21
    bl fn_8020E614
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80203154
    mr r3, r21
    mr r4, r29
    bl fn_8020E640
    b @L_80203164
@L_80203154:
    addi r22, r22, 0x1
@L_80203158:
    rlwinm r0, r22, 0, 16, 31
    cmplwi r0, 0x4
    blt @L_80203128
@L_80203164:
    addi r30, r30, 0x1
@L_80203168:
    rlwinm r0, r30, 0, 16, 31
    cmplw r0, r25
    blt @L_80202CAC
@L_80203174:
    addi r31, r31, 0x1
@L_80203178:
    rlwinm r0, r31, 0, 16, 31
    cmplw r0, r24
    blt @L_80202C80
    lmw r21, 0x14(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
