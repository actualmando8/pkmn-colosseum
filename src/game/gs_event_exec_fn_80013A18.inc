    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stmw r24, 0x50(r1)
    mr r27, r4
    mr r31, r5
    mulli r29, r3, 0x4c
    lis r4, lbl_80266918@ha
    lis r5, lbl_80266BD8@ha
    lwz r3, lbl_8047A2F8(r13)
    addi r0, r4, lbl_80266918@l
    addi r7, r5, lbl_80266BD8@l
    add r30, r0, r29
    lwz r4, 0x0(r7)
    addi r30, r30, 0x4
    lwz r26, 0x4(r7)
    lwz r28, 0x8(r7)
    li r9, 0x2b1c
    lwz r12, 0xc(r7)
    addi r6, r1, 0xc
    lwz r11, 0x10(r7)
    li r5, 0x0
    lwz r10, 0x14(r7)
    li r7, 0x0
    lwz r0, 0x0(r30)
    li r8, 0x0
    stw r4, 0x34(r1)
    clrlwi r4, r0, 24
    stw r26, 0x38(r1)
    stw r28, 0x3c(r1)
    stw r12, 0x40(r1)
    stw r11, 0x44(r1)
    stw r10, 0x48(r1)
    stw r9, lbl_8047A2DC(r13)
    bl fn_80129BC8
    lhz r0, 0xc(r1)
    cmpwi r0, 0x64
    ble @L_80013AB8
    li r24, 0x3
    b @L_80013ABC
    @L_80013AB8:
    li r24, 0x2
    @L_80013ABC:
    lwz r0, 0x0(r30)
    cmpwi r0, 0x0
    blt @L_80013AE8
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0xa
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80013B00
    @L_80013AE8:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0xa
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80013B00:
    mr r25, r3
    li r26, -0x1
    li r28, 0x0
    b @L_80013B44
    @L_80013B10:
    mr r3, r25
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80013B3C
    addi r26, r26, 0x1
    cmpw r26, r27
    blt @L_80013B3C
    mr r3, r25
    bl fn_80143C50
    b @L_80013B54
    @L_80013B3C:
    addi r28, r28, 0x1
    addi r25, r25, 0x4
    @L_80013B44:
    lhz r0, 0xa(r1)
    cmpw r28, r0
    blt @L_80013B10
    li r3, 0x0
    @L_80013B54:
    clrlwi r7, r3, 16
    cmpwi r7, 0x1
    bge @L_80013B68
    li r28, 0x0
    b @L_80013C20
    @L_80013B68:
    cmpwi r24, 0x2
    bne @L_80013B80
    li r0, 0x1
    li r25, 0x5b
    stw r0, 0x10(r1)
    b @L_80013B8C
    @L_80013B80:
    li r0, 0x2
    li r25, 0x5c
    stw r0, 0x10(r1)
    @L_80013B8C:
    lis r4, lbl_80266918@ha
    li r3, 0x1
    addi r4, r4, lbl_80266918@l
    li r0, 0x0
    add r4, r4, r29
    stw r25, 0x24(r1)
    lbz r6, 0x0(r4)
    lbz r5, 0x1(r4)
    lbz r4, 0x2(r4)
    stb r6, 0x20(r1)
    stb r5, 0x21(r1)
    stb r4, 0x22(r1)
    stw r3, 0x2c(r1)
    stw r7, 0x28(r1)
    stw r0, 0x30(r1)
    stw r3, lbl_8047A2FC(r13)
    bl fn_801046B8
    mr r4, r3
    mr r3, r25
    addi r5, r1, 0x10
    addi r9, r1, 0x20
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    mr r28, r3
    mr r3, r25
    bl fn_80102510
    mr r3, r25
    li r4, 0x1
    bl fn_80102428
    cmpwi r28, -0x1
    bne @L_80013C1C
    li r28, -0x1
    b @L_80013C20
    @L_80013C1C:
    lwz r28, lbl_8047A2FC(r13)
    @L_80013C20:
    cmpwi r28, 0x0
    bge @L_80013C38
    li r0, 0x0
    li r3, 0x0
    stw r0, 0x0(r31)
    b @L_80013DE8
    @L_80013C38:
    lwz r0, 0x0(r30)
    cmpwi r0, 0x0
    blt @L_80013C64
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80013C7C
    @L_80013C64:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80013C7C:
    mr r30, r3
    li r25, -0x1
    li r26, 0x0
    b @L_80013CC0
    @L_80013C8C:
    mr r3, r30
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80013CB8
    addi r25, r25, 0x1
    cmpw r25, r27
    blt @L_80013CB8
    mr r3, r30
    bl fn_80143C68
    b @L_80013CD0
    @L_80013CB8:
    addi r26, r26, 0x1
    addi r30, r30, 0x4
    @L_80013CC0:
    lhz r0, 0x8(r1)
    cmpw r26, r0
    blt @L_80013C8C
    li r3, 0x0
    @L_80013CD0:
    clrlwi r30, r3, 16
    li r3, 0x2d
    mr r4, r30
    bl fn_80132A38
    mr r4, r28
    li r3, 0x2f
    bl fn_80132A38
    lis r4, lbl_80266918@ha
    addi r3, r1, 0x34
    addi r0, r4, lbl_80266918@l
    li r7, 0x2b1d
    add r4, r0, r29
    li r0, 0x2
    lbz r6, 0x0(r4)
    lbz r5, 0x1(r4)
    lbz r4, 0x2(r4)
    stw r7, lbl_8047A2DC(r13)
    stb r6, 0x14(r1)
    stb r5, 0x15(r1)
    stb r4, 0x16(r1)
    stw r3, 0x18(r1)
    stw r0, 0x1c(r1)
    bl fn_801046B8
    mr r4, r3
    addi r9, r1, 0x14
    li r3, 0x5a
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    mr r25, r3
    li r3, 0x5a
    bl fn_80102510
    li r3, 0x5a
    li r4, 0x1
    bl fn_80102428
    cmpwi r25, -0x1
    bne @L_80013D78
    li r25, -0x1
    b @L_80013D8C
    @L_80013D78:
    cmpwi r25, 0x0
    blt @L_80013D88
    cmpwi r25, 0x2
    blt @L_80013D8C
    @L_80013D88:
    li r25, -0x1
    @L_80013D8C:
    cmpwi r25, 0x1
    beq @L_80013D9C
    cmpwi r25, -0x1
    bne @L_80013DAC
    @L_80013D9C:
    li r0, 0x0
    li r3, 0x0
    stw r0, 0x0(r31)
    b @L_80013DE8
    @L_80013DAC:
    mr r4, r30
    li r3, 0x2d
    bl fn_80132A38
    mr r4, r28
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x4269
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    stw r28, 0x0(r31)
    li r3, 0x0
    @L_80013DE8:
    lmw r24, 0x50(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
