    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    beq @L_80131168
    lis r4, -32714
    addi r4, r4, 13760
    lwz r0, 0x0(r4)
    cmplw r3, r0
    bgt @L_80131168
    subi r0, r3, 0x1
    lwz r3, 0xc(r4)
    mulli r0, r0, 0x34
    add r31, r3, r0
    lwz r0, 0x4(r31)
    cmpwi r0, -0x1
    beq @L_80131168
    b @L_8013116C
@L_80131168:
    li r31, 0x0
@L_8013116C:
    cmplwi r31, 0x0
    beq @L_801311E8
    lwz r0, 0x4(r31)
    cmpwi r0, 0x0
    beq @L_801311D8
    lwz r12, 0x10(r31)
    li r0, 0x2
    cmplwi r12, 0x0
    stw r0, 0x4(r31)
    beq @L_801311E8
    lwz r3, 0x24(r31)
    mtctr r12
    bctrl
    cmplwi r3, 0x0
    bne @L_801311D0
    lwz r12, 0x14(r31)
    li r0, 0x1
    cmplwi r12, 0x0
    stw r0, 0x4(r31)
    beq @L_801311C8
    lwz r3, 0x24(r31)
    mtctr r12
    bctrl
@L_801311C8:
    li r3, 0x0
    b @L_801311EC
@L_801311D0:
    li r3, 0x1
    b @L_801311EC
@L_801311D8:
    lis r3, -32729
    addi r3, r3, 10840
    crxor 6, 6, 6
    bl fn_800DD970
@L_801311E8:
    li r3, 0x0
@L_801311EC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
