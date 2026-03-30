    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x48(r1)
    stmw r20, 0x18(r1)
    mr r23, r3
    lis r3, -32719
    addi r25, r23, 0x0
    addi r31, r3, 6384
    lwz r26, -24648(r13)
@L_800A4D4C:
    lbz r3, 0x0(r23)
    extsb. r0, r3
    bne @L_800A4D60
    mr r3, r26
    b @L_800A5008
@L_800A4D60:
    extsb r0, r3
    cmpwi r0, 0x2f
    bne @L_800A4D78
    li r26, 0x0
    addi r23, r23, 0x1
    b @L_800A4D4C
@L_800A4D78:
    cmpwi r0, 0x2e
    bne @L_800A4DF0
    lbz r3, 0x1(r23)
    extsb r0, r3
    cmpwi r0, 0x2e
    bne @L_800A4DD0
    lbz r3, 0x2(r23)
    cmpwi r3, 0x2f
    bne @L_800A4DB4
    mulli r3, r26, 0xc
    lwz r4, -24660(r13)
    addi r0, r3, 0x4
    lwzx r26, r4, r0
    addi r23, r23, 0x3
    b @L_800A4D4C
@L_800A4DB4:
    extsb. r0, r3
    bne @L_800A4DF0
    mulli r0, r26, 0xc
    lwz r3, -24660(r13)
    add r3, r3, r0
    lwz r3, 0x4(r3)
    b @L_800A5008
@L_800A4DD0:
    cmpwi r0, 0x2f
    bne @L_800A4DE0
    addi r23, r23, 0x2
    b @L_800A4D4C
@L_800A4DE0:
    extsb. r0, r3
    bne @L_800A4DF0
    mr r3, r26
    b @L_800A5008
@L_800A4DF0:
    lwz r0, -24644(r13)
    cmplwi r0, 0x0
    bne @L_800A4EA4
    addi r28, r23, 0x0
    li r5, 0x0
    li r4, 0x0
    b @L_800A4E50
@L_800A4E0C:
    extsb r0, r3
    cmpwi r0, 0x2e
    bne @L_800A4E40
    subf r0, r23, r28
    cmpwi r0, 0x8
    bgt @L_800A4E2C
    cmpwi r5, 0x1
    bne @L_800A4E34
@L_800A4E2C:
    li r4, 0x1
    b @L_800A4E68
@L_800A4E34:
    addi r24, r28, 0x1
    li r5, 0x1
    b @L_800A4E4C
@L_800A4E40:
    cmpwi r0, 0x20
    bne @L_800A4E4C
    li r4, 0x1
@L_800A4E4C:
    addi r28, r28, 0x1
@L_800A4E50:
    lbz r3, 0x0(r28)
    extsb. r0, r3
    beq @L_800A4E68
    extsb r0, r3
    cmpwi r0, 0x2f
    bne @L_800A4E0C
@L_800A4E68:
    cmpwi r5, 0x1
    bne @L_800A4E80
    subf r0, r24, r28
    cmpwi r0, 0x3
    ble @L_800A4E80
    li r4, 0x1
@L_800A4E80:
    cmpwi r4, 0x0
    beq @L_800A4EC8
    addi r5, r31, 0x0
    crxor 6, 6, 6
    addi r6, r25, 0x0
    addi r3, r13, -32352
    li r4, 0x17b
    bl fn_800060F0
    b @L_800A4EC8
@L_800A4EA4:
    mr r28, r23
    b @L_800A4EB0
@L_800A4EAC:
    addi r28, r28, 0x1
@L_800A4EB0:
    lbz r3, 0x0(r28)
    extsb. r0, r3
    beq @L_800A4EC8
    extsb r0, r3
    cmpwi r0, 0x2f
    bne @L_800A4EAC
@L_800A4EC8:
    lbz r0, 0x0(r28)
    extsb. r0, r0
    bne @L_800A4EDC
    li r30, 0x0
    b @L_800A4EE0
@L_800A4EDC:
    li r30, 0x1
@L_800A4EE0:
    mulli r29, r26, 0xc
    subf r27, r23, r28
    addi r26, r26, 0x1
    b @L_800A4FD0
@L_800A4EF0:
    mulli r28, r26, 0xc
    lwzx r4, r3, r28
    rlwinm. r0, r4, 0, 0, 7
    bne @L_800A4F08
    li r0, 0x0
    b @L_800A4F0C
@L_800A4F08:
    li r0, 0x1
@L_800A4F0C:
    cmpwi r0, 0x0
    bne @L_800A4F1C
    cmpwi r30, 0x1
    beq @L_800A4F98
@L_800A4F1C:
    lwz r3, -24656(r13)
    rlwinm r0, r4, 0, 8, 31
    addi r21, r23, 0x0
    add r20, r3, r0
    b @L_800A4F64
@L_800A4F30:
    lbz r0, 0x0(r20)
    addi r20, r20, 0x1
    extsb r3, r0
    bl fn_800C7558
    lbz r0, 0x0(r21)
    addi r22, r3, 0x0
    addi r21, r21, 0x1
    extsb r3, r0
    bl fn_800C7558
    cmpw r3, r22
    beq @L_800A4F64
    li r0, 0x0
    b @L_800A4F90
@L_800A4F64:
    lbz r0, 0x0(r20)
    extsb. r0, r0
    bne @L_800A4F30
    lbz r3, 0x0(r21)
    cmpwi r3, 0x2f
    beq @L_800A4F84
    extsb. r0, r3
    bne @L_800A4F8C
@L_800A4F84:
    li r0, 0x1
    b @L_800A4F90
@L_800A4F8C:
    li r0, 0x0
@L_800A4F90:
    cmpwi r0, 0x1
    beq @L_800A4FEC
@L_800A4F98:
    lwz r0, -24660(r13)
    add r3, r0, r28
    lwz r0, 0x0(r3)
    rlwinm. r0, r0, 0, 0, 7
    bne @L_800A4FB4
    li r0, 0x0
    b @L_800A4FB8
@L_800A4FB4:
    li r0, 0x1
@L_800A4FB8:
    cmpwi r0, 0x0
    beq @L_800A4FC8
    lwz r0, 0x8(r3)
    b @L_800A4FCC
@L_800A4FC8:
    addi r0, r26, 0x1
@L_800A4FCC:
    mr r26, r0
@L_800A4FD0:
    lwz r3, -24660(r13)
    addi r0, r3, 0x8
    lwzx r0, r29, r0
    cmplw r26, r0
    blt @L_800A4EF0
    li r3, -0x1
    b @L_800A5008
@L_800A4FEC:
    cmpwi r30, 0x0
    bne @L_800A4FFC
    mr r3, r26
    b @L_800A5008
@L_800A4FFC:
    add r23, r27, r23
    addi r23, r23, 0x1
    b @L_800A4D4C
@L_800A5008:
    lmw r20, 0x18(r1)
    lwz r0, 0x4c(r1)
    addi r1, r1, 0x48
    mtlr r0
    blr
