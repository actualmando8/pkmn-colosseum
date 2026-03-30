    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    bne @L_801A0FE0
    li r31, 0x0
    b @L_801A1070
    @L_801A0FE0:
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801A0FFC
    lwz r3, 0x0(r30)
    bl fn_80193748
    cmplwi r3, 0x0
    bne @L_801A1038
    @L_801A0FFC:
    lwz r0, lbl_8047B298(r13)
    cmplwi r0, 0x0
    beq @L_801A1010
    lwz r3, lbl_8047B298(r13)
    b @L_801A1018
    @L_801A1010:
    lis r3, lbl_8036C8E0@ha
    addi r3, r3, lbl_8036C8E0@l
    @L_801A1018:
    bl fn_80193828
    mr. r31, r3
    bne @L_801A1054
    la r3, lbl_8047DB20(r2)
    li r4, 0x7df
    la r5, lbl_8047DB3C(r2)
    bl fn_80196E10
    b @L_801A1054
    @L_801A1038:
    bl fn_80193828
    mr. r31, r3
    bne @L_801A1054
    la r3, lbl_8047DB20(r2)
    li r4, 0x3d5
    la r5, lbl_8047DB3C(r2)
    bl fn_80196E10
    @L_801A1054:
    lwz r6, 0x0(r31)
    mr r3, r31
    mr r4, r30
    li r5, 0x0
    lwz r12, 0x3c(r6)
    mtctr r12
    bctrl
    @L_801A1070:
    mr r4, r30
    mr r3, r31
    bl fn_801A0744
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
