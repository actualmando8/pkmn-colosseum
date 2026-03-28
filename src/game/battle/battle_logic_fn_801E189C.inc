    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r5, -32697
    lis r7, -32698
    stw r0, 0x14(r1)
    li r0, 0x0
    clrlwi r9, r4, 24
    addi r8, r13, -21464
    stw r9, 0x4(r8)
    lis r6, -32738
    addi r5, r5, -32736
    li r8, 0x10
    stw r0, -21468(r13)
    addi r0, r7, 32008
    addi r4, r6, 6436
    addi r6, r5, 0xffc
    stw r3, -21464(r13)
    mr r3, r0
    addi r5, r13, -21464
    li r7, 0x1000
    li r9, 0x1
    bl fn_800A19CC
    lis r3, -32698
    addi r3, r3, 32008
    bl fn_800A1F94
    b @L_801E1908
@L_801E1904:
    bl fn_800F0308
@L_801E1908:
    lwz r0, -21468(r13)
    cmpwi r0, 0x0
    beq @L_801E1904
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
