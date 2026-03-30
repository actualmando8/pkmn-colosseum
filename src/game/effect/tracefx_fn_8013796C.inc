    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x2c
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_80131428
    mr. r31, r3
    beq @L_801379CC
    lis r5, 0x8013
    lis r6, 0x8013
    lis r7, 0x8013
    lis r8, 0x8013
    lis r4, 0x8013
    addi r5, r5, 31204
    addi r9, r8, 32020
    addi r6, r6, 31396
    addi r10, r4, 32600
    addi r7, r7, 31276
    li r4, 0x0
    li r8, 0x0
    bl fn_80131200
    mr r3, r31
    li r4, 0x0
    bl fn_8013139C
@L_801379CC:
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
