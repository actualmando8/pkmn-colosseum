    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stfd f31, 0x70(r1)
    psq_st f31, 0x78(r1), 0, 0
    stfd f30, 0x60(r1)
    psq_st f30, 0x68(r1), 0, 0
    stfd f29, 0x50(r1)
    psq_st f29, 0x58(r1), 0, 0
    stfd f28, 0x40(r1)
    psq_st f28, 0x48(r1), 0, 0
    stfd f27, 0x30(r1)
    psq_st f27, 0x38(r1), 0, 0
    stmw r26, 0x18(r1)
    lwz r0, lbl_8047A428(r13)
    lis r3, lbl_803A2688@ha
    addi r3, r3, lbl_803A2688@l
    li r4, 0x3
    mr r30, r3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r31, r3
    bl fn_8011F1A0
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8002EAD0
    bl fn_801440A0
    bl fn_80144064
    b @L_8002EAD4
    @L_8002EAD0:
    li r3, 0x1
    @L_8002EAD4:
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8002EBF0
    li r3, 0xd9
    li r4, 0x0
    bl fn_801021F8
    mr r3, r31
    bl fn_8011F4F0
    mr r0, r3
    li r3, 0x32
    mr r4, r0
    bl fn_80132A38
    li r3, 0xd9
    bl fn_80104704
    mr r30, r3
    li r4, 0x10b2
    bl fn_801046C8
    cmplwi r30, 0x0
    mr r30, r3
    beq @L_8002EB3C
    cmplwi r30, 0x0
    beq @L_8002EB3C
    li r4, 0x1
    bl fn_80109220
    li r0, 0x43dd
    stw r0, 0x4c(r30)
    @L_8002EB3C:
    li r3, 0x26
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    lfs f27, lbl_8047B9D4(r2)
    lfd f31, lbl_8047B9E0(r2)
    lis r30, 0x4330
    lfd f29, lbl_8047B9E8(r2)
    lfs f28, lbl_8047B9DC(r2)
    b @L_8002EB9C
    @L_8002EB64:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r30, 0x8(r1)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x14(r1)
    stw r30, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
    @L_8002EB9C:
    fcmpo cr0, f27, f28
    blt @L_8002EB64
    li r3, 0xd9
    bl fn_80104704
    mr r30, r3
    li r4, 0x10b2
    bl fn_801046C8
    cmplwi r30, 0x0
    beq @L_8002EBD8
    cmplwi r3, 0x0
    beq @L_8002EBD8
    li r0, 0x0
    li r4, 0x0
    stw r0, 0x4c(r3)
    bl fn_80109220
    @L_8002EBD8:
    li r3, 0xd9
    li r4, 0x1
    bl fn_801021F8
    li r0, 0x7
    stw r0, lbl_8047A42C(r13)
    b @L_8002EE38
    @L_8002EBF0:
    li r29, 0x0
    lwz r26, lbl_8047A428(r13)
    mr r28, r29
    b @L_8002EC90
    @L_8002EC00:
    clrlwi r0, r28, 16
    cmpw r0, r26
    beq @L_8002EC8C
    mr r3, r30
    mr r4, r28
    bl fn_8012AC08
    mr r27, r3
    bl fn_8011E850
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8002EC8C
    mr r3, r27
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002EC8C
    mr r3, r27
    bl fn_80075FEC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8002EC8C
    mr r3, r27
    bl fn_8011E8DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8002EC8C
    mr r3, r27
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8002EC8C
    li r29, 0x1
    @L_8002EC8C:
    addi r28, r28, 0x1
    @L_8002EC90:
    clrlwi r0, r28, 16
    cmplwi r0, 0x6
    blt @L_8002EC00
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    bne @L_8002EDB4
    li r3, 0xd9
    li r4, 0x0
    bl fn_801021F8
    mr r3, r31
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0xd9
    bl fn_80104704
    mr r30, r3
    li r4, 0x10b2
    bl fn_801046C8
    cmplwi r30, 0x0
    mr r30, r3
    beq @L_8002ED00
    cmplwi r30, 0x0
    beq @L_8002ED00
    li r4, 0x1
    bl fn_80109220
    li r0, 0x44e8
    stw r0, 0x4c(r30)
    @L_8002ED00:
    li r3, 0x26
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    lfs f27, lbl_8047B9D4(r2)
    lfd f31, lbl_8047B9E0(r2)
    lis r30, 0x4330
    lfd f29, lbl_8047B9E8(r2)
    lfs f28, lbl_8047B9DC(r2)
    b @L_8002ED60
    @L_8002ED28:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r30, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0xc(r1)
    stw r30, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
    @L_8002ED60:
    fcmpo cr0, f27, f28
    blt @L_8002ED28
    li r3, 0xd9
    bl fn_80104704
    mr r30, r3
    li r4, 0x10b2
    bl fn_801046C8
    cmplwi r30, 0x0
    beq @L_8002ED9C
    cmplwi r3, 0x0
    beq @L_8002ED9C
    li r0, 0x0
    li r4, 0x0
    stw r0, 0x4c(r3)
    bl fn_80109220
    @L_8002ED9C:
    li r3, 0xd9
    li r4, 0x1
    bl fn_801021F8
    li r0, 0x7
    stw r0, lbl_8047A42C(r13)
    b @L_8002EE38
    @L_8002EDB4:
    lwz r0, lbl_8047A428(r13)
    lfs f27, lbl_8047B9D4(r2)
    stw r0, lbl_8047A420(r13)
    lfd f28, lbl_8047B9E0(r2)
    lis r30, 0x4330
    lfd f30, lbl_8047B9E8(r2)
    lfs f31, lbl_8047B9D0(r2)
    b @L_8002EE0C
    @L_8002EDD4:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r30, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f29, f0, f28
    bl fn_800D3088
    stw r3, 0xc(r1)
    stw r30, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f29
    fadds f27, f27, f0
    @L_8002EE0C:
    fcmpo cr0, f27, f31
    blt @L_8002EDD4
    fmr f1, f31
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0xd9
    bl fn_80102510
    li r0, 0xc
    stw r0, lbl_8047A42C(r13)
    @L_8002EE38:
    psq_l f31, 0x78(r1), 0, 0
    lfd f31, 0x70(r1)
    psq_l f30, 0x68(r1), 0, 0
    lfd f30, 0x60(r1)
    psq_l f29, 0x58(r1), 0, 0
    lfd f29, 0x50(r1)
    psq_l f28, 0x48(r1), 0, 0
    lfd f28, 0x40(r1)
    psq_l f27, 0x38(r1), 0, 0
    lfd f27, 0x30(r1)
    lmw r26, 0x18(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
