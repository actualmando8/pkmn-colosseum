    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, lbl_8047A350(r13)
    la r4, lbl_80478880(r13)
    lbz r7, 0x8b(r3)
    li r5, -0x100
    slwi r0, r0, 2
    li r3, 0x0
    lwzx r6, r4, r0
    or r5, r7, r5
    li r4, 0x0
    bl fn_800FB680
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
