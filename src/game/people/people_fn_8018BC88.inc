    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    mr r27, r4
    mr r30, r5
    mr r31, r6
    li r29, 0x0
    b @L_8018BCE8
    @L_8018BCB0:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018BCE4
    lwz r0, 0x28(r3)
    cmplw r0, r26
    bne @L_8018BCE4
    lwz r0, 0x2c(r3)
    cmplw r0, r27
    bne @L_8018BCE4
    lwz r29, 0x4(r3)
    b @L_8018BD54
    @L_8018BCE4:
    addi r29, r29, 0x1
    @L_8018BCE8:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_8018BCB0
    li r28, 0x0
    b @L_8018BD44
    @L_8018BCFC:
    mr r3, r28
    bl fn_8018FD88
    mr r29, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018BD40
    lwz r0, 0x2c(r29)
    cmplw r0, r27
    bne @L_8018BD40
    lis r3, lbl_80273FD8@ha
    mr r4, r26
    addi r3, r3, lbl_80273FD8@l
    mr r5, r27
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r29, 0x4(r29)
    b @L_8018BD54
    @L_8018BD40:
    addi r28, r28, 0x1
    @L_8018BD44:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018BCFC
    li r29, 0x0
    @L_8018BD54:
    li r28, 0x0
    b @L_8018BD84
    @L_8018BD5C:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018BD80
    lwz r0, 0x4(r3)
    cmplw r0, r29
    bne @L_8018BD80
    b @L_8018BD94
    @L_8018BD80:
    addi r28, r28, 0x1
    @L_8018BD84:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018BD5C
    li r3, 0x0
    @L_8018BD94:
    cmplwi r3, 0x0
    beq @L_8018BDE0
    cmpwi r30, 0x0
    blt @L_8018BDD0
    lwz r3, 0x8(r3)
    mr r4, r30
    bl fn_800EE150
    mr r4, r31
    mr r28, r3
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r28
    bl fn_800EE828
    b @L_8018BDE0
    @L_8018BDD0:
    bl fn_8018FCBC
    mr r4, r3
    mr r3, r31
    bl fn_800E01D0
    @L_8018BDE0:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
