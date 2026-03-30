    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_801EF63C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80007968
    li r3, -0x1
    b @L_8000798C
    @L_80007968:
    lwz r3, lbl_8047A274(r13)
    li r4, 0x0
    bl fn_801F7258
    cmplwi r3, 0x0
    stw r3, lbl_8047A278(r13)
    bne @L_80007988
    li r3, -0x1
    b @L_8000798C
    @L_80007988:
    li r3, 0x1
    @L_8000798C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
