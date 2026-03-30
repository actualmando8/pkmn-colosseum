    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x44(r1)
    stmw r18, 0x8(r1)
    li r21, 0x0
    li r20, 0x0
    li r19, 0x0
    lwz r3, lbl_8047B610(r13)
    lbz r3, 0x1(r3)
    bl fn_801F025C
    mr r0, r3
    li r3, 0x2
    mr r24, r0
    mr r4, r24
    bl fn_801F025C
    mr r0, r3
    mr r3, r24
    mr r23, r0
    bl fn_80207BF4
    lwz r4, lbl_8047B610(r13)
    mr r30, r3
    lbz r31, 0x3(r4)
    lbz r3, 0x2(r4)
    rlwinm. r0, r31, 0, 31, 31
    beq @L_80220A2C
    rlwinm. r25, r31, 0, 30, 30
    li r0, 0x15
    beq @L_802208E0
    li r0, 0x2d
@L_802208E0:
    rlwinm r29, r31, 0, 28, 28
    rlwinm r28, r0, 0, 16, 31
    b @L_80220A04
@L_802208EC:
    rlwinm. r0, r3, 0, 31, 31
    rlwinm r26, r3, 0, 24, 31
    beq @L_802209F8
    rlwinm r27, r21, 0, 24, 31
    cmplwi r27, 0x7
    bgt @L_80220954
    lis r3, jumptable_8039A110@ha
    rlwinm r0, r27, 2, 0, 29
    addi r3, r3, jumptable_8039A110@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r18, 0xe6
    b @L_80220958
    li r18, 0xe7
    b @L_80220958
    li r18, 0xea
    b @L_80220958
    li r18, 0xe8
    b @L_80220958
    li r18, 0xe9
    b @L_80220958
    li r18, 0xeb
    b @L_80220958
    li r18, 0xec
    b @L_80220958
@L_80220954:
    li r18, 0x0
@L_80220958:
    mr r3, r24
    mr r5, r18
    li r4, 0x0
    li r6, 0x0
    bl fn_8012640C
    cmpwi r29, 0x0
    rlwinm r22, r3, 0, 24, 31
    beq @L_80220990
    cmplwi r22, 0x0
    beq @L_802209F8
    add r0, r28, r27
    addi r19, r19, 0x1
    rlwinm r20, r0, 0, 16, 31
    b @L_802209F8
@L_80220990:
    mr r3, r23
    li r4, 0x4c
    bl fn_801F6E98
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802209F8
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x1d
    beq @L_802209F8
    cmplwi r0, 0x49
    beq @L_802209F8
    cmplwi r0, 0x33
    bne @L_802209CC
    rlwinm r0, r18, 0, 16, 31
    cmplwi r0, 0xeb
    beq @L_802209F8
@L_802209CC:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x34
    bne @L_802209E4
    rlwinm r0, r18, 0, 16, 31
    cmplwi r0, 0xe6
    beq @L_802209F8
@L_802209E4:
    cmplwi r22, 0x0
    beq @L_802209F8
    add r0, r28, r27
    addi r19, r19, 0x1
    rlwinm r20, r0, 0, 16, 31
@L_802209F8:
    srawi r0, r26, 1
    addi r21, r21, 0x1
    rlwinm r3, r0, 0, 24, 31
@L_80220A04:
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802208EC
    cmpwi r19, 0x1
    ble @L_80220B08
    cmpwi r25, 0x0
    beq @L_80220A24
    li r20, 0x3a
    b @L_80220B08
@L_80220A24:
    li r20, 0x39
    b @L_80220B08
@L_80220A2C:
    rlwinm. r25, r31, 0, 30, 30
    li r0, 0xe
    beq @L_80220A3C
    li r0, 0x26
@L_80220A3C:
    rlwinm r18, r0, 0, 16, 31
    b @L_80220AE4
@L_80220A44:
    rlwinm. r0, r3, 0, 31, 31
    rlwinm r23, r3, 0, 24, 31
    beq @L_80220AD8
    rlwinm r22, r21, 0, 24, 31
    cmplwi r22, 0x7
    bgt @L_80220AAC
    lis r3, jumptable_8039A0F0@ha
    rlwinm r0, r22, 2, 0, 29
    addi r3, r3, jumptable_8039A0F0@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r5, 0xe6
    b @L_80220AB0
    li r5, 0xe7
    b @L_80220AB0
    li r5, 0xea
    b @L_80220AB0
    li r5, 0xe8
    b @L_80220AB0
    li r5, 0xe9
    b @L_80220AB0
    li r5, 0xeb
    b @L_80220AB0
    li r5, 0xec
    b @L_80220AB0
@L_80220AAC:
    li r5, 0x0
@L_80220AB0:
    mr r3, r24
    li r4, 0x0
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0xc
    bge @L_80220AD8
    add r0, r18, r22
    addi r19, r19, 0x1
    rlwinm r20, r0, 0, 16, 31
@L_80220AD8:
    srawi r0, r23, 1
    addi r21, r21, 0x1
    rlwinm r3, r0, 0, 24, 31
@L_80220AE4:
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80220A44
    cmpwi r19, 0x1
    ble @L_80220B08
    cmpwi r25, 0x0
    beq @L_80220B04
    li r20, 0x38
    b @L_80220B08
@L_80220B04:
    li r20, 0x37
@L_80220B08:
    rlwinm. r18, r31, 0, 29, 29
    beq @L_80220B28
    cmpwi r19, 0x2
    bge @L_80220B28
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x4
    stw r0, lbl_8047B610(r13)
    b @L_80220B78
@L_80220B28:
    cmpwi r19, 0x0
    beq @L_80220B6C
    lbz r0, lbl_8047B64A(r13)
    cmplwi r0, 0x0
    bne @L_80220B6C
    mr r3, r20
    bl fn_80220B8C
    cmpwi r18, 0x0
    beq @L_80220B5C
    cmpwi r19, 0x1
    ble @L_80220B5C
    li r0, 0x1
    stb r0, lbl_8047B64A(r13)
@L_80220B5C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x4
    stw r0, lbl_8047B610(r13)
    b @L_80220B78
@L_80220B6C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x4
    stw r0, lbl_8047B610(r13)
@L_80220B78:
    lmw r18, 0x8(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
