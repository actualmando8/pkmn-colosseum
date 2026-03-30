    nofralloc
    cmplwi r3, 0x0
    beq @L_801DABBC
    lwz r0, 0x10(r3)
    b @L_801DABC0
@L_801DABBC:
    li r0, 0x0
@L_801DABC0:
    cmpwi r0, 0x1
    beq @L_801DAC04
    bge @L_801DABE4
    cmpwi r0, -0x1
    beq @L_801DABFC
    bge @L_801DAC1C
    cmpwi r0, -0x2
    bge @L_801DABF4
    b @L_801DAC1C
@L_801DABE4:
    cmpwi r0, 0x3
    beq @L_801DAC14
    bge @L_801DAC1C
    b @L_801DAC0C
@L_801DABF4:
    lfs f1, -21272(r2)
    blr
@L_801DABFC:
    lfs f1, -21268(r2)
    blr
@L_801DAC04:
    lfs f1, -21264(r2)
    blr
@L_801DAC0C:
    lfs f1, -21260(r2)
    blr
@L_801DAC14:
    lfs f1, -21256(r2)
    blr
@L_801DAC1C:
    lfs f1, -21252(r2)
    blr
