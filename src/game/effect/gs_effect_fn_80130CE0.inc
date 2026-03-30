    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    rlwinm r25, r3, 0, 16, 31
    mulli r23, r25, 0x34
    mr r31, r3
    mr r3, r23
    bl fn_800E3534
    lis r4, -32714
    rlwinm. r0, r3, 0, 16, 31
    addi r4, r4, 13760
    sth r3, 0x4(r4)
    beq @L_80130E98
    mr r3, r0
    bl fn_800E27B0
    lis r4, -32714
    mr r24, r3
    addi r4, r4, 13760
    mr r5, r23
    stw r24, 0xc(r4)
    li r4, 0x0
    bl memset
    lis r3, -32714
    addi r4, r24, 0x34
    addi r5, r3, 13760
    li r0, 0x1
    stw r24, 0x8(r5)
    li r5, 0x0
    subi r3, r25, 0x1
    stw r5, 0x30(r24)
    cmpwi r3, 0x1
    li r5, 0x1
    stw r4, 0x2c(r24)
    sth r0, 0x0(r24)
    ble @L_80130E6C
    subi r0, r3, 0x1
    subi r23, r25, 0x9
    cmpwi r0, 0x8
    ble @L_80130E60
    b @L_80130E30
@L_80130D84:
    subi r6, r4, 0x34
    addi r0, r7, 0x1
    stw r6, 0x30(r4)
    addi r24, r4, 0x34
    addi r25, r4, 0x68
    addi r26, r5, 0x2
    stw r24, 0x2c(r4)
    addi r27, r4, 0x9c
    addi r28, r5, 0x3
    addi r29, r4, 0xd0
    sth r0, 0x0(r4)
    addi r30, r5, 0x4
    addi r12, r4, 0x104
    addi r11, r5, 0x5
    stw r4, 0x64(r4)
    addi r10, r4, 0x138
    addi r9, r5, 0x6
    addi r8, r4, 0x16c
    stw r25, 0x60(r4)
    addi r7, r5, 0x7
    addi r0, r5, 0x8
    addi r6, r4, 0x1a0
    sth r26, 0x34(r4)
    addi r5, r5, 0x8
    stw r24, 0x98(r4)
    stw r27, 0x94(r4)
    sth r28, 0x68(r4)
    stw r25, 0xcc(r4)
    stw r29, 0xc8(r4)
    sth r30, 0x9c(r4)
    stw r27, 0x100(r4)
    stw r12, 0xfc(r4)
    sth r11, 0xd0(r4)
    stw r29, 0x134(r4)
    stw r10, 0x130(r4)
    sth r9, 0x104(r4)
    stw r12, 0x168(r4)
    stw r8, 0x164(r4)
    sth r7, 0x138(r4)
    stw r10, 0x19c(r4)
    stw r6, 0x198(r4)
    sth r0, 0x16c(r4)
    addi r4, r4, 0x1a0
@L_80130E30:
    rlwinm r7, r5, 0, 16, 31
    cmpw r7, r23
    blt @L_80130D84
    b @L_80130E60
@L_80130E40:
    subi r6, r4, 0x34
    addi r0, r7, 0x1
    stw r6, 0x30(r4)
    addi r6, r4, 0x34
    addi r5, r5, 0x1
    stw r6, 0x2c(r4)
    sth r0, 0x0(r4)
    addi r4, r4, 0x34
@L_80130E60:
    rlwinm r7, r5, 0, 16, 31
    cmpw r7, r3
    blt @L_80130E40
@L_80130E6C:
    subi r0, r4, 0x34
    li r6, 0x0
    stw r0, 0x30(r4)
    li r5, -0x1
    lis r3, -32714
    rlwinm r0, r31, 0, 16, 31
    stw r6, 0x2c(r4)
    sth r31, 0x0(r4)
    stw r5, 0x4(r4)
    stw r0, 13760(r3)
    b @L_80130EA8
@L_80130E98:
    li r0, 0x0
    stw r0, 0xc(r4)
    stw r0, 0x8(r4)
    stw r0, 0x0(r4)
@L_80130EA8:
    lis r4, -32714
    lis r3, -32749
    addi r4, r4, 13760
    li r0, 0x0
    stw r0, 0x10(r4)
    addi r6, r3, 3944
    li r3, 0x1
    li r4, 0x7f
    li r5, 0x0
    bl fn_800FE834
    stw r3, -23136(r13)
    bl fn_801E12A0
    lis r4, -32749
    li r3, 0x1
    addi r6, r4, 3844
    li r5, 0x0
    li r4, 0x80
    bl fn_800FE834
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
