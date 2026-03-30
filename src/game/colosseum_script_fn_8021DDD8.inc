    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    addi r4, r1, 0x18
    lwz r3, lbl_8047B64C(r13)
    bl fn_801FE468
    lis r4, lbl_80478278@ha
    addi r3, r1, 0x18
    addi r4, r4, lbl_80478278@l
    addi r5, r1, 0x8
    bl fn_802621F4
    addi r3, r1, 0x8
    li r4, 0x1
    bl fn_802621C4
    addi r3, r1, 0x18
    li r4, 0x0
    bl fn_802621C4
    bl fn_802622E4
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
