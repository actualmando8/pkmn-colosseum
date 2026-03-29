    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r22, 0x28(r1)
    lbz r0, lbl_80478860(r13)
    li r26, 0x0
    li r25, 0x0
    li r28, 0x0
    cmplwi r0, 0x0
    li r31, 0x0
    beq @L_80018B74
    lis r3, lbl_802E4DB0@ha
    lis r4, lbl_802EF0A8@ha
    addi r3, r3, lbl_802E4DB0@l
    lwz r5, 0x0(r3)
    addi r6, r4, lbl_802EF0A8@l
    lwz r0, 0xc(r3)
    addi r4, r6, 0x4
    lwz r9, 0x18(r3)
    mulli r12, r5, 0x1c
    lwz r8, 0x24(r3)
    addi r5, r6, 0x8
    lwz r7, 0x30(r3)
    mulli r10, r0, 0x1c
    lwz r6, 0x3c(r3)
    lwz r0, 0x48(r3)
    lhax r11, r4, r12
    mulli r9, r9, 0x1c
    lhax r22, r5, r12
    lhax r23, r4, r10
    lhax r24, r5, r10
    mulli r8, r8, 0x1c
    lhax r30, r4, r9
    lhax r29, r5, r9
    sth r11, 0x4(r3)
    mulli r7, r7, 0x1c
    lhax r12, r4, r8
    lhax r11, r5, r8
    sth r22, 0x6(r3)
    mulli r6, r6, 0x1c
    lhax r10, r4, r7
    lhax r9, r5, r7
    sth r23, 0x10(r3)
    mulli r7, r0, 0x1c
    lhax r8, r4, r6
    lhax r6, r5, r6
    sth r24, 0x12(r3)
    lhax r0, r4, r7
    sth r30, 0x1c(r3)
    sth r29, 0x1e(r3)
    sth r12, 0x28(r3)
    sth r11, 0x2a(r3)
    sth r10, 0x34(r3)
    sth r9, 0x36(r3)
    sth r8, 0x40(r3)
    sth r6, 0x42(r3)
    sth r0, 0x4c(r3)
    lwz r6, 0x54(r3)
    li r0, 0x0
    lhax r7, r5, r7
    mulli r6, r6, 0x1c
    stb r0, lbl_80478860(r13)
    sth r7, 0x4e(r3)
    lhax r4, r4, r6
    lhax r0, r5, r6
    sth r4, 0x58(r3)
    sth r0, 0x5a(r3)
    @L_80018B74:
    li r3, 0x2
    bl fn_80103E68
    srwi r0, r3, 16
    sth r0, 0x18(r1)
    lbz r0, 0x19(r1)
    extsb r0, r0
    cmpwi r0, 0x0
    bgt @L_80018BA0
    li r0, 0x5
    addi r26, r1, 0x20
    stw r0, 0x20(r1)
    @L_80018BA0:
    lis r3, lbl_80266918@ha
    addi r30, r3, lbl_80266918@l
    b @L_80018EE4
    @L_80018BAC:
    cmpwi r28, 0x0
    beq @L_80018BBC
    mr r0, r25
    b @L_80018BC0
    @L_80018BBC:
    li r0, -0x1
    @L_80018BC0:
    stw r0, lbl_8047A2E8(r13)
    mr r27, r30
    li r29, 0x0
    @L_80018BCC:
    lwz r0, 0x1c(r27)
    clrlwi r3, r0, 16
    bl fn_80103E68
    srwi r0, r3, 16
    sth r0, 0xc(r1)
    lbz r0, 0xd(r1)
    extsb r0, r0
    cmpwi r0, 0x0
    bge @L_80018BF8
    li r0, 0x0
    stb r0, 0xd(r1)
    @L_80018BF8:
    lbz r0, 0xc(r1)
    extsb r0, r0
    cmpwi r0, 0x0
    bge @L_80018C10
    li r0, 0x0
    stb r0, 0xc(r1)
    @L_80018C10:
    lwz r0, 0x4(r27)
    cmpwi r0, 0x0
    blt @L_80018C3C
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80018C54
    @L_80018C3C:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80018C54:
    li r28, 0x0
    mr r23, r3
    mr r22, r28
    b @L_80018C84
    @L_80018C64:
    mr r3, r23
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80018C7C
    addi r28, r28, 0x1
    @L_80018C7C:
    addi r22, r22, 0x1
    addi r23, r23, 0x4
    @L_80018C84:
    lhz r0, 0x8(r1)
    cmpw r22, r0
    blt @L_80018C64
    lbz r0, 0xc(r1)
    lbz r3, 0xd(r1)
    extsb r4, r0
    extsb r0, r3
    add r0, r4, r0
    subf r0, r28, r0
    cmpwi r0, 0x0
    ble @L_80018D10
    extsb r0, r0
    subf r3, r0, r3
    extsb r0, r3
    stb r3, 0xd(r1)
    cmpwi r0, 0x0
    bge @L_80018CD0
    li r0, 0x0
    stb r0, 0xd(r1)
    @L_80018CD0:
    lbz r4, 0xc(r1)
    lbz r0, 0xd(r1)
    extsb r3, r4
    extsb r0, r0
    add r0, r3, r0
    subf r0, r28, r0
    cmpwi r0, 0x0
    ble @L_80018D10
    extsb r0, r0
    subf r3, r0, r4
    extsb r0, r3
    stb r3, 0xc(r1)
    cmpwi r0, 0x0
    bge @L_80018D10
    li r0, 0x0
    stb r0, 0xc(r1)
    @L_80018D10:
    lhz r3, 0xc(r1)
    addi r4, r1, 0x10
    lwz r0, 0x1c(r27)
    sth r3, 0x10(r1)
    clrlwi r3, r0, 16
    bl fn_80103EAC
    addi r27, r27, 0x4c
    addi r29, r29, 0x1
    cmpwi r29, 0x6
    blt @L_80018BCC
    li r0, -0x1
    stw r0, lbl_8047A2D8(r13)
    bl fn_801046B8
    mr r4, r3
    mr r5, r26
    li r3, 0x59
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    stw r3, lbl_8047A2D8(r13)
    bne @L_80018D78
    li r5, 0x0
    b @L_80018E64
    @L_80018D78:
    mulli r4, r3, 0x4c
    lis r3, lbl_80266918@ha
    addi r0, r3, lbl_80266918@l
    add r3, r0, r4
    lwz r0, 0x1c(r3)
    clrlwi r3, r0, 16
    bl fn_80103E68
    lwz r0, lbl_8047A2D8(r13)
    lis r4, lbl_80266918@ha
    srwi r5, r3, 16
    mulli r3, r0, 0x4c
    sth r5, 0x14(r1)
    addi r0, r4, lbl_80266918@l
    lbz r4, 0x14(r1)
    add r3, r0, r3
    lbz r0, 0x15(r1)
    lwz r5, 0x4(r3)
    extsb r3, r4
    extsb r0, r0
    cmpwi r5, 0x0
    add r22, r3, r0
    blt @L_80018DF0
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r5, 24
    addi r5, r1, 0xa
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80018E08
    @L_80018DF0:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0xa
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80018E08:
    mr r25, r3
    li r23, -0x1
    li r24, 0x0
    b @L_80018E4C
    @L_80018E18:
    mr r3, r25
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80018E44
    addi r23, r23, 0x1
    cmpw r23, r22
    blt @L_80018E44
    mr r3, r25
    bl fn_80143C68
    b @L_80018E5C
    @L_80018E44:
    addi r24, r24, 0x1
    addi r25, r25, 0x4
    @L_80018E4C:
    lhz r0, 0xa(r1)
    cmpw r24, r0
    blt @L_80018E18
    li r3, 0x0
    @L_80018E5C:
    mr r25, r22
    mr r5, r3
    @L_80018E64:
    clrlwi r0, r5, 16
    mr r27, r5
    cmplwi r0, 0x0
    li r26, 0x0
    li r28, 0x0
    beq @L_80018EEC
    lwz r3, lbl_8047A2E0(r13)
    mr r6, r25
    lwz r4, lbl_8047A2D8(r13)
    addi r7, r1, 0x1c
    bl fn_800188E0
    lhz r0, 0x1c(r1)
    cmplwi r0, 0x0
    beq @L_80018EA0
    mr r27, r0
    @L_80018EA0:
    cmpwi r3, 0x2
    beq @L_80018ED0
    bge @L_80018EBC
    cmpwi r3, 0x0
    beq @L_80018EE4
    bge @L_80018EC8
    b @L_80018EE4
    @L_80018EBC:
    cmpwi r3, 0x4
    bge @L_80018EE4
    b @L_80018ED8
    @L_80018EC8:
    li r31, 0x1
    b @L_80018EE4
    @L_80018ED0:
    li r28, 0x1
    b @L_80018EE4
    @L_80018ED8:
    li r0, 0x0
    addi r26, r1, 0x20
    stw r0, 0x20(r1)
    @L_80018EE4:
    cmpwi r31, 0x0
    beq @L_80018BAC
    @L_80018EEC:
    li r3, 0x59
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lwz r0, lbl_8047A2E4(r13)
    cmpwi r0, 0x0
    beq @L_80018F18
    bl fn_800FF660
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    @L_80018F18:
    mr r3, r27
    lmw r22, 0x28(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
