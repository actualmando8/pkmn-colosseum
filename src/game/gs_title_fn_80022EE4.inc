    nofralloc
    stwu r1, -0x130(r1)
    mflr r0
    stw r0, 0x134(r1)
    stw r31, 0x12c(r1)
    stw r30, 0x128(r1)
    stw r29, 0x124(r1)
    stw r28, 0x120(r1)
    mr r28, r3
    mr r29, r4
    li r4, 0x1
    bl fn_800141BC
    mr r31, r3
    cmpwi r31, 0x0
    blt @L_80022FA0
    addi r4, r1, 0x14
    addi r5, r1, 0x10
    bl fn_80014118
    lwz r3, 0x14(r1)
    bl fn_8011FC74
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80022F84
    lwz r4, 0x14(r1)
    addi r3, r1, 0x18
    lwz r5, 0x10(r1)
    clrlwi r6, r28, 16
    li r7, 0x0
    bl fn_80144574
    mr r30, r3
    extsh r0, r30
    cmpwi r0, 0x0
    bgt @L_80022FA0
    li r3, 0x2
    li r4, 0x4261
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_80022FA0
    @L_80022F84:
    li r3, 0x2
    li r4, 0x424c
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    @L_80022FA0:
    mr r3, r31
    bl fn_80014198
    cmpwi r31, 0x0
    blt @L_80023044
    extsh r0, r30
    cmpwi r0, 0x0
    ble @L_80023044
    lwz r3, 0x14(r1)
    clrlwi r5, r28, 16
    addi r6, r1, 0x8
    addi r7, r1, 0xc
    li r4, 0x1
    bl fn_80128A64
    lfs f1, lbl_8047B8A4(r2)
    mr r30, r3
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0x0
    bl fn_801096F8
    lwz r3, 0x14(r1)
    mr r4, r30
    lhz r5, 0x8(r1)
    addi r6, r1, 0xc
    li r7, 0x0
    li r8, 0x1
    li r9, 0x0
    li r10, 0x0
    bl fn_8012805C
    li r3, 0x1
    bl fn_801096F8
    lfs f1, lbl_8047B8A4(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r0, 0x1
    li r3, 0x0
    stw r0, 0x0(r29)
    b @L_80023048
    @L_80023044:
    li r3, 0x1
    @L_80023048:
    lwz r0, 0x134(r1)
    lwz r31, 0x12c(r1)
    lwz r30, 0x128(r1)
    lwz r29, 0x124(r1)
    lwz r28, 0x120(r1)
    mtlr r0
    addi r1, r1, 0x130
    blr
