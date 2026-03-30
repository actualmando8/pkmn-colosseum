    nofralloc
    mflr r0
    li r4, 0x0
    stw r0, 0x4(r1)
    li r5, 0x4
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    addi r30, r3, 0x0
    bl EXISelect
    cmpwi r3, 0x0
    bne @L_800AF784
    li r3, -0x3
    b @L_800AF7E4
    @L_800AF784:
    lis r0, 0x8900
    stw r0, 0xc(r1)
    addi r3, r30, 0x0
    addi r4, r1, 0xc
    li r5, 0x1
    li r6, 0x1
    li r7, 0x0
    bl EXIImm
    cntlzw r0, r3
    addi r3, r30, 0x0
    srwi r31, r0, 5
    bl EXISync
    cntlzw r0, r3
    srwi r0, r0, 5
    addi r3, r30, 0x0
    or r31, r31, r0
    bl EXIDeselect
    cntlzw r0, r3
    srwi r0, r0, 5
    or. r31, r31, r0
    beq @L_800AF7E0
    li r3, -0x3
    b @L_800AF7E4
    @L_800AF7E0:
    li r3, 0x0
    @L_800AF7E4:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
