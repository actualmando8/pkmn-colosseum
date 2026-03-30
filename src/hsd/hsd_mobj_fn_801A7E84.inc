    nofralloc
    cmplwi r3, 0x0
    stwu r1, -0x10(r1)
    beq @L_801A834C
    cmplwi r4, 0xd
    bgt @L_801A834C
    lis r6, jumptable_8036CB84@ha
    slwi r0, r4, 2
    addi r4, r6, jumptable_8036CB84@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_801A834C
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DC40(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A7ED8
    lfs f1, lbl_8047DC40(r2)
    b @L_801A7EEC
    @L_801A7ED8:
    lfs f0, lbl_8047DC2C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A7EEC
    lfs f1, lbl_8047DC2C(r2)
    @L_801A7EEC:
    lfs f0, lbl_8047DC44(r2)
    lwz r3, 0xc(r3)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x0(r3)
    b @L_801A834C
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_801A834C
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DC40(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A7F34
    lfs f1, lbl_8047DC40(r2)
    b @L_801A7F48
    @L_801A7F34:
    lfs f0, lbl_8047DC2C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A7F48
    lfs f1, lbl_8047DC2C(r2)
    @L_801A7F48:
    lfs f0, lbl_8047DC44(r2)
    lwz r3, 0xc(r3)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x1(r3)
    b @L_801A834C
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_801A834C
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DC40(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A7F90
    lfs f1, lbl_8047DC40(r2)
    b @L_801A7FA4
    @L_801A7F90:
    lfs f0, lbl_8047DC2C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A7FA4
    lfs f1, lbl_8047DC2C(r2)
    @L_801A7FA4:
    lfs f0, lbl_8047DC44(r2)
    lwz r3, 0xc(r3)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x2(r3)
    b @L_801A834C
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_801A834C
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DC40(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A7FEC
    lfs f1, lbl_8047DC40(r2)
    b @L_801A8000
    @L_801A7FEC:
    lfs f0, lbl_8047DC2C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A8000
    lfs f1, lbl_8047DC2C(r2)
    @L_801A8000:
    lfs f0, lbl_8047DC44(r2)
    lwz r3, 0xc(r3)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x4(r3)
    b @L_801A834C
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_801A834C
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DC40(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A8048
    lfs f1, lbl_8047DC40(r2)
    b @L_801A805C
    @L_801A8048:
    lfs f0, lbl_8047DC2C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A805C
    lfs f1, lbl_8047DC2C(r2)
    @L_801A805C:
    lfs f0, lbl_8047DC44(r2)
    lwz r3, 0xc(r3)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x5(r3)
    b @L_801A834C
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_801A834C
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DC40(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A80A4
    lfs f1, lbl_8047DC40(r2)
    b @L_801A80B8
    @L_801A80A4:
    lfs f0, lbl_8047DC2C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A80B8
    lfs f1, lbl_8047DC2C(r2)
    @L_801A80B8:
    lfs f0, lbl_8047DC44(r2)
    lwz r3, 0xc(r3)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x6(r3)
    b @L_801A834C
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_801A834C
    lfs f2, lbl_8047DC2C(r2)
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DC40(r2)
    fsubs f1, f2, f1
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A8108
    lfs f1, lbl_8047DC40(r2)
    b @L_801A811C
    @L_801A8108:
    lfs f0, lbl_8047DC2C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A811C
    lfs f1, lbl_8047DC2C(r2)
    @L_801A811C:
    lwz r3, 0xc(r3)
    stfs f1, 0xc(r3)
    b @L_801A834C
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_801A834C
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DC40(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A8150
    lfs f1, lbl_8047DC40(r2)
    b @L_801A8164
    @L_801A8150:
    lfs f0, lbl_8047DC2C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A8164
    lfs f1, lbl_8047DC2C(r2)
    @L_801A8164:
    lfs f0, lbl_8047DC44(r2)
    lwz r3, 0xc(r3)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8(r3)
    b @L_801A834C
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_801A834C
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DC40(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A81AC
    lfs f1, lbl_8047DC40(r2)
    b @L_801A81C0
    @L_801A81AC:
    lfs f0, lbl_8047DC2C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A81C0
    lfs f1, lbl_8047DC2C(r2)
    @L_801A81C0:
    lfs f0, lbl_8047DC44(r2)
    lwz r3, 0xc(r3)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x9(r3)
    b @L_801A834C
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_801A834C
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DC40(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A8208
    lfs f1, lbl_8047DC40(r2)
    b @L_801A821C
    @L_801A8208:
    lfs f0, lbl_8047DC2C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A821C
    lfs f1, lbl_8047DC2C(r2)
    @L_801A821C:
    lfs f0, lbl_8047DC44(r2)
    lwz r3, 0xc(r3)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0xa(r3)
    b @L_801A834C
    lwz r0, 0x10(r3)
    cmplwi r0, 0x0
    beq @L_801A834C
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DC40(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A8264
    lfs f1, lbl_8047DC40(r2)
    b @L_801A8278
    @L_801A8264:
    lfs f0, lbl_8047DC2C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A8278
    lfs f1, lbl_8047DC2C(r2)
    @L_801A8278:
    lfs f0, lbl_8047DC44(r2)
    lwz r3, 0x10(r3)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x1(r3)
    b @L_801A834C
    lwz r0, 0x10(r3)
    cmplwi r0, 0x0
    beq @L_801A834C
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DC40(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A82C0
    lfs f1, lbl_8047DC40(r2)
    b @L_801A82D4
    @L_801A82C0:
    lfs f0, lbl_8047DC2C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A82D4
    lfs f1, lbl_8047DC2C(r2)
    @L_801A82D4:
    lfs f0, lbl_8047DC44(r2)
    lwz r3, 0x10(r3)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x2(r3)
    b @L_801A834C
    lwz r0, 0x10(r3)
    cmplwi r0, 0x0
    beq @L_801A834C
    lfs f1, 0x0(r5)
    lfs f0, lbl_8047DC40(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A831C
    lfs f1, lbl_8047DC40(r2)
    b @L_801A8330
    @L_801A831C:
    lfs f0, lbl_8047DC2C(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801A8330
    lfs f1, lbl_8047DC2C(r2)
    @L_801A8330:
    lfs f0, lbl_8047DC44(r2)
    lwz r3, 0x10(r3)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x3(r3)
    @L_801A834C:
    addi r1, r1, 0x10
    blr
