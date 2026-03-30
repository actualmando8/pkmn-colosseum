    nofralloc
    mflr r0
    li r5, 0x4
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    addi r31, r4, 0x0
    li r4, 0x0
    stw r30, 0x18(r1)
    addi r30, r3, 0x0
    bl EXISelect
    cmpwi r3, 0x0
    bne @L_800AF5D8
    li r3, -0x3
    b @L_800AF648
    @L_800AF5D8:
    cmpwi r31, 0x0
    beq @L_800AF5E8
    lis r0, 0x8101
    b @L_800AF5EC
    @L_800AF5E8:
    lis r0, 0x8100
    @L_800AF5EC:
    stw r0, 0x10(r1)
    addi r3, r30, 0x0
    addi r4, r1, 0x10
    li r5, 0x2
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
    beq @L_800AF644
    li r3, -0x3
    b @L_800AF648
    @L_800AF644:
    li r3, 0x0
    @L_800AF648:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
