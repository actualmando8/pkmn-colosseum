    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r24, 0x30(r1)
    mr r24, r3
    mr r28, r4
    mr r29, r5
    clrlwi r3, r28, 16
    bl fn_801440A0
    bl fn_80143F9C
    clrlwi r0, r3, 24
    cmpwi r0, 0x0
    bne @L_80017EF0
    clrlwi r4, r28, 16
    li r3, 0x2d
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x426c
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x0
    b @L_800181B0
    @L_80017EF0:
    mulli r30, r24, 0x4c
    lis r3, lbl_80266918@ha
    addi r0, r3, lbl_80266918@l
    add r31, r0, r30
    addi r31, r31, 0x4
    lwz r4, 0x0(r31)
    cmpwi r4, 0x5
    beq @L_800180C0
    cmpwi r4, -0x1
    beq @L_800180C0
    li r0, 0x2b28
    lwz r3, lbl_8047A2F8(r13)
    stw r0, lbl_8047A2DC(r13)
    clrlwi r4, r4, 24
    addi r6, r1, 0xa
    li r5, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    lhz r0, 0xa(r1)
    cmpwi r0, 0x64
    ble @L_80017F50
    li r24, 0x3
    b @L_80017F54
    @L_80017F50:
    li r24, 0x2
    @L_80017F54:
    lwz r0, 0x0(r31)
    cmpwi r0, 0x0
    blt @L_80017F80
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80017F98
    @L_80017F80:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80017F98:
    mr r25, r3
    li r27, -0x1
    li r26, 0x0
    b @L_80017FDC
    @L_80017FA8:
    mr r3, r25
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80017FD4
    addi r27, r27, 0x1
    cmpw r27, r29
    blt @L_80017FD4
    mr r3, r25
    bl fn_80143C50
    b @L_80017FEC
    @L_80017FD4:
    addi r26, r26, 0x1
    addi r25, r25, 0x4
    @L_80017FDC:
    lhz r0, 0x8(r1)
    cmpw r26, r0
    blt @L_80017FA8
    li r3, 0x0
    @L_80017FEC:
    clrlwi r7, r3, 16
    cmpwi r7, 0x1
    bge @L_80018000
    li r0, 0x0
    b @L_800180B8
    @L_80018000:
    cmpwi r24, 0x2
    bne @L_80018018
    li r0, 0x1
    li r25, 0x5b
    stw r0, 0xc(r1)
    b @L_80018024
    @L_80018018:
    li r0, 0x2
    li r25, 0x5c
    stw r0, 0xc(r1)
    @L_80018024:
    lis r4, lbl_80266918@ha
    li r3, 0x1
    addi r4, r4, lbl_80266918@l
    li r0, 0x0
    add r4, r4, r30
    stw r25, 0x14(r1)
    lbz r6, 0x0(r4)
    lbz r5, 0x1(r4)
    lbz r4, 0x2(r4)
    stb r6, 0x10(r1)
    stb r5, 0x11(r1)
    stb r4, 0x12(r1)
    stw r3, 0x1c(r1)
    stw r7, 0x18(r1)
    stw r0, 0x20(r1)
    stw r3, lbl_8047A2FC(r13)
    bl fn_801046B8
    mr r4, r3
    mr r3, r25
    addi r5, r1, 0xc
    addi r9, r1, 0x10
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    mr r30, r3
    mr r3, r25
    bl fn_80102510
    mr r3, r25
    li r4, 0x1
    bl fn_80102428
    cmpwi r30, -0x1
    bne @L_800180B4
    li r0, -0x1
    b @L_800180B8
    @L_800180B4:
    lwz r0, lbl_8047A2FC(r13)
    @L_800180B8:
    mr r24, r0
    b @L_800180C4
    @L_800180C0:
    li r24, 0x1
    @L_800180C4:
    cmpwi r24, 0x0
    bge @L_800180D4
    li r3, 0x0
    b @L_800181B0
    @L_800180D4:
    mr r4, r28
    li r3, 0x0
    bl fn_80134420
    clrlwi r0, r3, 16
    cmpw r0, r24
    bge @L_800180F4
    li r0, 0x0
    b @L_80018108
    @L_800180F4:
    mr r4, r28
    clrlwi r5, r24, 16
    li r3, 0x0
    bl fn_8013467C
    li r0, 0x1
    @L_80018108:
    cmpwi r0, 0x0
    bne @L_80018140
    clrlwi r4, r28, 16
    li r3, 0x2d
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x2b49
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x0
    b @L_800181B0
    @L_80018140:
    lwz r0, 0x0(r31)
    cmpwi r0, -0x1
    bne @L_80018164
    lwz r3, lbl_8047A2F8(r13)
    mr r4, r28
    clrlwi r5, r24, 16
    extsh r6, r29
    bl fn_8012959C
    b @L_80018178
    @L_80018164:
    lwz r3, lbl_8047A2F8(r13)
    mr r4, r28
    clrlwi r5, r24, 16
    extsh r6, r29
    bl fn_801299C8
    @L_80018178:
    clrlwi r4, r28, 16
    li r3, 0x2d
    bl fn_80132A38
    mr r4, r24
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x4266
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x1
    @L_800181B0:
    lmw r24, 0x30(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
