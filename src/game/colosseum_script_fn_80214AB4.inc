    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, -32733
    li r6, 0x0
    stw r0, 0x14(r1)
    li r0, 0x0
    addi r4, r3, -5220
    addi r5, r1, 0x8
    stb r0, 0x8(r1)
    li r3, 0x0
    bl fn_801F37B0
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
