    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bne @L_801ED33C
    li r3, 0x0
    li r4, 0xb
    bl fn_80129280
    mr r31, r3
@L_801ED33C:
    cmplwi r31, 0x0
    beq @L_801ED374
    mr r3, r31
    bne @L_801ED358
    li r3, 0x0
    li r4, 0xb
    bl fn_80129280
@L_801ED358:
    cmplwi r3, 0x0
    beq @L_801ED368
    li r0, 0x0
    sth r0, 0x2(r3)
@L_801ED368:
    li r0, 0x0
    stb r0, 0x1(r31)
    stb r0, 0x0(r31)
@L_801ED374:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
