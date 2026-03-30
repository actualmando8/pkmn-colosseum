    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    bl fn_80105624
    lfs f1, -31728(r2)
    mr r31, r3
    lfs f0, -25440(r13)
    li r29, 0x0
    fcmpu cr0, f1, f0
    beq @L_80039AB8
    li r3, 0x0
    b @L_80039F30
@L_80039AB8:
    li r3, 0x0
    bl fn_801347D0
    clrlwi r26, r3, 16
    li r30, 0x0
    b @L_80039AE0
@L_80039ACC:
    extsh r4, r30
    li r3, 0x0
    bl fn_80134768
    bl fn_801429E8
    addi r30, r30, 0x1
@L_80039AE0:
    cmpw r30, r26
    blt @L_80039ACC
    lwz r0, -25448(r13)
    lwz r4, -25464(r13)
    lwz r3, -25460(r13)
    cmpwi r0, 0x0
    add r30, r4, r3
    bge @L_80039B98
    lhz r0, 0x4(r31)
    rlwinm r0, r0, 0, 24, 25
    cmpwi r0, 0x0
    beq @L_80039C88
    li r3, 0x0
    bl fn_801347D0
    clrlwi r25, r3, 16
    li r28, -0x1
    li r27, 0x0
    b @L_80039B64
@L_80039B28:
    extsh r4, r27
    li r3, 0x0
    bl fn_80134768
    mr r26, r3
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80039B60
    addi r28, r28, 0x1
    cmpw r28, r30
    blt @L_80039B60
    mr r3, r26
    bl fn_80143C68
    b @L_80039B70
@L_80039B60:
    addi r27, r27, 0x1
@L_80039B64:
    cmpw r27, r25
    blt @L_80039B28
    li r3, 0x0
@L_80039B70:
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_80039C88
    stw r30, -25448(r13)
    li r3, 0x3c6
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    b @L_80039C88
@L_80039B98:
    lhz r3, 0x4(r31)
    andi. r0, r3, 0xd0
    cmpwi r0, 0x0
    beq @L_80039C60
    li r3, 0x0
    bl fn_801347D0
    clrlwi r25, r3, 16
    li r26, -0x1
    li r27, 0x0
    b @L_80039BFC
@L_80039BC0:
    extsh r4, r27
    li r3, 0x0
    bl fn_80134768
    mr r28, r3
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80039BF8
    addi r26, r26, 0x1
    cmpw r26, r30
    blt @L_80039BF8
    mr r3, r28
    bl fn_80143C68
    b @L_80039C08
@L_80039BF8:
    addi r27, r27, 0x1
@L_80039BFC:
    cmpw r27, r25
    blt @L_80039BC0
    li r3, 0x0
@L_80039C08:
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_80039C40
    lwz r0, -25448(r13)
    extsh r5, r30
    li r3, 0x0
    extsh r4, r0
    bl fn_8013433C
    li r3, 0x3c6
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    b @L_80039C54
@L_80039C40:
    li r3, 0x3c7
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
@L_80039C54:
    li r0, -0x1
    stw r0, -25448(r13)
    b @L_80039C88
@L_80039C60:
    rlwinm r0, r3, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_80039C88
    li r0, -0x1
    li r3, 0x3c7
    stw r0, -25448(r13)
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
@L_80039C88:
    li r26, 0x0
    li r3, 0x0
    bl fn_801347D0
    clrlwi r25, r3, 16
    li r27, 0x0
    b @L_80039CC4
@L_80039CA0:
    extsh r4, r27
    li r3, 0x0
    bl fn_80134768
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80039CC0
    addi r26, r26, 0x1
@L_80039CC0:
    addi r27, r27, 0x1
@L_80039CC4:
    cmpw r27, r25
    blt @L_80039CA0
    lhz r0, 0x6(r31)
    addi r30, r26, 0x1
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80039D54
    lwz r4, -25460(r13)
    lwz r3, -25464(r13)
    addi r5, r4, 0x1
    add r0, r5, r3
    stw r5, -25460(r13)
    cmpw r0, r30
    blt @L_80039D08
    subi r0, r5, 0x1
    stw r0, -25460(r13)
    b @L_80039D54
@L_80039D08:
    cmpwi r5, 0x8
    blt @L_80039D2C
    addi r4, r3, 0x1
    subi r3, r5, 0x1
    li r0, 0x1
    stw r4, -25464(r13)
    stw r3, -25460(r13)
    stw r0, -25444(r13)
    b @L_80039D34
@L_80039D2C:
    li r0, 0x0
    stw r0, -25444(r13)
@L_80039D34:
    lfs f0, -31724(r2)
    li r29, 0x1
    li r3, 0x3c5
    li r4, 0x0
    stfs f0, -25440(r13)
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
@L_80039D54:
    lhz r0, 0x6(r31)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_80039DE4
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    bne @L_80039DE4
    lwz r0, -25460(r13)
    cmpwi r0, 0x0
    bgt @L_80039D88
    lwz r0, -25464(r13)
    cmpwi r0, 0x0
    ble @L_80039DE4
@L_80039D88:
    lwz r3, -25460(r13)
    subi r0, r3, 0x1
    cmpwi r0, 0x0
    stw r0, -25460(r13)
    bge @L_80039DBC
    lwz r3, -25464(r13)
    li r4, 0x0
    li r0, 0x1
    stw r4, -25460(r13)
    subi r3, r3, 0x1
    stw r3, -25464(r13)
    stw r0, -25444(r13)
    b @L_80039DC4
@L_80039DBC:
    li r0, 0x0
    stw r0, -25444(r13)
@L_80039DC4:
    lfs f0, -31720(r2)
    li r29, 0x1
    li r3, 0x3c5
    li r4, 0x0
    stfs f0, -25440(r13)
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
@L_80039DE4:
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_80039E48
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    bne @L_80039E48
    lwz r3, -25464(r13)
    addi r3, r3, 0x8
    cmpw r3, r30
    bge @L_80039E48
    lwz r0, -25460(r13)
    stw r3, -25464(r13)
    add r0, r3, r0
    cmpw r0, r30
    blt @L_80039E30
    subf r3, r3, r30
    subi r0, r3, 0x1
    stw r0, -25460(r13)
@L_80039E30:
    li r29, 0x1
    li r3, 0x3c5
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
@L_80039E48:
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_80039E8C
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    bne @L_80039E8C
    lwz r3, -25464(r13)
    subi r0, r3, 0x8
    cmpwi r0, 0x0
    blt @L_80039E8C
    stw r0, -25464(r13)
    li r3, 0x3c5
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
@L_80039E8C:
    lwz r0, -25448(r13)
    cmpwi r0, 0x0
    blt @L_80039EA0
    li r3, 0x1b69
    b @L_80039F28
@L_80039EA0:
    lwz r4, -25464(r13)
    li r3, 0x0
    lwz r0, -25460(r13)
    add r29, r4, r0
    bl fn_801347D0
    clrlwi r25, r3, 16
    li r26, -0x1
    li r27, 0x0
    b @L_80039F00
@L_80039EC4:
    extsh r4, r27
    li r3, 0x0
    bl fn_80134768
    mr r28, r3
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80039EFC
    addi r26, r26, 0x1
    cmpw r26, r29
    blt @L_80039EFC
    mr r3, r28
    bl fn_80143C68
    b @L_80039F0C
@L_80039EFC:
    addi r27, r27, 0x1
@L_80039F00:
    cmpw r27, r25
    blt @L_80039EC4
    li r3, 0x0
@L_80039F0C:
    clrlwi r3, r3, 16
    cmplwi r3, 0x0
    beq @L_80039F24
    bl fn_801440A0
    bl fn_80143F84
    b @L_80039F28
@L_80039F24:
    li r3, 0x1b68
@L_80039F28:
    stw r3, -25452(r13)
    li r3, 0x0
@L_80039F30:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
