    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    beq @L_801A6480
    mr r31, r3
    b @L_801A6478
    @L_801A6428:
    cmplwi r31, 0x0
    beq @L_801A6474
    lwz r5, 0x0(r31)
    mr r4, r31
    lwz r3, 0x48(r31)
    lwz r5, 0x40(r5)
    bl fn_801C27F4
    cmplwi r31, 0x0
    beq @L_801A6454
    lwz r3, 0x18(r31)
    b @L_801A6458
    @L_801A6454:
    li r3, 0x0
    @L_801A6458:
    bl fn_801919EC
    cmplwi r31, 0x0
    beq @L_801A646C
    lwz r3, 0x1c(r31)
    b @L_801A6470
    @L_801A646C:
    li r3, 0x0
    @L_801A6470:
    bl fn_801919EC
    @L_801A6474:
    lwz r31, 0xc(r31)
    @L_801A6478:
    cmplwi r31, 0x0
    bne @L_801A6428
    @L_801A6480:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
