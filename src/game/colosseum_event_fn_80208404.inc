    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    mr r27, r4
    mr r29, r5
    mr r28, r6
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    mr r3, r26
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r31, r3
    beq @L_80208540
    rlwinm. r0, r29, 0, 24, 31
    bne @L_80208468
    li r29, 0x3a
    b @L_8020848C
@L_80208468:
    cmplwi r0, 0x1
    bne @L_80208478
    li r29, 0x88
    b @L_8020848C
@L_80208478:
    cmplwi r0, 0x2
    bne @L_80208488
    li r29, 0x57
    b @L_8020848C
@L_80208488:
    li r29, 0xd9
@L_8020848C:
    rlwinm. r0, r28, 0, 24, 31
    bne @L_802084AC
    mr r3, r31
    mr r4, r29
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    b @L_80208540
@L_802084AC:
    cmplwi r0, 0x1
    bne @L_802084E4
    mr r3, r31
    mr r4, r29
    li r5, 0x4
    bl fn_801DA9E8
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80208540
    mr r3, r26
    mr r4, r30
    li r5, 0x1
    bl fn_80265598
    b @L_80208540
@L_802084E4:
    cmplwi r0, 0x2
    bne @L_8020850C
@L_802084EC:
    mr r3, r31
    mr r4, r29
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80208540
    bl fn_800F0308
    b @L_802084EC
@L_8020850C:
    cmplwi r0, 0x3
    bne @L_80208528
    mr r3, r31
    mr r4, r29
    li r5, 0x4
    bl fn_801DA8C4
    b @L_80208540
@L_80208528:
    cmplwi r0, 0x4
    bne @L_80208540
    mr r3, r31
    mr r4, r29
    li r5, 0x4
    bl fn_801DA9B4
@L_80208540:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
