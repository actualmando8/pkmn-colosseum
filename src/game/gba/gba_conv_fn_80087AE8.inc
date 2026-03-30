    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r30, r3
    mr r26, r4
    li r0, 0x0
    li r31, 0x1
    stw r0, 0x28(r30)
    lbz r0, 0x21(r30)
    extsb r3, r0
    cmpwi r3, 0x0
    blt @L_80087B30
    bl fn_800D0F44
    subis r0, r3, 0x4
    cmplwi r0, 0x0
    bne @L_80087B30
    li r31, 0x0
@L_80087B30:
    rlwinm r28, r26, 0, 30, 30
    rlwinm r27, r26, 0, 31, 31
    rlwinm r26, r26, 0, 29, 29
@L_80087B3C:
    li r3, 0x10c
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80087B58
    bl fn_800F0308
    b @L_80087B3C
@L_80087B58:
    cmpwi r28, 0x0
    beq @L_80087B84
    bl fn_80105624
    lhz r0, 0x4(r3)
    rlwinm r0, r0, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_80087B84
    li r0, 0x2
    li r3, 0x0
    stw r0, 0x28(r30)
    b @L_80087C50
@L_80087B84:
    lwz r0, 0x28(r30)
    cmplwi r0, 0x8
    bne @L_80087B98
    li r3, 0x0
    b @L_80087C50
@L_80087B98:
    cmpwi r27, 0x0
    beq @L_80087BF4
    bl fn_80106934
    mr r29, r3
    extsb r0, r29
    cmpwi r0, 0x0
    bne @L_80087BC4
    li r0, 0x1
    li r3, 0x1
    stw r0, 0x28(r30)
    b @L_80087C50
@L_80087BC4:
    bl fn_80105624
    lhz r0, 0x4(r3)
    rlwinm r0, r0, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_80087BF4
    extsb r0, r29
    cmpwi r0, -0x1
    bne @L_80087BF4
    li r0, 0x1
    li r3, 0x1
    stw r0, 0x28(r30)
    b @L_80087C50
@L_80087BF4:
    cmpwi r26, 0x0
    beq @L_80087C48
    lbz r3, 0x21(r30)
    extsb r3, r3
    bl fn_800D0F44
    cmplwi r3, 0x80
    beq @L_80087C48
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80087C38
    subis r0, r3, 0x4
    cmplwi r0, 0x0
    bne @L_80087C48
    li r0, 0x4
    li r3, 0x1
    stw r0, 0x28(r30)
    b @L_80087C50
@L_80087C38:
    subis r0, r3, 0x4
    cmplwi r0, 0x0
    beq @L_80087C48
    li r31, 0x1
@L_80087C48:
    bl fn_800F0308
    b @L_80087B3C
@L_80087C50:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
