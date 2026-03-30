    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r20, 0x20(r1)
    mr r22, r3
    mr r23, r4
    addi r25, r22, 0x1
    li r4, 0x2
    mr r3, r25
    bl fn_8008ABE4
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r24, r0, 0x64
    bl OSGetTick
    lis r5, lbl_803B6E18@ha
    rlwinm r4, r23, 8, 24, 31
    rlwinm r0, r23, 24, 16, 23
    rlwinm r7, r22, 3, 0, 28
    addi r5, r5, lbl_803B6E18@l
    lis r6, lbl_803B6E08@ha
    add r26, r5, r7
    rlwinm r5, r23, 8, 8, 15
    or r0, r4, r0
    rlwinm r4, r23, 24, 0, 7
    or r0, r5, r0
    mr r29, r3
    addi r28, r26, 0x4
    rlwinm r30, r22, 2, 0, 29
    addi r31, r6, lbl_803B6E08@l
    or r27, r4, r0
@L_80073184:
    bl OSGetTick
    subf r4, r29, r3
    mr r3, r22
    xor r0, r4, r24
    cntlzw r0, r0
    slw r0, r4, r0
    rlwinm r23, r0, 1, 31, 31
    bl fn_80073C38
    cmpwi r3, 0x0
    beq @L_800731B4
    mr r20, r3
    b @L_8007338C
@L_800731B4:
    li r0, 0x77
    mr r3, r22
    stw r0, 0x14(r1)
    addi r4, r1, 0xc
    addi r5, r1, 0x9
    stw r0, 0xc(r1)
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_800731E0
    li r20, 0xb
    b @L_800732B4
@L_800731E0:
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r20, r0, 0x64
    bl OSGetTick
    mr r21, r3
@L_80073208:
    bl OSGetTick
    subf r0, r21, r3
    cmplw r0, r20
    ble @L_80073220
    li r3, 0x1
    b @L_800732A0
@L_80073220:
    mr r3, r22
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_8007323C
    li r3, 0x2
    b @L_800732A0
@L_8007323C:
    lbz r0, 0x8(r1)
    andi. r0, r0, 0xa
    cmpwi r0, 0x8
    beq @L_8007327C
    lwz r12, 0x0(r26)
    cmplwi r12, 0x0
    beq @L_80073268
    mr r3, r22
    lwz r4, 0x0(r28)
    mtctr r12
    bctrl
@L_80073268:
    lwzx r0, r31, r30
    cmpwi r0, 0x0
    beq @L_80073208
    li r3, 0x3e8
    b @L_800732A0
@L_8007327C:
    mr r3, r22
    addi r4, r1, 0x10
    addi r5, r1, 0x9
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_8007329C
    li r3, 0x3
    b @L_800732A0
@L_8007329C:
    li r3, 0x0
@L_800732A0:
    cmpwi r3, 0x0
    beq @L_800732B0
    addi r20, r3, 0xb
    b @L_800732B4
@L_800732B0:
    li r20, 0x0
@L_800732B4:
    cmpwi r20, 0x0
    beq @L_800732C0
    b @L_8007338C
@L_800732C0:
    lwz r0, 0x10(r1)
    rlwinm r0, r0, 8, 24, 31
    cmplwi r0, 0x77
    beq @L_800732D8
    li r20, 0xf
    b @L_8007338C
@L_800732D8:
    stw r27, 0x14(r1)
    mr r3, r22
    addi r4, r1, 0x14
    addi r5, r1, 0xa
    bl fn_8025F648
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r20, r0, 0x64
    bl OSGetTick
    mr r21, r3
@L_80073314:
    bl OSGetTick
    subf r0, r21, r3
    cmplw r0, r20
    ble @L_8007332C
    li r20, 0x10
    b @L_8007338C
@L_8007332C:
    mr r3, r22
    addi r4, r1, 0xa
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80073348
    li r20, 0x11
    b @L_8007338C
@L_80073348:
    lbz r0, 0xa(r1)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80073388
    lwz r12, 0x0(r26)
    cmplwi r12, 0x0
    beq @L_80073374
    mr r3, r22
    lwz r4, 0x0(r28)
    mtctr r12
    bctrl
@L_80073374:
    lwzx r0, r31, r30
    cmpwi r0, 0x0
    beq @L_80073314
    li r20, 0x3e8
    b @L_8007338C
@L_80073388:
    li r20, 0x0
@L_8007338C:
    cmpwi r20, 0x1
    bne @L_8007339C
    cmpwi r23, 0x0
    beq @L_80073184
@L_8007339C:
    cmpwi r20, 0x0
    mr r3, r25
    beq @L_800733B0
    li r4, 0x1
    b @L_800733B4
@L_800733B0:
    li r4, 0x3
@L_800733B4:
    bl fn_8008ABE4
    mr r3, r20
    lmw r20, 0x20(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
