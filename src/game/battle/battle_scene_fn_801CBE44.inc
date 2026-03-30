    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r7, -32698
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r3
    mr r30, r4
    mr r31, r5
    addi r3, r7, 28968
    mr r27, r6
    mr r4, r28
    mr r5, r30
    bl fn_801CBBAC
    lis r3, -32698
    lis r4, -32698
    addi r5, r3, 28968
    add r28, r28, r27
    lwz r3, 0x0(r5)
    addi r29, r4, 29008
    nor r0, r3, r3
    lwzu r3, 0x4(r5)
    stw r0, 0x0(r29)
    nor r0, r3, r3
    lwz r3, 0x4(r5)
    stw r0, 0x4(r29)
    nor r0, r3, r3
    lwz r3, 0x8(r5)
    stw r0, 0x8(r29)
    nor r0, r3, r3
    lwz r3, 0xc(r5)
    stw r0, 0xc(r29)
    nor r0, r3, r3
    stw r0, 0x10(r29)
    b @L_801CBF34
@L_801CBECC:
    lwz r6, 0x0(r28)
    mr r3, r29
    lwz r0, 0x0(r29)
    mr r4, r28
    li r5, 0x14
    xor r0, r6, r0
    stw r0, 0x0(r28)
    lwz r6, 0x4(r28)
    lwz r0, 0x4(r29)
    xor r0, r6, r0
    stw r0, 0x4(r28)
    lwz r6, 0x8(r28)
    lwz r0, 0x8(r29)
    xor r0, r6, r0
    stw r0, 0x8(r28)
    lwz r6, 0xc(r28)
    lwz r0, 0xc(r29)
    xor r0, r6, r0
    stw r0, 0xc(r28)
    lwz r0, 0x10(r29)
    lwz r6, 0x10(r28)
    xor r0, r6, r0
    stw r0, 0x10(r28)
    bl fn_801CBBAC
    addi r28, r28, 0x14
    addi r27, r27, 0x14
@L_801CBF34:
    cmplw r27, r30
    blt @L_801CBECC
    lis r4, -32698
    mr r3, r31
    addi r4, r4, 28968
    li r5, 0x14
    bl memcpy
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
