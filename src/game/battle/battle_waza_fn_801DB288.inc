    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    lhz r5, 0x30(r3)
    cmplwi r5, 0x0
    beq @L_801DB3DC
    lis r3, -32698
    li r6, 0x0
    addi r3, r3, 31936
    lhz r0, 0x4(r3)
    lwz r3, 0x0(r3)
    mtctr r0
    cmpwi r0, 0x0
    ble @L_801DB2FC
@L_801DB2D0:
    lwz r4, 0x68(r3)
    b @L_801DB2EC
@L_801DB2D8:
    lhz r0, 0x30(r4)
    cmplw r0, r5
    bne @L_801DB2E8
    addi r6, r6, 0x1
@L_801DB2E8:
    lwz r4, 0x34(r4)
@L_801DB2EC:
    cmplwi r4, 0x0
    bne @L_801DB2D8
    addi r3, r3, 0x8c
    bdnz @L_801DB2D0
@L_801DB2FC:
    lis r3, -32698
    li r0, 0x2
    addi r3, r3, 31956
    li r4, 0x0
    mtctr r0
@L_801DB310:
    lhz r0, 0x0(r3)
    cmplw r5, r0
    bne @L_801DB320
    addi r6, r6, 0x1
@L_801DB320:
    lhzu r0, 0x2(r3)
    cmplw r5, r0
    bne @L_801DB330
    addi r6, r6, 0x1
@L_801DB330:
    lhzu r0, 0x2(r3)
    cmplw r5, r0
    bne @L_801DB340
    addi r6, r6, 0x1
@L_801DB340:
    lhzu r0, 0x2(r3)
    cmplw r5, r0
    bne @L_801DB350
    addi r6, r6, 0x1
@L_801DB350:
    lhzu r0, 0x2(r3)
    cmplw r5, r0
    bne @L_801DB360
    addi r6, r6, 0x1
@L_801DB360:
    lhzu r0, 0x2(r3)
    cmplw r5, r0
    bne @L_801DB370
    addi r6, r6, 0x1
@L_801DB370:
    lhzu r0, 0x2(r3)
    cmplw r5, r0
    bne @L_801DB380
    addi r6, r6, 0x1
@L_801DB380:
    lhzu r0, 0x2(r3)
    cmplw r5, r0
    bne @L_801DB390
    addi r6, r6, 0x1
@L_801DB390:
    addi r3, r3, 0x2
    addi r4, r4, 0x7
    bdnz @L_801DB310
    cmpwi r6, 0x1
    bne @L_801DB3D4
    mr r30, r5
    mr r31, r5
    b @L_801DB3B4
@L_801DB3B0:
    bl fn_800F0308
@L_801DB3B4:
    mr r3, r31
    bl fn_8017B2CC
    cmpwi r3, 0x1
    beq @L_801DB3B0
    mr r3, r30
    bl fn_800F915C
    mr r3, r30
    bl fn_8017B1CC
@L_801DB3D4:
    li r0, 0x0
    sth r0, 0x30(r29)
@L_801DB3DC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
