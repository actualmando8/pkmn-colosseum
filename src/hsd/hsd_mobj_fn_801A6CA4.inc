    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_801A6CCC
    la r3, lbl_8047DC18(r2)
    li r4, 0x495
    la r5, lbl_8047DC20(r2)
    bl fn_80196E10
    @L_801A6CCC:
    lwz r3, lbl_8047B2DC(r13)
    b @L_801A6CE0
    @L_801A6CD4:
    cmplw r3, r31
    beq @L_801A6CF4
    lwz r3, 0x8(r3)
    @L_801A6CE0:
    cmplwi r3, 0x0
    bne @L_801A6CD4
    lwz r0, lbl_8047B2DC(r13)
    stw r0, 0x8(r31)
    stw r31, lbl_8047B2DC(r13)
    @L_801A6CF4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
