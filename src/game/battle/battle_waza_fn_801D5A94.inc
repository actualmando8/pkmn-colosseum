    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    lis r4, -32698
    lis r5, -32728
    stw r0, 0x54(r1)
    addi r4, r4, 29688
    stmw r23, 0x2c(r1)
    addi r30, r4, 0x1c
    mr r31, r3
    addi r29, r5, -27848
    mr r3, r30
    li r28, 0x0
    li r27, 0x0
    bl strlen
    cmplwi r3, 0x0
    beq @L_801D5B68
    mr r3, r30
    bl fn_80167F28
    mr. r25, r3
    bne @L_801D5AFC
    mr r4, r30
    addi r3, r29, 0x6c
    crxor 6, 6, 6
    bl fn_800DD970
    li r26, 0x0
    b @L_801D5B0C
@L_801D5AFC:
    bl fn_80167E5C
    mr r26, r3
    mr r3, r25
    bl fn_80167E64
@L_801D5B0C:
    lis r3, -32698
    mr r28, r26
    addi r3, r3, 29688
    addi r24, r3, 0x41c
    mr r3, r24
    bl strlen
    cmplwi r3, 0x0
    beq @L_801D5B68
    mr r3, r24
    bl fn_80167F28
    mr. r25, r3
    bne @L_801D5B54
    mr r4, r24
    addi r3, r29, 0x6c
    crxor 6, 6, 6
    bl fn_800DD970
    li r26, 0x0
    b @L_801D5B64
@L_801D5B54:
    bl fn_80167E5C
    mr r26, r3
    mr r3, r25
    bl fn_80167E64
@L_801D5B64:
    mr r27, r26
@L_801D5B68:
    bl fn_801DB154
    mr. r26, r3
    beq @L_801D5D8C
    cmpwi r28, 0x0
    beq @L_801D5D8C
    addi r0, r28, 0x1f
    li r4, 0x20
    rlwinm r3, r0, 0, 0, 26
    bl fn_800E2C04
    rlwinm. r0, r3, 0, 16, 31
    beq @L_801D5D8C
    lis r4, -32698
    addi r4, r4, 29688
    sth r3, 0x660(r4)
    bl fn_800E27B0
    lis r4, -32698
    mr r0, r3
    addi r25, r4, 29688
    mr r3, r30
    stw r0, 0x65c(r25)
    mr r23, r0
    bl fn_80167F28
    mr. r24, r3
    bne @L_801D5BDC
    mr r4, r30
    addi r3, r29, 0x9c
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801D5C1C
@L_801D5BDC:
    addi r0, r28, 0x1f
    mr r4, r23
    rlwinm r5, r0, 0, 0, 26
    li r6, 0x0
    bl fn_80167ED0
    cmpwi r3, 0x0
    bge @L_801D5C14
    mr r4, r30
    addi r3, r29, 0xc8
    crxor 6, 6, 6
    bl fn_800DD970
    mr r3, r24
    bl fn_80167E64
    b @L_801D5C1C
@L_801D5C14:
    mr r3, r24
    bl fn_80167E64
@L_801D5C1C:
    lwz r4, 0x65c(r25)
    mr r3, r26
    bl fn_801DD45C
    lis r3, -32698
    li r0, 0x1
    addi r30, r3, 29688
    mr r3, r26
    stw r0, 0x4(r30)
    li r4, 0x1
    bl fn_801DA4E8
    lwz r8, 0xc(r29)
    li r3, 0x0
    lwz r7, 0x10(r29)
    lwz r6, 0x14(r29)
    lwz r5, 0x18(r29)
    lwz r4, 0x1c(r29)
    lwz r0, 0x20(r29)
    stw r8, 0x14(r1)
    stw r7, 0x18(r1)
    stw r6, 0x1c(r1)
    stw r5, 0x8(r1)
    stw r4, 0xc(r1)
    stw r0, 0x10(r1)
    bl fn_801C3A64
    stfs f1, 0x8(r1)
    addi r4, r1, 0x14
    lwz r3, 0x24(r26)
    bl fn_800E4170
    lwz r3, 0x24(r26)
    addi r4, r1, 0x8
    bl fn_800E43A4
    li r0, -0x1
    cmpwi r27, 0x0
    stb r0, 0x76(r26)
    beq @L_801D5D78
    addi r0, r27, 0x1f
    li r4, 0x20
    rlwinm r3, r0, 0, 0, 26
    bl fn_800E2C04
    rlwinm. r0, r3, 0, 16, 31
    beq @L_801D5D78
    lis r4, -32698
    addi r4, r4, 29688
    sth r3, 0x668(r4)
    bl fn_800E27B0
    lis r4, -32698
    mr r23, r3
    addi r28, r4, 29688
    addi r25, r28, 0x41c
    stw r3, 0x664(r28)
    mr r3, r25
    bl fn_80167F28
    mr. r24, r3
    bne @L_801D5D08
    mr r4, r25
    addi r3, r29, 0x9c
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801D5D48
@L_801D5D08:
    addi r0, r27, 0x1f
    mr r4, r23
    rlwinm r5, r0, 0, 0, 26
    li r6, 0x0
    bl fn_80167ED0
    cmpwi r3, 0x0
    bge @L_801D5D40
    mr r4, r25
    addi r3, r29, 0xc8
    crxor 6, 6, 6
    bl fn_800DD970
    mr r3, r24
    bl fn_80167E64
    b @L_801D5D48
@L_801D5D40:
    mr r3, r24
    bl fn_80167E64
@L_801D5D48:
    lwz r4, 0x664(r28)
    mr r3, r26
    bl fn_801DDB4C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D5D68
    li r0, 0x2
    stw r0, 0x4(r30)
    b @L_801D5D78
@L_801D5D68:
    mr r3, r26
    li r4, 0x0
    li r5, 0x0
    bl fn_801DA8C4
@L_801D5D78:
    lis r3, -32698
    rlwinm r0, r31, 2, 0, 29
    addi r3, r3, 29688
    add r3, r3, r0
    stw r26, 0x61c(r3)
@L_801D5D8C:
    lmw r23, 0x2c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
