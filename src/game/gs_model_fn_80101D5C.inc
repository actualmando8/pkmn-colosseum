    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, lbl_8047ACF0(r13)
    cmpwi r0, 0x0
    beq @L_80101D7C
    li r3, 0x3
    bl fn_800B8C58
@L_80101D7C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

