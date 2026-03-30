    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r20, 0x20(r1)
    mr r20, r3
    mr r21, r4
    mr r22, r5
    lis r3, lbl_803A3334@ha
    li r24, 0x0
    addi r3, r3, lbl_803A3334@l
    li r23, 0x1
    lbz r28, 0x24(r3)
    li r26, 0x1
    li r27, 0x0
    b @L_80034BE8
@L_80034B98:
    mr r5, r27
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r25, r3
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80034BE4
    mr r3, r25
    li r4, 0x0
    li r5, 0x7a
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 24, 31
    rlwinm r0, r26, 0, 24, 31
    cmplw r0, r3
    bge @L_80034BE4
    mr r26, r3
@L_80034BE4:
    addi r27, r27, 0x1
@L_80034BE8:
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_80034B98
    extsb r0, r28
    lis r3, lbl_803A3334@ha
    mulli r27, r0, 0xe
    addi r29, r1, 0x8
    addi r30, r3, lbl_803A3334@l
    rlwinm r26, r26, 0, 24, 31
    mr r28, r30
    li r25, 0x0
    lis r3, 0x8000
    subi r31, r3, 0x1
    b @L_80034D10
@L_80034C20:
    stw r31, 0x0(r29)
    extsb r4, r25
    lwz r3, lbl_8047A434(r13)
    bl fn_80082FE4
    addi r0, r3, 0x1c
    lbzx r0, r27, r0
    cmplwi r0, 0x0
    beq @L_80034D04
    lbz r0, 0x5e(r28)
    lis r3, lbl_803A3334@ha
    li r5, 0x0
    extsb r0, r0
    addi r4, r3, lbl_803A3334@l
    mulli r0, r0, 0x28
    mr r6, r5
    add r7, r4, r0
    b @L_80034C9C
@L_80034C64:
    rlwinm r3, r6, 0, 16, 31
    addi r0, r3, 0x3b9
    lbzx r0, r7, r0
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80034C98
    mulli r3, r0, 0x2a
    rlwinm r0, r5, 0, 24, 31
    add r3, r4, r3
    lbz r3, 0x517(r3)
    cmplw r0, r3
    bge @L_80034C98
    mr r5, r3
@L_80034C98:
    addi r6, r6, 0x1
@L_80034C9C:
    rlwinm r0, r6, 0, 16, 31
    cmplwi r0, 0x4
    blt @L_80034C64
    rlwinm r3, r5, 0, 24, 31
    extsb r0, r24
    subf r4, r3, r26
    addi r3, r1, 0x8
    srawi r5, r4, 31
    rlwinm r0, r0, 2, 0, 29
    xor r4, r5, r4
    subf r4, r5, r4
    stw r4, 0x0(r29)
    lwz r4, 0x0(r29)
    lwzx r0, r3, r0
    cmpw r4, r0
    bge @L_80034CE8
    extsb r24, r25
    li r23, 0x1
    b @L_80034D04
@L_80034CE8:
    bne @L_80034D04
    addi r23, r23, 0x1
    mr r3, r23
    bl fn_800E0C04
    cmplwi r3, 0x0
    bne @L_80034D04
    extsb r24, r25
@L_80034D04:
    addi r29, r29, 0x4
    addi r28, r28, 0x1
    addi r25, r25, 0x1
@L_80034D10:
    lbz r0, 0x58(r30)
    extsb r0, r0
    cmpw r25, r0
    blt @L_80034C20
    extsb r0, r24
    lis r3, 0x8000
    rlwinm r0, r0, 2, 0, 29
    addi r4, r1, 0x8
    lwzx r0, r4, r0
    subi r4, r3, 0x1
    xor r0, r4, r0
    srawi r3, r0, 1
    and r0, r0, r4
    subf r0, r0, r3
    rlwinm r0, r0, 1, 31, 31
    stb r0, 0x0(r20)
    lbz r0, 0x0(r20)
    cmplwi r0, 0x0
    beq @L_80034D80
    stb r24, 0x0(r21)
    lis r3, lbl_803A3334@ha
    addi r3, r3, lbl_803A3334@l
    lbz r0, 0x0(r21)
    extsb r0, r0
    add r3, r3, r0
    lbz r0, 0x5e(r3)
    stb r0, 0x0(r22)
    b @L_80034DAC
@L_80034D80:
    lbz r4, 0x58(r30)
    lis r3, lbl_803A3334@ha
    addi r3, r3, lbl_803A3334@l
    subi r0, r4, 0x1
    extsb r0, r0
    stb r0, 0x0(r21)
    lbz r0, 0x0(r21)
    extsb r0, r0
    add r3, r3, r0
    lbz r0, 0x5b(r3)
    stb r0, 0x0(r22)
@L_80034DAC:
    lmw r20, 0x20(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
