    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    mr r0, r3
    mulli r3, r0, 0xc
    stw r0, lbl_8047ABC0(r13)
    bl fn_800E3534
    clrlwi. r0, r3, 16
    sth r3, lbl_8047ABB8(r13)
    beq @L_800EE8E0
    mr r3, r0
    bl fn_800E27B0
    li r5, 0x0
    stw r3, lbl_8047ABBC(r13)
    mr r4, r5
    li r6, 0x0
    b @L_800EE8D4
    @L_800EE8C4:
    lwz r3, lbl_8047ABBC(r13)
    addi r6, r6, 0x1
    stbx r4, r3, r5
    addi r5, r5, 0xc
    @L_800EE8D4:
    lwz r0, lbl_8047ABC0(r13)
    cmplw r6, r0
    blt @L_800EE8C4
    @L_800EE8E0:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
