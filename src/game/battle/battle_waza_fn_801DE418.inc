    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r31, r3
    rlwinm. r3, r3, 0, 16, 31
    beq @L_801DE440
    lwz r0, -31576(r13)
    cmplw r3, r0
    blt @L_801DE448
@L_801DE440:
    li r3, 0x0
    b @L_801DE584
@L_801DE448:
    mulli r0, r3, 0xc
    lis r3, -32713
    addi r3, r3, 2112
    lwzx r27, r3, r0
    add r3, r3, r0
    lwz r26, 0x4(r3)
    cmplwi r27, 0x0
    beq @L_801DE470
    cmplwi r26, 0x0
    bne @L_801DE498
@L_801DE470:
    lis r3, -32713
    addi r3, r3, 2112
    lwz r27, 0xc(r3)
    lwz r26, 0x10(r3)
    cmplwi r27, 0x0
    beq @L_801DE490
    cmplwi r26, 0x0
    bne @L_801DE498
@L_801DE490:
    li r3, 0x0
    b @L_801DE584
@L_801DE498:
    mr r28, r27
    mr r3, r27
    bl fn_8017B2CC
    cmpwi r3, 0x0
    bge @L_801DE4B8
    mr r3, r27
    bl fn_8017B3E4
    b @L_801DE4EC
@L_801DE4B8:
    mr r3, r27
    bl fn_8017B2CC
    cmpwi r3, 0x0
    bne @L_801DE4EC
    cmplwi r26, 0x0
    beq @L_801DE4EC
    mr r3, r27
    mr r4, r26
    bl fn_800F9318
    cmplwi r3, 0x0
    bne @L_801DE4EC
    mr r3, r27
    bl fn_8017B3E4
@L_801DE4EC:
    lis r3, -32728
    addi r30, r3, -26168
@L_801DE4F4:
    mr r3, r28
    bl fn_8017B2CC
    mr. r29, r3
    bge @L_801DE510
    mr r3, r30
    crxor 6, 6, 6
    bl fn_800DD970
@L_801DE510:
    cmpwi r29, 0x0
    beq @L_801DE520
    bl fn_800F0308
    b @L_801DE4F4
@L_801DE520:
    mr r3, r27
    mr r4, r26
    bl fn_800F9318
    mr. r26, r3
    bne @L_801DE53C
    li r3, 0x0
    b @L_801DE584
@L_801DE53C:
    bl fn_801DB154
    mr. r28, r3
    bne @L_801DE550
    li r3, 0x0
    b @L_801DE584
@L_801DE550:
    sth r31, 0x70(r28)
    li r0, 0x0
    mr r4, r26
    sth r27, 0x72(r28)
    stb r0, 0x75(r28)
    bl fn_801DD45C
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801DE580
    mr r3, r28
    bl fn_801DB100
    li r3, 0x0
    b @L_801DE584
@L_801DE580:
    mr r3, r28
@L_801DE584:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
