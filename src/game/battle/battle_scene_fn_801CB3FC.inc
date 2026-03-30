    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r26, r3
    mr r27, r4
    mr r28, r5
    mr r29, r6
    mr r30, r7
    mr r31, r8
    bl fn_800F037C
    cmplwi r3, 0x0
    beq @L_801CB438
    bl fn_800F036C
    b @L_801CB454
@L_801CB438:
    lis r3, -32729
    lis r4, -32713
    addi r3, r3, 22728
    addi r4, r4, -9048
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x7f
@L_801CB454:
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x7
    ble @L_801CB464
    li r27, 0x7
@L_801CB464:
    stw r30, 0x8(r1)
    add r3, r3, r27
    mr r4, r26
    mr r9, r28
    stw r31, 0xc(r1)
    mr r10, r29
    li r5, 0x1000
    li r6, 0x1
    li r7, 0x0
    li r8, 0x4
    crxor 6, 6, 6
    bl fn_800F7318
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
