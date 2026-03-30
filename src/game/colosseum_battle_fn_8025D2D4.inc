    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bl fn_8006B09C
    lhz r3, 0x0(r3)
    cmplwi r3, 0x0
    bne @L_8025D300
    li r3, 0x0
    b @L_8025D350
@L_8025D300:
    bl fn_801FCCC4
    bl fn_801FCBA4
    mulli r0, r3, 0x14
    lwz r3, -31260(r13)
    cmpwi r31, 0x0
    add r3, r3, r0
    bne @L_8025D338
    lwz r0, 0xc(r3)
    lis r3, 0xf94
    addi r3, r3, 0x1200
    cmplwi r0, 0x0
    beq @L_8025D350
    mr r3, r0
    b @L_8025D350
@L_8025D338:
    lwz r0, 0x10(r3)
    lis r3, 0xf8f
    addi r3, r3, 0x1200
    cmplwi r0, 0x0
    beq @L_8025D350
    mr r3, r0
@L_8025D350:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
