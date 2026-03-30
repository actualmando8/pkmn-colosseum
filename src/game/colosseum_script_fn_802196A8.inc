    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x12
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    bl fn_801F025C
    mr r31, r3
    bl fn_80205B8C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    li r4, 0x0
    li r5, 0xf0
    li r6, 0x0
    bl fn_8012640C
    rlwinm r29, r3, 0, 16, 31
    mr r3, r30
    mr r4, r29
    bl fn_80123B5C
    mr r28, r3
    extsb. r0, r28
    bge @L_8021970C
    li r0, 0x0
    b @L_80219724
@L_8021970C:
    mr r3, r30
    extsb r6, r28
    li r4, 0x0
    li r5, 0x80
    bl fn_8012640C
    rlwinm r0, r3, 0, 24, 31
@L_80219724:
    cmplwi r29, 0xa5
    mr r27, r0
    beq @L_80219748
    cmplwi r29, 0xe3
    beq @L_80219748
    cmplwi r29, 0x77
    beq @L_80219748
    cmplwi r29, 0xffff
    bne @L_80219750
@L_80219748:
    li r0, 0x1
    b @L_80219754
@L_80219750:
    li r0, 0x0
@L_80219754:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80219768
    li r27, 0x0
    li r28, -0x1
@L_80219768:
    mr r3, r31
    li r4, 0x2a
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_802197E4
    extsb. r0, r28
    blt @L_802197E4
    rlwinm. r0, r27, 0, 24, 31
    beq @L_802197E4
    mr r3, r31
    li r4, 0x2a
    li r5, 0x0
    bl fn_8020248C
    mr r3, r30
    extsb r6, r28
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r5, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x2a
    bl fn_80201B2C
    mr r3, r31
    extsb r5, r28
    li r4, 0x2a
    bl fn_80201764
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_802197F0
@L_802197E4:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_802197F0:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
