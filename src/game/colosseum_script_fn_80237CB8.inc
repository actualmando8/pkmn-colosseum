    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    mr r27, r4
    mr r28, r5
    li r30, 0x0
    li r29, 0x0
    b @L_80237D98
@L_80237CE0:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    mr r3, r26
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
    bne @L_80237D78
    mr r3, r26
    mr r4, r27
    mr r5, r31
    bl fn_801F8424
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80237D68
    mr r3, r27
    mr r4, r29
    bl fn_80207B8C
    b @L_80237D7C
@L_80237D68:
    mr r3, r27
    mr r4, r29
    bl fn_80207B8C
    b @L_80237D7C
@L_80237D78:
    li r3, 0x9
@L_80237D7C:
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x9
    beq @L_80237D94
    clrlslwi r0, r30, 16, 1
    addi r30, r30, 0x1
    sthx r3, r28, r0
@L_80237D94:
    addi r29, r29, 0x1
@L_80237D98:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_80237CE0
    mr r3, r30
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
