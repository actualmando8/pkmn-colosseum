    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, lbl_8047B248(r13)
    cmpwi r0, 0x0
    beq @L_80197640
    lwz r3, lbl_8047B250(r13)
    li r5, 0x3c
    lwz r4, lbl_8047B254(r13)
    bl fn_80197650
    mr r0, r3
    lwz r3, lbl_8047B258(r13)
    stw r0, lbl_8047B250(r13)
    li r5, 0x40
    lwz r4, lbl_8047B25C(r13)
    bl fn_80197650
    stw r3, lbl_8047B258(r13)
@L_80197640:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
