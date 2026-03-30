    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r4
    beq @L_801FE548
    cmplwi r30, 0x0
    beq @L_801FE548
    cmplwi r3, 0x0
    bne @L_801FE49C
    li r31, 0x0
    b @L_801FE4B0
    @L_801FE49C:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
    @L_801FE4B0:
    mr r3, r31
    li r4, 0x0
    li r5, 0x87
    li r6, 0x0
    bl fn_8012640C
    sth r3, 0x2(r30)
    mr r3, r31
    li r4, 0x0
    li r5, 0x88
    li r6, 0x0
    bl fn_8012640C
    sth r3, 0x4(r30)
    mr r3, r31
    li r4, 0x0
    li r5, 0x89
    li r6, 0x0
    bl fn_8012640C
    sth r3, 0x6(r30)
    mr r3, r31
    li r4, 0x0
    li r5, 0x8a
    li r6, 0x0
    bl fn_8012640C
    sth r3, 0xa(r30)
    mr r3, r31
    li r4, 0x0
    li r5, 0x8b
    li r6, 0x0
    bl fn_8012640C
    sth r3, 0xc(r30)
    mr r3, r31
    li r4, 0x0
    li r5, 0x8c
    li r6, 0x0
    bl fn_8012640C
    sth r3, 0x8(r30)
    li r0, 0x0
    stb r0, 0x0(r30)
    @L_801FE548:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
