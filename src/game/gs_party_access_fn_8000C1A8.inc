    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, lbl_8047A298(r13)
    cmplwi r0, 0x0
    bne @L_8000C1D4
    li r3, 0x0
    li r0, 0x1
    stb r3, lbl_8047A299(r13)
    stb r0, lbl_8047A298(r13)
    b @L_8000C1F0
    @L_8000C1D4:
    li r0, 0x0
    stb r0, lbl_8047A298(r13)
    bl fn_80175FFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8000C1F0
    bl fn_80176004
    @L_8000C1F0:
    bl fn_801E11CC
    lbz r4, lbl_8047A298(r13)
    bl fn_801E11D4
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
