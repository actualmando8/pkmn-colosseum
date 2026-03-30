    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stmw r27, 0x14(r1)
    addi r28, r3, 0x0
    lis r3, lbl_803FC620@ha
    mulli r5, r28, 0x110
    addi r0, r3, lbl_803FC620@l
    add r31, r0, r5
    lwz r29, 0xd0(r31)
    li r27, 0x0
    mr. r30, r4
    stw r27, 0xd0(r31)
    blt @L_800B4738
    mr r3, r31
    bl fn_800B1E0C
    lhz r0, 0xbc(r31)
    li r5, 0x4
    lwz r4, 0x10c(r31)
    slwi r0, r0, 6
    add r30, r3, r0
    addi r3, r30, 0x0
    bl memcpy
    lwz r4, 0x10c(r31)
    addi r3, r30, 0x4
    li r5, 0x2
    addi r4, r4, 0x4
    bl memcpy
    li r0, 0x4
    stb r0, 0x34(r30)
    li r0, -0x1
    lis r3, 0x8000
    stb r27, 0x35(r30)
    lhz r4, 0xbe(r31)
    sth r4, 0x36(r30)
    stb r27, 0x7(r30)
    stw r0, 0x2c(r30)
    sth r27, 0x30(r30)
    sth r27, 0x32(r30)
    stw r0, 0x3c(r30)
    lhz r0, 0x32(r30)
    clrrwi r0, r0, 2
    ori r0, r0, 0x1
    sth r0, 0x32(r30)
    lwz r4, 0xc0(r31)
    stw r27, 0x8(r4)
    lhz r0, 0x36(r30)
    lwz r4, 0xc0(r31)
    sth r0, 0x10(r4)
    lwz r0, 0xf8(r3)
    srwi r27, r0, 2
    bl OSGetTime
    addi r6, r27, 0x0
    li r5, 0x0
    bl fn_800C4928
    stw r4, 0x28(r30)
    addi r3, r28, 0x0
    addi r4, r29, 0x0
    bl fn_800B1FAC
    mr. r30, r3
    bge @L_800B4760
    @L_800B4738:
    addi r3, r31, 0x0
    addi r4, r30, 0x0
    bl fn_800B02F4
    cmplwi r29, 0x0
    beq @L_800B4760
    addi r12, r29, 0x0
    mtlr r12
    addi r3, r28, 0x0
    addi r4, r30, 0x0
    blrl
    @L_800B4760:
    lmw r27, 0x14(r1)
    lwz r0, 0x2c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
