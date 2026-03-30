    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, -23392(r13)
    lwz r4, -23408(r13)
    mtctr r0
    cmplwi r0, 0x0
    beq @L_800FEC78
@L_800FEC54:
    lwz r0, 0x8(r4)
    cmpwi r0, 0x3
    bne @L_800FEC70
    lwz r0, 0x18(r4)
    cmplw r0, r3
    bne @L_800FEC70
    b @L_800FEC7C
@L_800FEC70:
    addi r4, r4, 0x24
    bdnz @L_800FEC54
@L_800FEC78:
    li r4, 0x0
@L_800FEC7C:
    cmplwi r4, 0x0
    beq @L_800FECA8
    li r0, 0x0
    stb r0, 0x15(r4)
    lwz r0, 0xc(r4)
    cmpwi r0, 0x1
    bne @L_800FECA8
    lwz r3, 0x1c(r4)
    cmplwi r3, 0x0
    beq @L_800FECA8
    bl fn_800F0424
@L_800FECA8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
