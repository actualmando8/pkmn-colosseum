    nofralloc
    cmplwi r3, 0x0
    stwu r1, -0x10(r1)
    beq @L_8019BB70
    cmplwi r4, 0x15
    bgt @L_8019BB70
    lis r6, jumptable_8036C864@ha
    slwi r0, r4, 2
    addi r4, r6, jumptable_8036C864@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    lfs f0, 0x0(r5)
    stfs f0, 0x10(r3)
    b @L_8019BB70
    lfs f0, 0x0(r5)
    stfs f0, 0x14(r3)
    b @L_8019BB70
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DA68(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_8019B9A8
    lfs f1, lbl_8047DA68(r2)
    b @L_8019B9BC
    @L_8019B9A8:
    lfs f0, lbl_8047DA6C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_8019B9BC
    lfs f1, lbl_8047DA6C(r2)
    @L_8019B9BC:
    lfs f0, lbl_8047DA70(r2)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x18(r3)
    b @L_8019BB70
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DA68(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_8019B9F4
    lfs f1, lbl_8047DA68(r2)
    b @L_8019BA08
    @L_8019B9F4:
    lfs f0, lbl_8047DA6C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_8019BA08
    lfs f1, lbl_8047DA6C(r2)
    @L_8019BA08:
    lfs f0, lbl_8047DA70(r2)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x19(r3)
    b @L_8019BB70
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DA68(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_8019BA40
    lfs f1, lbl_8047DA68(r2)
    b @L_8019BA54
    @L_8019BA40:
    lfs f0, lbl_8047DA6C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_8019BA54
    lfs f1, lbl_8047DA6C(r2)
    @L_8019BA54:
    lfs f0, lbl_8047DA70(r2)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x1a(r3)
    b @L_8019BB70
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DA68(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_8019BA8C
    lfs f1, lbl_8047DA68(r2)
    b @L_8019BAA0
    @L_8019BA8C:
    lfs f0, lbl_8047DA6C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_8019BAA0
    lfs f1, lbl_8047DA6C(r2)
    @L_8019BAA0:
    lfs f0, lbl_8047DA70(r2)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x1b(r3)
    b @L_8019BB70
    lfs f0, 0x0(r5)
    cmplwi r3, 0x0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    beq @L_8019BADC
    lwz r3, 0xc(r3)
    b @L_8019BAE0
    @L_8019BADC:
    li r3, 0x0
    @L_8019BAE0:
    cmplwi r3, 0x0
    beq @L_8019BB70
    cmpwi r0, -0x140
    bgt @L_8019BAFC
    li r0, -0x140
    sth r0, 0xc(r3)
    b @L_8019BB70
    @L_8019BAFC:
    cmpwi r0, 0x140
    blt @L_8019BB10
    li r0, 0x140
    sth r0, 0xc(r3)
    b @L_8019BB70
    @L_8019BB10:
    sth r0, 0xc(r3)
    b @L_8019BB70
    lfs f0, 0x0(r5)
    cmplwi r3, 0x0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    beq @L_8019BB38
    lwz r3, 0xc(r3)
    b @L_8019BB3C
    @L_8019BB38:
    li r3, 0x0
    @L_8019BB3C:
    cmplwi r3, 0x0
    beq @L_8019BB70
    cmpwi r0, 0x0
    bgt @L_8019BB58
    li r0, 0x0
    sth r0, 0xe(r3)
    b @L_8019BB70
    @L_8019BB58:
    cmpwi r0, 0x280
    blt @L_8019BB6C
    li r0, 0x280
    sth r0, 0xe(r3)
    b @L_8019BB70
    @L_8019BB6C:
    sth r0, 0xe(r3)
    @L_8019BB70:
    addi r1, r1, 0x10
    blr
