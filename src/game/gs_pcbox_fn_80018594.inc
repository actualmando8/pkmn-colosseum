    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stmw r27, 0x5c(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    mr r31, r6
    li r0, 0x0
    sth r0, 0x0(r31)
    lwz r0, lbl_8047A2E0(r13)
    cmpwi r0, 0x0
    bne @L_800186CC
    clrlwi r0, r29, 16
    cmplwi r0, 0x21e
    bne @L_800186CC
    lwz r0, lbl_8047A2D8(r13)
    lis r3, lbl_80266918@ha
    li r4, 0x2b41
    mulli r0, r0, 0x4c
    addi r3, r3, lbl_80266918@l
    stw r4, lbl_8047A2DC(r13)
    add r4, r3, r0
    lbzx r3, r3, r0
    lbz r0, 0x1(r4)
    lbz r4, 0x2(r4)
    slwi r3, r3, 24
    slwi r0, r0, 16
    slwi r4, r4, 8
    or r0, r3, r0
    or r0, r4, r0
    ori r3, r0, 0xff
    bl fn_80019754
    mr r28, r3
    clrlwi r27, r28, 16
    cmplwi r27, 0x0
    bne @L_80018630
    li r3, 0x1
    b @L_800188CC
    @L_80018630:
    sth r28, 0x0(r31)
    mr r3, r27
    bl fn_801440A0
    lwz r0, lbl_8047A2E0(r13)
    cmpwi r0, 0x0
    bne @L_80018650
    bl fn_80143DE4
    b @L_80018654
    @L_80018650:
    bl fn_80143DCC
    @L_80018654:
    cmplwi r3, 0x0
    bne @L_80018680
    li r3, 0x2
    li r4, 0x4261
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r3, 0x0
    b @L_800188CC
    @L_80018680:
    mr r12, r3
    mr r3, r27
    addi r4, r1, 0x8
    mtctr r12
    bctrl
    cmpwi r3, 0x2
    bne @L_800186C4
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    ble @L_800186BC
    lwz r3, lbl_8047A2F8(r13)
    mr r4, r28
    clrlwi r5, r0, 16
    li r6, -0x1
    bl fn_801299C8
    @L_800186BC:
    li r3, 0x4
    b @L_800188CC
    @L_800186C4:
    li r3, 0x1
    b @L_800188CC
    @L_800186CC:
    mr r5, r28
    mr r6, r30
    addi r3, r1, 0x18
    li r4, 0x5
    bl fn_80017CB8
    lwz r5, lbl_8047A2D8(r13)
    lis r4, lbl_80266918@ha
    mr r31, r3
    addi r0, r1, 0x18
    mulli r5, r5, 0x4c
    addi r3, r4, lbl_80266918@l
    li r4, 0x135
    stw r0, 0x10(r1)
    add r5, r3, r5
    stw r4, lbl_8047A2DC(r13)
    lbz r4, 0x0(r5)
    lbz r3, 0x1(r5)
    lbz r0, 0x2(r5)
    stb r4, 0xc(r1)
    stb r3, 0xd(r1)
    stb r0, 0xe(r1)
    stw r31, 0x14(r1)
    bl fn_801046B8
    mr r4, r3
    addi r9, r1, 0xc
    li r3, 0x5a
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    mr r27, r3
    li r3, 0x5a
    bl fn_80102510
    li r3, 0x5a
    li r4, 0x1
    bl fn_80102428
    cmpwi r27, -0x1
    bne @L_80018774
    li r27, -0x1
    b @L_80018788
    @L_80018774:
    cmpwi r27, 0x0
    blt @L_80018784
    cmpw r27, r31
    blt @L_80018788
    @L_80018784:
    li r27, -0x1
    @L_80018788:
    cmpwi r27, 0x0
    bge @L_80018798
    li r3, 0x1
    b @L_800188CC
    @L_80018798:
    mulli r0, r27, 0xc
    addi r3, r1, 0x1c
    lwzx r12, r3, r0
    cmplwi r12, 0x0
    bne @L_800187B4
    li r3, 0x1
    b @L_800188CC
    @L_800187B4:
    mr r3, r28
    mr r4, r30
    addi r5, r1, 0x8
    mtctr r12
    bctrl
    cmpwi r3, 0x2
    beq @L_8001885C
    bge @L_800187E4
    cmpwi r3, 0x0
    beq @L_800187F4
    bge @L_80018854
    b @L_800188C8
    @L_800187E4:
    cmpwi r3, 0x4
    beq @L_8001886C
    bge @L_800188C8
    b @L_80018864
    @L_800187F4:
    lwz r5, 0x8(r1)
    cmpwi r5, 0x0
    ble @L_80018848
    mulli r0, r28, 0x4c
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    add r3, r3, r0
    lwz r0, 0x4(r3)
    cmpwi r0, -0x1
    bne @L_80018834
    lwz r3, lbl_8047A2F8(r13)
    mr r4, r29
    clrlwi r5, r5, 16
    extsh r6, r30
    bl fn_8012959C
    b @L_80018848
    @L_80018834:
    lwz r3, lbl_8047A2F8(r13)
    mr r4, r29
    clrlwi r5, r5, 16
    extsh r6, r30
    bl fn_801299C8
    @L_80018848:
    stw r30, lbl_8047A2B8(r13)
    li r3, 0x0
    b @L_800188CC
    @L_80018854:
    li r3, 0x2
    b @L_800188CC
    @L_8001885C:
    li r3, 0x3
    b @L_800188CC
    @L_80018864:
    li r3, 0x1
    b @L_800188CC
    @L_8001886C:
    lwz r5, 0x8(r1)
    cmpwi r5, 0x0
    ble @L_800188C0
    mulli r0, r28, 0x4c
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    add r3, r3, r0
    lwz r0, 0x4(r3)
    cmpwi r0, -0x1
    bne @L_800188AC
    lwz r3, lbl_8047A2F8(r13)
    mr r4, r29
    clrlwi r5, r5, 16
    extsh r6, r30
    bl fn_8012959C
    b @L_800188C0
    @L_800188AC:
    lwz r3, lbl_8047A2F8(r13)
    mr r4, r29
    clrlwi r5, r5, 16
    extsh r6, r30
    bl fn_801299C8
    @L_800188C0:
    li r3, 0x4
    b @L_800188CC
    @L_800188C8:
    li r3, 0x0
    @L_800188CC:
    lmw r27, 0x5c(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
