    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r29, 0x24(r1)
    mr r29, r4
    mr r30, r3
    li r3, 0x2
    mr r4, r30
    mr r5, r29
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_80261DC4
    li r31, 0x0
    b @L_80261E24
@L_80261DC4:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    rlwinm r31, r3, 0, 16, 31
    mr r3, r30
    mr r4, r29
    bl fn_801F0134
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80261DF4
    li r31, 0x0
    b @L_80261E24
@L_80261DF4:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80261E0C
    li r31, 0x0
    b @L_80261E24
@L_80261E0C:
    mr r4, r31
    rlwinm r6, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    bl fn_801F76B8
    mr r31, r3
@L_80261E24:
    mr r3, r30
    addi r4, r1, 0x8
    bl fn_801F7954
    lwz r4, 0x8(r1)
    mr r3, r31
    lhz r0, 0xc(r1)
    addi r9, r1, 0x10
    stw r4, 0x10(r1)
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    sth r0, 0x14(r1)
    li r7, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    lmw r29, 0x24(r1)
    li r3, 0x1
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
