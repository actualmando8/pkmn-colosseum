    nofralloc
    mflr r0
    cmpwi r5, 0x13
    stw r0, 0x4(r1)
    li r0, 0x0
    addi r8, r3, 0x0
    stwu r1, -0x30(r1)
    stw r31, 0x2c(r1)
    rlwinm r31, r5, 0, 28, 31
    stw r30, 0x28(r1)
    addi r30, r6, 0x0
    lwz r7, -29632(r13)
    stb r0, 0x200(r7)
    addi r7, r4, 0x0
    bne @L_800B9734
    li r31, 0xb
@L_800B9734:
    cmpwi r5, 0x26
    beq @L_800B9754
    bge @L_800B9770
    cmpwi r5, 0x4
    bge @L_800B9770
    cmpwi r5, 0x0
    bge @L_800B9754
    b @L_800B9770
@L_800B9754:
    lwz r3, -29632(r13)
    lwzu r0, 0x1fc(r3)
    rlwinm r0, r0, 0, 17, 14
    oris r0, r0, 0x1
    ori r0, r0, 0x8000
    stw r0, 0x0(r3)
    b @L_800B9784
@L_800B9770:
    lwz r3, -29632(r13)
    lwzu r0, 0x1fc(r3)
    rlwinm r0, r0, 0, 17, 14
    oris r0, r0, 0x1
    stw r0, 0x0(r3)
@L_800B9784:
    rlwinm r4, r5, 0, 27, 27
    lwz r3, -29632(r13)
    subi r0, r4, 0x10
    cntlzw r0, r0
    rlwinm r0, r0, 27, 24, 31
    stb r0, 0x200(r3)
    addi r4, r8, 0x0
    addi r6, r1, 0x20
    lwzu r0, 0x1fc(r3)
    addi r8, r1, 0x18
    rlwinm r0, r0, 0, 29, 27
    rlwimi r0, r31, 0, 28, 28
    stw r0, 0x0(r3)
    addi r3, r5, 0x0
    addi r5, r7, 0x0
    rlwinm r31, r31, 0, 29, 31
    addi r7, r1, 0x1c
    bl fn_800BA91C
    lwz r3, -29632(r13)
    li r0, 0x0
    stw r0, 0x1f8(r3)
    addi r7, r3, 0x1f8
    addi r8, r3, 0x1fc
    lwz r5, 0x20(r1)
    clrlslwi r3, r30, 24, 9
    lwz r4, 0x18(r1)
    rlwinm r0, r31, 4, 0, 27
    lwz r6, 0x0(r7)
    mullw r4, r5, r4
    rlwinm r5, r6, 0, 0, 21
    or r4, r5, r4
    stw r4, 0x0(r7)
    lwz r4, 0x0(r7)
    rlwinm r4, r4, 0, 8, 31
    oris r4, r4, 0x4d00
    stw r4, 0x0(r7)
    lwz r4, 0x0(r8)
    rlwinm r4, r4, 0, 23, 21
    or r3, r4, r3
    stw r3, 0x0(r8)
    lwz r3, 0x0(r8)
    rlwinm r3, r3, 0, 28, 24
    or r0, r3, r0
    stw r0, 0x0(r8)
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
