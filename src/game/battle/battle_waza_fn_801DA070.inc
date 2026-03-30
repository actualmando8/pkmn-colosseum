    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr. r31, r3
    stw r30, 0x18(r1)
    beq @L_801DA20C
    lis r3, -32698
    addi r3, r3, 31936
    lwz r3, 0x8(r3)
    cmplwi r3, 0x0
    beq @L_801DA20C
    lwz r0, 0x80(r31)
    cmplwi r0, 0x0
    bne @L_801DA20C
    li r4, 0x0
    li r5, 0x0
    bl fn_801190DC
    stw r3, 0x80(r31)
    lwz r3, 0x80(r31)
    cmplwi r3, 0x0
    beq @L_801DA20C
    lbz r0, 0x18(r31)
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x1
    beq @L_801DA0E0
    li r4, 0x0
    bl fn_80118C88
@L_801DA0E0:
    lhz r0, 0x32(r31)
    lwz r4, 0x2c(r31)
    mulli r0, r0, 0xd4
    lwz r3, 0x24(r31)
    add r30, r4, r0
    bl fn_800E6BC8
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DA110
    lwz r3, 0x24(r31)
    bl fn_800EE0E8
    subi r4, r3, 0x1
    b @L_801DA114
@L_801DA110:
    lwz r4, 0x54(r30)
@L_801DA114:
    cmpwi r4, 0x0
    ble @L_801DA128
    lwz r3, 0x24(r31)
    bl fn_800EE150
    b @L_801DA12C
@L_801DA128:
    li r3, 0x0
@L_801DA12C:
    cmplwi r3, 0x0
    mr r30, r3
    beq @L_801DA20C
    cmplwi r31, 0x0
    beq @L_801DA148
    lwz r0, 0x10(r31)
    b @L_801DA14C
@L_801DA148:
    li r0, 0x0
@L_801DA14C:
    cmpwi r0, 0x1
    beq @L_801DA190
    bge @L_801DA170
    cmpwi r0, -0x1
    beq @L_801DA188
    bge @L_801DA1A8
    cmpwi r0, -0x2
    bge @L_801DA180
    b @L_801DA1A8
@L_801DA170:
    cmpwi r0, 0x3
    beq @L_801DA1A0
    bge @L_801DA1A8
    b @L_801DA198
@L_801DA180:
    lfs f1, -21272(r2)
    b @L_801DA1AC
@L_801DA188:
    lfs f1, -21268(r2)
    b @L_801DA1AC
@L_801DA190:
    lfs f1, -21264(r2)
    b @L_801DA1AC
@L_801DA198:
    lfs f1, -21260(r2)
    b @L_801DA1AC
@L_801DA1A0:
    lfs f1, -21256(r2)
    b @L_801DA1AC
@L_801DA1A8:
    lfs f1, -21252(r2)
@L_801DA1AC:
    fmr f2, f1
    addi r3, r1, 0x8
    fmr f3, f1
    bl fn_800E01F4
    lwz r3, 0x80(r31)
    mr r4, r30
    li r5, 0x4
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_80118FB0
    lwz r3, 0x80(r31)
    li r4, 0x1
    bl fn_80118D18
    lwz r3, 0x80(r31)
    li r4, 0x1
    bl fn_80118CAC
    lwz r3, 0x80(r31)
    addi r4, r1, 0x8
    li r5, 0x1
    li r6, 0x0
    bl fn_80118DE0
    mr r3, r30
    bl fn_800EE828
@L_801DA20C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
