    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, -23392(r13)
    lwz r4, -23408(r13)
    mtctr r0
    cmplwi r0, 0x0
    beq @L_800FECFC
@L_800FECD8:
    lwz r0, 0x8(r4)
    cmpwi r0, 0x3
    bne @L_800FECF4
    lwz r0, 0x18(r4)
    cmplw r0, r3
    bne @L_800FECF4
    b @L_800FED00
@L_800FECF4:
    addi r4, r4, 0x24
    bdnz @L_800FECD8
@L_800FECFC:
    li r4, 0x0
@L_800FED00:
    cmplwi r4, 0x0
    beq @L_800FED2C
    li r0, 0x1
    stb r0, 0x15(r4)
    lwz r0, 0xc(r4)
    cmpwi r0, 0x1
    bne @L_800FED2C
    lwz r3, 0x1c(r4)
    cmplwi r3, 0x0
    beq @L_800FED2C
    bl fn_800F0438
@L_800FED2C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
