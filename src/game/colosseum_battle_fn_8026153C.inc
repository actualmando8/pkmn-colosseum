    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r30, 0x18(r1)
    mr r30, r3
    addi r31, r30, 0x4
    bl fn_801C3108
    mr r4, r3
    mr r3, r31
    li r5, 0x44
    bl memcpy
    li r6, 0x1
    addi r0, r31, 0x44
    li r7, 0x0
    lis r3, 0x8026
    addi r4, r3, 6220
    stb r6, 0x8(r1)
    addi r5, r1, 0x8
    li r3, 0x0
    stb r6, 0x9(r1)
    li r6, 0x0
    sth r7, 0xa(r1)
    stw r31, 0xc(r1)
    stw r0, 0x10(r1)
    bl fn_801F2B5C
    lhz r31, 0xa(r1)
    li r0, 0x0
    lis r3, 0x8026
    addi r5, r1, 0x8
    sth r0, 0xa(r1)
    addi r4, r3, 5896
    li r3, 0x0
    li r6, 0x0
    bl fn_801F37B0
    lhz r0, 0xa(r1)
    li r3, 0x1
    sth r31, 0x0(r30)
    sth r0, 0x2(r30)
    bl fn_801EF8F4
    bl fn_801C3114
    bl fn_801DAC90
    lmw r30, 0x18(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
