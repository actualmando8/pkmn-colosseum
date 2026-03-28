    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r3, -32697
    stw r0, 0x24(r1)
    addi r4, r3, -21408
    stw r31, 0x1c(r1)
    lwz r0, 0xa0(r4)
    cmpwi r0, 0x0
    beq @L_801E4044
    lbz r0, 0xa4(r4)
    cmplwi r0, 0x0
    beq @L_801E4044
    li r0, 0x0
    lwz r3, -21428(r13)
    stb r0, 0xa5(r4)
    stb r0, 0xa4(r4)
    bl fn_800A8850
    lis r3, -32697
    addi r3, r3, -21408
    lwz r0, 0xb0(r3)
    cmpwi r0, 0x0
    bne @L_801E3FB4
    bl fn_800A7AFC
    bl fn_801E1D0C
@L_801E3FB4:
    bl fn_801E5400
    lis r3, -32697
    addi r3, r3, -21408
    lbz r0, 0xa7(r3)
    cmplwi r0, 0x0
    beq @L_801E3FFC
    lwz r3, -31520(r13)
    bl fn_8014F838
    lwz r3, -31516(r13)
    li r0, -0x1
    stw r0, -31520(r13)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_801E3FF8
    bl fn_8014F838
    li r0, -0x1
    stw r0, -31516(r13)
@L_801E3FF8:
    bl fn_801E4DAC
@L_801E3FFC:
    lis r3, -32697
    addi r31, r3, -23404
@L_801E4004:
    mr r3, r31
    addi r4, r1, 0x8
    li r5, 0x0
    bl fn_8009F2F8
    cmpwi r3, 0x1
    bne @L_801E4024
    lwz r0, 0x8(r1)
    b @L_801E4028
@L_801E4024:
    li r0, 0x0
@L_801E4028:
    cmplwi r0, 0x0
    bne @L_801E4004
    lis r3, -32697
    li r0, 0x0
    addi r3, r3, -21408
    stw r0, 0xa8(r3)
    stw r0, 0xac(r3)
@L_801E4044:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
