    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r24, r5
    mr r25, r6
    subi r31, r3, 0x1
    li r29, 0x4
    li r28, 0x4
    lwz r4, -20336(r2)
    lwz r0, -20332(r2)
    stw r4, 0xc(r1)
    stw r0, 0x8(r1)
    b @L_801B8D34
@L_801B8BBC:
    rlwinm r0, r31, 2, 0, 29
    li r30, 0x0
    lwzx r0, r25, r0
    mulli r0, r0, 0x28
    add r26, r24, r0
    lwz r27, 0x0(r26)
    b @L_801B8C84
@L_801B8BD8:
    rlwinm r0, r30, 3, 0, 28
    add r3, r27, r0
    lwz r3, 0x38(r3)
    bl fn_801B7C60
    cmpwi r3, 0x1
    bne @L_801B8C48
    rlwinm r0, r30, 3, 0, 28
    add r3, r27, r0
    lbz r0, 0x35(r3)
    cmplwi r0, 0x1
    bne @L_801B8C28
    rlwinm r0, r30, 3, 0, 28
    addi r4, r1, 0xc
    add r3, r27, r0
    lwz r3, 0x38(r3)
    lbz r5, 0xc(r3)
    lbzx r3, r4, r5
    subi r0, r3, 0x1
    stbx r0, r4, r5
    b @L_801B8C48
@L_801B8C28:
    rlwinm r0, r30, 3, 0, 28
    addi r4, r1, 0x8
    add r3, r27, r0
    lwz r3, 0x38(r3)
    lbz r5, 0x18(r3)
    lbzx r3, r4, r5
    subi r0, r3, 0x1
    stbx r0, r4, r5
@L_801B8C48:
    rlwinm r0, r30, 3, 0, 28
    add r3, r27, r0
    lwz r3, 0x58(r3)
    bl fn_801B7C60
    cmpwi r3, 0x1
    bne @L_801B8C80
    rlwinm r0, r30, 3, 0, 28
    addi r4, r1, 0x8
    add r3, r27, r0
    lwz r3, 0x58(r3)
    lbz r5, 0x18(r3)
    lbzx r3, r4, r5
    subi r0, r3, 0x1
    stbx r0, r4, r5
@L_801B8C80:
    addi r30, r30, 0x1
@L_801B8C84:
    cmpwi r30, 0x4
    blt @L_801B8BD8
    lwz r27, 0x0(r26)
    lwz r0, 0x8(r27)
    cmpwi r0, 0x0
    ble @L_801B8CE0
    li r30, 0x3
    b @L_801B8CD8
@L_801B8CA4:
    addi r3, r1, 0xc
    lbzx r0, r3, r30
    cmplwi r0, 0x0
    bne @L_801B8CD4
    lwz r0, 0x8(r27)
    addi r3, r1, 0xc
    cmpw r29, r30
    stb r30, 0xc(r27)
    stbx r0, r3, r30
    ble @L_801B8CE0
    mr r29, r30
    b @L_801B8CE0
@L_801B8CD4:
    subi r30, r30, 0x1
@L_801B8CD8:
    cmpwi r30, 0x0
    bge @L_801B8CA4
@L_801B8CE0:
    lwz r0, 0x14(r27)
    cmpwi r0, 0x0
    ble @L_801B8D30
    li r30, 0x3
    b @L_801B8D28
@L_801B8CF4:
    addi r3, r1, 0x8
    lbzx r0, r3, r30
    cmplwi r0, 0x0
    bne @L_801B8D24
    lwz r0, 0x14(r27)
    addi r3, r1, 0x8
    cmpw r28, r30
    stb r30, 0x18(r27)
    stbx r0, r3, r30
    ble @L_801B8D30
    mr r28, r30
    b @L_801B8D30
@L_801B8D24:
    subi r30, r30, 0x1
@L_801B8D28:
    cmpwi r30, 0x0
    bge @L_801B8CF4
@L_801B8D30:
    subi r31, r31, 0x1
@L_801B8D34:
    cmpwi r31, 0x0
    bge @L_801B8BBC
    subfic r3, r29, 0x4
    subfic r0, r28, 0x4
    lmw r24, 0x10(r1)
    add r3, r3, r0
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
