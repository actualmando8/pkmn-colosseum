    nofralloc
    lwz r0, -23404(r13)
    lwz r7, -23408(r13)
    mtctr r0
    cmplwi r0, 0x0
    beq @L_800FEFB8
@L_800FEFA0:
    lwz r0, 0x8(r7)
    cmpwi r0, 0x0
    bne @L_800FEFB0
    b @L_800FEFBC
@L_800FEFB0:
    addi r7, r7, 0x24
    bdnz @L_800FEFA0
@L_800FEFB8:
    li r7, 0x0
@L_800FEFBC:
    cmplwi r7, 0x0
    beqlr
    li r6, 0x0
    li r0, 0x1
    stw r6, 0x0(r7)
    stw r6, 0x4(r7)
    stw r0, 0x8(r7)
    stw r0, 0xc(r7)
    stw r4, 0x10(r7)
    stb r3, 0x14(r7)
    stb r6, 0x15(r7)
    lwz r0, 0xc(r7)
    cmpwi r0, 0x0
    bne @L_800FEFFC
    stw r5, 0x18(r7)
    b @L_800FF008
@L_800FEFFC:
    stw r5, 0x18(r7)
    sth r6, 0x20(r7)
    stw r6, 0x1c(r7)
@L_800FF008:
    lwz r4, -23380(r13)
    cmplwi r4, 0x0
    bne @L_800FF024
    stw r7, -23380(r13)
    blr
    b @L_800FF024
@L_800FF020:
    mr r4, r5
@L_800FF024:
    lwz r5, 0x4(r4)
    cmplwi r5, 0x0
    beq @L_800FF040
    lbz r3, 0x14(r4)
    lbz r0, 0x14(r7)
    cmplw r3, r0
    blt @L_800FF020
@L_800FF040:
    cmplwi r5, 0x0
    bne @L_800FF06C
    lbz r3, 0x14(r4)
    lbz r0, 0x14(r7)
    cmplw r3, r0
    bge @L_800FF06C
    stw r4, 0x0(r7)
    li r0, 0x0
    stw r0, 0x4(r7)
    stw r7, 0x4(r4)
    blr
@L_800FF06C:
    lwz r3, 0x0(r4)
    cmplwi r3, 0x0
    beq @L_800FF07C
    stw r7, 0x4(r3)
@L_800FF07C:
    lwz r0, 0x0(r4)
    stw r0, 0x0(r7)
    stw r4, 0x4(r7)
    stw r7, 0x0(r4)
    lwz r0, -23380(r13)
    cmplw r0, r4
    bnelr
    stw r7, -23380(r13)
    blr
