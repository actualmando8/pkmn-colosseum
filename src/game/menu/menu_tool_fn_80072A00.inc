    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r18, 0x18(r1)
    mr r27, r3
    addi r29, r27, 0x1
    li r4, 0x2
    mr r3, r29
    bl fn_8008ABE4
    mr r3, r27
    bl fn_80073C38
    cmpwi r3, 0x0
    beq @L_80072A3C
    mr r28, r3
    b @L_80072C50
@L_80072A3C:
    li r0, 0x60
    mr r3, r27
    stw r0, 0x14(r1)
    addi r4, r1, 0x14
    addi r5, r1, 0xa
    bl fn_8025F648
    mr r28, r3
    cmpwi r28, 0x0
    beq @L_80072A68
    li r28, 0xb
    b @L_80072C50
@L_80072A68:
    lis r3, 0x8000
    lwz r0, 0xf8(r3)
    rlwinm r0, r0, 30, 2, 31
    mulli r20, r0, 0x3
    bl OSGetTick
    lis r5, lbl_803B6E18@ha
    lis r4, lbl_803B6E08@ha
    rlwinm r6, r27, 3, 0, 28
    mr r21, r3
    addi r0, r5, lbl_803B6E18@l
    rlwinm r22, r27, 2, 0, 29
    add r30, r0, r6
    addi r23, r4, lbl_803B6E08@l
    addi r31, r30, 0x4
    lis r3, 0x1062
    lis r25, 0x8000
    addi r24, r3, 0x4dd3
@L_80072AAC:
    bl OSGetTick
    subf r0, r21, r3
    cmplw r0, r20
    ble @L_80072AC4
    li r28, 0x10
    b @L_80072C50
@L_80072AC4:
    lwz r0, 0xf8(r25)
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r24, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r19, r0, 0x5
    bl OSGetTick
    mr r26, r3
@L_80072AE0:
    bl OSGetTick
    subf r4, r26, r3
    mr r3, r27
    xor r0, r4, r19
    cntlzw r0, r0
    slw r0, r4, r0
    rlwinm r18, r0, 1, 31, 31
    bl fn_80073C38
    cmpwi r3, 0x1
    bne @L_80072B10
    cmpwi r18, 0x0
    beq @L_80072AE0
@L_80072B10:
    cmpwi r3, 0x0
    beq @L_80072B20
    mr r18, r3
    b @L_80072C44
@L_80072B20:
    li r0, 0xaa
    mr r3, r27
    stw r0, 0xc(r1)
    addi r4, r1, 0xc
    addi r5, r1, 0x9
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_80072B48
    li r18, 0xb
    b @L_80072C1C
@L_80072B48:
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r19, r0, 0x64
    bl OSGetTick
    mr r26, r3
@L_80072B70:
    bl OSGetTick
    subf r0, r26, r3
    cmplw r0, r19
    ble @L_80072B88
    li r3, 0x1
    b @L_80072C08
@L_80072B88:
    mr r3, r27
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80072BA4
    li r3, 0x2
    b @L_80072C08
@L_80072BA4:
    lbz r0, 0x8(r1)
    andi. r0, r0, 0xa
    cmpwi r0, 0x8
    beq @L_80072BE4
    lwz r12, 0x0(r30)
    cmplwi r12, 0x0
    beq @L_80072BD0
    mr r3, r27
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
@L_80072BD0:
    lwzx r0, r23, r22
    cmpwi r0, 0x0
    beq @L_80072B70
    li r3, 0x3e8
    b @L_80072C08
@L_80072BE4:
    mr r3, r27
    addi r4, r1, 0x10
    addi r5, r1, 0x9
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_80072C04
    li r3, 0x3
    b @L_80072C08
@L_80072C04:
    li r3, 0x0
@L_80072C08:
    cmpwi r3, 0x0
    beq @L_80072C18
    addi r18, r3, 0xb
    b @L_80072C1C
@L_80072C18:
    li r18, 0x0
@L_80072C1C:
    cmpwi r18, 0x0
    beq @L_80072C28
    b @L_80072C44
@L_80072C28:
    lwz r0, 0x10(r1)
    rlwinm r0, r0, 8, 24, 31
    cmplwi r0, 0xaa
    beq @L_80072C40
    li r18, 0xf
    b @L_80072C44
@L_80072C40:
    li r18, 0x0
@L_80072C44:
    bl fn_800F0308
    cmpwi r18, 0x0
    bne @L_80072AAC
@L_80072C50:
    mr r3, r29
    li r4, 0x1
    bl fn_8008ABE4
    mr r3, r28
    lmw r18, 0x18(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
