    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, lbl_8047B308(r13)
    bl fn_801A6990
    cmpwi r3, 0x0
    beq @L_801ADD38
    li r3, 0x0
    li r0, 0x0
    stw r3, lbl_8047B308(r13)
    stw r0, lbl_8047B30C(r13)
@L_801ADD38:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
