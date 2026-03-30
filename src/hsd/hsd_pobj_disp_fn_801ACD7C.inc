    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_800B7D3C
    li r3, 0x0
    li r0, 0x0
    stw r3, lbl_8047B2FC(r13)
    stw r0, lbl_8047B300(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
