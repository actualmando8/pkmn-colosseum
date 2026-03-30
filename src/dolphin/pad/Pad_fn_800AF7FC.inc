    nofralloc
    mflr r0
    lis r4, lbl_803FC620@ha
    stw r0, 0x4(r1)
    addi r4, r4, lbl_803FC620@l
    addi r0, r4, 0xe0
    stwu r1, -0x18(r1)
    cmplw r3, r0
    stw r31, 0x14(r1)
    li r31, 0x0
    stw r30, 0x10(r1)
    addi r30, r4, 0x0
    beq @L_800AF848
    addi r0, r4, 0x1f0
    cmplw r3, r0
    addi r4, r4, 0x110
    addi r30, r4, 0x0
    li r31, 0x1
    beq @L_800AF848
    li r31, 0x2
    @L_800AF848:
    lwz r0, 0x0(r30)
    cmpwi r0, 0x0
    beq @L_800AF888
    addi r3, r31, 0x0
    li r4, 0x0
    bl fn_8009870C
    lwz r0, 0xcc(r30)
    cmplwi r0, 0x0
    mr r12, r0
    beq @L_800AF888
    li r0, 0x0
    mtlr r12
    stw r0, 0xcc(r30)
    addi r3, r31, 0x0
    li r4, -0x5
    blrl
    @L_800AF888:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
