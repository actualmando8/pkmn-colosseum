    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stmw r26, 0x98(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    li r30, 0x0
    bl fn_8019F01C
    lis r4, lbl_80465678@ha
    mr r31, r3
    addi r5, r4, lbl_80465678@l
    lwz r5, 0x0(r5)
    lis r4, lbl_80465678@ha
    addi r3, r4, lbl_80465678@l
    cmplw r5, r31
    lwz r3, 0x4(r3)
    beq @L_801AB2C4
    cmplwi r3, 0x1
    beq @L_801AB2C4
    ori r30, r30, 0x1
@L_801AB2C4:
    lis r4, lbl_80465678@ha
    lis r3, lbl_80465678@ha
    addi r4, r4, lbl_80465678@l
    lwz r0, 0x14(r27)
    lwz r5, 0x8(r4)
    addi r3, r3, lbl_80465678@l
    lwz r3, 0xc(r3)
    cmplw r5, r0
    beq @L_801AB2F4
    cmplwi r3, 0x1
    beq @L_801AB2F4
    ori r30, r30, 0x2
@L_801AB2F4:
    cmpwi r30, 0x0
    beq @L_801AB524
    lwz r0, 0x14(r31)
    li r26, 0x0
    rlwinm. r0, r0, 0, 24, 24
    beq @L_801AB310
    ori r26, r26, 0x1
@L_801AB310:
    li r3, 0x0
    li r4, 0x1
    bl fn_801BE490
    cmplwi r3, 0x0
    beq @L_801AB328
    ori r26, r26, 0x3
@L_801AB328:
    li r3, 0x0
    li r4, 0x5
    bl fn_801BE490
    cmplwi r3, 0x0
    beq @L_801AB340
    ori r26, r26, 0x3
@L_801AB340:
    li r3, 0x0
    li r4, 0x2
    bl fn_801BE490
    cmplwi r3, 0x0
    beq @L_801AB358
    ori r26, r26, 0x5
@L_801AB358:
    or r30, r30, r26
    ori r0, r30, 0x1
    cmpwi r0, 0x0
    beq @L_801AB410
    li r3, 0x0
    bl fn_800BD554
    mr r3, r29
    li r4, 0x0
    bl fn_800BD4B4
    lis r3, lbl_8036CC40@ha
    rlwinm. r0, r30, 0, 31, 31
    addi r4, r3, lbl_8036CC40@l
    lwz r3, 0xc(r4)
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
    beq @L_801AB410
    mr r3, r29
    addi r4, r1, 0x68
    bl fn_800A2FAC
    cmplwi r3, 0x0
    bne @L_801AB3B8
    mr r3, r29
    addi r4, r1, 0x68
    bl fn_800A2D64
@L_801AB3B8:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_801AB3E4
    addi r3, r1, 0x68
    li r4, 0x0
    bl fn_800BD504
    lis r3, lbl_8036CC40@ha
    addi r4, r3, lbl_8036CC40@l
    lwz r3, 0xc(r4)
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
@L_801AB3E4:
    rlwinm. r0, r30, 0, 29, 30
    beq @L_801AB410
    addi r3, r1, 0x68
    li r4, 0x1e
    li r5, 0x0
    bl fn_800BD58C
    lis r3, lbl_8036CC40@ha
    addi r4, r3, lbl_8036CC40@l
    lwz r3, 0xc(r4)
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
@L_801AB410:
    ori r0, r30, 0x2
    cmpwi r0, 0x0
    beq @L_801AB524
    lwz r26, 0x14(r27)
    cmplwi r26, 0x0
    beq @L_801AB470
    cmplwi r26, 0x0
    bne @L_801AB440
    addi r3, r2, -23004
    li r4, 0x25d
    addi r5, r2, -22996
    bl fn_80196E10
@L_801AB440:
    lwz r0, 0x14(r26)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801AB460
    lwz r0, 0x14(r26)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801AB460
    li r3, 0x1
@L_801AB460:
    cmpwi r3, 0x0
    beq @L_801AB470
    mr r3, r26
    bl fn_8019D9DC
@L_801AB470:
    lwz r4, 0x14(r27)
    mr r3, r28
    addi r5, r1, 0x8
    addi r4, r4, 0x44
    bl fn_800A2D98
    addi r3, r1, 0x8
    li r4, 0x3
    bl fn_800BD4B4
    lis r3, lbl_8036CC40@ha
    rlwinm. r0, r30, 0, 31, 31
    addi r4, r3, lbl_8036CC40@l
    lwz r3, 0xc(r4)
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
    beq @L_801AB524
    addi r3, r1, 0x8
    addi r4, r1, 0x38
    bl fn_800A2FAC
    cmplwi r3, 0x0
    bne @L_801AB4CC
    addi r3, r1, 0x8
    addi r4, r1, 0x38
    bl fn_800A2D64
@L_801AB4CC:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_801AB4F8
    addi r3, r1, 0x38
    li r4, 0x3
    bl fn_800BD504
    lis r3, lbl_8036CC40@ha
    addi r4, r3, lbl_8036CC40@l
    lwz r3, 0xc(r4)
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
@L_801AB4F8:
    rlwinm. r0, r30, 0, 29, 30
    beq @L_801AB524
    addi r3, r1, 0x38
    li r4, 0x21
    li r5, 0x0
    bl fn_800BD58C
    lis r3, lbl_8036CC40@ha
    addi r4, r3, lbl_8036CC40@l
    lwz r3, 0xc(r4)
    addi r0, r3, 0x1
    stw r0, 0xc(r4)
@L_801AB524:
    lmw r26, 0x98(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
