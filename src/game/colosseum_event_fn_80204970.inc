    nofralloc
    cmplwi r3, 0x0
    stwu r1, -0x160(r1)
    beq @L_80204A08
    cmplwi r4, 0x0
    beq @L_80204A08
    li r0, 0x2a
    addi r8, r1, 0x4
    subi r6, r3, 0x4
    mtctr r0
@L_80204994:
    lwz r5, 0x4(r6)
    lwzu r0, 0x8(r6)
    stw r5, 0x4(r8)
    stwu r0, 0x8(r8)
    bdnz @L_80204994
    lwz r7, 0x4(r6)
    li r0, 0x2a
    subi r6, r3, 0x4
    subi r5, r4, 0x4
    stw r7, 0x4(r8)
    mtctr r0
@L_802049C0:
    lwz r3, 0x4(r5)
    lwzu r0, 0x8(r5)
    stw r3, 0x4(r6)
    stwu r0, 0x8(r6)
    bdnz @L_802049C0
    lwz r3, 0x4(r5)
    li r0, 0x2a
    subi r5, r4, 0x4
    addi r4, r1, 0x4
    stw r3, 0x4(r6)
    mtctr r0
@L_802049EC:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_802049EC
    lwz r0, 0x4(r4)
    stw r0, 0x4(r5)
@L_80204A08:
    addi r1, r1, 0x160
    blr
