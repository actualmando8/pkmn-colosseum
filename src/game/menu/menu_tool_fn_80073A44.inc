    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r24, 0x20(r1)
    mr r30, r3
    mr r31, r4
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r24, r0, 0x5
    bl OSGetTick
    mr r27, r3
@L_80073A84:
    bl OSGetTick
    subf r4, r27, r3
    mr r3, r30
    xor r0, r4, r24
    cntlzw r0, r0
    slw r0, r4, r0
    rlwinm r25, r0, 1, 31, 31
    bl fn_80073C38
    cmpwi r3, 0x1
    bne @L_80073AB4
    cmpwi r25, 0x0
    beq @L_80073A84
@L_80073AB4:
    cmpwi r3, 0x0
    beq @L_80073AC0
    b @L_80073C24
@L_80073AC0:
    li r0, 0xaa
    mr r3, r30
    stw r0, 0xc(r1)
    addi r4, r1, 0xc
    addi r5, r1, 0x9
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_80073AE8
    li r3, 0xb
    b @L_80073BDC
@L_80073AE8:
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
    rlwinm r6, r30, 3, 0, 28
    mr r27, r3
    addi r0, r5, lbl_803B6E18@l
    rlwinm r28, r30, 2, 0, 29
    add r24, r0, r6
    addi r29, r4, lbl_803B6E08@l
    addi r25, r24, 0x4
@L_80073B30:
    bl OSGetTick
    subf r0, r27, r3
    cmplw r0, r26
    ble @L_80073B48
    li r3, 0x1
    b @L_80073BC8
@L_80073B48:
    mr r3, r30
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80073B64
    li r3, 0x2
    b @L_80073BC8
@L_80073B64:
    lbz r0, 0x8(r1)
    andi. r0, r0, 0xa
    cmpwi r0, 0x8
    beq @L_80073BA4
    lwz r12, 0x0(r24)
    cmplwi r12, 0x0
    beq @L_80073B90
    mr r3, r30
    lwz r4, 0x0(r25)
    mtctr r12
    bctrl
@L_80073B90:
    lwzx r0, r29, r28
    cmpwi r0, 0x0
    beq @L_80073B30
    li r3, 0x3e8
    b @L_80073BC8
@L_80073BA4:
    mr r3, r30
    addi r4, r1, 0x10
    addi r5, r1, 0x9
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_80073BC4
    li r3, 0x3
    b @L_80073BC8
@L_80073BC4:
    li r3, 0x0
@L_80073BC8:
    cmpwi r3, 0x0
    beq @L_80073BD8
    addi r3, r3, 0xb
    b @L_80073BDC
@L_80073BD8:
    li r3, 0x0
@L_80073BDC:
    cmpwi r3, 0x0
    beq @L_80073BE8
    b @L_80073C24
@L_80073BE8:
    lwz r4, 0x10(r1)
    rlwinm r5, r4, 8, 24, 31
    cmplwi r5, 0xaa
    beq @L_80073C00
    li r3, 0xf
    b @L_80073C24
@L_80073C00:
    rlwinm r0, r4, 24, 16, 23
    rlwinm r3, r4, 8, 8, 15
    or r0, r5, r0
    rlwinm r4, r4, 24, 0, 7
    or r0, r3, r0
    li r3, 0x0
    or r0, r4, r0
    rlwinm r0, r0, 16, 16, 31
    sth r0, 0x0(r31)
@L_80073C24:
    lmw r24, 0x20(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
