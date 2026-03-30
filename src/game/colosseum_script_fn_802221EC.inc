    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    mr r27, r4
    mr r31, r5
    mr r28, r6
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r27
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    beq @L_802222E0
    mr r4, r26
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802222E0
    mr r3, r30
    mr r4, r26
    li r5, 0x4
    bl fn_801DA9E8
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80222284
    mr r3, r27
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
@L_80222284:
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802222E0
    rlwinm r31, r26, 0, 16, 31
@L_80222294:
    mr r3, r30
    mr r4, r31
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802222B4
    bl fn_800F0308
    b @L_80222294
@L_802222B4:
    mr r3, r30
    mr r4, r31
    li r5, 0x4
    bl fn_801DA8C4
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802222E0
    mr r3, r27
    mr r4, r29
    li r5, 0x0
    bl fn_8026532C
@L_802222E0:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
