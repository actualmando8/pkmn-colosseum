    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r3, lbl_8047AB20(r13)
    slwi r3, r3, 6
    bl fn_800E3534
    clrlwi. r0, r3, 16
    sth r3, lbl_8047AB18(r13)
    beq @L_800DF8BC
    mr r3, r0
    bl fn_800E27B0
    li r5, 0x0
    stw r3, lbl_8047AB1C(r13)
    mr r4, r5
    li r6, 0x0
    b @L_800DF8A4
    @L_800DF894:
    lwz r3, lbl_8047AB1C(r13)
    addi r6, r6, 0x1
    stbx r4, r3, r5
    addi r5, r5, 0x40
    @L_800DF8A4:
    lwz r0, lbl_8047AB20(r13)
    cmplw r6, r0
    blt @L_800DF894
    lis r3, lbl_80315490@ha
    addi r3, r3, lbl_80315490@l
    bl fn_801A7CFC
    @L_800DF8BC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
