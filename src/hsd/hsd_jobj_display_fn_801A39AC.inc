    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr. r24, r3
    mr r31, r4
    mr r30, r5
    beq @L_801A3CF0
    lwz r0, 0xc(r24)
    cmplwi r0, 0x0
    bne @L_801A39EC
    lis r4, 0x8027
    addi r3, r2, -23424
    addi r5, r4, 19780
    li r4, 0xad
    bl fn_80196E10
@L_801A39EC:
    lwz r3, 0xc(r24)
    cmplwi r31, 0x0
    li r0, 0x2
    lwz r3, 0x10(r3)
    subf r4, r24, r3
    subf r3, r3, r24
    nor r3, r4, r3
    srawi r3, r3, 31
    add r0, r3, r0
    mr r5, r0
    beq @L_801A3A2C
    mr r12, r31
    mr r3, r24
    mr r4, r30
    mtctr r12
    bctrl
@L_801A3A2C:
    lwz r0, 0x14(r24)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A3CF0
    lwz r28, 0x10(r24)
    b @L_801A3CE8
@L_801A3A40:
    cmplwi r28, 0x0
    beq @L_801A3CE4
    lwz r0, 0xc(r28)
    cmplwi r0, 0x0
    bne @L_801A3A68
    lis r4, 0x8027
    addi r3, r2, -23424
    addi r5, r4, 19780
    li r4, 0xad
    bl fn_80196E10
@L_801A3A68:
    lwz r3, 0xc(r28)
    cmplwi r31, 0x0
    li r5, 0x2
    lwz r0, 0x10(r3)
    subf r3, r28, r0
    subf r0, r0, r28
    nor r0, r3, r0
    srawi r0, r0, 31
    add r5, r0, r5
    beq @L_801A3AA4
    mr r12, r31
    mr r3, r28
    mr r4, r30
    mtctr r12
    bctrl
@L_801A3AA4:
    lwz r0, 0x14(r28)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A3CE4
    lwz r29, 0x10(r28)
    b @L_801A3CDC
@L_801A3AB8:
    cmplwi r29, 0x0
    beq @L_801A3CD8
    lwz r0, 0xc(r29)
    cmplwi r0, 0x0
    bne @L_801A3AE0
    lis r4, 0x8027
    addi r3, r2, -23424
    addi r5, r4, 19780
    li r4, 0xad
    bl fn_80196E10
@L_801A3AE0:
    lwz r3, 0xc(r29)
    cmplwi r31, 0x0
    li r5, 0x2
    lwz r0, 0x10(r3)
    subf r3, r29, r0
    subf r0, r0, r29
    nor r0, r3, r0
    srawi r0, r0, 31
    add r5, r0, r5
    beq @L_801A3B1C
    mr r12, r31
    mr r3, r29
    mr r4, r30
    mtctr r12
    bctrl
@L_801A3B1C:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A3CD8
    lwz r27, 0x10(r29)
    b @L_801A3CD0
@L_801A3B30:
    cmplwi r27, 0x0
    beq @L_801A3CCC
    lwz r0, 0xc(r27)
    cmplwi r0, 0x0
    bne @L_801A3B58
    lis r4, 0x8027
    addi r3, r2, -23424
    addi r5, r4, 19780
    li r4, 0xad
    bl fn_80196E10
@L_801A3B58:
    lwz r3, 0xc(r27)
    cmplwi r31, 0x0
    li r5, 0x2
    lwz r0, 0x10(r3)
    subf r3, r27, r0
    subf r0, r0, r27
    nor r0, r3, r0
    srawi r0, r0, 31
    add r5, r0, r5
    beq @L_801A3B94
    mr r12, r31
    mr r3, r27
    mr r4, r30
    mtctr r12
    bctrl
@L_801A3B94:
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A3CCC
    lwz r26, 0x10(r27)
    b @L_801A3CC4
@L_801A3BA8:
    cmplwi r26, 0x0
    beq @L_801A3CC0
    lwz r0, 0xc(r26)
    cmplwi r0, 0x0
    bne @L_801A3BD0
    lis r4, 0x8027
    addi r3, r2, -23424
    addi r5, r4, 19780
    li r4, 0xad
    bl fn_80196E10
@L_801A3BD0:
    lwz r3, 0xc(r26)
    cmplwi r31, 0x0
    li r5, 0x2
    lwz r0, 0x10(r3)
    subf r3, r26, r0
    subf r0, r0, r26
    nor r0, r3, r0
    srawi r0, r0, 31
    add r5, r0, r5
    beq @L_801A3C0C
    mr r12, r31
    mr r3, r26
    mr r4, r30
    mtctr r12
    bctrl
@L_801A3C0C:
    lwz r0, 0x14(r26)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A3CC0
    lwz r25, 0x10(r26)
    b @L_801A3CB8
@L_801A3C20:
    cmplwi r25, 0x0
    beq @L_801A3CB4
    lwz r0, 0xc(r25)
    cmplwi r0, 0x0
    bne @L_801A3C48
    lis r4, 0x8027
    addi r3, r2, -23424
    addi r5, r4, 19780
    li r4, 0xad
    bl fn_80196E10
@L_801A3C48:
    lwz r3, 0xc(r25)
    cmplwi r31, 0x0
    li r5, 0x2
    lwz r0, 0x10(r3)
    subf r3, r25, r0
    subf r0, r0, r25
    nor r0, r3, r0
    srawi r0, r0, 31
    add r5, r0, r5
    beq @L_801A3C84
    mr r12, r31
    mr r3, r25
    mr r4, r30
    mtctr r12
    bctrl
@L_801A3C84:
    lwz r0, 0x14(r25)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A3CB4
    lwz r24, 0x10(r25)
    b @L_801A3CAC
@L_801A3C98:
    mr r3, r24
    mr r4, r31
    mr r5, r30
    bl fn_801A39AC
    lwz r24, 0x8(r24)
@L_801A3CAC:
    cmplwi r24, 0x0
    bne @L_801A3C98
@L_801A3CB4:
    lwz r25, 0x8(r25)
@L_801A3CB8:
    cmplwi r25, 0x0
    bne @L_801A3C20
@L_801A3CC0:
    lwz r26, 0x8(r26)
@L_801A3CC4:
    cmplwi r26, 0x0
    bne @L_801A3BA8
@L_801A3CCC:
    lwz r27, 0x8(r27)
@L_801A3CD0:
    cmplwi r27, 0x0
    bne @L_801A3B30
@L_801A3CD8:
    lwz r29, 0x8(r29)
@L_801A3CDC:
    cmplwi r29, 0x0
    bne @L_801A3AB8
@L_801A3CE4:
    lwz r28, 0x8(r28)
@L_801A3CE8:
    cmplwi r28, 0x0
    bne @L_801A3A40
@L_801A3CF0:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
