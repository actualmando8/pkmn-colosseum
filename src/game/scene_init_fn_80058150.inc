    nofralloc
    stwu r1, -0x1f0(r1)
    mflr r0
    stw r0, 0x1f4(r1)
    stmw r25, 0x1d4(r1)
    li r3, 0x0
    bl fn_801CAC6C
    li r3, 0x70b
    bl fn_8017B3E4
    b @L_80058178
    @L_80058174:
    bl fn_800F0308
    @L_80058178:
    li r3, 0x70b
    bl fn_8017B2CC
    cmpwi r3, 0x1
    beq @L_80058174
    li r25, 0x0
    mr r26, r25
    @L_80058190:
    clrlwi r5, r26, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    cmplwi r3, 0x0
    beq @L_800581BC
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800581BC
    addi r25, r25, 0x1
    @L_800581BC:
    addi r26, r26, 0x1
    cmpwi r26, 0x6
    blt @L_80058190
    li r27, 0x0
    b @L_8005821C
    @L_800581D0:
    li r26, 0x0
    b @L_80058208
    @L_800581D8:
    extsb r4, r27
    extsb r5, r26
    li r3, 0x0
    bl fn_80134EF0
    cmplwi r3, 0x0
    beq @L_80058204
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80058204
    addi r25, r25, 0x1
    @L_80058204:
    addi r26, r26, 0x1
    @L_80058208:
    bl fn_801347D8
    extsb r0, r3
    cmpw r26, r0
    blt @L_800581D8
    addi r27, r27, 0x1
    @L_8005821C:
    bl fn_801347E0
    extsb r0, r3
    cmpw r27, r0
    blt @L_800581D0
    addi r30, r25, 0x1
    cmpwi r30, 0x0
    ble @L_8005859C
    slwi r3, r30, 2
    bl fn_800E3534
    mr r29, r3
    bl fn_800E27B0
    cmplwi r3, 0x0
    stw r3, lbl_8047A590(r13)
    mr r31, r3
    beq @L_800585A4
    li r27, 0x0
    @L_8005825C:
    clrlwi r5, r27, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r28, r3
    cmplwi r28, 0x0
    beq @L_800583D4
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800583D4
    mr r3, r28
    bl fn_8011F5C8
    mr r25, r3
    clrlwi r0, r25, 16
    cmplwi r0, 0x0
    bne @L_800582A8
    li r3, -0x1
    b @L_800583C0
    @L_800582A8:
    cmplwi r0, 0xc9
    bne @L_80058374
    lis r3, lbl_802676F0@ha
    li r0, 0x1c
    addi r3, r3, lbl_802676F0@l
    addi r5, r1, 0xe4
    subi r4, r3, 0x4
    mtctr r0
    @L_800582C8:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_800582C8
    mr r3, r28
    li r4, 0x0
    li r5, 0x6f
    li r6, 0x0
    bl fn_8012640C
    bl fn_80122334
    mr r25, r3
    clrlwi r0, r25, 24
    cmplwi r0, 0x1c
    blt @L_8005830C
    li r3, -0x1
    b @L_800583C0
    @L_8005830C:
    mr r3, r28
    bl fn_8011F5C8
    mr r4, r3
    clrlwi r0, r4, 16
    cmplwi r0, 0x0
    bne @L_8005832C
    li r3, -0x1
    b @L_800583C0
    @L_8005832C:
    mr r3, r28
    li r5, 0xc1
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80058360
    clrlwi r0, r25, 24
    addi r3, r1, 0xe8
    slwi r4, r0, 3
    addi r0, r4, 0x4
    lwzx r3, r3, r0
    b @L_800583C0
    @L_80058360:
    clrlwi r0, r25, 24
    addi r3, r1, 0xe8
    slwi r0, r0, 3
    lwzx r3, r3, r0
    b @L_800583C0
    @L_80058374:
    mr r3, r28
    mr r4, r25
    li r5, 0xc1
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    mr r4, r25
    cmplwi r0, 0x0
    li r3, 0x0
    li r5, 0x5a
    beq @L_800583A8
    li r0, 0x1
    b @L_800583AC
    @L_800583A8:
    li r0, 0x0
    @L_800583AC:
    clrlwi r6, r0, 16
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_800583C0
    li r3, -0x1
    @L_800583C0:
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_800583D4
    stw r3, 0x0(r31)
    addi r31, r31, 0x4
    @L_800583D4:
    addi r27, r27, 0x1
    cmpwi r27, 0x6
    blt @L_8005825C
    li r28, 0x0
    b @L_80058580
    @L_800583E8:
    li r27, 0x0
    b @L_8005856C
    @L_800583F0:
    extsb r4, r28
    extsb r5, r27
    li r3, 0x0
    bl fn_80134EF0
    mr r26, r3
    cmplwi r26, 0x0
    beq @L_80058568
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80058568
    mr r3, r26
    bl fn_8011F5C8
    mr r25, r3
    clrlwi r0, r25, 16
    cmplwi r0, 0x0
    bne @L_8005843C
    li r3, -0x1
    b @L_80058554
    @L_8005843C:
    cmplwi r0, 0xc9
    bne @L_80058508
    lis r3, lbl_802676F0@ha
    li r0, 0x1c
    addi r3, r3, lbl_802676F0@l
    addi r5, r1, 0x4
    subi r4, r3, 0x4
    mtctr r0
    @L_8005845C:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_8005845C
    mr r3, r26
    li r4, 0x0
    li r5, 0x6f
    li r6, 0x0
    bl fn_8012640C
    bl fn_80122334
    mr r25, r3
    clrlwi r0, r25, 24
    cmplwi r0, 0x1c
    blt @L_800584A0
    li r3, -0x1
    b @L_80058554
    @L_800584A0:
    mr r3, r26
    bl fn_8011F5C8
    mr r4, r3
    clrlwi r0, r4, 16
    cmplwi r0, 0x0
    bne @L_800584C0
    li r3, -0x1
    b @L_80058554
    @L_800584C0:
    mr r3, r26
    li r5, 0xc1
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800584F4
    clrlwi r0, r25, 24
    addi r3, r1, 0x8
    slwi r4, r0, 3
    addi r0, r4, 0x4
    lwzx r3, r3, r0
    b @L_80058554
    @L_800584F4:
    clrlwi r0, r25, 24
    addi r3, r1, 0x8
    slwi r0, r0, 3
    lwzx r3, r3, r0
    b @L_80058554
    @L_80058508:
    mr r3, r26
    mr r4, r25
    li r5, 0xc1
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 24
    mr r4, r25
    cmplwi r0, 0x0
    li r3, 0x0
    li r5, 0x5a
    beq @L_8005853C
    li r0, 0x1
    b @L_80058540
    @L_8005853C:
    li r0, 0x0
    @L_80058540:
    clrlwi r6, r0, 16
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80058554
    li r3, -0x1
    @L_80058554:
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_80058568
    stw r3, 0x0(r31)
    addi r31, r31, 0x4
    @L_80058568:
    addi r27, r27, 0x1
    @L_8005856C:
    bl fn_801347D8
    extsb r0, r3
    cmpw r27, r0
    blt @L_800583F0
    addi r28, r28, 0x1
    @L_80058580:
    bl fn_801347E0
    extsb r0, r3
    cmpw r28, r0
    blt @L_800583E8
    li r0, 0x0
    stw r0, 0x0(r31)
    b @L_800585A4
    @L_8005859C:
    li r0, 0x0
    stw r0, lbl_8047A590(r13)
    @L_800585A4:
    bl fn_800FF560
    lis r5, fn_80057E70@ha
    mr r4, r3
    addi r8, r5, fn_80057E70@l
    li r3, 0x1
    li r5, 0x4000
    li r6, 0x1
    li r7, 0x1
    bl fn_800F07A8
    li r3, 0x19
    bl fn_80102510
    li r3, 0x1a
    bl fn_80102510
    li r3, 0x1b
    bl fn_80102510
    li r3, 0x19
    li r4, 0x1
    bl fn_80102428
    li r3, 0x1a
    li r4, 0x1
    bl fn_80102428
    li r3, 0x1b
    li r4, 0x1
    bl fn_80102428
    li r3, 0x0
    bl fn_80055B98
    li r3, 0x1a
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x1b
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x19
    li r4, 0x0
    bl fn_8010264C
    lis r3, lbl_803A9A08@ha
    addi r3, r3, lbl_803A9A08@l
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80058660
    lwz r0, 0x4(r3)
    cmpwi r0, 0x0
    beq @L_80058658
    li r0, 0x1
    b @L_80058664
    @L_80058658:
    li r0, 0x0
    b @L_80058664
    @L_80058660:
    li r0, 0x2
    @L_80058664:
    cmpwi r0, 0x2
    bne @L_80058684
    li r3, 0x48a
    bl fn_8017B1CC
    li r3, 0x48a
    bl fn_800F915C
    li r4, 0x1
    b @L_800586B0
    @L_80058684:
    cmpwi r0, 0x1
    bne @L_8005869C
    lis r3, lbl_803A9A08@ha
    li r0, 0x0
    addi r3, r3, lbl_803A9A08@l
    stw r0, 0x8(r3)
    @L_8005869C:
    lis r3, lbl_803A9A08@ha
    li r0, 0x1
    addi r3, r3, lbl_803A9A08@l
    li r4, 0x0
    stw r0, 0xc(r3)
    @L_800586B0:
    cmpwi r4, 0x0
    bne @L_800586FC
    lis r3, lbl_803A9A08@ha
    addi r26, r3, lbl_803A9A08@l
    b @L_800586C8
    @L_800586C4:
    bl fn_800F0308
    @L_800586C8:
    lwz r0, 0x0(r26)
    cmpwi r0, 0x0
    beq @L_800586F0
    lwz r0, 0x4(r26)
    cmpwi r0, 0x0
    beq @L_800586E8
    li r0, 0x1
    b @L_800586F4
    @L_800586E8:
    li r0, 0x0
    b @L_800586F4
    @L_800586F0:
    li r0, 0x2
    @L_800586F4:
    cmpwi r0, 0x2
    bne @L_800586C4
    @L_800586FC:
    cmpwi r30, 0x0
    ble @L_80058714
    mr r3, r29
    bl fn_800E24B0
    mr r3, r29
    bl fn_800E209C
    @L_80058714:
    li r3, 0x70b
    bl fn_8017B1CC
    li r3, 0x70b
    bl fn_800F915C
    li r3, 0x0
    bl fn_801CAAF4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80058740
    li r3, 0xdde
    bl fn_80190528
    @L_80058740:
    lmw r25, 0x1d4(r1)
    lwz r0, 0x1f4(r1)
    mtlr r0
    addi r1, r1, 0x1f0
    blr
