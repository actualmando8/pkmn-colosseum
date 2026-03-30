    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    mr r28, r4
    mr r27, r5
    lbz r3, 0x1a(r26)
    lis r4, lbl_8026F1C8@ha
    lbz r0, 0x8(r28)
    addi r31, r4, lbl_8026F1C8@l
    cmplw r3, r0
    beq @L_8008277C
    addi r3, r31, 0x0
    addi r5, r31, 0x38
    li r4, 0x225
    bl fn_80196E10
@L_8008277C:
    cmplwi r26, 0x0
    lbz r29, 0x26(r28)
    lbz r30, 0x24(r28)
    bne @L_8008279C
    addi r3, r31, 0x0
    li r4, 0x17f
    addi r5, r2, -29984
    bl fn_80196E10
@L_8008279C:
    extsb r3, r27
    li r4, 0x0
    cmpwi r3, 0x0
    blt @L_800827C0
    lbz r0, 0x1b(r26)
    extsb r0, r0
    cmpw r3, r0
    bge @L_800827C0
    li r4, 0x1
@L_800827C0:
    cmpwi r4, 0x0
    bne @L_800827D8
    addi r3, r31, 0x0
    addi r5, r31, 0x10
    li r4, 0x180
    bl fn_80196E10
@L_800827D8:
    lbz r3, 0x1c(r26)
    extsb r4, r27
    lbz r0, 0x1d(r26)
    extsb r3, r3
    extsb r0, r0
    mullw r0, r3, r0
    rlwinm r3, r0, 4, 0, 27
    addi r0, r3, 0x76
    mullw r0, r4, r0
    add r28, r26, r0
    addi r28, r28, 0x24
    cmplwi r28, 0x0
    bne @L_8008281C
    addi r3, r31, 0x0
    li r4, 0x198
    addi r5, r2, -29976
    bl fn_80196E10
@L_8008281C:
    lbz r0, 0x1c(r26)
    extsb r3, r30
    extsb r0, r0
    cmpw r3, r0
    blt @L_80082840
    addi r3, r31, 0x0
    addi r5, r31, 0x68
    li r4, 0x199
    bl fn_80196E10
@L_80082840:
    lbz r0, 0x1d(r26)
    extsb r3, r29
    extsb r0, r0
    cmpw r3, r0
    blt @L_80082864
    addi r3, r31, 0x0
    addi r5, r31, 0x80
    li r4, 0x19a
    bl fn_80196E10
@L_80082864:
    lbz r3, 0x1d(r26)
    extsb r0, r27
    extsb r5, r30
    extsb r6, r29
    extsb r4, r3
    li r3, 0x0
    mullw r4, r5, r4
    cmpwi r0, 0x0
    add r0, r6, r4
    rlwinm r4, r0, 4, 0, 27
    addi r4, r4, 0x76
    add r4, r28, r4
    stb r3, 0xc(r4)
    sth r3, 0x0(r4)
    bne @L_80082948
    cmplwi r26, 0x0
    bne @L_800828B8
    addi r3, r31, 0x0
    li r4, 0x17f
    addi r5, r2, -29984
    bl fn_80196E10
@L_800828B8:
    lbz r0, 0x1b(r26)
    extsb r0, r0
    cmpwi r0, 0x0
    bgt @L_800828D8
    addi r3, r31, 0x0
    addi r5, r31, 0x10
    li r4, 0x180
    bl fn_80196E10
@L_800828D8:
    addi r27, r26, 0x24
    cmplwi r27, 0x0
    bne @L_800828F4
    addi r3, r31, 0x0
    li r4, 0x1f1
    addi r5, r2, -29976
    bl fn_80196E10
@L_800828F4:
    lbz r3, 0x1c(r26)
    lbz r0, 0x1d(r26)
    extsb r3, r3
    extsb r0, r0
    mullw r0, r3, r0
    mtctr r0
    cmpwi r0, 0x0
    ble @L_80082930
@L_80082914:
    lbz r0, 0x82(r27)
    cmplwi r0, 0x0
    beq @L_80082928
    li r0, 0x1
    b @L_80082934
@L_80082928:
    addi r27, r27, 0x10
    bdnz @L_80082914
@L_80082930:
    li r0, 0x0
@L_80082934:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80082948
    li r3, 0x1
    b @L_8008294C
@L_80082948:
    li r3, 0x0
@L_8008294C:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

