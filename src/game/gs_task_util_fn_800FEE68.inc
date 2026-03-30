    nofralloc
    lwz r0, -23404(r13)
    lwz r6, -23408(r13)
    mulli r0, r0, 0x24
    lwz r7, -23400(r13)
    add r8, r6, r0
    mtctr r7
    cmplwi r7, 0x0
    beq @L_800FEEA0
@L_800FEE88:
    lwz r0, 0x8(r8)
    cmpwi r0, 0x0
    bne @L_800FEE98
    b @L_800FEEA4
@L_800FEE98:
    addi r8, r8, 0x24
    bdnz @L_800FEE88
@L_800FEEA0:
    li r8, 0x0
@L_800FEEA4:
    cmplwi r8, 0x0
    beqlr
    li r7, 0x0
    li r6, 0x3
    stw r7, 0x0(r8)
    li r0, 0x1
    stw r7, 0x4(r8)
    stw r6, 0x8(r8)
    stw r0, 0xc(r8)
    stw r4, 0x10(r8)
    stb r3, 0x14(r8)
    stb r7, 0x15(r8)
    lwz r0, 0xc(r8)
    cmpwi r0, 0x0
    bne @L_800FEEE8
    stw r5, 0x18(r8)
    b @L_800FEEF4
@L_800FEEE8:
    stw r5, 0x18(r8)
    sth r7, 0x20(r8)
    stw r7, 0x1c(r8)
@L_800FEEF4:
    lwz r4, -23380(r13)
    cmplwi r4, 0x0
    bne @L_800FEF10
    stw r8, -23380(r13)
    blr
    b @L_800FEF10
@L_800FEF0C:
    mr r4, r5
@L_800FEF10:
    lwz r5, 0x4(r4)
    cmplwi r5, 0x0
    beq @L_800FEF2C
    lbz r3, 0x14(r4)
    lbz r0, 0x14(r8)
    cmplw r3, r0
    blt @L_800FEF0C
@L_800FEF2C:
    cmplwi r5, 0x0
    bne @L_800FEF58
    lbz r3, 0x14(r4)
    lbz r0, 0x14(r8)
    cmplw r3, r0
    bge @L_800FEF58
    stw r4, 0x0(r8)
    li r0, 0x0
    stw r0, 0x4(r8)
    stw r8, 0x4(r4)
    blr
@L_800FEF58:
    lwz r3, 0x0(r4)
    cmplwi r3, 0x0
    beq @L_800FEF68
    stw r8, 0x4(r3)
@L_800FEF68:
    lwz r0, 0x0(r4)
    stw r0, 0x0(r8)
    stw r4, 0x4(r8)
    stw r8, 0x0(r4)
    lwz r0, -23380(r13)
    cmplw r0, r4
    bnelr
    stw r8, -23380(r13)
    blr
