    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    lis r3, -32698
    lis r5, -32728
    stw r0, 0x44(r1)
    addi r4, r3, 29688
    stmw r21, 0x14(r1)
    addi r31, r5, -27848
    lwz r0, 0x624(r4)
    rlwinm r0, r0, 2, 0, 29
    add r3, r4, r0
    lwz r29, 0x61c(r3)
    cmplwi r29, 0x0
    beq @L_801D6028
    lwz r0, 0x24(r29)
    cmplwi r0, 0x0
    beq @L_801D6028
    addi r30, r4, 0x21c
    mr r3, r30
    bl fn_80167F28
    mr. r27, r3
    bne @L_801D5E10
    mr r4, r30
    addi r3, r31, 0x6c
    crxor 6, 6, 6
    bl fn_800DD970
    li r22, 0x0
    b @L_801D5E20
@L_801D5E10:
    bl fn_80167E5C
    mr r22, r3
    mr r3, r27
    bl fn_80167E64
@L_801D5E20:
    cmpwi r22, 0x0
    beq @L_801D6028
    addi r0, r22, 0x1f
    li r4, 0x20
    rlwinm r3, r0, 0, 0, 26
    bl fn_800E2C04
    rlwinm. r0, r3, 0, 16, 31
    mr r28, r3
    beq @L_801D6028
    bl fn_800E27B0
    mr r0, r3
    mr r3, r30
    mr r27, r0
    bl fn_80167F28
    mr. r21, r3
    bne @L_801D5E74
    mr r4, r30
    addi r3, r31, 0x9c
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801D5EB4
@L_801D5E74:
    addi r0, r22, 0x1f
    mr r4, r27
    rlwinm r5, r0, 0, 0, 26
    li r6, 0x0
    bl fn_80167ED0
    cmpwi r3, 0x0
    bge @L_801D5EAC
    mr r4, r30
    addi r3, r31, 0xc8
    crxor 6, 6, 6
    bl fn_800DD970
    mr r3, r21
    bl fn_80167E64
    b @L_801D5EB4
@L_801D5EAC:
    mr r3, r21
    bl fn_80167E64
@L_801D5EB4:
    lwz r0, 0x0(r27)
    cmpwi r0, 0x2
    beq @L_801D5EF0
    bge @L_801D5ED4
    cmpwi r0, 0x0
    beq @L_801D5EE0
    bge @L_801D5EE8
    b @L_801D5EFC
@L_801D5ED4:
    cmpwi r0, 0x4
    bge @L_801D5EFC
    b @L_801D5EF8
@L_801D5EE0:
    li r26, 0x0
    b @L_801D5EFC
@L_801D5EE8:
    li r26, 0x1
    b @L_801D5EFC
@L_801D5EF0:
    li r26, 0x2
    b @L_801D5EFC
@L_801D5EF8:
    li r26, 0x3
@L_801D5EFC:
    lwz r0, 0x4(r27)
    cmpwi r0, 0x2
    beq @L_801D5F38
    bge @L_801D5F1C
    cmpwi r0, 0x0
    beq @L_801D5F28
    bge @L_801D5F30
    b @L_801D5F44
@L_801D5F1C:
    cmpwi r0, 0x4
    bge @L_801D5F44
    b @L_801D5F40
@L_801D5F28:
    li r25, 0x0
    b @L_801D5F44
@L_801D5F30:
    li r25, 0x1
    b @L_801D5F44
@L_801D5F38:
    li r25, 0x2
    b @L_801D5F44
@L_801D5F40:
    li r25, 0x3
@L_801D5F44:
    lwz r0, 0x8(r27)
    cmpwi r0, 0x2
    beq @L_801D5F80
    bge @L_801D5F64
    cmpwi r0, 0x0
    beq @L_801D5F70
    bge @L_801D5F78
    b @L_801D5F8C
@L_801D5F64:
    cmpwi r0, 0x4
    bge @L_801D5F8C
    b @L_801D5F88
@L_801D5F70:
    li r24, 0x0
    b @L_801D5F8C
@L_801D5F78:
    li r24, 0x1
    b @L_801D5F8C
@L_801D5F80:
    li r24, 0x2
    b @L_801D5F8C
@L_801D5F88:
    li r24, 0x3
@L_801D5F8C:
    lwz r0, 0xc(r27)
    cmpwi r0, 0x2
    beq @L_801D5FC8
    bge @L_801D5FAC
    cmpwi r0, 0x0
    beq @L_801D5FB8
    bge @L_801D5FC0
    b @L_801D5FD4
@L_801D5FAC:
    cmpwi r0, 0x4
    bge @L_801D5FD4
    b @L_801D5FD0
@L_801D5FB8:
    li r23, 0x0
    b @L_801D5FD4
@L_801D5FC0:
    li r23, 0x1
    b @L_801D5FD4
@L_801D5FC8:
    li r23, 0x2
    b @L_801D5FD4
@L_801D5FD0:
    li r23, 0x3
@L_801D5FD4:
    lwz r21, 0x10(r27)
    mr r3, r28
    bl fn_800E24B0
    mr r3, r28
    bl fn_800E209C
    srawi r4, r21, 24
    rlwinm r3, r21, 16, 24, 31
    rlwinm r0, r21, 24, 24, 31
    stb r4, 0xb(r1)
    mr r4, r26
    mr r5, r25
    stb r3, 0xa(r1)
    mr r6, r24
    mr r7, r23
    stb r0, 0x9(r1)
    stb r21, 0x8(r1)
    lwz r3, 0x24(r29)
    bl fn_800E5E34
    lwz r3, 0x24(r29)
    addi r4, r1, 0x8
    bl fn_800E5BE0
@L_801D6028:
    lmw r21, 0x14(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
