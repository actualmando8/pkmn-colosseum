    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r29, r3
    bl fn_800D0F44
    subis r0, r3, 0x4
    cmplwi r0, 0x0
    beq @L_80073C64
    li r3, 0x1
    b @L_80073E70
@L_80073C64:
    mr r3, r29
    addi r4, r1, 0x9
    bl fn_8025F484
    cmpwi r3, 0x0
    beq @L_80073C80
    li r3, 0x2
    b @L_80073E70
@L_80073C80:
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r25, r0, 0x5
    bl OSGetTick
    lis r5, lbl_803B6E18@ha
    lis r4, lbl_803B6E08@ha
    rlwinm r6, r29, 3, 0, 28
    mr r28, r3
    addi r0, r5, lbl_803B6E18@l
    rlwinm r27, r29, 2, 0, 29
    add r31, r0, r6
    addi r26, r4, lbl_803B6E08@l
    addi r30, r31, 0x4
@L_80073CC8:
    bl OSGetTick
    subf r0, r28, r3
    cmplw r0, r25
    ble @L_80073CE0
    li r3, 0x1
    b @L_80073D60
@L_80073CE0:
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80073CFC
    li r3, 0x2
    b @L_80073D60
@L_80073CFC:
    lbz r0, 0x8(r1)
    andi. r0, r0, 0xa
    cmpwi r0, 0x8
    beq @L_80073D3C
    lwz r12, 0x0(r31)
    cmplwi r12, 0x0
    beq @L_80073D28
    mr r3, r29
    lwz r4, 0x0(r30)
    mtctr r12
    bctrl
@L_80073D28:
    lwzx r0, r26, r27
    cmpwi r0, 0x0
    beq @L_80073CC8
    li r3, 0x3e8
    b @L_80073D60
@L_80073D3C:
    mr r3, r29
    addi r4, r1, 0xc
    addi r5, r1, 0x9
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_80073D5C
    li r3, 0x3
    b @L_80073D60
@L_80073D5C:
    li r3, 0x0
@L_80073D60:
    cmpwi r3, 0x0
    beq @L_80073D70
    addi r3, r3, 0x2
    b @L_80073E70
@L_80073D70:
    lwz r3, 0xc(r1)
    lwz r0, lbl_8047A60C(r13)
    cmplw r3, r0
    beq @L_80073D88
    li r3, 0x6
    b @L_80073E70
@L_80073D88:
    mr r3, r29
    addi r4, r1, 0x9
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80073DA4
    li r3, 0x7
    b @L_80073E70
@L_80073DA4:
    mr r3, r29
    addi r4, r1, 0xc
    addi r5, r1, 0x9
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_80073DC4
    li r3, 0x8
    b @L_80073E70
@L_80073DC4:
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r25, r0, 0x64
    bl OSGetTick
    lis r4, lbl_803B6E08@ha
    mr r26, r3
    rlwinm r27, r29, 2, 0, 29
    addi r28, r4, lbl_803B6E08@l
@L_80073DF8:
    bl OSGetTick
    subf r0, r26, r3
    cmplw r0, r25
    ble @L_80073E10
    li r3, 0x9
    b @L_80073E70
@L_80073E10:
    mr r3, r29
    addi r4, r1, 0x9
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80073E2C
    li r3, 0xa
    b @L_80073E70
@L_80073E2C:
    lbz r0, 0x9(r1)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80073E6C
    lwz r12, 0x0(r31)
    cmplwi r12, 0x0
    beq @L_80073E58
    mr r3, r29
    lwz r4, 0x0(r30)
    mtctr r12
    bctrl
@L_80073E58:
    lwzx r0, r28, r27
    cmpwi r0, 0x0
    beq @L_80073DF8
    li r3, 0x3e8
    b @L_80073E70
@L_80073E6C:
    li r3, 0x0
@L_80073E70:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
