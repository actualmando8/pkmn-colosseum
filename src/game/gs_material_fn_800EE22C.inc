    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplw r3, r4
    stw r0, 0x14(r1)
    bne @L_800EE248
    li r3, 0x0
    b @L_800EE278
    @L_800EE248:
    li r7, 0x0
    li r6, -0x1
    lis r5, fn_800EE8F0@ha
    stb r7, lbl_8047ABC4(r13)
    addi r0, r5, fn_800EE8F0@l
    li r5, 0x0
    stw r4, lbl_8047ABD0(r13)
    mr r4, r0
    stw r7, lbl_8047ABC8(r13)
    stw r6, lbl_8047ABCC(r13)
    bl fn_801A3918
    lwz r3, lbl_8047ABCC(r13)
    @L_800EE278:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
