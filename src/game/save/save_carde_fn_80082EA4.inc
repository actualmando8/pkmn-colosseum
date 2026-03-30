    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r3
    mr r27, r4
    mr r29, r5
    mr r30, r6
    cmplwi r28, 0x0
    lis r3, lbl_8026F1C8@ha
    addi r31, r3, lbl_8026F1C8@l
    bne @L_80082EE4
    addi r3, r31, 0x0
    li r4, 0x17f
    addi r5, r2, -29984
    bl fn_80196E10
@L_80082EE4:
    extsb r3, r27
    li r4, 0x0
    cmpwi r3, 0x0
    blt @L_80082F08
    lbz r0, 0x1b(r28)
    extsb r0, r0
    cmpw r3, r0
    bge @L_80082F08
    li r4, 0x1
@L_80082F08:
    cmpwi r4, 0x0
    bne @L_80082F20
    addi r3, r31, 0x0
    addi r5, r31, 0x10
    li r4, 0x180
    bl fn_80196E10
@L_80082F20:
    lbz r3, 0x1c(r28)
    extsb r4, r27
    lbz r0, 0x1d(r28)
    extsb r3, r3
    extsb r0, r0
    mullw r0, r3, r0
    rlwinm r3, r0, 4, 0, 27
    addi r0, r3, 0x76
    mullw r0, r4, r0
    add r27, r28, r0
    addi r27, r27, 0x24
    cmplwi r27, 0x0
    bne @L_80082F64
    addi r3, r31, 0x0
    li r4, 0x198
    addi r5, r2, -29976
    bl fn_80196E10
@L_80082F64:
    lbz r0, 0x1c(r28)
    extsb r3, r29
    extsb r0, r0
    cmpw r3, r0
    blt @L_80082F88
    addi r3, r31, 0x0
    addi r5, r31, 0x68
    li r4, 0x199
    bl fn_80196E10
@L_80082F88:
    lbz r0, 0x1d(r28)
    extsb r3, r30
    extsb r0, r0
    cmpw r3, r0
    blt @L_80082FAC
    addi r3, r31, 0x0
    addi r5, r31, 0x80
    li r4, 0x19a
    bl fn_80196E10
@L_80082FAC:
    lbz r0, 0x1d(r28)
    extsb r3, r29
    extsb r4, r30
    extsb r0, r0
    mullw r0, r3, r0
    add r0, r4, r0
    rlwinm r3, r0, 4, 0, 27
    addi r3, r3, 0x76
    add r3, r27, r3
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

