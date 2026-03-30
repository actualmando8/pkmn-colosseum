    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, lbl_80400EE0@ha
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    addi r31, r4, lbl_80400EE0@l
    lbz r0, 0x0(r31)
    cmplwi r0, 0x1
    bne @L_800DC2CC
    lwz r0, 0x4(r31)
    cmplw r0, r3
    bne @L_800DC2CC
    b @L_800DC324
    @L_800DC2CC:
    lbzu r0, 0x14(r31)
    cmplwi r0, 0x1
    bne @L_800DC2E8
    lwz r0, 0x4(r31)
    cmplw r0, r3
    bne @L_800DC2E8
    b @L_800DC324
    @L_800DC2E8:
    lbzu r0, 0x14(r31)
    cmplwi r0, 0x1
    bne @L_800DC304
    lwz r0, 0x4(r31)
    cmplw r0, r3
    bne @L_800DC304
    b @L_800DC324
    @L_800DC304:
    lbzu r0, 0x14(r31)
    cmplwi r0, 0x1
    bne @L_800DC320
    lwz r0, 0x4(r31)
    cmplw r0, r3
    bne @L_800DC320
    b @L_800DC324
    @L_800DC320:
    li r31, 0x0
    @L_800DC324:
    cmplwi r31, 0x0
    bne @L_800DC334
    li r3, 0x0
    b @L_800DC37C
    @L_800DC334:
    lwz r3, 0x4(r31)
    bl fn_800EF504
    li r4, 0x0
    lis r3, lbl_80400EE0@ha
    stb r4, 0x0(r31)
    li r0, 0x4
    addi r3, r3, lbl_80400EE0@l
    stb r4, lbl_8047AAE0(r13)
    mtctr r0
    @L_800DC358:
    lbz r0, 0x0(r3)
    cmplwi r0, 0x1
    bne @L_800DC370
    li r0, 0x1
    stb r0, lbl_8047AAE0(r13)
    b @L_800DC378
    @L_800DC370:
    addi r3, r3, 0x14
    bdnz @L_800DC358
    @L_800DC378:
    li r3, 0x1
    @L_800DC37C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
