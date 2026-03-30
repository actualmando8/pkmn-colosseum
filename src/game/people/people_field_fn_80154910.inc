    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r5, 0x400
    li r6, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r7, 0x114(r3)
    lwz r8, 0x118(r3)
    and r0, r7, r6
    and r3, r8, r5
    xor r3, r3, r6
    xor r0, r0, r6
    or. r0, r3, r0
    bne @L_8015495C
    or r0, r8, r5
    stw r0, 0x118(r31)
    stw r7, 0x114(r31)
    b @L_80154964
    @L_8015495C:
    lwz r0, 0x4(r4)
    clrlwi r6, r0, 24
    @L_80154964:
    lwz r0, 0x0(r4)
    lis r3, 0x51ec
    subi r7, r3, 0x7ae1
    clrrwi r3, r0, 16
    mulhw r3, r7, r3
    srawi r3, r3, 5
    srwi r5, r3, 31
    add. r5, r3, r5
    bge @L_801549B0
    lwz r3, 0x4(r4)
    srwi r3, r3, 16
    extsb r3, r3
    slwi r3, r3, 8
    mulhw r3, r7, r3
    srawi r3, r3, 5
    srwi r7, r3, 31
    add r3, r3, r7
    subf r5, r3, r5
    b @L_801549D4
    @L_801549B0:
    lwz r3, 0x4(r4)
    srwi r3, r3, 16
    extsb r3, r3
    slwi r3, r3, 8
    mulhw r3, r7, r3
    srawi r3, r3, 5
    srwi r7, r3, 31
    add r3, r3, r7
    add r5, r5, r3
    @L_801549D4:
    lwz r4, 0x4(r4)
    addi r3, r31, 0x260
    extrwi r8, r4, 8, 16
    extrwi r4, r0, 8, 16
    neg r7, r8
    or r0, r7, r8
    srwi r7, r0, 31
    bl fn_80160F10
    lwz r0, 0x214(r31)
    ori r0, r0, 0x4
    stw r0, 0x214(r31)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
