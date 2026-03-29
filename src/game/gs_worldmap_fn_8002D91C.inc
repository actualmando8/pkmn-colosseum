    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r31, r3
    lwz r4, lbl_80478E54(r13)
    slwi r0, r31, 2
    li r5, 0x0
    la r30, lbl_8047A3FC(r13)
    stw r3, lbl_8047A3FC(r13)
    add r4, r4, r0
    stw r5, 0x4(r30)
    lbz r28, 0x1(r4)
    cmpwi r28, 0x1
    beq @L_8002DA94
    bge @L_8002DAA0
    cmpwi r28, 0x0
    bge @L_8002D968
    b @L_8002DAA0
    @L_8002D968:
    li r3, 0x0
    li r4, 0xc
    li r5, 0x0
    bl fn_8012A5B0
    mr r7, r3
    mr r4, r31
    addi r3, r1, 0x9
    li r5, 0x0
    li r6, 0x4b
    li r8, -0x1
    crxor 6, 6, 6
    bl fn_8002A0B8
    lbz r7, 0x9(r1)
    mr r4, r3
    li r3, 0x2
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    b @L_8002DA24
    @L_8002D9B4:
    li r3, 0x1
    bl fn_801069FC
    cmpwi r26, 0x1
    beq @L_8002D9E4
    bge @L_8002D9F4
    cmpwi r26, 0x0
    bge @L_8002D9D4
    b @L_8002D9F4
    @L_8002D9D4:
    mr r3, r31
    mr r4, r28
    bl fn_8002D154
    b @L_8002D9F4
    @L_8002D9E4:
    mr r4, r31
    li r3, 0x3
    li r5, 0x0
    bl fn_80018F54
    @L_8002D9F4:
    mr r4, r31
    addi r3, r1, 0x9
    li r5, 0x1
    li r6, -0x1
    crxor 6, 6, 6
    bl fn_8002A0B8
    lbz r7, 0x9(r1)
    mr r4, r3
    li r3, 0x2
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    @L_8002DA24:
    li r3, 0x62
    li r4, 0x1
    bl fn_8010264C
    mr r29, r3
    li r3, 0x62
    bl fn_80102510
    li r3, 0x62
    li r4, 0x1
    bl fn_80102428
    cmpwi r29, -0x1
    beq @L_8002DA58
    cmpwi r29, 0x2
    bne @L_8002DA60
    @L_8002DA58:
    li r26, 0x2
    b @L_8002DA74
    @L_8002DA60:
    cmpwi r29, 0x0
    bne @L_8002DA70
    li r26, 0x0
    b @L_8002DA74
    @L_8002DA70:
    li r26, 0x1
    @L_8002DA74:
    cmpwi r26, 0x2
    bne @L_8002D9B4
    mr r3, r31
    li r4, 0x2
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A2CC
    b @L_8002DC3C
    @L_8002DA94:
    mr r4, r28
    bl fn_8002CE6C
    b @L_8002DC3C
    @L_8002DAA0:
    li r26, 0x0
    li r3, 0x7198
    bl fn_800E3534
    mr r27, r3
    bl fn_800E27B0
    stw r3, lbl_8047A3DC(r13)
    mr r4, r31
    addi r3, r1, 0x8
    li r5, 0x0
    li r6, -0x1
    crxor 6, 6, 6
    bl fn_80029FAC
    lbz r7, 0x8(r1)
    mr r4, r3
    li r3, 0x2
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    b @L_8002DB90
    @L_8002DAEC:
    li r3, 0x1
    bl fn_801069FC
    cmpwi r29, 0x2
    beq @L_8002DB3C
    bge @L_8002DB10
    cmpwi r29, 0x0
    beq @L_8002DB1C
    bge @L_8002DB2C
    b @L_8002DB58
    @L_8002DB10:
    cmpwi r29, 0x4
    bge @L_8002DB58
    b @L_8002DB54
    @L_8002DB1C:
    mr r3, r31
    mr r4, r28
    bl fn_8002C408
    b @L_8002DB58
    @L_8002DB2C:
    mr r3, r31
    mr r4, r28
    bl fn_8002C284
    b @L_8002DB58
    @L_8002DB3C:
    mr r3, r31
    li r4, 0xb
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A1C4
    b @L_8002DB58
    @L_8002DB54:
    li r26, 0x1
    @L_8002DB58:
    cmpwi r26, 0x0
    bne @L_8002DBFC
    mr r4, r31
    addi r3, r1, 0x8
    li r5, 0x1
    li r6, -0x1
    crxor 6, 6, 6
    bl fn_80029FAC
    lbz r7, 0x8(r1)
    mr r4, r3
    li r3, 0x2
    li r5, 0x1
    li r6, 0x0
    bl fn_80106ADC
    @L_8002DB90:
    li r3, 0x83
    li r4, 0x1
    bl fn_8010264C
    mr r29, r3
    li r3, 0x83
    bl fn_80102510
    li r3, 0x83
    li r4, 0x1
    bl fn_80102428
    cmpwi r29, 0x1
    beq @L_8002DBE0
    bge @L_8002DBCC
    cmpwi r29, 0x0
    bge @L_8002DBD8
    b @L_8002DBF0
    @L_8002DBCC:
    cmpwi r29, 0x3
    bge @L_8002DBF0
    b @L_8002DBE8
    @L_8002DBD8:
    li r29, 0x0
    b @L_8002DBF4
    @L_8002DBE0:
    li r29, 0x1
    b @L_8002DBF4
    @L_8002DBE8:
    li r29, 0x2
    b @L_8002DBF4
    @L_8002DBF0:
    li r29, 0x3
    @L_8002DBF4:
    cmpwi r29, 0x3
    bne @L_8002DAEC
    @L_8002DBFC:
    cmplwi r28, 0x3
    beq @L_8002DC24
    cmplwi r28, 0x2
    beq @L_8002DC24
    mr r3, r31
    li r4, 0x2
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_8002A1C4
    b @L_8002DC2C
    @L_8002DC24:
    li r3, 0x1
    bl fn_801069FC
    @L_8002DC2C:
    mr r3, r27
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
    @L_8002DC3C:
    lwz r0, 0x4(r30)
    cmpwi r0, 0x0
    beq @L_8002DC58
    bl fn_800FF660
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    @L_8002DC58:
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
