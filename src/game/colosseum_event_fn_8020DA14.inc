    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r29, r4
    mr r27, r3
    mr r3, r29
    bl fn_8020D9E8
    bl fn_8020D9D0
    mr r28, r3
    mr r3, r29
    bl fn_8020D9E8
    bl fn_8020D9B8
    mr r30, r3
    mr r3, r29
    bl fn_8020D9E8
    bl fn_8020D9A0
    mr r31, r3
    li r29, 0x0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r5, r3, 0, 16, 31
    mr r3, r30
    mr r4, r27
    bl fn_801F02AC
    mr r30, r3
    mr r3, r28
    mr r4, r27
    bl fn_8020A8E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020DAB8
    mr r4, r30
    mr r5, r27
    rlwinm r3, r31, 0, 16, 31
    li r6, 0x0
    bl fn_80136078
    li r29, 0x1
@L_8020DAB8:
    mr r3, r29
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
