    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r29, r4
    mr r31, r5
    mr r28, r3
    li r4, 0x0
    li r5, 0x43
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    mr r3, r28
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80237E94
    mr r3, r28
    mr r4, r29
    mr r5, r30
    bl fn_801F8424
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80237E84
    mr r3, r29
    li r4, 0x0
    bl fn_80207B8C
    b @L_80237E98
@L_80237E84:
    mr r3, r29
    li r4, 0x0
    bl fn_80207B8C
    b @L_80237E98
@L_80237E94:
    li r3, 0x9
@L_80237E98:
    rlwinm r4, r31, 0, 16, 31
    rlwinm r0, r3, 0, 16, 31
    cmplw r4, r0
    beq @L_80237F54
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    mr r3, r28
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80237F40
    mr r3, r28
    mr r4, r29
    mr r5, r30
    bl fn_801F8424
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80237F30
    mr r3, r29
    li r4, 0x1
    bl fn_80207B8C
    b @L_80237F44
@L_80237F30:
    mr r3, r29
    li r4, 0x1
    bl fn_80207B8C
    b @L_80237F44
@L_80237F40:
    li r3, 0x9
@L_80237F44:
    rlwinm r4, r31, 0, 16, 31
    rlwinm r0, r3, 0, 16, 31
    cmplw r4, r0
    bne @L_80237F5C
@L_80237F54:
    li r3, 0x1
    b @L_80237F60
@L_80237F5C:
    li r3, 0x0
@L_80237F60:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
