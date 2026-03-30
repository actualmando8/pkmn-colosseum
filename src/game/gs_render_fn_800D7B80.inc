    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    mr r0, r3
    mulli r3, r0, 0x190
    stw r0, lbl_8047AAB0(r13)
    bl fn_800E3534
    clrlwi. r0, r3, 16
    sth r3, lbl_8047AAA8(r13)
    beq @L_800D7BE8
    mr r3, r0
    bl fn_800E27B0
    li r5, 0x0
    stw r3, lbl_8047AAAC(r13)
    mr r4, r5
    li r6, 0x0
    b @L_800D7BD4
    @L_800D7BC4:
    lwz r3, lbl_8047AAAC(r13)
    addi r6, r6, 0x1
    stbx r4, r3, r5
    addi r5, r5, 0x190
    @L_800D7BD4:
    lwz r0, lbl_8047AAB0(r13)
    cmplw r6, r0
    blt @L_800D7BC4
    li r0, 0x0
    stw r0, lbl_8047AAB4(r13)
    @L_800D7BE8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
