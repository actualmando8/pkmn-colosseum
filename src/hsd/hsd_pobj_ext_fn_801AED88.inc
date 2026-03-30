    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r26, 0x28(r1)
    mr. r27, r3
    mr r28, r4
    mr r29, r5
    beq @L_801AEFDC
    lis r3, -32729
    cmplwi r27, 0x0
    addi r5, r3, 20840
    li r31, 0x0
    lwz r4, 0x0(r5)
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r4, 0x8(r1)
    stw r3, 0xc(r1)
    stw r0, 0x10(r1)
    bne @L_801AEDD8
    b @L_801AEF30
@L_801AEDD8:
    mr r30, r27
    b @L_801AEED8
@L_801AEDE0:
    lwz r0, 0x4(r30)
    rlwinm r3, r0, 0, 1, 3
    subis r0, r3, 0x1000
    cmplwi r0, 0x0
    bne @L_801AEED4
    lwz r0, 0x4(r30)
    rlwinm. r0, r0, 0, 0, 0
    beq @L_801AEED4
    cmplwi r30, 0x0
    bne @L_801AEE10
    li r0, 0x0
    b @L_801AEE18
@L_801AEE10:
    lwz r0, 0x4(r30)
    rlwinm r0, r0, 0, 4, 31
@L_801AEE18:
    cmplwi r0, 0x1
    bne @L_801AEED4
    lwz r0, 0x8(r30)
    cmplwi r0, 0x0
    bne @L_801AEE40
    lis r4, -32729
    addi r3, r2, -22864
    addi r5, r4, 21172
    li r4, 0x1f2
    bl fn_80196E10
@L_801AEE40:
    lwz r26, 0x8(r30)
    cmplwi r26, 0x0
    beq @L_801AEE94
    cmplwi r26, 0x0
    bne @L_801AEE64
    addi r3, r2, -22832
    li r4, 0x25d
    addi r5, r2, -22840
    bl fn_80196E10
@L_801AEE64:
    lwz r0, 0x14(r26)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801AEE84
    lwz r0, 0x14(r26)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801AEE84
    li r3, 0x1
@L_801AEE84:
    cmpwi r3, 0x0
    beq @L_801AEE94
    mr r3, r26
    bl fn_8019D9DC
@L_801AEE94:
    lwz r5, 0x8(r30)
    addi r31, r31, 0x1
    lwz r4, 0x8(r30)
    lwz r3, 0x8(r30)
    lfs f1, 0x8(r1)
    lfs f0, 0x50(r5)
    lfs f3, 0xc(r1)
    fadds f4, f1, f0
    lfs f2, 0x60(r4)
    lfs f1, 0x10(r1)
    lfs f0, 0x70(r3)
    fadds f2, f3, f2
    stfs f4, 0x8(r1)
    fadds f0, f1, f0
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
@L_801AEED4:
    lwz r30, 0x0(r30)
@L_801AEED8:
    cmplwi r30, 0x0
    bne @L_801AEDE0
    cmpwi r31, 0x0
    beq @L_801AEF30
    xoris r3, r31, 0x8000
    lis r0, 0x4330
    stw r3, 0x24(r1)
    lfd f1, -22808(r2)
    stw r0, 0x20(r1)
    lfs f4, -22812(r2)
    lfd f0, 0x20(r1)
    lfs f2, 0x8(r1)
    fsubs f3, f0, f1
    lfs f1, 0xc(r1)
    lfs f0, 0x10(r1)
    fdivs f3, f4, f3
    fmuls f2, f3, f2
    fmuls f1, f3, f1
    fmuls f0, f3, f0
    stfs f2, 0x14(r1)
    stfs f1, 0x18(r1)
    stfs f0, 0x1c(r1)
@L_801AEF30:
    cmpwi r31, 0x0
    beq @L_801AEF68
    mr r12, r29
    mr r3, r28
    addi r5, r1, 0x14
    li r4, 0x35
    mtctr r12
    bctrl
    mr r12, r29
    mr r3, r28
    li r4, 0x38
    li r5, 0x0
    mtctr r12
    bctrl
@L_801AEF68:
    mr r3, r27
    mr r4, r28
    mr r5, r29
    bl fn_801AF560
    mr r3, r27
    mr r4, r28
    mr r5, r29
    bl fn_801AF224
    mr r3, r27
    mr r4, r28
    mr r5, r29
    bl fn_801AEFF0
    mr r26, r27
    b @L_801AEFD4
@L_801AEFA0:
    lwz r0, 0x4(r26)
    rlwinm. r0, r0, 0, 1, 3
    bne @L_801AEFD0
    lwz r0, 0x4(r26)
    rlwinm. r0, r0, 0, 0, 0
    beq @L_801AEFD0
    lwz r0, 0x4(r26)
    mr r5, r28
    mr r6, r29
    addi r3, r26, 0x8
    rlwinm r4, r0, 0, 4, 31
    bl fn_801AE008
@L_801AEFD0:
    lwz r26, 0x0(r26)
@L_801AEFD4:
    cmplwi r26, 0x0
    bne @L_801AEFA0
@L_801AEFDC:
    lmw r26, 0x28(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
