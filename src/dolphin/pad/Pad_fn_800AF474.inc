    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stmw r27, 0x14(r1)
    addi r27, r3, 0x0
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    mulli r4, r27, 0x110
    addi r3, r27, 0x0
    add r29, r0, r4
    bl EXIDeselect
    cntlzw r0, r3
    addi r3, r27, 0x0
    srwi r31, r0, 5
    bl EXIUnlock
    lwz r0, 0xc8(r29)
    cmplwi r0, 0x0
    mr r28, r0
    beq @L_800AF508
    li r30, 0x0
    cmpwi r31, 0x0
    stw r30, 0xc8(r29)
    bne @L_800AF4E4
    mr r3, r27
    bl fn_80098944
    cmpwi r3, 0x0
    beq @L_800AF4E4
    li r30, 0x1
    @L_800AF4E4:
    cmpwi r30, 0x0
    beq @L_800AF4F4
    li r4, 0x0
    b @L_800AF4F8
    @L_800AF4F4:
    li r4, -0x3
    @L_800AF4F8:
    addi r12, r28, 0x0
    mtlr r12
    addi r3, r27, 0x0
    blrl
    @L_800AF508:
    lmw r27, 0x14(r1)
    lwz r0, 0x2c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
