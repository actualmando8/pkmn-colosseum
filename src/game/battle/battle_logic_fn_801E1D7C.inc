    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, -32738
    lis r5, -32697
    stw r0, 0x14(r1)
    addi r4, r4, 7196
    mr r8, r3
    li r7, 0x1000
    stw r31, 0xc(r1)
    addi r31, r5, -28608
    addi r6, r31, 0x0
    li r5, 0x0
    addi r3, r31, 0x1000
    li r9, 0x1
    addi r6, r6, 0x1000
    bl fn_800A19CC
    cmpwi r3, 0x0
    bne @L_801E1DCC
    li r3, 0x0
    b @L_801E1E08
@L_801E1DCC:
    addi r3, r31, 0x13d0
    addi r4, r31, 0x1368
    li r5, 0xa
    bl fn_8009F1D0
    addi r3, r31, 0x13b0
    addi r4, r31, 0x1340
    li r5, 0xa
    bl fn_8009F1D0
    addi r3, r31, 0x1390
    addi r4, r31, 0x1318
    li r5, 0xa
    bl fn_8009F1D0
    li r0, 0x1
    li r3, 0x1
    stw r0, -21440(r13)
@L_801E1E08:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
