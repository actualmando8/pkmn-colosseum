    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    mr r0, r3
    mulli r3, r0, 0x14
    stw r0, lbl_8047AC60(r13)
    bl fn_800E3534
    clrlwi. r0, r3, 16
    sth r3, lbl_8047AC58(r13)
    beq @L_800F96D4
    mr r3, r0
    bl fn_800E27B0
    li r5, 0x0
    stw r3, lbl_8047AC5C(r13)
    mr r4, r5
    li r6, 0x0
    b @L_800F96C8
@L_800F96B4:
    lwz r3, lbl_8047AC5C(r13)
    addi r0, r5, 0x4
    addi r5, r5, 0x14
    addi r6, r6, 0x1
    stwx r4, r3, r0
@L_800F96C8:
    lwz r0, lbl_8047AC60(r13)
    cmplw r6, r0
    blt @L_800F96B4
@L_800F96D4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
