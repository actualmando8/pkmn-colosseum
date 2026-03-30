    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_801EF63C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80007330
    li r3, -0x1
    b @L_80007354
    @L_80007330:
    lwz r3, lbl_8047A278(r13)
    li r4, 0x0
    bl fn_801F986C
    cmplwi r3, 0x0
    stw r3, lbl_8047A27C(r13)
    bne @L_80007350
    li r3, -0x1
    b @L_80007354
    @L_80007350:
    bl fn_80007364
    @L_80007354:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
