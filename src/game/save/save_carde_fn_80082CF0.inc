    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r25, r3
    mr r26, r4
    mr r27, r5
    lbz r3, 0x1a(r25)
    lis r4, lbl_8026F1C8@ha
    lbz r0, 0x8(r26)
    addi r31, r4, lbl_8026F1C8@l
    cmplw r3, r0
    beq @L_80082D34
    addi r3, r31, 0x0
    addi r5, r31, 0x38
    li r4, 0x1b0
    bl fn_80196E10
@L_80082D34:
    cmplwi r25, 0x0
    lbz r29, 0x26(r26)
    lbz r30, 0x24(r26)
    bne @L_80082D54
    addi r3, r31, 0x0
    li r4, 0x17f
    addi r5, r2, -29984
    bl fn_80196E10
@L_80082D54:
    extsb r3, r27
    li r4, 0x0
    cmpwi r3, 0x0
    blt @L_80082D78
    lbz r0, 0x1b(r25)
    extsb r0, r0
    cmpw r3, r0
    bge @L_80082D78
    li r4, 0x1
@L_80082D78:
    cmpwi r4, 0x0
    bne @L_80082D90
    addi r3, r31, 0x0
    addi r5, r31, 0x10
    li r4, 0x180
    bl fn_80196E10
@L_80082D90:
    lbz r3, 0x1c(r25)
    extsb r4, r27
    lbz r0, 0x1d(r25)
    extsb r3, r3
    extsb r0, r0
    mullw r0, r3, r0
    rlwinm r3, r0, 4, 0, 27
    addi r0, r3, 0x76
    mullw r0, r4, r0
    add r28, r25, r0
    addi r28, r28, 0x24
    cmplwi r28, 0x0
    bne @L_80082DD4
    addi r3, r31, 0x0
    li r4, 0x198
    addi r5, r2, -29976
    bl fn_80196E10
@L_80082DD4:
    lbz r0, 0x1c(r25)
    extsb r3, r30
    extsb r0, r0
    cmpw r3, r0
    blt @L_80082DF8
    addi r3, r31, 0x0
    addi r5, r31, 0x68
    li r4, 0x199
    bl fn_80196E10
@L_80082DF8:
    lbz r0, 0x1d(r25)
    extsb r3, r29
    extsb r0, r0
    cmpw r3, r0
    blt @L_80082E1C
    addi r3, r31, 0x0
    addi r5, r31, 0x80
    li r4, 0x19a
    bl fn_80196E10
@L_80082E1C:
    lbz r4, 0x1d(r25)
    extsb r0, r27
    add r3, r26, r0
    extsb r5, r30
    extsb r4, r4
    lbz r0, 0x5b(r3)
    mullw r4, r5, r4
    extsb r5, r29
    extsb r0, r0
    mulli r3, r0, 0x28
    add r0, r5, r4
    rlwinm r4, r0, 4, 0, 27
    addi r27, r4, 0x76
    addi r29, r3, 0x3ac
    add r27, r28, r27
    add r29, r26, r29
    mr r3, r27
    mr r4, r29
    bl fn_800CAA3C
    li r0, 0x1
    mr r3, r27
    stb r0, 0xc(r27)
    lhz r0, 0x22(r29)
    sth r0, 0xe(r27)
    lbz r0, 0x24(r26)
    lbz r5, 0x25(r26)
    extsb r0, r0
    add r4, r25, r0
    stb r5, 0x1e(r4)
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

