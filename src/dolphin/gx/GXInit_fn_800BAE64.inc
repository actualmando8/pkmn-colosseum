    nofralloc
    mflr r0
    addi r7, r13, -32152
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    lis r31, 0xcc01
    stw r30, 0x20(r1)
    li r30, 0x61
    stw r29, 0x1c(r1)
    addi r29, r5, 0x0
    addi r5, r13, -32136
    stw r28, 0x18(r1)
    mr r28, r3
    lwz r6, 0x0(r3)
    addi r3, r13, -32168
    lbzx r0, r3, r29
    addi r3, r13, -32160
    rlwinm r0, r0, 24, 0, 7
    rlwimi r0, r6, 0, 8, 31
    stw r0, 0x0(r28)
    addi r6, r13, -32144
    lbzx r0, r3, r29
    addi r3, r13, -32128
    lwz r8, 0x4(r28)
    rlwinm r0, r0, 24, 0, 7
    rlwimi r0, r8, 0, 8, 31
    stw r0, 0x4(r28)
    lbzx r0, r7, r29
    lwz r8, 0x8(r28)
    rlwinm r0, r0, 24, 0, 7
    rlwimi r0, r8, 0, 8, 31
    stw r0, 0x8(r28)
    lbzx r0, r6, r29
    lwz r7, 0x0(r4)
    rlwinm r0, r0, 24, 0, 7
    rlwimi r0, r7, 0, 8, 31
    stw r0, 0x0(r4)
    lbzx r0, r5, r29
    lwz r6, 0x4(r4)
    rlwinm r0, r0, 24, 0, 7
    rlwimi r0, r6, 0, 8, 31
    stw r0, 0x4(r4)
    lbzx r0, r3, r29
    lwz r5, 0xc(r28)
    rlwinm r0, r0, 24, 0, 7
    rlwimi r0, r5, 0, 8, 31
    stw r0, 0xc(r28)
    stb r30, -0x8000(r31)
    lwz r0, 0x0(r28)
    stw r0, -0x8000(r31)
    stb r30, -0x8000(r31)
    lwz r0, 0x4(r28)
    stw r0, -0x8000(r31)
    stb r30, -0x8000(r31)
    lwz r0, 0x8(r28)
    stw r0, -0x8000(r31)
    stb r30, -0x8000(r31)
    lwz r0, 0x0(r4)
    stw r0, -0x8000(r31)
    stb r30, -0x8000(r31)
    lwz r0, 0x4(r4)
    stw r0, -0x8000(r31)
    stb r30, -0x8000(r31)
    lwz r0, 0xc(r28)
    stw r0, -0x8000(r31)
    lbz r0, 0x1f(r28)
    rlwinm. r0, r0, 0, 30, 30
    bne @L_800BAFAC
    lwz r4, -29632(r13)
    lwz r3, 0x18(r28)
    lwz r12, 0x414(r4)
    mtlr r12
    blrl
    addi r4, r13, -32120
    lwz r5, 0x4(r3)
    lbzx r0, r4, r29
    rlwinm r0, r0, 24, 0, 7
    rlwimi r0, r5, 0, 8, 31
    stw r0, 0x4(r3)
    stb r30, -0x8000(r31)
    lwz r0, 0x4(r3)
    stw r0, -0x8000(r31)
@L_800BAFAC:
    lwz r5, -29632(r13)
    rlwinm r4, r29, 2, 0, 29
    lwz r3, 0x8(r28)
    li r0, 0x0
    add r4, r5, r4
    stw r3, 0x45c(r4)
    lwz r3, 0x0(r28)
    stw r3, 0x47c(r4)
    lwz r3, 0x4f4(r5)
    ori r3, r3, 0x1
    stw r3, 0x4f4(r5)
    sth r0, 0x2(r5)
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    lwz r28, 0x18(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
