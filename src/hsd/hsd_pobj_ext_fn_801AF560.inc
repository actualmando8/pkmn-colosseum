    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    lis r6, -32729
    stw r0, 0xa4(r1)
    stmw r25, 0x84(r1)
    addi r31, r6, 20840
    mr. r26, r3
    mr r28, r4
    mr r27, r5
    li r30, 0x0
    lwz r10, 0xc(r31)
    lwz r9, 0x10(r31)
    lwz r8, 0x14(r31)
    lwz r7, 0x18(r31)
    lwz r6, 0x1c(r31)
    lwz r5, 0x20(r31)
    lwz r4, 0x0(r31)
    lwz r3, 0x4(r31)
    lwz r0, 0x8(r31)
    stw r10, 0x64(r1)
    stw r9, 0x68(r1)
    stw r8, 0x6c(r1)
    stw r7, 0x58(r1)
    stw r6, 0x5c(r1)
    stw r5, 0x60(r1)
    stw r4, 0x34(r1)
    stw r3, 0x38(r1)
    stw r0, 0x3c(r1)
    bne @L_801AF5D8
    b @L_801AF72C
@L_801AF5D8:
    mr r29, r26
    b @L_801AF6D4
@L_801AF5E0:
    lwz r0, 0x4(r29)
    rlwinm r3, r0, 0, 1, 3
    subis r0, r3, 0x1000
    cmplwi r0, 0x0
    bne @L_801AF6D0
    lwz r0, 0x4(r29)
    rlwinm. r0, r0, 0, 0, 0
    beq @L_801AF6D0
    cmplwi r29, 0x0
    bne @L_801AF610
    li r0, 0x0
    b @L_801AF618
@L_801AF610:
    lwz r0, 0x4(r29)
    rlwinm r0, r0, 0, 4, 31
@L_801AF618:
    cmplwi r0, 0x2
    bne @L_801AF6D0
    lwz r0, 0x8(r29)
    cmplwi r0, 0x0
    bne @L_801AF63C
    addi r5, r31, 0x14c
    addi r3, r2, -22864
    li r4, 0x1f2
    bl fn_80196E10
@L_801AF63C:
    lwz r25, 0x8(r29)
    cmplwi r25, 0x0
    beq @L_801AF690
    cmplwi r25, 0x0
    bne @L_801AF660
    addi r3, r2, -22832
    li r4, 0x25d
    addi r5, r2, -22840
    bl fn_80196E10
@L_801AF660:
    lwz r0, 0x14(r25)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801AF680
    lwz r0, 0x14(r25)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801AF680
    li r3, 0x1
@L_801AF680:
    cmpwi r3, 0x0
    beq @L_801AF690
    mr r3, r25
    bl fn_8019D9DC
@L_801AF690:
    lwz r5, 0x8(r29)
    addi r30, r30, 0x1
    lwz r4, 0x8(r29)
    lwz r3, 0x8(r29)
    lfs f1, 0x34(r1)
    lfs f0, 0x50(r5)
    lfs f3, 0x38(r1)
    fadds f4, f1, f0
    lfs f2, 0x60(r4)
    lfs f1, 0x3c(r1)
    lfs f0, 0x70(r3)
    fadds f2, f3, f2
    stfs f4, 0x34(r1)
    fadds f0, f1, f0
    stfs f2, 0x38(r1)
    stfs f0, 0x3c(r1)
@L_801AF6D0:
    lwz r29, 0x0(r29)
@L_801AF6D4:
    cmplwi r29, 0x0
    bne @L_801AF5E0
    cmpwi r30, 0x0
    beq @L_801AF72C
    xoris r3, r30, 0x8000
    lis r0, 0x4330
    stw r3, 0x74(r1)
    lfd f1, -22808(r2)
    stw r0, 0x70(r1)
    lfs f4, -22812(r2)
    lfd f0, 0x70(r1)
    lfs f2, 0x34(r1)
    fsubs f3, f0, f1
    lfs f1, 0x38(r1)
    lfs f0, 0x3c(r1)
    fdivs f3, f4, f3
    fmuls f2, f3, f2
    fmuls f1, f3, f1
    fmuls f0, f3, f0
    stfs f2, 0x4c(r1)
    stfs f1, 0x50(r1)
    stfs f0, 0x54(r1)
@L_801AF72C:
    cmpwi r30, 0x0
    beq @L_801AFC98
    lfs f0, 0x50(r28)
    addi r3, r1, 0x4c
    addi r4, r1, 0x40
    addi r5, r1, 0x4c
    stfs f0, 0x40(r1)
    lfs f0, 0x60(r28)
    stfs f0, 0x44(r1)
    lfs f0, 0x70(r28)
    stfs f0, 0x48(r1)
    bl fn_800A3A9C
    lwz r4, 0x0(r31)
    cmplwi r26, 0x0
    lwz r3, 0x4(r31)
    li r30, 0x0
    lwz r0, 0x8(r31)
    stw r4, 0x28(r1)
    stw r3, 0x2c(r1)
    stw r0, 0x30(r1)
    bne @L_801AF784
    b @L_801AF8D8
@L_801AF784:
    mr r29, r26
    b @L_801AF880
@L_801AF78C:
    lwz r0, 0x4(r29)
    rlwinm r3, r0, 0, 1, 3
    subis r0, r3, 0x1000
    cmplwi r0, 0x0
    bne @L_801AF87C
    lwz r0, 0x4(r29)
    rlwinm. r0, r0, 0, 0, 0
    beq @L_801AF87C
    cmplwi r29, 0x0
    bne @L_801AF7BC
    li r0, 0x0
    b @L_801AF7C4
@L_801AF7BC:
    lwz r0, 0x4(r29)
    rlwinm r0, r0, 0, 4, 31
@L_801AF7C4:
    cmplwi r0, 0x3
    bne @L_801AF87C
    lwz r0, 0x8(r29)
    cmplwi r0, 0x0
    bne @L_801AF7E8
    addi r5, r31, 0x14c
    addi r3, r2, -22864
    li r4, 0x1f2
    bl fn_80196E10
@L_801AF7E8:
    lwz r25, 0x8(r29)
    cmplwi r25, 0x0
    beq @L_801AF83C
    cmplwi r25, 0x0
    bne @L_801AF80C
    addi r3, r2, -22832
    li r4, 0x25d
    addi r5, r2, -22840
    bl fn_80196E10
@L_801AF80C:
    lwz r0, 0x14(r25)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801AF82C
    lwz r0, 0x14(r25)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801AF82C
    li r3, 0x1
@L_801AF82C:
    cmpwi r3, 0x0
    beq @L_801AF83C
    mr r3, r25
    bl fn_8019D9DC
@L_801AF83C:
    lwz r5, 0x8(r29)
    addi r30, r30, 0x1
    lwz r4, 0x8(r29)
    lwz r3, 0x8(r29)
    lfs f1, 0x28(r1)
    lfs f0, 0x50(r5)
    lfs f3, 0x2c(r1)
    fadds f4, f1, f0
    lfs f2, 0x60(r4)
    lfs f1, 0x30(r1)
    lfs f0, 0x70(r3)
    fadds f2, f3, f2
    stfs f4, 0x28(r1)
    fadds f0, f1, f0
    stfs f2, 0x2c(r1)
    stfs f0, 0x30(r1)
@L_801AF87C:
    lwz r29, 0x0(r29)
@L_801AF880:
    cmplwi r29, 0x0
    bne @L_801AF78C
    cmpwi r30, 0x0
    beq @L_801AF8D8
    xoris r3, r30, 0x8000
    lis r0, 0x4330
    stw r3, 0x74(r1)
    lfd f1, -22808(r2)
    stw r0, 0x70(r1)
    lfs f4, -22812(r2)
    lfd f0, 0x70(r1)
    lfs f2, 0x28(r1)
    fsubs f3, f0, f1
    lfs f1, 0x2c(r1)
    lfs f0, 0x30(r1)
    fdivs f3, f4, f3
    fmuls f2, f3, f2
    fmuls f1, f3, f1
    fmuls f0, f3, f0
    stfs f2, 0x58(r1)
    stfs f1, 0x5c(r1)
    stfs f0, 0x60(r1)
@L_801AF8D8:
    cmpwi r30, 0x0
    beq @L_801AF8F4
    addi r3, r1, 0x58
    addi r4, r1, 0x40
    addi r5, r1, 0x58
    bl fn_800A3A9C
    b @L_801AF930
@L_801AF8F4:
    addi r3, r1, 0x4c
    addi r4, r1, 0x58
    bl fn_800A3B7C
    lfs f2, -22812(r2)
    lfd f0, -22768(r2)
    fsubs f1, f2, f1
    fabs f1, f1
    fcmpo cr0, f1, f0
    bge @L_801AF930
    lfs f2, -22848(r2)
    lfs f1, -22848(r2)
    lfs f0, -22812(r2)
    stfs f2, 0x58(r1)
    stfs f1, 0x5c(r1)
    stfs f0, 0x60(r1)
@L_801AF930:
    lwz r0, 0x74(r28)
    cmplwi r0, 0x0
    beq @L_801AF958
    lwz r5, 0x74(r28)
    lwz r4, 0x0(r5)
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r4, 0x64(r1)
    stw r3, 0x68(r1)
    stw r0, 0x6c(r1)
@L_801AF958:
    addi r3, r1, 0x4c
    addi r4, r1, 0x58
    addi r5, r1, 0x10
    bl fn_800A3B9C
    addi r3, r1, 0x4c
    addi r4, r1, 0x4c
    bl fn_800A3B7C
    lfs f3, -22796(r2)
    lfs f2, -22812(r2)
    fadds f1, f3, f1
    lfs f0, -22848(r2)
    fdivs f1, f2, f1
    fcmpo cr0, f1, f0
    ble @L_801AF9E8
    frsqrte f8, f1
    lfd f7, -22792(r2)
    lfd f4, -22784(r2)
    lfd f5, -22792(r2)
    fmul f6, f8, f8
    lfd f2, -22784(r2)
    lfd f3, -22792(r2)
    fmul f7, f7, f8
    lfd f0, -22784(r2)
    fnmsub f4, f1, f6, f4
    fmul f8, f7, f4
    fmul f4, f8, f8
    fmul f5, f5, f8
    fnmsub f2, f1, f4, f2
    fmul f8, f5, f2
    fmul f2, f8, f8
    fmul f3, f3, f8
    fnmsub f0, f1, f2, f0
    fmul f8, f3, f0
    fmul f1, f1, f8
    frsp f1, f1
    b @L_801AFA74
@L_801AF9E8:
    lfd f0, -22776(r2)
    fcmpo cr0, f1, f0
    bge @L_801AFA00
    lis r3, -32696
    lfs f1, -30016(r3)
    b @L_801AFA74
@L_801AFA00:
    stfs f1, 0xc(r1)
    lis r0, 0x7f80
    lwz r3, 0xc(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_801AFA28
    bge @L_801AFA60
    cmpwi r3, 0x0
    beq @L_801AFA44
    b @L_801AFA60
@L_801AFA28:
    lwz r0, 0xc(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801AFA3C
    li r0, 0x1
    b @L_801AFA64
@L_801AFA3C:
    li r0, 0x2
    b @L_801AFA64
@L_801AFA44:
    lwz r0, 0xc(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801AFA58
    li r0, 0x5
    b @L_801AFA64
@L_801AFA58:
    li r0, 0x3
    b @L_801AFA64
@L_801AFA60:
    li r0, 0x4
@L_801AFA64:
    cmpwi r0, 0x1
    bne @L_801AFA74
    lis r3, -32696
    lfs f1, -30016(r3)
@L_801AFA74:
    addi r3, r1, 0x4c
    addi r4, r1, 0x4c
    bl fn_800A3AC0
    addi r3, r1, 0x10
    addi r4, r1, 0x10
    bl fn_800A3B7C
    lfs f3, -22796(r2)
    lfs f2, -22812(r2)
    fadds f1, f3, f1
    lfs f0, -22848(r2)
    fdivs f1, f2, f1
    fcmpo cr0, f1, f0
    ble @L_801AFB00
    frsqrte f8, f1
    lfd f7, -22792(r2)
    lfd f4, -22784(r2)
    lfd f5, -22792(r2)
    fmul f6, f8, f8
    lfd f2, -22784(r2)
    lfd f3, -22792(r2)
    fmul f7, f7, f8
    lfd f0, -22784(r2)
    fnmsub f4, f1, f6, f4
    fmul f8, f7, f4
    fmul f4, f8, f8
    fmul f5, f5, f8
    fnmsub f2, f1, f4, f2
    fmul f8, f5, f2
    fmul f2, f8, f8
    fmul f3, f3, f8
    fnmsub f0, f1, f2, f0
    fmul f8, f3, f0
    fmul f1, f1, f8
    frsp f1, f1
    b @L_801AFB8C
@L_801AFB00:
    lfd f0, -22776(r2)
    fcmpo cr0, f1, f0
    bge @L_801AFB18
    lis r3, -32696
    lfs f1, -30016(r3)
    b @L_801AFB8C
@L_801AFB18:
    stfs f1, 0x8(r1)
    lis r0, 0x7f80
    lwz r3, 0x8(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_801AFB40
    bge @L_801AFB78
    cmpwi r3, 0x0
    beq @L_801AFB5C
    b @L_801AFB78
@L_801AFB40:
    lwz r0, 0x8(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801AFB54
    li r0, 0x1
    b @L_801AFB7C
@L_801AFB54:
    li r0, 0x2
    b @L_801AFB7C
@L_801AFB5C:
    lwz r0, 0x8(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801AFB70
    li r0, 0x5
    b @L_801AFB7C
@L_801AFB70:
    li r0, 0x3
    b @L_801AFB7C
@L_801AFB78:
    li r0, 0x4
@L_801AFB7C:
    cmpwi r0, 0x1
    bne @L_801AFB8C
    lis r3, -32696
    lfs f1, -30016(r3)
@L_801AFB8C:
    addi r3, r1, 0x10
    addi r4, r1, 0x10
    bl fn_800A3AC0
    addi r3, r1, 0x10
    addi r4, r1, 0x4c
    addi r5, r1, 0x58
    bl fn_800A3B9C
    lfs f1, 0x4c(r1)
    mr r12, r27
    lfs f0, 0x64(r1)
    mr r3, r28
    lfs f3, 0x50(r1)
    addi r5, r1, 0x1c
    lfs f2, 0x64(r1)
    fmuls f4, f1, f0
    lfs f1, 0x54(r1)
    li r4, 0x32
    lfs f0, 0x64(r1)
    fmuls f2, f3, f2
    stfs f4, 0x1c(r1)
    fmuls f0, f1, f0
    stfs f2, 0x20(r1)
    stfs f0, 0x24(r1)
    mtctr r12
    bctrl
    lfs f1, 0x58(r1)
    mr r12, r27
    lfs f0, 0x68(r1)
    mr r3, r28
    lfs f3, 0x5c(r1)
    addi r5, r1, 0x1c
    lfs f2, 0x68(r1)
    fmuls f4, f1, f0
    lfs f1, 0x60(r1)
    li r4, 0x33
    lfs f0, 0x68(r1)
    fmuls f2, f3, f2
    stfs f4, 0x1c(r1)
    fmuls f0, f1, f0
    stfs f2, 0x20(r1)
    stfs f0, 0x24(r1)
    mtctr r12
    bctrl
    lfs f1, 0x10(r1)
    mr r12, r27
    lfs f0, 0x6c(r1)
    mr r3, r28
    lfs f3, 0x14(r1)
    addi r5, r1, 0x1c
    lfs f2, 0x6c(r1)
    fmuls f4, f1, f0
    lfs f1, 0x18(r1)
    li r4, 0x34
    lfs f0, 0x6c(r1)
    fmuls f2, f3, f2
    stfs f4, 0x1c(r1)
    fmuls f0, f1, f0
    stfs f2, 0x20(r1)
    stfs f0, 0x24(r1)
    mtctr r12
    bctrl
    mr r12, r27
    mr r3, r28
    li r4, 0x37
    li r5, 0x0
    mtctr r12
    bctrl
@L_801AFC98:
    lmw r25, 0x84(r1)
    lwz r0, 0xa4(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
