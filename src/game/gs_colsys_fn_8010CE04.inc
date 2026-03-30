    nofralloc
    cmplwi r3, 0x0
    beqlr
    lwz r0, 0x0(r3)
    li r5, 0x0
    add r0, r0, r3
    stw r0, 0x0(r3)
    lwz r4, 0x0(r3)
    b @L_8010CFD4
@L_8010CE24:
    lwz r0, 0x24(r4)
    cmplwi r0, 0x0
    beq @L_8010CE78
    add r0, r0, r3
    stw r0, 0x24(r4)
    lwz r6, 0x24(r4)
    lwz r0, 0x0(r6)
    cmplwi r0, 0x0
    beq @L_8010CE50
    add r0, r0, r3
    stw r0, 0x0(r6)
@L_8010CE50:
    lwz r0, 0x8(r6)
    cmplwi r0, 0x0
    beq @L_8010CE64
    add r0, r0, r3
    stw r0, 0x8(r6)
@L_8010CE64:
    lwz r0, 0xc(r6)
    cmplwi r0, 0x0
    beq @L_8010CE78
    add r0, r0, r3
    stw r0, 0xc(r6)
@L_8010CE78:
    lwz r0, 0x28(r4)
    cmplwi r0, 0x0
    beq @L_8010CECC
    add r0, r0, r3
    stw r0, 0x28(r4)
    lwz r6, 0x28(r4)
    lwz r0, 0x0(r6)
    cmplwi r0, 0x0
    beq @L_8010CEA4
    add r0, r0, r3
    stw r0, 0x0(r6)
@L_8010CEA4:
    lwz r0, 0x8(r6)
    cmplwi r0, 0x0
    beq @L_8010CEB8
    add r0, r0, r3
    stw r0, 0x8(r6)
@L_8010CEB8:
    lwz r0, 0xc(r6)
    cmplwi r0, 0x0
    beq @L_8010CECC
    add r0, r0, r3
    stw r0, 0xc(r6)
@L_8010CECC:
    lwz r0, 0x2c(r4)
    cmplwi r0, 0x0
    beq @L_8010CF20
    add r0, r0, r3
    stw r0, 0x2c(r4)
    lwz r6, 0x2c(r4)
    lwz r0, 0x0(r6)
    cmplwi r0, 0x0
    beq @L_8010CEF8
    add r0, r0, r3
    stw r0, 0x0(r6)
@L_8010CEF8:
    lwz r0, 0x8(r6)
    cmplwi r0, 0x0
    beq @L_8010CF0C
    add r0, r0, r3
    stw r0, 0x8(r6)
@L_8010CF0C:
    lwz r0, 0xc(r6)
    cmplwi r0, 0x0
    beq @L_8010CF20
    add r0, r0, r3
    stw r0, 0xc(r6)
@L_8010CF20:
    lwz r0, 0x30(r4)
    cmplwi r0, 0x0
    beq @L_8010CF4C
    add r0, r0, r3
    stw r0, 0x30(r4)
    lwz r6, 0x30(r4)
    lwz r0, 0x0(r6)
    cmplwi r0, 0x0
    beq @L_8010CF4C
    add r0, r0, r3
    stw r0, 0x0(r6)
@L_8010CF4C:
    lwz r0, 0x34(r4)
    cmplwi r0, 0x0
    beq @L_8010CFA0
    add r0, r0, r3
    stw r0, 0x34(r4)
    lwz r6, 0x34(r4)
    lwz r0, 0x0(r6)
    cmplwi r0, 0x0
    beq @L_8010CF78
    add r0, r0, r3
    stw r0, 0x0(r6)
@L_8010CF78:
    lwz r0, 0x8(r6)
    cmplwi r0, 0x0
    beq @L_8010CF8C
    add r0, r0, r3
    stw r0, 0x8(r6)
@L_8010CF8C:
    lwz r0, 0xc(r6)
    cmplwi r0, 0x0
    beq @L_8010CFA0
    add r0, r0, r3
    stw r0, 0xc(r6)
@L_8010CFA0:
    lwz r0, 0x38(r4)
    cmplwi r0, 0x0
    beq @L_8010CFCC
    add r0, r0, r3
    stw r0, 0x38(r4)
    lwz r6, 0x38(r4)
    lwz r0, 0x0(r6)
    cmplwi r0, 0x0
    beq @L_8010CFCC
    add r0, r0, r3
    stw r0, 0x0(r6)
@L_8010CFCC:
    addi r5, r5, 0x1
    addi r4, r4, 0x40
@L_8010CFD4:
    lwz r0, 0x4(r3)
    cmplw r5, r0
    blt @L_8010CE24
    blr
