    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r16, 0x20(r1)
    mr r31, r3
    cmpwi r4, 0x0
    lis r3, lbl_80268780@ha
    addi r30, r3, lbl_80268780@l
    beq @L_80074844
    lis r3, lbl_803B6E40@ha
    lwz r18, lbl_8047A604(r13)
    addi r0, r3, lbl_803B6E40@l
    mr r22, r0
    b @L_80074854
@L_80074844:
    lis r3, lbl_803D6E40@ha
    lwz r18, lbl_8047A608(r13)
    addi r0, r3, lbl_803D6E40@l
    mr r22, r0
@L_80074854:
    li r16, 0x0
@L_80074858:
    cmplwi r16, 0x20
    ble @L_8007486C
    lis r3, 0xdd65
    addi r20, r3, 0x4321
    b @L_80074878
@L_8007486C:
    bl OSGetTick
    rlwinm r0, r3, 0, 8, 31
    oris r20, r0, 0xdd00
@L_80074878:
    li r3, 0x0
    mr r5, r20
    mr r4, r3
    li r0, 0x4
    mtctr r0
@L_8007488C:
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_8007489C
    addi r3, r3, 0x1
@L_8007489C:
    rlwinm r5, r5, 31, 1, 31
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_800748B0
    addi r3, r3, 0x1
@L_800748B0:
    rlwinm r5, r5, 31, 1, 31
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_800748C4
    addi r3, r3, 0x1
@L_800748C4:
    rlwinm r5, r5, 31, 1, 31
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_800748D8
    addi r3, r3, 0x1
@L_800748D8:
    rlwinm r5, r5, 31, 1, 31
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_800748EC
    addi r3, r3, 0x1
@L_800748EC:
    rlwinm r5, r5, 31, 1, 31
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80074900
    addi r3, r3, 0x1
@L_80074900:
    rlwinm r5, r5, 31, 1, 31
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80074914
    addi r3, r3, 0x1
@L_80074914:
    rlwinm r5, r5, 31, 1, 31
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80074928
    addi r3, r3, 0x1
@L_80074928:
    rlwinm r5, r5, 31, 1, 31
    addi r4, r4, 0x7
    bdnz @L_8007488C
    cmplwi r3, 0xa
    addi r16, r16, 0x1
    blt @L_80074858
    cmplwi r3, 0x18
    bgt @L_80074858
    rlwinm r3, r20, 8, 24, 31
    rlwinm r0, r20, 24, 16, 23
    rlwinm r4, r20, 8, 8, 15
    rlwinm r5, r20, 24, 0, 7
    or r0, r3, r0
    mr r3, r31
    or r0, r4, r0
    addi r4, r1, 0xc
    or r0, r5, r0
    addi r5, r1, 0x8
    stw r0, 0xc(r1)
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_80074988
    li r3, 0x1
    b @L_8007537C
@L_80074988:
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r16, r0, 0x64
    bl OSGetTick
    lis r5, lbl_803B6E18@ha
    lis r4, lbl_803B6E08@ha
    rlwinm r6, r31, 3, 0, 28
    mr r17, r3
    addi r0, r5, lbl_803B6E18@l
    rlwinm r21, r31, 2, 0, 29
    add r29, r0, r6
    addi r19, r4, lbl_803B6E08@l
    addi r28, r29, 0x4
@L_800749D0:
    bl OSGetTick
    subf r0, r17, r3
    cmplw r0, r16
    ble @L_800749E8
    li r3, 0x2
    b @L_8007537C
@L_800749E8:
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80074A04
    li r3, 0x3
    b @L_8007537C
@L_80074A04:
    lbz r0, 0x8(r1)
    cmplwi r0, 0x38
    beq @L_80074A40
    lwz r12, 0x0(r29)
    cmplwi r12, 0x0
    beq @L_80074A2C
    mr r3, r31
    lwz r4, 0x0(r28)
    mtctr r12
    bctrl
@L_80074A2C:
    lwzx r0, r19, r21
    cmpwi r0, 0x0
    beq @L_800749D0
    li r3, 0x3e8
    b @L_8007537C
@L_80074A40:
    mr r3, r31
    addi r4, r1, 0x10
    addi r5, r1, 0x8
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_80074A60
    li r3, 0x4
    b @L_8007537C
@L_80074A60:
    lwz r5, 0x10(r1)
    rlwinm r3, r5, 8, 24, 31
    rlwinm r0, r5, 24, 16, 23
    rlwinm r4, r5, 8, 8, 15
    rlwinm r5, r5, 24, 0, 7
    or r0, r3, r0
    or r0, r4, r0
    or r4, r5, r0
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0xee
    beq @L_80074AA0
    addi r3, r30, 0x68
    crxor 6, 6, 6
    bl OSReport
    li r21, 0x0
    b @L_80074B80
@L_80074AA0:
    li r3, 0x0
    rlwinm r21, r4, 0, 0, 23
    mr r5, r3
    li r0, 0x3
    mtctr r0
@L_80074AB4:
    rlwinm r0, r4, 0, 0, 0
    cmplwi r0, 0x0
    beq @L_80074AC4
    addi r3, r3, 0x1
@L_80074AC4:
    rlwinm r4, r4, 1, 0, 30
    rlwinm r0, r4, 0, 0, 0
    cmplwi r0, 0x0
    beq @L_80074AD8
    addi r3, r3, 0x1
@L_80074AD8:
    rlwinm r4, r4, 1, 0, 30
    rlwinm r0, r4, 0, 0, 0
    cmplwi r0, 0x0
    beq @L_80074AEC
    addi r3, r3, 0x1
@L_80074AEC:
    rlwinm r4, r4, 1, 0, 30
    rlwinm r0, r4, 0, 0, 0
    cmplwi r0, 0x0
    beq @L_80074B00
    addi r3, r3, 0x1
@L_80074B00:
    rlwinm r4, r4, 1, 0, 30
    rlwinm r0, r4, 0, 0, 0
    cmplwi r0, 0x0
    beq @L_80074B14
    addi r3, r3, 0x1
@L_80074B14:
    rlwinm r4, r4, 1, 0, 30
    rlwinm r0, r4, 0, 0, 0
    cmplwi r0, 0x0
    beq @L_80074B28
    addi r3, r3, 0x1
@L_80074B28:
    rlwinm r4, r4, 1, 0, 30
    rlwinm r0, r4, 0, 0, 0
    cmplwi r0, 0x0
    beq @L_80074B3C
    addi r3, r3, 0x1
@L_80074B3C:
    rlwinm r4, r4, 1, 0, 30
    rlwinm r0, r4, 0, 0, 0
    cmplwi r0, 0x0
    beq @L_80074B50
    addi r3, r3, 0x1
@L_80074B50:
    rlwinm r4, r4, 1, 0, 30
    addi r5, r5, 0x7
    bdnz @L_80074AB4
    cmplwi r3, 0x7
    blt @L_80074B6C
    cmplwi r3, 0xe
    ble @L_80074B80
@L_80074B6C:
    mr r4, r21
    addi r3, r30, 0x98
    crxor 6, 6, 6
    bl OSReport
    li r21, 0x0
@L_80074B80:
    cmplwi r21, 0x0
    bne @L_80074B90
    li r3, 0x5
    b @L_8007537C
@L_80074B90:
    addi r0, r18, 0x7
    mr r3, r31
    rlwinm r27, r0, 0, 0, 28
    addi r4, r1, 0xc
    rlwinm r6, r27, 29, 3, 31
    addi r5, r1, 0x8
    subi r8, r6, 0x1
    rlwinm r6, r8, 8, 24, 31
    rlwinm r0, r8, 24, 16, 23
    rlwinm r7, r8, 8, 8, 15
    or r0, r6, r0
    rlwinm r6, r8, 24, 0, 7
    or r0, r7, r0
    or r0, r6, r0
    stw r0, 0xc(r1)
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_80074BE0
    li r3, 0x6
    b @L_8007537C
@L_80074BE0:
    lis r4, 0x8000
    lis r3, 0x1062
    lwz r0, 0xf8(r4)
    addi r3, r3, 0x4dd3
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r3, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r16, r0, 0x64
    bl OSGetTick
    lis r4, lbl_803B6E08@ha
    mr r19, r3
    rlwinm r18, r31, 2, 0, 29
    addi r17, r4, lbl_803B6E08@l
@L_80074C14:
    bl OSGetTick
    subf r0, r19, r3
    cmplw r0, r16
    ble @L_80074C2C
    li r3, 0x7
    b @L_8007537C
@L_80074C2C:
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80074C48
    li r3, 0x8
    b @L_8007537C
@L_80074C48:
    lbz r3, 0x8(r1)
    rlwinm r0, r3, 0, 26, 27
    cmpwi r0, 0x30
    beq @L_80074C60
    li r3, 0x9
    b @L_8007537C
@L_80074C60:
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80074C9C
    lwz r12, 0x0(r29)
    cmplwi r12, 0x0
    beq @L_80074C88
    mr r3, r31
    lwz r4, 0x0(r28)
    mtctr r12
    bctrl
@L_80074C88:
    lwzx r0, r17, r18
    cmpwi r0, 0x0
    beq @L_80074C14
    li r3, 0x3e8
    b @L_8007537C
@L_80074C9C:
    lis r3, 0x6177
    xor r5, r20, r21
    addi r0, r3, 0x614b
    lis r3, lbl_803B6E08@ha
    mullw r4, r5, r0
    mr r24, r5
    rlwinm r17, r31, 2, 0, 29
    addi r18, r3, lbl_803B6E08@l
    li r25, 0x30
    li r23, 0x0
    addi r26, r4, 0x1
    lis r3, 0x1062
    lis r20, 0x8000
    addi r19, r3, 0x4dd3
    b @L_80074F34
@L_80074CD8:
    lwz r5, 0x0(r22)
    cmplwi r23, 0xa0
    rlwinm r3, r5, 8, 24, 31
    rlwinm r0, r5, 24, 16, 23
    rlwinm r4, r5, 8, 8, 15
    rlwinm r5, r5, 24, 0, 7
    or r0, r3, r0
    or r0, r4, r0
    or r0, r5, r0
    mr r5, r0
    blt @L_80074E18
    subf r5, r24, r0
    xor r4, r24, r0
    xor r5, r5, r26
    li r3, 0x20
    li r0, 0x4
    mtctr r0
@L_80074D1C:
    rlwinm r0, r4, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80074D34
    rlwinm r4, r4, 31, 1, 31
    xori r4, r4, 0xa1c1
    b @L_80074D38
@L_80074D34:
    rlwinm r4, r4, 31, 1, 31
@L_80074D38:
    rlwinm r0, r4, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80074D50
    rlwinm r4, r4, 31, 1, 31
    xori r4, r4, 0xa1c1
    b @L_80074D54
@L_80074D50:
    rlwinm r4, r4, 31, 1, 31
@L_80074D54:
    rlwinm r0, r4, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80074D6C
    rlwinm r4, r4, 31, 1, 31
    xori r4, r4, 0xa1c1
    b @L_80074D70
@L_80074D6C:
    rlwinm r4, r4, 31, 1, 31
@L_80074D70:
    rlwinm r0, r4, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80074D88
    rlwinm r4, r4, 31, 1, 31
    xori r4, r4, 0xa1c1
    b @L_80074D8C
@L_80074D88:
    rlwinm r4, r4, 31, 1, 31
@L_80074D8C:
    rlwinm r0, r4, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80074DA4
    rlwinm r4, r4, 31, 1, 31
    xori r4, r4, 0xa1c1
    b @L_80074DA8
@L_80074DA4:
    rlwinm r4, r4, 31, 1, 31
@L_80074DA8:
    rlwinm r0, r4, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80074DC0
    rlwinm r4, r4, 31, 1, 31
    xori r4, r4, 0xa1c1
    b @L_80074DC4
@L_80074DC0:
    rlwinm r4, r4, 31, 1, 31
@L_80074DC4:
    rlwinm r0, r4, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80074DDC
    rlwinm r4, r4, 31, 1, 31
    xori r4, r4, 0xa1c1
    b @L_80074DE0
@L_80074DDC:
    rlwinm r4, r4, 31, 1, 31
@L_80074DE0:
    rlwinm r0, r4, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80074DF8
    rlwinm r4, r4, 31, 1, 31
    xori r4, r4, 0xa1c1
    b @L_80074DFC
@L_80074DF8:
    rlwinm r4, r4, 31, 1, 31
@L_80074DFC:
    subi r3, r3, 0x7
    bdnz @L_80074D1C
    lis r3, 0x6177
    mr r24, r4
    addi r0, r3, 0x614b
    mullw r3, r26, r0
    addi r26, r3, 0x1
@L_80074E18:
    rlwinm r3, r5, 8, 24, 31
    rlwinm r0, r5, 24, 16, 23
    rlwinm r4, r5, 8, 8, 15
    rlwinm r5, r5, 24, 0, 7
    or r0, r3, r0
    mr r3, r31
    or r0, r4, r0
    addi r4, r1, 0xc
    or r0, r5, r0
    addi r5, r1, 0x8
    stw r0, 0xc(r1)
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_80074E58
    li r3, 0xa
    b @L_8007537C
@L_80074E58:
    lbz r0, 0x8(r1)
    rlwinm r0, r0, 0, 26, 27
    cmplw r0, r25
    beq @L_80074E70
    li r3, 0x12
    b @L_8007537C
@L_80074E70:
    lwz r0, 0xf8(r20)
    xori r25, r25, 0x10
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r19, r0
    rlwinm r0, r0, 26, 6, 31
    mulli r16, r0, 0x64
    bl OSGetTick
    mr r21, r3
@L_80074E90:
    bl OSGetTick
    subf r0, r21, r3
    cmplw r0, r16
    ble @L_80074EA8
    li r3, 0xb
    b @L_8007537C
@L_80074EA8:
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80074EC4
    li r3, 0xc
    b @L_8007537C
@L_80074EC4:
    lbz r3, 0x8(r1)
    rlwinm r0, r3, 0, 26, 26
    cmpwi r0, 0x0
    bne @L_80074EDC
    li r3, 0xd
    b @L_8007537C
@L_80074EDC:
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80074F18
    lwz r12, 0x0(r29)
    cmplwi r12, 0x0
    beq @L_80074F04
    mr r3, r31
    lwz r4, 0x0(r28)
    mtctr r12
    bctrl
@L_80074F04:
    lwzx r0, r18, r17
    cmpwi r0, 0x0
    beq @L_80074E90
    li r3, 0x3e8
    b @L_8007537C
@L_80074F18:
    lwzx r0, r18, r17
    cmpwi r0, 0x0
    beq @L_80074F2C
    li r3, 0x3e8
    b @L_8007537C
@L_80074F2C:
    addi r23, r23, 0x4
    addi r22, r22, 0x4
@L_80074F34:
    cmplw r23, r27
    blt @L_80074CD8
    lis r3, 0x8000
    lwz r0, 0xf8(r3)
    rlwinm r0, r0, 30, 2, 31
    mulli r17, r0, 0xa
    bl OSGetTick
    lis r4, lbl_803B6E08@ha
    mr r19, r3
    rlwinm r18, r31, 2, 0, 29
    addi r16, r4, lbl_803B6E08@l
@L_80074F60:
    bl OSGetTick
    subf r0, r19, r3
    cmplw r0, r17
    ble @L_80074F78
    li r3, 0xe
    b @L_8007537C
@L_80074F78:
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_80074F94
    li r3, 0xf
    b @L_8007537C
@L_80074F94:
    lbz r3, 0x8(r1)
    rlwinm r0, r3, 0, 26, 27
    cmpwi r0, 0x30
    beq @L_80074FAC
    li r3, 0x10
    b @L_8007537C
@L_80074FAC:
    andi. r0, r3, 0xa
    cmpwi r0, 0x8
    bne @L_800752CC
    mr r3, r31
    addi r4, r1, 0x10
    addi r5, r1, 0x8
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_80074FD8
    li r3, 0x11
    b @L_8007537C
@L_80074FD8:
    lwz r4, 0x10(r1)
    rlwinm r5, r4, 8, 24, 31
    cmplwi r5, 0xff
    bne @L_800752A8
    rlwinm r0, r4, 24, 16, 23
    rlwinm r3, r4, 8, 8, 15
    or r0, r5, r0
    rlwinm r4, r4, 24, 0, 7
    or r0, r3, r0
    mr r5, r24
    or r16, r4, r0
    addi r3, r30, 0xcc
    mr r4, r16
    crxor 6, 6, 6
    bl OSReport
    rlwinm r7, r16, 24, 8, 31
    li r3, 0x0
    li r4, -0x1
@L_80075020:
    xor r6, r4, r24
    li r5, 0x20
    li r0, 0x4
    mtctr r0
@L_80075030:
    rlwinm r0, r6, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80075048
    rlwinm r6, r6, 31, 1, 31
    xori r6, r6, 0xa1c1
    b @L_8007504C
@L_80075048:
    rlwinm r6, r6, 31, 1, 31
@L_8007504C:
    rlwinm r0, r6, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80075064
    rlwinm r6, r6, 31, 1, 31
    xori r6, r6, 0xa1c1
    b @L_80075068
@L_80075064:
    rlwinm r6, r6, 31, 1, 31
@L_80075068:
    rlwinm r0, r6, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80075080
    rlwinm r6, r6, 31, 1, 31
    xori r6, r6, 0xa1c1
    b @L_80075084
@L_80075080:
    rlwinm r6, r6, 31, 1, 31
@L_80075084:
    rlwinm r0, r6, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_8007509C
    rlwinm r6, r6, 31, 1, 31
    xori r6, r6, 0xa1c1
    b @L_800750A0
@L_8007509C:
    rlwinm r6, r6, 31, 1, 31
@L_800750A0:
    rlwinm r0, r6, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_800750B8
    rlwinm r6, r6, 31, 1, 31
    xori r6, r6, 0xa1c1
    b @L_800750BC
@L_800750B8:
    rlwinm r6, r6, 31, 1, 31
@L_800750BC:
    rlwinm r0, r6, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_800750D4
    rlwinm r6, r6, 31, 1, 31
    xori r6, r6, 0xa1c1
    b @L_800750D8
@L_800750D4:
    rlwinm r6, r6, 31, 1, 31
@L_800750D8:
    rlwinm r0, r6, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_800750F0
    rlwinm r6, r6, 31, 1, 31
    xori r6, r6, 0xa1c1
    b @L_800750F4
@L_800750F0:
    rlwinm r6, r6, 31, 1, 31
@L_800750F4:
    rlwinm r0, r6, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_8007510C
    rlwinm r6, r6, 31, 1, 31
    xori r6, r6, 0xa1c1
    b @L_80075110
@L_8007510C:
    rlwinm r6, r6, 31, 1, 31
@L_80075110:
    subi r5, r5, 0x7
    bdnz @L_80075030
    cmplw r7, r6
    bne @L_80075230
    xori r5, r4, 0xbb
    li r3, 0x20
    li r0, 0x4
    mtctr r0
@L_80075130:
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80075148
    rlwinm r5, r5, 31, 1, 31
    xori r5, r5, 0xa1c1
    b @L_8007514C
@L_80075148:
    rlwinm r5, r5, 31, 1, 31
@L_8007514C:
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80075164
    rlwinm r5, r5, 31, 1, 31
    xori r5, r5, 0xa1c1
    b @L_80075168
@L_80075164:
    rlwinm r5, r5, 31, 1, 31
@L_80075168:
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_80075180
    rlwinm r5, r5, 31, 1, 31
    xori r5, r5, 0xa1c1
    b @L_80075184
@L_80075180:
    rlwinm r5, r5, 31, 1, 31
@L_80075184:
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_8007519C
    rlwinm r5, r5, 31, 1, 31
    xori r5, r5, 0xa1c1
    b @L_800751A0
@L_8007519C:
    rlwinm r5, r5, 31, 1, 31
@L_800751A0:
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_800751B8
    rlwinm r5, r5, 31, 1, 31
    xori r5, r5, 0xa1c1
    b @L_800751BC
@L_800751B8:
    rlwinm r5, r5, 31, 1, 31
@L_800751BC:
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_800751D4
    rlwinm r5, r5, 31, 1, 31
    xori r5, r5, 0xa1c1
    b @L_800751D8
@L_800751D4:
    rlwinm r5, r5, 31, 1, 31
@L_800751D8:
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_800751F0
    rlwinm r5, r5, 31, 1, 31
    xori r5, r5, 0xa1c1
    b @L_800751F4
@L_800751F0:
    rlwinm r5, r5, 31, 1, 31
@L_800751F4:
    rlwinm r0, r5, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_8007520C
    rlwinm r5, r5, 31, 1, 31
    xori r5, r5, 0xa1c1
    b @L_80075210
@L_8007520C:
    rlwinm r5, r5, 31, 1, 31
@L_80075210:
    subi r3, r3, 0x7
    bdnz @L_80075130
    oris r16, r5, 0xbb00
    addi r3, r30, 0x100
    mr r5, r16
    crxor 6, 6, 6
    bl OSReport
    b @L_80075258
@L_80075230:
    addis r4, r4, 0x100
    addi r3, r3, 0x1
    cmplwi r3, 0x100
    blt @L_80075020
    mr r4, r16
    mr r5, r24
    addi r3, r30, 0x130
    crxor 6, 6, 6
    bl OSReport
    li r16, 0x0
@L_80075258:
    cmplwi r16, 0x0
    bne @L_80075268
    li r3, 0x12
    b @L_8007537C
@L_80075268:
    rlwinm r3, r16, 8, 24, 31
    rlwinm r0, r16, 24, 16, 23
    rlwinm r4, r16, 8, 8, 15
    rlwinm r5, r16, 24, 0, 7
    or r0, r3, r0
    mr r3, r31
    or r0, r4, r0
    addi r4, r1, 0xc
    or r0, r5, r0
    addi r5, r1, 0x8
    stw r0, 0xc(r1)
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_800752FC
    li r3, 0x13
    b @L_8007537C
@L_800752A8:
    cmplwi r5, 0xcc
    beq @L_800752B8
    li r3, 0x14
    b @L_8007537C
@L_800752B8:
    lwzx r0, r16, r18
    cmpwi r0, 0x0
    beq @L_80074F60
    li r3, 0x3e8
    b @L_8007537C
@L_800752CC:
    lwz r12, 0x0(r29)
    cmplwi r12, 0x0
    beq @L_800752E8
    mr r3, r31
    lwz r4, 0x0(r28)
    mtctr r12
    bctrl
@L_800752E8:
    lwzx r0, r16, r18
    cmpwi r0, 0x0
    beq @L_80074F60
    li r3, 0x3e8
    b @L_8007537C
@L_800752FC:
    lis r3, 0x8000
    lwz r0, 0xf8(r3)
    rlwinm r19, r0, 30, 2, 31
    bl OSGetTick
    lis r4, lbl_803B6E08@ha
    mr r18, r3
    rlwinm r17, r31, 2, 0, 29
    addi r16, r4, lbl_803B6E08@l
@L_8007531C:
    bl OSGetTick
    subf r0, r18, r3
    cmplw r0, r19
    ble @L_80075334
    li r3, 0x15
    b @L_8007537C
@L_80075334:
    mr r3, r31
    bl fn_800D0F44
    subis r0, r3, 0x4
    cmplwi r0, 0x0
    bne @L_80075378
    lwz r12, 0x0(r29)
    cmplwi r12, 0x0
    beq @L_80075364
    mr r3, r31
    lwz r4, 0x0(r28)
    mtctr r12
    bctrl
@L_80075364:
    lwzx r0, r16, r17
    cmpwi r0, 0x0
    beq @L_8007531C
    li r3, 0x3e8
    b @L_8007537C
@L_80075378:
    li r3, 0x0
@L_8007537C:
    lmw r16, 0x20(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
