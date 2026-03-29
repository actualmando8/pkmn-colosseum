    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    lfs f31, lbl_8047A578(r13)
    lfs f2, lbl_8047BEC0(r2)
    fcmpo cr0, f31, f2
    ble @L_8005689C
    lfs f1, lbl_8047A57C(r13)
    lfs f0, lbl_8047BEB4(r2)
    fadds f1, f1, f31
    fcmpo cr0, f1, f0
    stfs f1, lbl_8047A57C(r13)
    cror eq, gt, eq
    bne @L_8005689C
    stfs f0, lbl_8047A57C(r13)
    stfs f2, lbl_8047A578(r13)
    @L_8005689C:
    lfs f3, lbl_8047A578(r13)
    lfs f2, lbl_8047BEC0(r2)
    fcmpo cr0, f3, f2
    bge @L_800568D0
    lfs f1, lbl_8047A57C(r13)
    lfs f0, lbl_8047BED0(r2)
    fadds f1, f1, f3
    fcmpo cr0, f1, f0
    stfs f1, lbl_8047A57C(r13)
    cror eq, lt, eq
    bne @L_800568D0
    stfs f0, lbl_8047A57C(r13)
    stfs f2, lbl_8047A578(r13)
    @L_800568D0:
    lfs f0, lbl_8047BEC0(r2)
    fcmpu cr0, f0, f31
    beq @L_80056A10
    lis r3, lbl_8026768C@ha
    lwz r6, lbl_8047A580(r13)
    addi r5, r3, lbl_8026768C@l
    lwz r4, 0x0(r5)
    cmpwi r6, 0x0
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r4, 0x14(r1)
    stw r3, 0x18(r1)
    stw r0, 0x1c(r1)
    blt @L_80056910
    cmpwi r6, 0x3
    blt @L_80056918
    @L_80056910:
    li r3, -0x1
    b @L_80056924
    @L_80056918:
    slwi r0, r6, 2
    addi r3, r1, 0x14
    lwzx r3, r3, r0
    @L_80056924:
    cmpwi r3, 0x0
    blt @L_80056970
    lfs f1, lbl_8047BEC0(r2)
    lfs f0, lbl_8047A578(r13)
    fcmpu cr0, f1, f0
    bne @L_80056948
    li r4, 0x0
    bl fn_80102254
    b @L_80056970
    @L_80056948:
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_80056970
    lfs f1, lbl_8047BED4(r2)
    lfs f0, lbl_8047A57C(r13)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r0, 0x24(r1)
    sth r0, 0x84(r3)
    @L_80056970:
    lis r3, lbl_8026768C@ha
    lwz r6, lbl_8047A584(r13)
    addi r5, r3, lbl_8026768C@l
    lwz r4, 0x0(r5)
    cmpwi r6, 0x0
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r4, 0x8(r1)
    stw r3, 0xc(r1)
    stw r0, 0x10(r1)
    blt @L_800569A4
    cmpwi r6, 0x3
    blt @L_800569AC
    @L_800569A4:
    li r3, -0x1
    b @L_800569B8
    @L_800569AC:
    slwi r0, r6, 2
    addi r3, r1, 0x8
    lwzx r3, r3, r0
    @L_800569B8:
    cmpwi r3, 0x0
    blt @L_80056A10
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_80056A10
    lfs f2, lbl_8047BED4(r2)
    lfs f1, lbl_8047A57C(r13)
    lfs f0, lbl_8047BEC0(r2)
    fmuls f1, f2, f1
    fcmpo cr0, f31, f0
    fctiwz f0, f1
    stfd f0, 0x20(r1)
    lwz r0, 0x24(r1)
    sth r0, 0x84(r3)
    ble @L_80056A04
    lha r4, 0x84(r3)
    addi r0, r4, 0x1a6
    sth r0, 0x84(r3)
    b @L_80056A10
    @L_80056A04:
    lha r4, 0x84(r3)
    subi r0, r4, 0x1a6
    sth r0, 0x84(r3)
    @L_80056A10:
    lfs f2, lbl_8047A574(r13)
    lfs f1, lbl_8047BED8(r2)
    lfs f0, lbl_8047BEB4(r2)
    fadds f1, f2, f1
    fcmpo cr0, f1, f0
    stfs f1, lbl_8047A574(r13)
    ble @L_80056A34
    fsubs f0, f1, f0
    stfs f0, lbl_8047A574(r13)
    @L_80056A34:
    lfs f2, lbl_8047A570(r13)
    lfs f1, lbl_8047BEB4(r2)
    fcmpo cr0, f2, f1
    bge @L_80056A60
    lfs f0, lbl_8047BED8(r2)
    fadds f0, f2, f0
    fcmpo cr0, f0, f1
    stfs f0, lbl_8047A570(r13)
    cror eq, gt, eq
    bne @L_80056A60
    stfs f1, lbl_8047A570(r13)
    @L_80056A60:
    psq_l f31, 0x38(r1), 0, 0
    lwz r0, 0x44(r1)
    lfd f31, 0x30(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
