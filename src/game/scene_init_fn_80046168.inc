    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    li r30, 0x1
    li r7, 0x0
    lis r3, lbl_802E52C8@ha
    lfd f3, lbl_8047BCB0(r2)
    lfs f1, lbl_8047BD1C(r2)
    addi r5, r3, lbl_802E52C8@l
    lfs f6, lbl_8047BC94(r2)
    lis r4, 0x4330
    lfs f5, lbl_8047BCA0(r2)
    lfs f4, lbl_8047BD18(r2)
    @L_800461A0:
    lwz r3, lbl_8047A4EC(r13)
    addi r0, r3, 0x1
    cmplwi r0, 0x8
    stw r0, lbl_8047A4EC(r13)
    blt @L_800461BC
    li r0, 0x0
    stw r0, lbl_8047A4EC(r13)
    @L_800461BC:
    lwz r8, lbl_8047A4EC(r13)
    li r6, 0x0
    li r3, 0x1
    mulli r0, r8, 0x24
    subi r8, r8, 0x1
    add r10, r5, r0
    stw r6, 0x0(r10)
    mulli r0, r8, 0x24
    stw r6, 0x8(r10)
    add r9, r5, r0
    stfs f6, 0xc(r10)
    stfs f6, 0x14(r10)
    stw r6, 0x10(r10)
    stw r3, 0x18(r10)
    stfs f5, 0x1c(r10)
    stfs f4, 0x20(r10)
    cmpwi r8, 0x0
    addi r3, r8, 0x1
    blt @L_8004636C
    srwi r0, r3, 2
    cmplwi r0, 0x0
    mtctr r0
    beq @L_80046324
    @L_80046218:
    lwz r0, 0x10(r9)
    stw r4, 0x8(r1)
    xoris r0, r0, 0x8000
    lfs f0, 0x4(r9)
    stw r0, 0xc(r1)
    lfd f2, 0x8(r1)
    fsubs f2, f2, f3
    stw r4, 0x8(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    stw r0, 0x10(r9)
    lfs f0, 0x20(r9)
    fadds f0, f0, f1
    stfs f0, 0x20(r9)
    subi r9, r9, 0x24
    lwz r0, 0x10(r9)
    lfs f0, 0x4(r9)
    xoris r0, r0, 0x8000
    stw r0, 0xc(r1)
    lfd f2, 0x8(r1)
    fsubs f2, f2, f3
    stw r4, 0x8(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    stw r0, 0x10(r9)
    lfs f0, 0x20(r9)
    fadds f0, f0, f1
    stfs f0, 0x20(r9)
    subi r9, r9, 0x24
    lwz r0, 0x10(r9)
    lfs f0, 0x4(r9)
    xoris r0, r0, 0x8000
    stw r0, 0xc(r1)
    lfd f2, 0x8(r1)
    fsubs f2, f2, f3
    stw r4, 0x8(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    stw r0, 0x10(r9)
    lfs f0, 0x20(r9)
    fadds f0, f0, f1
    stfs f0, 0x20(r9)
    subi r9, r9, 0x24
    lwz r0, 0x10(r9)
    lfs f0, 0x4(r9)
    xoris r0, r0, 0x8000
    stw r0, 0xc(r1)
    lfd f2, 0x8(r1)
    fsubs f2, f2, f3
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    stw r0, 0x10(r9)
    lfs f0, 0x20(r9)
    fadds f0, f0, f1
    stfs f0, 0x20(r9)
    subi r9, r9, 0x24
    bdnz @L_80046218
    andi. r3, r3, 0x3
    beq @L_8004636C
    @L_80046324:
    mtctr r3
    @L_80046328:
    lwz r0, 0x10(r9)
    stw r4, 0x8(r1)
    xoris r0, r0, 0x8000
    lfs f0, 0x4(r9)
    stw r0, 0xc(r1)
    lfd f2, 0x8(r1)
    fsubs f2, f2, f3
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    stw r0, 0x10(r9)
    lfs f0, 0x20(r9)
    fadds f0, f0, f1
    stfs f0, 0x20(r9)
    subi r9, r9, 0x24
    bdnz @L_80046328
    @L_8004636C:
    lwz r0, lbl_8047A4EC(r13)
    addi r6, r5, 0xfc
    cmplwi r0, 0x7
    subfic r3, r0, 0x7
    bge @L_800464E4
    srwi r0, r3, 2
    cmplwi r0, 0x0
    mtctr r0
    beq @L_8004649C
    @L_80046390:
    lwz r0, 0x10(r6)
    stw r4, 0x10(r1)
    xoris r0, r0, 0x8000
    lfs f0, 0x4(r6)
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    stw r4, 0x10(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r6)
    lfs f0, 0x20(r6)
    fadds f0, f0, f1
    stfs f0, 0x20(r6)
    subi r6, r6, 0x24
    lwz r0, 0x10(r6)
    lfs f0, 0x4(r6)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    stw r4, 0x10(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r6)
    lfs f0, 0x20(r6)
    fadds f0, f0, f1
    stfs f0, 0x20(r6)
    subi r6, r6, 0x24
    lwz r0, 0x10(r6)
    lfs f0, 0x4(r6)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    stw r4, 0x10(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r6)
    lfs f0, 0x20(r6)
    fadds f0, f0, f1
    stfs f0, 0x20(r6)
    subi r6, r6, 0x24
    lwz r0, 0x10(r6)
    lfs f0, 0x4(r6)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r6)
    lfs f0, 0x20(r6)
    fadds f0, f0, f1
    stfs f0, 0x20(r6)
    subi r6, r6, 0x24
    bdnz @L_80046390
    andi. r3, r3, 0x3
    beq @L_800464E4
    @L_8004649C:
    mtctr r3
    @L_800464A0:
    lwz r0, 0x10(r6)
    stw r4, 0x10(r1)
    xoris r0, r0, 0x8000
    lfs f0, 0x4(r6)
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r6)
    lfs f0, 0x20(r6)
    fadds f0, f0, f1
    stfs f0, 0x20(r6)
    subi r6, r6, 0x24
    bdnz @L_800464A0
    @L_800464E4:
    addi r7, r7, 0x1
    cmpwi r7, 0x8
    blt @L_800461A0
    lis r3, lbl_803A6818@ha
    addi r31, r3, lbl_803A6818@l
    @L_800464F8:
    lwz r0, 0x20(r31)
    cmpwi r0, 0x1
    beq @L_80046B1C
    bge @L_800472A8
    cmpwi r0, 0x0
    bge @L_80046514
    b @L_800472A8
    @L_80046514:
    li r3, 0x3c
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x3f
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x3d
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x9f
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x96
    li r4, 0x0
    bl fn_8010264C
    li r3, 0xa2
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, 0x0
    blt @L_80046AE8
    lis r3, lbl_803A6818@ha
    lwz r4, lbl_8047A4E4(r13)
    addi r3, r3, lbl_803A6818@l
    lwz r0, 0x0(r3)
    slwi r0, r0, 1
    lhzx r0, r4, r0
    rlwinm r0, r0, 0, 16, 16
    cmpwi r0, 0x0
    beq @L_80046590
    li r0, 0x0
    b @L_80046594
    @L_80046590:
    li r0, 0x1
    @L_80046594:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_800472A8
    li r3, 0x3e
    li r4, 0x0
    bl fn_8010264C
    lwz r27, lbl_8047A4E0(r13)
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    cmplwi r27, 0x0
    lwz r0, 0x0(r3)
    beq @L_8004667C
    beq @L_80046640
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r28, r3, r0
    cmplwi r28, 0x8000
    blt @L_800465E0
    clrlwi r28, r28, 18
    @L_800465E0:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r27
    clrlwi r4, r28, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r28
    li r3, 0x0
    bl fn_8025FDDC
    mr r29, r3
    mr r4, r28
    li r3, 0x0
    bl fn_8025FD34
    mr r28, r3
    mr r3, r27
    mr r4, r29
    bl fn_8011DFE0
    mr r3, r27
    mr r4, r28
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_80046644
    @L_80046640:
    li r3, 0x0
    @L_80046644:
    cmplwi r3, 0x0
    beq @L_80046670
    bl fn_8011F5C8
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_80046664
    bl fn_8011E15C
    b @L_80046684
    @L_80046664:
    lis r3, 0x1
    subi r3, r3, 0x1
    b @L_80046684
    @L_80046670:
    lis r3, 0x1
    subi r3, r3, 0x1
    b @L_80046684
    @L_8004667C:
    lis r3, 0x1
    subi r3, r3, 0x1
    @L_80046684:
    clrlwi r29, r3, 16
    li r4, 0x0
    mr r3, r29
    li r5, 0x0
    bl fn_80166AB8
    lwz r26, lbl_8047A4E0(r13)
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    cmplwi r26, 0x0
    lwz r0, 0x0(r3)
    beq @L_80046760
    beq @L_8004672C
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r28, r3, r0
    cmplwi r28, 0x8000
    blt @L_800466CC
    clrlwi r28, r28, 18
    @L_800466CC:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r26
    clrlwi r4, r28, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r28
    li r3, 0x0
    bl fn_8025FDDC
    mr r27, r3
    mr r4, r28
    li r3, 0x0
    bl fn_8025FD34
    mr r28, r3
    mr r3, r26
    mr r4, r27
    bl fn_8011DFE0
    mr r3, r26
    mr r4, r28
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_80046730
    @L_8004672C:
    li r3, 0x0
    @L_80046730:
    cmplwi r3, 0x0
    beq @L_80046758
    bl fn_8011F5C8
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_80046750
    bl fn_8011CA60
    b @L_80046764
    @L_80046750:
    li r3, 0x0
    b @L_80046764
    @L_80046758:
    li r3, 0x0
    b @L_80046764
    @L_80046760:
    li r3, 0x0
    @L_80046764:
    lwz r4, lbl_8047A4EC(r13)
    addi r0, r4, 0x1
    cmplwi r0, 0x8
    stw r0, lbl_8047A4EC(r13)
    blt @L_80046780
    li r0, 0x0
    stw r0, lbl_8047A4EC(r13)
    @L_80046780:
    lwz r5, lbl_8047A4EC(r13)
    lis r4, lbl_802E52C8@ha
    addi r6, r4, lbl_802E52C8@l
    lfs f2, lbl_8047BC94(r2)
    mulli r0, r5, 0x24
    subi r7, r5, 0x1
    li r5, 0x0
    lfs f1, lbl_8047BCA0(r2)
    li r4, 0x1
    lfs f0, lbl_8047BD18(r2)
    add r8, r6, r0
    stw r3, 0x0(r8)
    mulli r0, r7, 0x24
    stw r29, 0x8(r8)
    add r6, r6, r0
    stfs f2, 0xc(r8)
    stfs f2, 0x14(r8)
    stw r5, 0x10(r8)
    stw r4, 0x18(r8)
    stfs f1, 0x1c(r8)
    stfs f0, 0x20(r8)
    cmpwi r7, 0x0
    lfd f3, lbl_8047BCB0(r2)
    lfs f1, lbl_8047BD1C(r2)
    addi r3, r7, 0x1
    lis r4, 0x4330
    blt @L_80046950
    srwi r0, r3, 2
    cmplwi r0, 0x0
    mtctr r0
    beq @L_80046908
    @L_800467FC:
    lwz r0, 0x10(r6)
    stw r4, 0x10(r1)
    xoris r0, r0, 0x8000
    lfs f0, 0x4(r6)
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    stw r4, 0x10(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r6)
    lfs f0, 0x20(r6)
    fadds f0, f0, f1
    stfs f0, 0x20(r6)
    subi r6, r6, 0x24
    lwz r0, 0x10(r6)
    lfs f0, 0x4(r6)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    stw r4, 0x10(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r6)
    lfs f0, 0x20(r6)
    fadds f0, f0, f1
    stfs f0, 0x20(r6)
    subi r6, r6, 0x24
    lwz r0, 0x10(r6)
    lfs f0, 0x4(r6)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    stw r4, 0x10(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r6)
    lfs f0, 0x20(r6)
    fadds f0, f0, f1
    stfs f0, 0x20(r6)
    subi r6, r6, 0x24
    lwz r0, 0x10(r6)
    lfs f0, 0x4(r6)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r6)
    lfs f0, 0x20(r6)
    fadds f0, f0, f1
    stfs f0, 0x20(r6)
    subi r6, r6, 0x24
    bdnz @L_800467FC
    andi. r3, r3, 0x3
    beq @L_80046950
    @L_80046908:
    mtctr r3
    @L_8004690C:
    lwz r0, 0x10(r6)
    stw r4, 0x10(r1)
    xoris r0, r0, 0x8000
    lfs f0, 0x4(r6)
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r6)
    lfs f0, 0x20(r6)
    fadds f0, f0, f1
    stfs f0, 0x20(r6)
    subi r6, r6, 0x24
    bdnz @L_8004690C
    @L_80046950:
    lis r3, lbl_802E52C8@ha
    lwz r0, lbl_8047A4EC(r13)
    addi r3, r3, lbl_802E52C8@l
    addi r5, r3, 0xfc
    cmplwi r0, 0x7
    lfd f3, lbl_8047BCB0(r2)
    lfs f1, lbl_8047BD1C(r2)
    subfic r3, r0, 0x7
    lis r4, 0x4330
    bge @L_80046ADC
    srwi r0, r3, 2
    cmplwi r0, 0x0
    mtctr r0
    beq @L_80046A94
    @L_80046988:
    lwz r0, 0x10(r5)
    stw r4, 0x10(r1)
    xoris r0, r0, 0x8000
    lfs f0, 0x4(r5)
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    stw r4, 0x10(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r5)
    lfs f0, 0x20(r5)
    fadds f0, f0, f1
    stfs f0, 0x20(r5)
    subi r5, r5, 0x24
    lwz r0, 0x10(r5)
    lfs f0, 0x4(r5)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    stw r4, 0x10(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r5)
    lfs f0, 0x20(r5)
    fadds f0, f0, f1
    stfs f0, 0x20(r5)
    subi r5, r5, 0x24
    lwz r0, 0x10(r5)
    lfs f0, 0x4(r5)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    stw r4, 0x10(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r5)
    lfs f0, 0x20(r5)
    fadds f0, f0, f1
    stfs f0, 0x20(r5)
    subi r5, r5, 0x24
    lwz r0, 0x10(r5)
    lfs f0, 0x4(r5)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r5)
    lfs f0, 0x20(r5)
    fadds f0, f0, f1
    stfs f0, 0x20(r5)
    subi r5, r5, 0x24
    bdnz @L_80046988
    andi. r3, r3, 0x3
    beq @L_80046ADC
    @L_80046A94:
    mtctr r3
    @L_80046A98:
    lwz r0, 0x10(r5)
    stw r4, 0x10(r1)
    xoris r0, r0, 0x8000
    lfs f0, 0x4(r5)
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r5)
    lfs f0, 0x20(r5)
    fadds f0, f0, f1
    stfs f0, 0x20(r5)
    subi r5, r5, 0x24
    bdnz @L_80046A98
    @L_80046ADC:
    li r0, 0x1
    stw r0, 0x20(r31)
    b @L_800472A8
    @L_80046AE8:
    lwz r0, 0x1c(r31)
    cmpwi r0, 0x2
    bne @L_80046B0C
    lfs f1, lbl_8047BC94(r2)
    li r0, 0x8
    lfs f0, lbl_8047BCBC(r2)
    stw r0, 0x1c(r31)
    stfs f1, 0x50(r31)
    stfs f0, 0x58(r31)
    @L_80046B0C:
    li r0, 0x0
    li r30, 0x0
    stw r0, 0x14(r31)
    b @L_800472A8
    @L_80046B1C:
    lis r3, lbl_802E52C8@ha
    li r29, 0x0
    addi r27, r3, lbl_802E52C8@l
    mr r28, r29
    @L_80046B2C:
    lwz r0, 0x0(r27)
    cmplwi r0, 0x0
    beq @L_80046B60
    lwz r3, 0x8(r27)
    bl fn_801666BC
    cmpwi r3, 0x2
    beq @L_80046B5C
    bge @L_80046B54
    cmpwi r3, 0x0
    b @L_80046B60
    @L_80046B54:
    cmpwi r3, 0x4
    b @L_80046B60
    @L_80046B5C:
    li r29, 0x1
    @L_80046B60:
    addi r27, r27, 0x24
    addi r28, r28, 0x1
    cmpwi r28, 0x8
    blt @L_80046B2C
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    beq @L_80046CC4
    li r3, 0x1
    bl fn_800F7AF0
    mr r29, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r29
    rlwinm r0, r0, 0, 28, 28
    cmplwi r0, 0x0
    beq @L_80046BA8
    li r0, 0x1
    b @L_80046CB8
    @L_80046BA8:
    li r3, 0x1
    bl fn_800F7AF0
    mr r29, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r29
    rlwinm r0, r0, 0, 29, 29
    cmplwi r0, 0x0
    beq @L_80046BD4
    li r0, 0x1
    b @L_80046CB8
    @L_80046BD4:
    li r3, 0x1
    li r4, 0x0
    bl fn_800F7A08
    mr r29, r3
    li r3, 0x1
    li r4, 0x0
    bl fn_800F7A7C
    extsb r0, r3
    cmpwi r0, 0x0
    bge @L_80046C00
    neg r0, r0
    @L_80046C00:
    cmpwi r0, 0x20
    bgt @L_80046C20
    extsb r0, r29
    cmpwi r0, 0x0
    bge @L_80046C18
    neg r0, r0
    @L_80046C18:
    cmpwi r0, 0x20
    ble @L_80046CB4
    @L_80046C20:
    extsb r4, r3
    extsb r0, r29
    lis r3, 0x4330
    lfd f2, lbl_8047BCB0(r2)
    xoris r4, r4, 0x8000
    xoris r0, r0, 0x8000
    stw r4, 0x14(r1)
    stw r3, 0x10(r1)
    lfd f0, 0x10(r1)
    stw r0, 0xc(r1)
    fsubs f1, f0, f2
    stw r3, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f2, f0, f2
    bl fn_800CE2D8
    frsp f2, f1
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f2, f0
    ble @L_80046C74
    fmr f1, f2
    b @L_80046C78
    @L_80046C74:
    fneg f1, f2
    @L_80046C78:
    lfs f0, lbl_8047BCFC(r2)
    fcmpo cr0, f1, f0
    bge @L_80046C8C
    li r0, 0x1
    b @L_80046CB8
    @L_80046C8C:
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f2, f0
    ble @L_80046C9C
    b @L_80046CA0
    @L_80046C9C:
    fneg f2, f2
    @L_80046CA0:
    lfs f0, lbl_8047BD00(r2)
    fcmpo cr0, f2, f0
    ble @L_80046CB4
    li r0, 0x1
    b @L_80046CB8
    @L_80046CB4:
    li r0, 0x0
    @L_80046CB8:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_80046CE0
    @L_80046CC4:
    li r3, 0x3e
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r0, 0x0
    stw r0, 0x20(r31)
    b @L_800472A4
    @L_80046CE0:
    li r3, 0x1
    bl fn_800F7AF0
    mr r29, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r29
    rlwinm r0, r0, 0, 23, 23
    cmplwi r0, 0x0
    beq @L_80047238
    lwz r26, lbl_8047A4E0(r13)
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    cmplwi r26, 0x0
    lwz r0, 0x0(r3)
    beq @L_80046DD4
    beq @L_80046D98
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r28, r3, r0
    cmplwi r28, 0x8000
    blt @L_80046D38
    clrlwi r28, r28, 18
    @L_80046D38:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r26
    clrlwi r4, r28, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r28
    li r3, 0x0
    bl fn_8025FDDC
    mr r29, r3
    mr r4, r28
    li r3, 0x0
    bl fn_8025FD34
    mr r28, r3
    mr r3, r26
    mr r4, r29
    bl fn_8011DFE0
    mr r3, r26
    mr r4, r28
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_80046D9C
    @L_80046D98:
    li r3, 0x0
    @L_80046D9C:
    cmplwi r3, 0x0
    beq @L_80046DC8
    bl fn_8011F5C8
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_80046DBC
    bl fn_8011E15C
    b @L_80046DDC
    @L_80046DBC:
    lis r3, 0x1
    subi r3, r3, 0x1
    b @L_80046DDC
    @L_80046DC8:
    lis r3, 0x1
    subi r3, r3, 0x1
    b @L_80046DDC
    @L_80046DD4:
    lis r3, 0x1
    subi r3, r3, 0x1
    @L_80046DDC:
    clrlwi r29, r3, 16
    li r4, 0x0
    mr r3, r29
    li r5, 0x0
    bl fn_80166AB8
    lwz r26, lbl_8047A4E0(r13)
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    cmplwi r26, 0x0
    lwz r0, 0x0(r3)
    beq @L_80046EB8
    beq @L_80046E84
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r28, r3, r0
    cmplwi r28, 0x8000
    blt @L_80046E24
    clrlwi r28, r28, 18
    @L_80046E24:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r26
    clrlwi r4, r28, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r28
    li r3, 0x0
    bl fn_8025FDDC
    mr r4, r28
    mr r28, r3
    li r3, 0x0
    bl fn_8025FD34
    mr r27, r3
    mr r3, r26
    mr r4, r28
    bl fn_8011DFE0
    mr r3, r26
    mr r4, r27
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_80046E88
    @L_80046E84:
    li r3, 0x0
    @L_80046E88:
    cmplwi r3, 0x0
    beq @L_80046EB0
    bl fn_8011F5C8
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_80046EA8
    bl fn_8011CA60
    b @L_80046EBC
    @L_80046EA8:
    li r3, 0x0
    b @L_80046EBC
    @L_80046EB0:
    li r3, 0x0
    b @L_80046EBC
    @L_80046EB8:
    li r3, 0x0
    @L_80046EBC:
    lwz r4, lbl_8047A4EC(r13)
    addi r0, r4, 0x1
    cmplwi r0, 0x8
    stw r0, lbl_8047A4EC(r13)
    blt @L_80046ED8
    li r0, 0x0
    stw r0, lbl_8047A4EC(r13)
    @L_80046ED8:
    lwz r5, lbl_8047A4EC(r13)
    lis r4, lbl_802E52C8@ha
    addi r6, r4, lbl_802E52C8@l
    lfs f2, lbl_8047BC94(r2)
    mulli r0, r5, 0x24
    subi r7, r5, 0x1
    li r5, 0x0
    lfs f1, lbl_8047BCA0(r2)
    li r4, 0x1
    lfs f0, lbl_8047BD18(r2)
    add r8, r6, r0
    stw r3, 0x0(r8)
    mulli r0, r7, 0x24
    stw r29, 0x8(r8)
    add r6, r6, r0
    stfs f2, 0xc(r8)
    stfs f2, 0x14(r8)
    stw r5, 0x10(r8)
    stw r4, 0x18(r8)
    stfs f1, 0x1c(r8)
    stfs f0, 0x20(r8)
    cmpwi r7, 0x0
    lfd f3, lbl_8047BCB0(r2)
    lfs f1, lbl_8047BD1C(r2)
    addi r3, r7, 0x1
    lis r4, 0x4330
    blt @L_800470A8
    srwi r0, r3, 2
    cmplwi r0, 0x0
    mtctr r0
    beq @L_80047060
    @L_80046F54:
    lwz r0, 0x10(r6)
    stw r4, 0x10(r1)
    xoris r0, r0, 0x8000
    lfs f0, 0x4(r6)
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    stw r4, 0x10(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r6)
    lfs f0, 0x20(r6)
    fadds f0, f0, f1
    stfs f0, 0x20(r6)
    subi r6, r6, 0x24
    lwz r0, 0x10(r6)
    lfs f0, 0x4(r6)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    stw r4, 0x10(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r6)
    lfs f0, 0x20(r6)
    fadds f0, f0, f1
    stfs f0, 0x20(r6)
    subi r6, r6, 0x24
    lwz r0, 0x10(r6)
    lfs f0, 0x4(r6)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    stw r4, 0x10(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r6)
    lfs f0, 0x20(r6)
    fadds f0, f0, f1
    stfs f0, 0x20(r6)
    subi r6, r6, 0x24
    lwz r0, 0x10(r6)
    lfs f0, 0x4(r6)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r6)
    lfs f0, 0x20(r6)
    fadds f0, f0, f1
    stfs f0, 0x20(r6)
    subi r6, r6, 0x24
    bdnz @L_80046F54
    andi. r3, r3, 0x3
    beq @L_800470A8
    @L_80047060:
    mtctr r3
    @L_80047064:
    lwz r0, 0x10(r6)
    stw r4, 0x10(r1)
    xoris r0, r0, 0x8000
    lfs f0, 0x4(r6)
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r6)
    lfs f0, 0x20(r6)
    fadds f0, f0, f1
    stfs f0, 0x20(r6)
    subi r6, r6, 0x24
    bdnz @L_80047064
    @L_800470A8:
    lis r3, lbl_802E52C8@ha
    lwz r0, lbl_8047A4EC(r13)
    addi r3, r3, lbl_802E52C8@l
    addi r5, r3, 0xfc
    cmplwi r0, 0x7
    lfd f3, lbl_8047BCB0(r2)
    lfs f1, lbl_8047BD1C(r2)
    subfic r3, r0, 0x7
    lis r4, 0x4330
    bge @L_800472A4
    srwi r0, r3, 2
    cmplwi r0, 0x0
    mtctr r0
    beq @L_800471EC
    @L_800470E0:
    lwz r0, 0x10(r5)
    stw r4, 0x10(r1)
    xoris r0, r0, 0x8000
    lfs f0, 0x4(r5)
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    stw r4, 0x10(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r5)
    lfs f0, 0x20(r5)
    fadds f0, f0, f1
    stfs f0, 0x20(r5)
    subi r5, r5, 0x24
    lwz r0, 0x10(r5)
    lfs f0, 0x4(r5)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    stw r4, 0x10(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r5)
    lfs f0, 0x20(r5)
    fadds f0, f0, f1
    stfs f0, 0x20(r5)
    subi r5, r5, 0x24
    lwz r0, 0x10(r5)
    lfs f0, 0x4(r5)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    stw r4, 0x10(r1)
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r5)
    lfs f0, 0x20(r5)
    fadds f0, f0, f1
    stfs f0, 0x20(r5)
    subi r5, r5, 0x24
    lwz r0, 0x10(r5)
    lfs f0, 0x4(r5)
    xoris r0, r0, 0x8000
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r5)
    lfs f0, 0x20(r5)
    fadds f0, f0, f1
    stfs f0, 0x20(r5)
    subi r5, r5, 0x24
    bdnz @L_800470E0
    andi. r3, r3, 0x3
    beq @L_800472A4
    @L_800471EC:
    mtctr r3
    @L_800471F0:
    lwz r0, 0x10(r5)
    stw r4, 0x10(r1)
    xoris r0, r0, 0x8000
    lfs f0, 0x4(r5)
    stw r0, 0x14(r1)
    lfd f2, 0x10(r1)
    fsubs f2, f2, f3
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x10(r5)
    lfs f0, 0x20(r5)
    fadds f0, f0, f1
    stfs f0, 0x20(r5)
    subi r5, r5, 0x24
    bdnz @L_800471F0
    b @L_800472A4
    @L_80047238:
    li r3, 0x1
    bl fn_800F7AF0
    mr r29, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r29
    rlwinm r0, r0, 0, 22, 22
    cmplwi r0, 0x0
    beq @L_800472A4
    lwz r0, 0x1c(r31)
    cmpwi r0, 0x2
    bne @L_800472A0
    li r0, 0x8
    li r3, 0x3e
    stw r0, 0x1c(r31)
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x3c
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lfs f1, lbl_8047BC94(r2)
    lfs f0, lbl_8047BCBC(r2)
    stfs f1, 0x50(r31)
    stfs f0, 0x58(r31)
    @L_800472A0:
    li r30, 0x0
    @L_800472A4:
    bl fn_800F0308
    @L_800472A8:
    cmpwi r30, 0x0
    bne @L_800464F8
    li r0, 0x0
    stw r0, 0x20(r31)
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
