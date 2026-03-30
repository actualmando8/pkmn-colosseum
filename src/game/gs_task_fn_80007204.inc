    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_801EF63C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80007228
    li r3, -0x1
    b @L_8000724C
    @L_80007228:
    lwz r3, lbl_8047A278(r13)
    li r4, 0x3
    bl fn_801F986C
    cmplwi r3, 0x0
    stw r3, lbl_8047A27C(r13)
    bne @L_80007248
    li r3, -0x1
    b @L_8000724C
    @L_80007248:
    bl fn_80007364
    @L_8000724C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
