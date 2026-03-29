    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    li r3, 0x60
    li r0, 0x0
    stw r3, lbl_8047A4DC(r13)
    li r3, 0x140
    li r4, 0x20
    stw r0, lbl_8047A4D0(r13)
    bl fn_800E2C04
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8003CF78
    bl fn_800E27B0
    b @L_8003CF7C
    @L_8003CF78:
    li r3, 0x0
    @L_8003CF7C:
    lwz r0, lbl_8047A4DC(r13)
    li r4, 0x20
    stw r3, lbl_8047A4D0(r13)
    slwi r3, r0, 2
    addi r0, r3, 0x1f
    clrrwi r3, r0, 5
    bl fn_800E2C04
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8003CFAC
    bl fn_800E27B0
    b @L_8003CFB0
    @L_8003CFAC:
    li r3, 0x0
    @L_8003CFB0:
    lwz r0, lbl_8047A4DC(r13)
    li r4, 0x20
    stw r3, lbl_8047A4D4(r13)
    slwi r3, r0, 3
    addi r0, r3, 0x1f
    clrrwi r3, r0, 5
    bl fn_800E2C04
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8003CFE0
    bl fn_800E27B0
    b @L_8003CFE4
    @L_8003CFE0:
    li r3, 0x0
    @L_8003CFE4:
    li r31, 0x0
    stw r3, lbl_8047A4D8(r13)
    mr r30, r31
    li r28, 0x0
    li r29, 0x0
    b @L_8003D0AC
    @L_8003CFFC:
    addi r0, r29, 0x1
    clrlwi r3, r0, 16
    bl fn_801EE248
    mr r27, r3
    clrlwi r0, r27, 16
    cmplwi r0, 0x0
    beq @L_8003D0A8
    addi r0, r29, 0x1
    clrlwi r3, r0, 16
    bl fn_801EE8F4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8003D048
    addi r0, r29, 0x1
    clrlwi r3, r0, 16
    bl fn_801EE614
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8003D0A8
    @L_8003D048:
    lwz r4, lbl_8047A4D4(r13)
    addi r0, r29, 0x1
    clrlwi r3, r0, 16
    sthx r27, r4, r31
    addi r0, r31, 0x2
    lwz r4, lbl_8047A4D4(r13)
    sthx r3, r4, r0
    bl fn_801EE0BC
    lwz r6, lbl_8047A4D8(r13)
    addi r5, r30, 0x4
    addi r4, r31, 0x2
    addi r0, r30, 0x6
    stwx r3, r6, r30
    addi r30, r30, 0x8
    addi r28, r28, 0x1
    lwz r6, lbl_8047A4D4(r13)
    lwz r3, lbl_8047A4D8(r13)
    lhzx r6, r6, r31
    addi r31, r31, 0x4
    sthx r6, r3, r5
    lwz r5, lbl_8047A4D4(r13)
    lwz r3, lbl_8047A4D8(r13)
    lhzx r4, r5, r4
    sthx r4, r3, r0
    @L_8003D0A8:
    addi r29, r29, 0x1
    @L_8003D0AC:
    lwz r0, lbl_8047A4DC(r13)
    cmpw r29, r0
    blt @L_8003CFFC
    srwi r0, r28, 31
    stw r28, lbl_8047A4DC(r13)
    add r0, r0, r28
    lwz r27, lbl_8047A4D8(r13)
    srawi r31, r0, 1
    b @L_8003D160
    @L_8003D0D0:
    mr r30, r31
    slwi r25, r31, 3
    b @L_8003D14C
    @L_8003D0DC:
    subf r29, r31, r30
    slwi r0, r29, 3
    add r26, r27, r0
    b @L_8003D124
    @L_8003D0EC:
    mr r4, r26
    addi r3, r1, 0x8
    li r5, 0x8
    bl memcpy
    mr r3, r26
    mr r4, r24
    li r5, 0x8
    bl memcpy
    mr r3, r24
    addi r4, r1, 0x8
    li r5, 0x8
    bl memcpy
    subf r26, r25, r26
    subf r29, r31, r29
    @L_8003D124:
    cmpwi r29, 0x0
    blt @L_8003D148
    add r0, r29, r31
    lwz r3, 0x0(r26)
    slwi r0, r0, 3
    add r24, r27, r0
    lwz r0, 0x0(r24)
    cmpw r3, r0
    bgt @L_8003D0EC
    @L_8003D148:
    addi r30, r30, 0x1
    @L_8003D14C:
    cmpw r30, r28
    blt @L_8003D0DC
    srwi r0, r31, 31
    add r0, r0, r31
    srawi r31, r0, 1
    @L_8003D160:
    cmpwi r31, 0x0
    bgt @L_8003D0D0
    li r8, 0x0
    mr r6, r8
    mr r7, r8
    b @L_8003D1B0
    @L_8003D178:
    lwz r5, lbl_8047A4D8(r13)
    addi r0, r6, 0x4
    addi r4, r6, 0x6
    lwz r3, lbl_8047A4D4(r13)
    lhzx r5, r5, r0
    addi r0, r7, 0x2
    addi r6, r6, 0x8
    addi r8, r8, 0x1
    sthx r5, r3, r7
    addi r7, r7, 0x4
    lwz r5, lbl_8047A4D8(r13)
    lwz r3, lbl_8047A4D4(r13)
    lhzx r4, r5, r4
    sthx r4, r3, r0
    @L_8003D1B0:
    lwz r0, lbl_8047A4DC(r13)
    cmpw r8, r0
    blt @L_8003D178
    lwz r3, lbl_8047A4D8(r13)
    bl fn_800E202C
    mr r27, r3
    clrlwi r0, r27, 16
    cmplwi r0, 0x0
    beq @L_8003D1E0
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
    @L_8003D1E0:
    li r0, 0x0
    stw r0, lbl_8047A4D8(r13)
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
