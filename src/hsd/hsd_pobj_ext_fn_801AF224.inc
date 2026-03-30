    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stw r31, 0x5c(r1)
    mr. r31, r4
    stw r30, 0x58(r1)
    mr r30, r3
    stw r29, 0x54(r1)
    mr r29, r5
    bne @L_801AF25C
    addi r3, r2, -22864
    li r4, 0x276
    addi r5, r2, -22800
    bl fn_80196E10
@L_801AF25C:
    cmplwi r30, 0x0
    bne @L_801AF26C
    li r4, 0x0
    b @L_801AF2B8
@L_801AF26C:
    mr r4, r30
    b @L_801AF2AC
@L_801AF274:
    lwz r0, 0x4(r4)
    rlwinm. r0, r0, 0, 0, 0
    beq @L_801AF2A8
    lwz r0, 0x4(r4)
    rlwinm r3, r0, 0, 1, 3
    subis r0, r3, 0x1000
    cmplwi r0, 0x0
    bne @L_801AF2A8
    lwz r0, 0x4(r4)
    rlwinm r0, r0, 0, 4, 31
    cmplwi r0, 0x4
    bne @L_801AF2A8
    b @L_801AF2B8
@L_801AF2A8:
    lwz r4, 0x0(r4)
@L_801AF2AC:
    cmplwi r4, 0x0
    bne @L_801AF274
    li r4, 0x0
@L_801AF2B8:
    mr. r30, r4
    beq @L_801AF544
    lwz r30, 0x8(r30)
    cmplwi r30, 0x0
    bne @L_801AF2DC
    addi r3, r2, -22832
    li r4, 0x47c
    addi r5, r2, -22840
    bl fn_80196E10
@L_801AF2DC:
    cmplwi r30, 0x0
    beq @L_801AF32C
    cmplwi r30, 0x0
    bne @L_801AF2FC
    addi r3, r2, -22832
    li r4, 0x25d
    addi r5, r2, -22840
    bl fn_80196E10
@L_801AF2FC:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801AF31C
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801AF31C
    li r3, 0x1
@L_801AF31C:
    cmpwi r3, 0x0
    beq @L_801AF32C
    mr r3, r30
    bl fn_8019D9DC
@L_801AF32C:
    addi r3, r30, 0x44
    addi r4, r1, 0x18
    bl fn_800A2D64
    li r30, 0x0
    b @L_801AF524
@L_801AF340:
    rlwinm r0, r30, 2, 0, 29
    addi r3, r1, 0x18
    lfsx f2, r3, r0
    rlwinm r5, r30, 2, 0, 29
    addi r4, r1, 0x28
    rlwinm r0, r30, 2, 0, 29
    addi r3, r1, 0x38
    lfsx f1, r4, r5
    lfsx f0, r3, r0
    addi r3, r1, 0xc
    stfs f2, 0xc(r1)
    stfs f1, 0x10(r1)
    stfs f0, 0x14(r1)
    bl fn_800A3B38
    lfs f0, -22796(r2)
    fcmpo cr0, f1, f0
    ble @L_801AF38C
    lfs f0, -22812(r2)
    fdivs f1, f0, f1
@L_801AF38C:
    rlwinm r3, r30, 2, 0, 29
    rlwinm r0, r30, 2, 0, 29
    add r4, r31, r3
    rlwinm r5, r30, 2, 0, 29
    add r3, r31, r0
    lfs f2, 0x44(r4)
    lfs f0, 0x44(r3)
    rlwinm r0, r30, 2, 0, 29
    add r4, r31, r5
    rlwinm r5, r30, 2, 0, 29
    add r3, r31, r0
    fmuls f0, f2, f0
    rlwinm r0, r30, 2, 0, 29
    lfs f3, 0x54(r4)
    lfs f2, 0x54(r3)
    add r4, r31, r5
    add r3, r31, r0
    fmadds f2, f3, f2, f0
    lfs f4, 0x64(r4)
    lfs f3, 0x64(r3)
    lfs f0, -22848(r2)
    fmadds f9, f4, f3, f2
    fcmpo cr0, f9, f0
    ble @L_801AF444
    frsqrte f8, f9
    lfd f7, -22792(r2)
    lfd f4, -22784(r2)
    lfd f5, -22792(r2)
    fmul f6, f8, f8
    lfd f2, -22784(r2)
    lfd f3, -22792(r2)
    fmul f7, f7, f8
    lfd f0, -22784(r2)
    fnmsub f4, f9, f6, f4
    fmul f8, f7, f4
    fmul f4, f8, f8
    fmul f5, f5, f8
    fnmsub f2, f9, f4, f2
    fmul f8, f5, f2
    fmul f2, f8, f8
    fmul f3, f3, f8
    fnmsub f0, f9, f2, f0
    fmul f8, f3, f0
    fmul f9, f9, f8
    frsp f9, f9
    b @L_801AF4D0
@L_801AF444:
    lfd f0, -22776(r2)
    fcmpo cr0, f9, f0
    bge @L_801AF45C
    lis r3, -32696
    lfs f9, -30016(r3)
    b @L_801AF4D0
@L_801AF45C:
    stfs f9, 0x8(r1)
    lis r0, 0x7f80
    lwz r3, 0x8(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_801AF484
    bge @L_801AF4BC
    cmpwi r3, 0x0
    beq @L_801AF4A0
    b @L_801AF4BC
@L_801AF484:
    lwz r0, 0x8(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801AF498
    li r0, 0x1
    b @L_801AF4C0
@L_801AF498:
    li r0, 0x2
    b @L_801AF4C0
@L_801AF4A0:
    lwz r0, 0x8(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801AF4B4
    li r0, 0x5
    b @L_801AF4C0
@L_801AF4B4:
    li r0, 0x3
    b @L_801AF4C0
@L_801AF4BC:
    li r0, 0x4
@L_801AF4C0:
    cmpwi r0, 0x1
    bne @L_801AF4D0
    lis r3, -32696
    lfs f9, -30016(r3)
@L_801AF4D0:
    fmuls f1, f1, f9
    lfs f3, 0xc(r1)
    lfs f2, 0x10(r1)
    lis r3, -32713
    lfs f0, 0x14(r1)
    addi r0, r3, -12920
    fmuls f3, f3, f1
    rlwinm r4, r30, 2, 0, 29
    fmuls f2, f2, f1
    add r4, r0, r4
    fmuls f0, f0, f1
    mr r12, r29
    mr r3, r31
    addi r5, r1, 0xc
    lwz r4, 0x0(r4)
    stfs f3, 0xc(r1)
    stfs f2, 0x10(r1)
    stfs f0, 0x14(r1)
    mtctr r12
    bctrl
    addi r30, r30, 0x1
@L_801AF524:
    cmpwi r30, 0x3
    blt @L_801AF340
    mr r12, r29
    mr r3, r31
    li r4, 0x37
    li r5, 0x0
    mtctr r12
    bctrl
@L_801AF544:
    lwz r0, 0x64(r1)
    lwz r31, 0x5c(r1)
    lwz r30, 0x58(r1)
    lwz r29, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
