    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, -32697
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    addi r31, r3, -21408
    lwz r0, 0xa0(r31)
    cmpwi r0, 0x0
    beq @L_801E40E0
    lbz r0, 0xa4(r31)
    cmplwi r0, 0x1
    beq @L_801E4090
    cmplwi r0, 0x4
    bne @L_801E40E0
@L_801E4090:
    cmplwi r0, 0x4
    bne @L_801E40B0
    lis r3, -32697
    addi r3, r3, -21408
    lbz r0, 0xa7(r3)
    cmplwi r0, 0x0
    beq @L_801E40B0
    bl fn_801E2CA8
@L_801E40B0:
    lis r3, -32697
    li r6, 0x2
    addi r4, r3, -21408
    li r5, 0x0
    li r0, -0x1
    stb r6, 0xa4(r31)
    li r3, 0x1
    stw r5, 0xd0(r4)
    stw r5, 0xd4(r4)
    stw r0, 0xcc(r4)
    stw r0, 0xc8(r4)
    b @L_801E40E4
@L_801E40E0:
    li r3, 0x0
@L_801E40E4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
