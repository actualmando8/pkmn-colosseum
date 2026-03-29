    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, 0x60(r3)
    lbz r0, 0x1c(r3)
    cmplwi r0, 0x0
    beq @L_8002B060
    cmplwi r0, 0x1
    bne @L_8002B074
    @L_8002B060:
    li r3, 0x0
    li r4, 0x0
    li r5, -0x1
    li r6, 0x2b2e
    bl fn_800FB680
    @L_8002B074:
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
