    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r29, r3
    bne @L_801EF154
    li r3, 0x0
    li r4, 0xf
    bl fn_80129280
    mr r29, r3
@L_801EF154:
    li r30, 0x0
    li r28, 0x0
    stw r30, 0x0(r29)
    li r31, 0x0
@L_801EF164:
    addi r5, r30, 0x4
    addi r27, r31, 0x490
    add r5, r29, r5
    li r4, 0x0
    stb r4, 0x0(r5)
    add r27, r29, r27
    li r0, -0x1
    rlwinm r3, r28, 0, 16, 31
    sth r4, 0x2(r5)
    stw r4, 0x4(r5)
    stw r4, 0x8(r5)
    stb r4, 0xc(r27)
    sth r4, 0x0(r27)
    sth r0, 0x14(r27)
    bl fn_801EEF08
    sth r3, 0x16(r27)
    li r0, 0x0
    rlwinm r3, r28, 0, 16, 31
    stb r0, 0xd(r27)
    bl fn_801EEFF4
    sth r3, 0x10(r27)
    addi r31, r31, 0x18
    addi r30, r30, 0xc
    stb r28, 0xe(r27)
    addi r28, r28, 0x1
    cmpwi r28, 0x61
    blt @L_801EF164
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
