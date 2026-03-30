    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    b @L_80097F38
@L_80097F34:
    bl fn_800F0308
@L_80097F38:
    bl fn_8010B560
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80097F34
    lis r3, -32704
    li r4, 0x0
    addi r3, r3, -19584
    li r5, 0x44
    bl memset
    lis r3, -32704
    li r4, 0x8e
    addi r31, r3, -19584
    li r9, 0x0
    li r0, -0x1
    stb r4, 0x0(r31)
    mr r5, r28
    mr r7, r29
    stw r9, 0x8(r31)
    mr r8, r30
    li r3, 0x8e
    li r4, 0x0
    stw r28, 0xc(r31)
    li r6, 0x0
    sth r9, 0x18(r31)
    stw r29, 0x10(r31)
    stw r30, 0x14(r31)
    stw r0, 0x4(r31)
    bl fn_8009769C
    lwz r3, 0x4(r31)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
