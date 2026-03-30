    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r27, 0x2c(r1)
    mr r29, r3
    lbz r0, 0x16(r3)
    lwz r31, 0x24(r3)
    cmplwi r0, 0x0
    beq @L_801D9C48
    li r3, 0x0
    b @L_801D9E08
@L_801D9C48:
    cmplwi r31, 0x0
    beq @L_801D9DE0
    mr r3, r31
    bl fn_800E3CC8
    lhz r5, 0x32(r29)
    lbz r0, 0x18(r29)
    mulli r3, r5, 0xd4
    lwz r4, 0x2c(r29)
    rlwinm r0, r0, 0, 28, 28
    addi r30, r3, 0x8c
    cmpwi r0, 0x8
    add r30, r4, r30
    bne @L_801D9C84
    li r3, 0x1
    b @L_801D9E08
@L_801D9C84:
    lwz r28, 0x4(r30)
    cmpwi r28, 0x0
    bne @L_801D9CE4
    cmplwi r5, 0xa
    bne @L_801D9CA8
    mr r3, r29
    bl fn_801DEE14
    li r3, 0x1
    b @L_801D9E08
@L_801D9CA8:
    li r0, 0x1
    sth r0, 0x34(r29)
    lwz r0, 0x8(r30)
    cmpwi r0, 0x0
    bne @L_801D9CD0
    lwz r4, 0xc(r30)
    mr r3, r29
    li r5, 0x0
    bl fn_801DF070
    b @L_801D9DD8
@L_801D9CD0:
    mr r3, r29
    li r4, 0x0
    li r5, 0x0
    bl fn_801DEF0C
    b @L_801D9DD8
@L_801D9CE4:
    li r0, 0x0
    mr r3, r29
    sth r0, 0x34(r29)
    bl fn_801DF160
    lbz r0, 0x18(r29)
    mr r27, r3
    rlwinm r0, r0, 0, 29, 29
    cmpwi r0, 0x4
    beq @L_801D9D1C
    li r0, 0x0
    mr r3, r31
    stb r0, 0x19(r29)
    li r4, 0x1
    bl fn_800EC188
@L_801D9D1C:
    mr r3, r31
    mr r4, r27
    bl fn_800ECCA8
    mr r3, r31
    li r4, 0x0
    bl fn_800ECB74
    lfs f1, -21336(r2)
    mr r3, r31
    bl fn_800EC9DC
    mr r3, r31
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_800EC4D0
    lfs f0, 0xc(r1)
    srawi r5, r28, 1
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    divw r0, r5, r0
    stw r0, 0x4(r30)
    lfs f0, 0xc(r1)
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r4, 0x1c(r1)
    divw r0, r5, r4
    mullw r0, r0, r4
    subf. r5, r0, r5
    bne @L_801D9DA4
    lwz r3, 0x4(r30)
    cmpwi r3, 0x0
    beq @L_801D9DA4
    subi r0, r3, 0x1
    mr r5, r4
    stw r0, 0x4(r30)
@L_801D9DA4:
    xoris r3, r5, 0x8000
    lis r0, 0x4330
    stw r3, 0x1c(r1)
    mr r3, r31
    lfd f1, -21280(r2)
    stw r0, 0x18(r1)
    lfs f2, 0xc(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f1
    fsubs f1, f2, f0
    bl fn_800ECA78
    mr r3, r31
    bl fn_800EC990
@L_801D9DD8:
    li r3, 0x1
    b @L_801D9E08
@L_801D9DE0:
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D9DF4
    li r3, 0x4
    bl fn_801D744C
@L_801D9DF4:
    lis r3, -32728
    addi r3, r3, -26816
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
@L_801D9E08:
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
