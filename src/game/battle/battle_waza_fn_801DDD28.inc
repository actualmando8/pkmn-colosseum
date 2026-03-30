    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr. r31, r3
    mr r26, r4
    mr r27, r5
    bne @L_801DDD50
    li r3, 0x0
    b @L_801DDED0
@L_801DDD50:
    rlwinm. r0, r27, 0, 16, 31
    bne @L_801DDD60
    li r3, 0x0
    b @L_801DDED0
@L_801DDD60:
    addi r7, r1, 0xc
    addi r8, r1, 0x8
    bl fn_801DDEE4
    lwz r0, 0xc(r1)
    cmplwi r0, 0x0
    beq @L_801DDD84
    lwz r0, 0x8(r1)
    cmplwi r0, 0x0
    bne @L_801DDD8C
@L_801DDD84:
    li r3, 0x0
    b @L_801DDED0
@L_801DDD8C:
    mr r3, r31
    mr r4, r26
    mr r5, r27
    bl fn_801DD0C8
    cmplwi r3, 0x0
    beq @L_801DDDAC
    li r3, 0x1
    b @L_801DDED0
@L_801DDDAC:
    lwz r28, 0xc(r1)
    lwz r29, 0x8(r1)
    mr r3, r28
    bl fn_8017B2CC
    cmpwi r3, 0x0
    bge @L_801DDDD0
    mr r3, r28
    bl fn_8017B3E4
    b @L_801DDE04
@L_801DDDD0:
    mr r3, r28
    bl fn_8017B2CC
    cmpwi r3, 0x0
    bne @L_801DDE04
    cmplwi r29, 0x0
    beq @L_801DDE04
    mr r3, r28
    mr r4, r29
    bl fn_800F9318
    cmplwi r3, 0x0
    bne @L_801DDE04
    mr r3, r28
    bl fn_8017B3E4
@L_801DDE04:
    lis r3, -32728
    addi r30, r3, -26168
@L_801DDE0C:
    mr r3, r28
    bl fn_8017B2CC
    mr. r29, r3
    bge @L_801DDE28
    mr r3, r30
    crxor 6, 6, 6
    bl fn_800DD970
@L_801DDE28:
    cmpwi r29, 0x0
    beq @L_801DDE38
    bl fn_800F0308
    b @L_801DDE0C
@L_801DDE38:
    lwz r3, 0xc(r1)
    lwz r4, 0x8(r1)
    bl fn_800F9318
    mr. r29, r3
    bne @L_801DDE54
    li r3, 0x0
    b @L_801DDED0
@L_801DDE54:
    bl fn_801DBFB0
    mr. r28, r3
    bne @L_801DDE68
    li r3, 0x0
    b @L_801DDED0
@L_801DDE68:
    sth r26, 0x2c(r28)
    mr r4, r29
    sth r27, 0x2e(r28)
    lwz r0, 0xc(r1)
    sth r0, 0x30(r28)
    stw r31, 0x3c(r28)
    bl fn_801DC014
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801DDE9C
    mr r3, r28
    bl fn_801DBDDC
    li r3, 0x0
    b @L_801DDED0
@L_801DDE9C:
    li r0, 0x0
    stb r0, 0x14(r28)
    stb r0, 0x15(r28)
    lwz r0, 0x68(r31)
    stw r0, 0x34(r28)
    lwz r3, 0x68(r31)
    cmplwi r3, 0x0
    beq @L_801DDEC0
    stw r28, 0x38(r3)
@L_801DDEC0:
    li r0, 0x0
    li r3, 0x1
    stw r0, 0x38(r28)
    stw r28, 0x68(r31)
@L_801DDED0:
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
