    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x19
    li r4, 0x0
    stw r0, 0x14(r1)
    bl fn_801F025C
    lis r4, -32735
    mr r5, r3
    li r3, 0x0
    li r6, 0x0
    addi r4, r4, 19708
    bl fn_801F37B0
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
