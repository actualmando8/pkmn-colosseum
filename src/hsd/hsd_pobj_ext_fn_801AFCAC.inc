    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    lis r6, -32729
    cmplwi r3, 0x0
    stw r0, 0x44(r1)
    addi r7, r6, 20840
    stmw r27, 0x2c(r1)
    mr r28, r4
    mr r29, r5
    li r30, 0x0
    lwz r6, 0x0(r7)
    lwz r4, 0x4(r7)
    lwz r0, 0x8(r7)
    stw r6, 0x8(r1)
    stw r4, 0xc(r1)
    stw r0, 0x10(r1)
    bne @L_801AFCF8
    mr r3, r30
    b @L_801AFE54
@L_801AFCF8:
    mr r31, r3
    b @L_801AFDF8
@L_801AFD00:
    lwz r0, 0x4(r31)
    rlwinm r3, r0, 0, 1, 3
    subis r0, r3, 0x1000
    cmplwi r0, 0x0
    bne @L_801AFDF4
    lwz r0, 0x4(r31)
    rlwinm. r0, r0, 0, 0, 0
    beq @L_801AFDF4
    cmplwi r31, 0x0
    bne @L_801AFD30
    li r0, 0x0
    b @L_801AFD38
@L_801AFD30:
    lwz r0, 0x4(r31)
    rlwinm r0, r0, 0, 4, 31
@L_801AFD38:
    cmplw r28, r0
    bne @L_801AFDF4
    lwz r0, 0x8(r31)
    cmplwi r0, 0x0
    bne @L_801AFD60
    lis r4, -32729
    addi r3, r2, -22864
    addi r5, r4, 21172
    li r4, 0x1f2
    bl fn_80196E10
@L_801AFD60:
    lwz r27, 0x8(r31)
    cmplwi r27, 0x0
    beq @L_801AFDB4
    cmplwi r27, 0x0
    bne @L_801AFD84
    addi r3, r2, -22832
    li r4, 0x25d
    addi r5, r2, -22840
    bl fn_80196E10
@L_801AFD84:
    lwz r0, 0x14(r27)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801AFDA4
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801AFDA4
    li r3, 0x1
@L_801AFDA4:
    cmpwi r3, 0x0
    beq @L_801AFDB4
    mr r3, r27
    bl fn_8019D9DC
@L_801AFDB4:
    lwz r5, 0x8(r31)
    addi r30, r30, 0x1
    lwz r4, 0x8(r31)
    lwz r3, 0x8(r31)
    lfs f1, 0x8(r1)
    lfs f0, 0x50(r5)
    lfs f3, 0xc(r1)
    lfs f2, 0x60(r4)
    fadds f4, f1, f0
    lfs f1, 0x10(r1)
    lfs f0, 0x70(r3)
    fadds f2, f3, f2
    stfs f4, 0x8(r1)
    fadds f0, f1, f0
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
@L_801AFDF4:
    lwz r31, 0x0(r31)
@L_801AFDF8:
    cmplwi r31, 0x0
    bne @L_801AFD00
    cmpwi r30, 0x0
    beq @L_801AFE50
    xoris r3, r30, 0x8000
    lis r0, 0x4330
    stw r3, 0x1c(r1)
    lfd f1, -22808(r2)
    stw r0, 0x18(r1)
    lfs f4, -22812(r2)
    lfd f0, 0x18(r1)
    lfs f2, 0x8(r1)
    fsubs f3, f0, f1
    lfs f1, 0xc(r1)
    lfs f0, 0x10(r1)
    fdivs f3, f4, f3
    fmuls f2, f3, f2
    fmuls f1, f3, f1
    fmuls f0, f3, f0
    stfs f2, 0x0(r29)
    stfs f1, 0x4(r29)
    stfs f0, 0x8(r29)
@L_801AFE50:
    mr r3, r30
@L_801AFE54:
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
