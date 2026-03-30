    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r4
    mr r27, r3
    mr r29, r5
    mr r31, r6
    mr r30, r7
    mr r3, r28
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm. r0, r31, 0, 16, 31
    mr r31, r3
    bne @L_80210DE0
    mr r3, r28
    mr r4, r27
    mr r6, r30
    li r5, 0x1
    bl fn_80208750
    mr r3, r29
    mr r4, r27
    mr r6, r30
    li r5, 0x2
    bl fn_80208750
    mr r3, r28
    mr r4, r27
    mr r6, r30
    li r5, 0x3
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r28
    mr r4, r27
    li r5, 0x1
    li r6, 0x1
    bl fn_802085C4
    li r3, 0x12
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r29
    mr r4, r27
    li r5, 0x2
    li r6, 0x0
    bl fn_802085C4
    b @L_80210E40
@L_80210DE0:
    cmplwi r0, 0x1
    bne @L_80210E40
    mr r3, r28
    mr r4, r27
    mr r6, r30
    li r5, 0x3
    bl fn_80208750
    cmpwi r31, 0x0
    ble @L_80210E1C
    mr r4, r28
    li r3, 0x32
    li r5, 0x0
    li r6, 0x1
    bl fn_802221EC
    b @L_80210E40
@L_80210E1C:
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r28
    mr r4, r27
    li r5, 0x3
    li r6, 0x0
    bl fn_802085C4
@L_80210E40:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
