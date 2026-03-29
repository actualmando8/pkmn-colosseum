    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r30, r4
    mr r31, r5
    mulli r0, r3, 0x4c
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    add r3, r3, r0
    lwz r0, 0x4(r3)
    cmpwi r0, 0x0
    blt @L_800137A0
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_800137B8
    @L_800137A0:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_800137B8:
    mr r27, r3
    li r29, -0x1
    li r28, 0x0
    b @L_80013800
    @L_800137C8:
    mr r3, r27
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800137F8
    addi r29, r29, 0x1
    cmpw r29, r30
    blt @L_800137F8
    mr r3, r27
    bl fn_80143C68
    mr r29, r3
    b @L_80013810
    @L_800137F8:
    addi r28, r28, 0x1
    addi r27, r27, 0x4
    @L_80013800:
    lhz r0, 0x8(r1)
    cmpw r28, r0
    blt @L_800137C8
    li r29, 0x0
    @L_80013810:
    lwz r3, lbl_8047A2F8(r13)
    mr r4, r29
    li r5, 0x1
    li r6, -0x1
    bl fn_80129A78
    cmpwi r3, 0x0
    ble @L_80013858
    li r3, 0x2
    li r4, 0x4263
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x0
    li r3, 0x0
    stw r0, 0x0(r31)
    b @L_800138A0
    @L_80013858:
    lwz r3, lbl_8047A2F8(r13)
    mr r4, r29
    extsh r6, r30
    li r5, 0x1
    bl fn_8012959C
    clrlwi r4, r29, 16
    li r3, 0x2d
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x4268
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x0
    li r3, 0x0
    stw r0, 0x0(r31)
    @L_800138A0:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
