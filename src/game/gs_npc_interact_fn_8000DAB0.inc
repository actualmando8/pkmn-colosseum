    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, lbl_8047A2A4(r13)
    li r4, 0x1
    bl fn_8010264C
    stw r3, lbl_8047A2B0(r13)
    lwz r3, lbl_8047A2A8(r13)
    bl fn_800F0384
    lwz r3, lbl_8047A2B0(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
