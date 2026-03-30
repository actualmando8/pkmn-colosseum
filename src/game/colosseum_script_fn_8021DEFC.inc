    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, -32734
    li r3, 0x0
    stw r0, 0x14(r1)
    addi r4, r4, -8388
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
