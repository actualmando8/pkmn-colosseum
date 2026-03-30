    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, lbl_8047A298(r13)
    cmplwi r0, 0x0
    beq @L_8000C134
    bl fn_80175FFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8000C10C
    bl fn_80176004
    b @L_8000C134
    @L_8000C10C:
    lbz r0, lbl_8047A299(r13)
    cmplwi r0, 0x0
    bne @L_8000C124
    li r3, 0x0
    bl fn_80176030
    b @L_8000C134
    @L_8000C124:
    cmplwi r0, 0x1
    bne @L_8000C134
    li r3, 0x1
    bl fn_80176030
    @L_8000C134:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
