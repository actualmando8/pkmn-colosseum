    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r31, r3
    fmr f31, f1
    bl fn_801D1E50
    cmpwi r3, 0x0
    blt @L_801D27A0
    li r3, 0x0
    b @L_801D29B4
@L_801D27A0:
    mr r3, r31
    bl fn_801D15D0
    rlwinm r3, r3, 0, 16, 31
    cmplwi r3, 0xffff
    bne @L_801D27BC
    li r3, 0x0
    b @L_801D29B4
@L_801D27BC:
    lis r0, 0x4330
    stw r3, 0xc(r1)
    lfd f2, -21712(r2)
    stw r0, 0x8(r1)
    lfs f3, -21720(r2)
    lfd f1, 0x8(r1)
    lfs f0, -21716(r2)
    fsubs f1, f1, f2
    fmuls f1, f3, f1
    fdivs f0, f1, f0
    fcmpo cr0, f31, f0
    cror eq, gt, eq
    bne @L_801D27F8
    li r3, 0x0
    b @L_801D29B4
@L_801D27F8:
    mr r3, r31
    bl fn_801D142C
    mr r29, r3
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0xff
    bne @L_801D2818
    li r3, 0x0
    b @L_801D29B4
@L_801D2818:
    mr r3, r31
    bl fn_801D13E4
    mr r30, r3
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0xffff
    bne @L_801D2838
    li r3, 0x0
    b @L_801D29B4
@L_801D2838:
    bl fn_801D1338
    rlwinm r0, r29, 0, 24, 31
    li r5, 0x0
    cmplwi r0, 0x6
    bgt @L_801D28F8
    lis r4, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r4, r4, -7888
    lwzx r0, r4, r0
    mtctr r0
    bctr
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r30, 0, 16, 31
    cmplw r3, r0
    bne @L_801D28F8
    li r5, 0x1
    b @L_801D28F8
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r30, 0, 16, 31
    cmplw r3, r0
    ble @L_801D28F8
    li r5, 0x1
    b @L_801D28F8
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r30, 0, 16, 31
    cmplw r3, r0
    bge @L_801D28F8
    li r5, 0x1
    b @L_801D28F8
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r30, 0, 16, 31
    cmplw r3, r0
    blt @L_801D28F8
    li r5, 0x1
    b @L_801D28F8
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r30, 0, 16, 31
    cmplw r3, r0
    bgt @L_801D28F8
    li r5, 0x1
    b @L_801D28F8
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r30, 0, 16, 31
    cmplw r3, r0
    beq @L_801D28F8
    li r5, 0x1
    b @L_801D28F8
    li r5, 0x1
@L_801D28F8:
    cmpwi r5, 0x0
    bne @L_801D2908
    li r3, 0x0
    b @L_801D29B4
@L_801D2908:
    mr r3, r31
    bl fn_801D158C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0xff
    bne @L_801D2924
    li r3, 0x0
    b @L_801D29B4
@L_801D2924:
    cmpwi r0, 0x1
    li r29, 0x0
    beq @L_801D2954
    bge @L_801D2940
    cmpwi r0, 0x0
    bge @L_801D294C
    b @L_801D29B0
@L_801D2940:
    cmpwi r0, 0x3
    bge @L_801D29B0
    b @L_801D297C
@L_801D294C:
    li r29, 0x1
    b @L_801D29B0
@L_801D2954:
    mr r3, r31
    bl fn_801D1548
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_801D29B0
    bl fn_801906A0
    cmplwi r3, 0x0
    beq @L_801D29B0
    li r29, 0x1
    b @L_801D29B0
@L_801D297C:
    mr r3, r31
    bl fn_801D1548
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_801D29B0
    cmplwi r3, 0x0
    beq @L_801D29B0
    li r4, 0x0
    crxor 6, 6, 6
    bl fn_800F7434
    cmpwi r3, 0x0
    beq @L_801D29B0
    li r29, 0x1
@L_801D29B0:
    mr r3, r29
@L_801D29B4:
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
