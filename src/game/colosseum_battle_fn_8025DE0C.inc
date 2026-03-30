    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_8025DE40
    bl fn_801DAC3C
    cmplwi r3, 0x0
    beq @L_8025DE40
    mr r4, r31
    bl fn_800E43A4
@L_8025DE40:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
