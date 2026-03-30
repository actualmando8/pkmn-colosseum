    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    lbz r12, 0x3b(r1)
    stw r31, 0x2c(r1)
    mr r31, r8
    stw r30, 0x28(r1)
    mr r30, r7
    stw r29, 0x24(r1)
    mr r29, r6
    stw r28, 0x20(r1)
    mr r28, r5
    lwz r11, lbl_8047AA80(r13)
    lwz r0, 0x0(r11)
    lwz r11, 0x3c(r1)
    cmpwi r0, 0x1
    bne @L_800DBDF4
    stw r9, 0x8(r1)
    clrlwi r0, r10, 24
    mr r5, r3
    mr r6, r4
    stw r0, 0xc(r1)
    mr r7, r28
    mr r8, r29
    mr r9, r30
    stw r12, 0x10(r1)
    mr r10, r31
    li r3, 0x52
    li r4, 0xa
    stw r11, 0x14(r1)
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DBE3C
    @L_800DBDF4:
    lis r5, lbl_80400B28@ha
    li r7, 0x1
    addi r6, r5, lbl_80400B28@l
    li r0, 0x0
    mulli r5, r3, 0x14
    add r3, r6, r3
    stb r7, 0x1fc(r3)
    add r3, r6, r5
    stbu r0, 0x20c(r3)
    stb r4, 0x1(r3)
    stb r28, 0x3(r3)
    stb r29, 0x4(r3)
    stb r30, 0x2(r3)
    stb r31, 0x6(r3)
    stb r9, 0x7(r3)
    stb r10, 0x8(r3)
    stb r12, 0x9(r3)
    stb r11, 0x5(r3)
    @L_800DBE3C:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
