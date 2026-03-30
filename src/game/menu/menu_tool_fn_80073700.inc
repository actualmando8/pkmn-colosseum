    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r21, 0x24(r1)
    mr r24, r3
    mr r30, r4
    bl fn_80073C38
    cmpwi r3, 0x0
    beq @L_80073728
    b @L_8007397C
@L_80073728:
    li r0, 0x99
    mr r3, r24
    stw r0, 0xc(r1)
    addi r4, r1, 0xc
    addi r5, r1, 0x9
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_80073750
    li r3, 0xb
    b @L_80073844
@L_80073750:
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r26, r0, 0x64
    bl OSGetTick
    lis r5, lbl_803B6E18@ha
    lis r4, lbl_803B6E08@ha
    rlwinm r6, r24, 3, 0, 28
    mr r25, r3
    addi r0, r5, lbl_803B6E18@l
    rlwinm r23, r24, 2, 0, 29
    add r28, r0, r6
    addi r22, r4, lbl_803B6E08@l
    addi r27, r28, 0x4
@L_80073798:
    bl OSGetTick
    subf r0, r25, r3
    cmplw r0, r26
    ble @L_800737B0
    li r3, 0x1
    b @L_80073830
@L_800737B0:
    mr r3, r24
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_800737CC
    li r3, 0x2
    b @L_80073830
@L_800737CC:
    lbz r0, 0x8(r1)
    andi. r0, r0, 0xa
    cmpwi r0, 0x8
    beq @L_8007380C
    lwz r12, 0x0(r28)
    cmplwi r12, 0x0
    beq @L_800737F8
    mr r3, r24
    lwz r4, 0x0(r27)
    mtctr r12
    bctrl
@L_800737F8:
    lwzx r0, r22, r23
    cmpwi r0, 0x0
    beq @L_80073798
    li r3, 0x3e8
    b @L_80073830
@L_8007380C:
    mr r3, r24
    addi r4, r1, 0x10
    addi r5, r1, 0x9
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_8007382C
    li r3, 0x3
    b @L_80073830
@L_8007382C:
    li r3, 0x0
@L_80073830:
    cmpwi r3, 0x0
    beq @L_80073840
    addi r3, r3, 0xb
    b @L_80073844
@L_80073840:
    li r3, 0x0
@L_80073844:
    cmpwi r3, 0x0
    beq @L_80073850
    b @L_8007397C
@L_80073850:
    lwz r0, 0x10(r1)
    rlwinm r0, r0, 8, 24, 31
    cmplwi r0, 0x99
    beq @L_80073868
    li r3, 0xf
    b @L_8007397C
@L_80073868:
    lis r4, lbl_803B6E18@ha
    lis r3, lbl_803B6E08@ha
    rlwinm r5, r24, 3, 0, 28
    rlwinm r28, r24, 2, 0, 29
    addi r0, r4, lbl_803B6E18@l
    addi r29, r3, lbl_803B6E08@l
    add r26, r0, r5
    li r25, 0x0
    addi r27, r26, 0x4
    lis r3, 0x1062
    mr r23, r30
    addi r30, r3, 0x4dd3
    lis r31, 0x8000
@L_8007389C:
    lwz r0, 0xf8(r31)
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r30, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r21, r0, 0x64
    bl OSGetTick
    mr r22, r3
@L_800738B8:
    bl OSGetTick
    subf r0, r22, r3
    cmplw r0, r21
    ble @L_800738D0
    li r3, 0x10
    b @L_8007397C
@L_800738D0:
    mr r3, r24
    addi r4, r1, 0xa
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_800738EC
    li r3, 0x11
    b @L_8007397C
@L_800738EC:
    lbz r0, 0xa(r1)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    bne @L_8007392C
    lwz r12, 0x0(r26)
    cmplwi r12, 0x0
    beq @L_80073918
    mr r3, r24
    lwz r4, 0x0(r27)
    mtctr r12
    bctrl
@L_80073918:
    lwzx r0, r29, r28
    cmpwi r0, 0x0
    beq @L_800738B8
    li r3, 0x3e8
    b @L_8007397C
@L_8007392C:
    mr r3, r24
    addi r4, r1, 0x10
    addi r5, r1, 0xa
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_8007394C
    li r3, 0x12
    b @L_8007397C
@L_8007394C:
    lwz r0, 0x10(r1)
    stw r0, 0x0(r23)
    lwzx r0, r29, r28
    cmpwi r0, 0x0
    beq @L_80073968
    li r3, 0x3e8
    b @L_8007397C
@L_80073968:
    addi r25, r25, 0x4
    addi r23, r23, 0x4
    cmpwi r25, 0x278
    blt @L_8007389C
    li r3, 0x0
@L_8007397C:
    lmw r21, 0x24(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
