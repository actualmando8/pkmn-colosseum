    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x40(r1)
    stmw r26, 0x28(r1)
    addi r29, r3, 0x0
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    mulli r8, r29, 0x110
    addi r26, r4, 0x0
    addi r30, r5, 0x0
    addi r31, r6, 0x0
    addi r27, r7, 0x0
    addi r3, r29, 0x0
    add r28, r0, r8
    li r4, 0x0
    li r5, 0x4
    bl EXISelect
    cmpwi r3, 0x0
    bne @L_800B06E8
    li r3, -0x3
    b @L_800B07C4
    @L_800B06E8:
    clrrwi r26, r26, 12
    addi r3, r1, 0x1c
    li r4, 0x0
    li r5, 0x5
    bl memset
    li r0, 0x52
    cmpwi r27, 0x0
    stb r0, 0x1c(r1)
    bne @L_800B0730
    extrwi r0, r26, 2, 1
    stb r0, 0x1d(r1)
    extrwi r0, r26, 8, 3
    extrwi r3, r26, 2, 11
    stb r0, 0x1e(r1)
    extrwi r0, r26, 7, 13
    stb r3, 0x1f(r1)
    stb r0, 0x20(r1)
    b @L_800B0740
    @L_800B0730:
    srwi r0, r26, 24
    stb r0, 0x1d(r1)
    extrwi r0, r26, 8, 8
    stb r0, 0x1e(r1)
    @L_800B0740:
    addi r3, r29, 0x0
    addi r4, r1, 0x1c
    li r5, 0x5
    li r6, 0x1
    bl fn_80098368
    lwz r4, 0x80(r28)
    cntlzw r0, r3
    lwz r5, 0x14(r28)
    addi r3, r29, 0x0
    srwi r28, r0, 5
    addi r4, r4, 0x200
    li r6, 0x1
    bl fn_80098368
    cntlzw r0, r3
    srwi r0, r0, 5
    addi r3, r29, 0x0
    addi r4, r30, 0x0
    addi r5, r31, 0x0
    or r28, r28, r0
    li r6, 0x0
    bl fn_80098368
    cntlzw r0, r3
    srwi r0, r0, 5
    addi r3, r29, 0x0
    or r28, r28, r0
    bl EXIDeselect
    cntlzw r0, r3
    srwi r0, r0, 5
    or. r28, r28, r0
    beq @L_800B07C0
    li r3, -0x3
    b @L_800B07C4
    @L_800B07C0:
    li r3, 0x0
    @L_800B07C4:
    lmw r26, 0x28(r1)
    lwz r0, 0x44(r1)
    addi r1, r1, 0x40
    mtlr r0
    blr
