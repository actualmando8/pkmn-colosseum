    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr. r26, r3
    beq @L_801DAB98
    lwz r3, 0x24(r26)
    bl fn_800EC0E8
    li r28, 0x0
@L_801DAAD0:
    neg r0, r28
    li r27, 0x0
    or r0, r0, r28
    rlwinm r29, r0, 1, 31, 31
@L_801DAAE0:
    cmpwi r27, 0x0
    bne @L_801DAAF0
    li r31, 0x10
    b @L_801DAB04
@L_801DAAF0:
    cmpwi r27, 0x1
    bne @L_801DAB00
    li r31, 0x1000
    b @L_801DAB04
@L_801DAB00:
    li r31, 0x2000
@L_801DAB04:
    lwz r3, 0x24(r26)
    bl fn_800E3CBC
    rlwinm r0, r3, 0, 24, 31
    cmplw r29, r0
    bne @L_801DAB24
    lwz r3, 0x24(r26)
    mr r4, r31
    bl fn_800E3760
@L_801DAB24:
    lwz r30, 0x68(r26)
    b @L_801DAB4C
@L_801DAB2C:
    lbz r0, 0x14(r30)
    cmplwi r0, 0x0
    beq @L_801DAB48
    mr r3, r30
    mr r4, r31
    mr r5, r29
    bl fn_801DB8FC
@L_801DAB48:
    lwz r30, 0x34(r30)
@L_801DAB4C:
    cmplwi r30, 0x0
    bne @L_801DAB2C
    bl fn_800D2584
    cmplwi r3, 0x0
    beq @L_801DAB80
    lwz r3, 0xc(r3)
    bl fn_80195A6C
    cmpwi r3, 0x0
    beq @L_801DAB80
    mr r3, r31
    mr r4, r29
    bl fn_80118104
    bl fn_80195A48
@L_801DAB80:
    addi r27, r27, 0x1
    cmpwi r27, 0x3
    blt @L_801DAAE0
    addi r28, r28, 0x1
    cmpwi r28, 0x2
    blt @L_801DAAD0
@L_801DAB98:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
