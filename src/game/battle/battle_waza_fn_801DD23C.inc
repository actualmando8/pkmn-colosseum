    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_801DD3CC
    lhz r31, 0x30(r30)
    cmplwi r31, 0x0
    beq @L_801DD274
    mr r3, r31
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
@L_801DD274:
    cmplwi r30, 0x0
    bne @L_801DD284
    li r0, 0x0
    b @L_801DD2B0
@L_801DD284:
    lbz r0, 0x77(r30)
    cmplwi r0, 0x0
    bne @L_801DD298
    li r0, 0x0
    b @L_801DD2B0
@L_801DD298:
    lbz r0, 0x4e(r30)
    cmplwi r0, 0x0
    bne @L_801DD2AC
    li r0, 0x0
    b @L_801DD2B0
@L_801DD2AC:
    lbz r0, 0x4f(r30)
@L_801DD2B0:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_801DD30C
    cmplwi r30, 0x0
    beq @L_801DD30C
    lbz r0, 0x4f(r30)
    cmplwi r0, 0x0
    beq @L_801DD30C
    lbz r0, 0x4e(r30)
    cmplwi r0, 0x0
    beq @L_801DD30C
    lbz r0, 0x4c(r30)
    lwz r31, 0x24(r30)
    cmplwi r0, 0x0
    beq @L_801DD2F0
    mr r3, r31
    bl fn_800E5D40
@L_801DD2F0:
    lbz r0, 0x4d(r30)
    cmplwi r0, 0x0
    beq @L_801DD304
    mr r3, r31
    bl fn_800E5A74
@L_801DD304:
    li r0, 0x0
    stb r0, 0x4f(r30)
@L_801DD30C:
    lwz r3, 0x24(r30)
    cmplwi r3, 0x0
    beq @L_801DD324
    li r4, 0x0
    li r5, 0x0
    bl fn_800EC154
@L_801DD324:
    mr r3, r30
    li r4, 0x0
    bl fn_801DA4E8
    lwz r0, 0xc(r30)
    cmplwi r0, 0x0
    beq @L_801DD350
    lwz r3, 0x28(r30)
    bl fn_801193BC
    lwz r3, 0x0(r30)
    lwz r4, 0xc(r30)
    bl fn_800F9210
@L_801DD350:
    addi r3, r30, 0x50
    bl fn_80140138
    lwz r3, 0x0(r30)
    cmplwi r3, 0x0
    beq @L_801DD3A4
    lwz r4, 0x4(r30)
    cmplwi r4, 0x0
    beq @L_801DD374
    bl fn_800F9210
@L_801DD374:
    lwz r4, 0x8(r30)
    cmplwi r4, 0x0
    beq @L_801DD388
    lwz r3, 0x0(r30)
    bl fn_800F9210
@L_801DD388:
    lhz r31, 0x7c(r30)
    cmplwi r31, 0x0
    beq @L_801DD3A4
    mr r3, r31
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
@L_801DD3A4:
    mr r3, r30
    bl fn_801DB1CC
    mr r3, r30
    bl fn_801D9E34
    mr r3, r30
    bl fn_801DA014
    mr r3, r30
    li r4, 0x0
    li r5, 0x8c
    bl memset
@L_801DD3CC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
