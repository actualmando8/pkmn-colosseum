    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lbz r0, 0x15c(r3)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8003E6F4
    bge @L_8003E3D0
    cmpwi r0, 0x0
    beq @L_8003E3E0
    bge @L_8003E3E8
    b @L_8003F02C
    @L_8003E3D0:
    cmpwi r0, 0x4
    beq @L_8003ED1C
    bge @L_8003F02C
    b @L_8003EA08
    @L_8003E3E0:
    bl fn_8003F040
    b @L_8003F02C
    @L_8003E3E8:
    li r3, 0x640
    li r4, 0x20
    bl fn_800E2C04
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8003E408
    bl fn_800E27B0
    b @L_8003E40C
    @L_8003E408:
    li r3, 0x0
    @L_8003E40C:
    mr r30, r3
    li r3, 0x320
    li r4, 0x20
    bl fn_800E2C04
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8003E430
    bl fn_800E27B0
    b @L_8003E434
    @L_8003E430:
    li r3, 0x0
    @L_8003E434:
    li r29, 0x0
    mr r31, r3
    mr r28, r29
    b @L_8003E53C
    @L_8003E444:
    clrlwi r0, r28, 16
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r27, r3, r0
    rlwinm r0, r27, 0, 16, 16
    cmpwi r0, 0x0
    bne @L_8003E538
    lwz r24, lbl_8047A4E0(r13)
    cmplwi r24, 0x0
    beq @L_8003E514
    mr r26, r27
    beq @L_8003E4E0
    cmplwi r27, 0x8000
    blt @L_8003E480
    clrlwi r26, r27, 18
    @L_8003E480:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r24
    clrlwi r4, r26, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r26
    li r3, 0x0
    bl fn_8025FDDC
    mr r25, r3
    mr r4, r26
    li r3, 0x0
    bl fn_8025FD34
    mr r26, r3
    mr r3, r24
    mr r4, r25
    bl fn_8011DFE0
    mr r3, r24
    mr r4, r26
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_8003E4E4
    @L_8003E4E0:
    li r3, 0x0
    @L_8003E4E4:
    cmplwi r3, 0x0
    beq @L_8003E50C
    bl fn_8011F5C8
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_8003E504
    bl fn_8011E18C
    b @L_8003E518
    @L_8003E504:
    li r3, 0x0
    b @L_8003E518
    @L_8003E50C:
    li r3, 0x0
    b @L_8003E518
    @L_8003E514:
    li r3, 0x0
    @L_8003E518:
    clrlwi r0, r29, 16
    addi r29, r29, 0x1
    slwi r4, r0, 1
    slwi r0, r0, 2
    sthx r27, r31, r4
    add r4, r30, r0
    sth r3, 0x2(r4)
    sth r28, 0x0(r4)
    @L_8003E538:
    addi r28, r28, 0x1
    @L_8003E53C:
    lhz r0, lbl_8047A4E8(r13)
    clrlwi r3, r28, 16
    cmplw r3, r0
    blt @L_8003E444
    mr r3, r30
    clrlwi r4, r29, 16
    li r5, 0x1
    bl fn_8003F2DC
    clrlwi r0, r29, 16
    li r4, 0x0
    cmplwi r0, 0x0
    ble @L_8003E694
    cmplwi r0, 0x8
    subi r0, r29, 0x8
    ble @L_8003E65C
    clrlwi r0, r0, 16
    b @L_8003E650
    @L_8003E580:
    clrlwi r5, r4, 16
    lwz r12, lbl_8047A4E4(r13)
    slwi r3, r5, 2
    addi r4, r4, 0x8
    add r3, r30, r3
    slwi r24, r5, 1
    lhz r5, 0x0(r3)
    addi r11, r24, 0x2
    addi r10, r24, 0x4
    addi r9, r24, 0x6
    slwi r5, r5, 1
    addi r8, r24, 0x8
    lhzx r28, r31, r5
    addi r7, r24, 0xa
    addi r6, r24, 0xc
    addi r5, r24, 0xe
    sthx r28, r12, r24
    lhz r28, 0x4(r3)
    lwz r12, lbl_8047A4E4(r13)
    slwi r28, r28, 1
    lhzx r28, r31, r28
    sthx r28, r12, r11
    lhz r12, 0x8(r3)
    lwz r11, lbl_8047A4E4(r13)
    slwi r12, r12, 1
    lhzx r12, r31, r12
    sthx r12, r11, r10
    lhz r11, 0xc(r3)
    lwz r10, lbl_8047A4E4(r13)
    slwi r11, r11, 1
    lhzx r11, r31, r11
    sthx r11, r10, r9
    lhz r10, 0x10(r3)
    lwz r9, lbl_8047A4E4(r13)
    slwi r10, r10, 1
    lhzx r10, r31, r10
    sthx r10, r9, r8
    lhz r9, 0x14(r3)
    lwz r8, lbl_8047A4E4(r13)
    slwi r9, r9, 1
    lhzx r9, r31, r9
    sthx r9, r8, r7
    lhz r8, 0x18(r3)
    lwz r7, lbl_8047A4E4(r13)
    slwi r8, r8, 1
    lhzx r8, r31, r8
    sthx r8, r7, r6
    lhz r6, 0x1c(r3)
    lwz r3, lbl_8047A4E4(r13)
    slwi r6, r6, 1
    lhzx r6, r31, r6
    sthx r6, r3, r5
    @L_8003E650:
    clrlwi r3, r4, 16
    cmplw r3, r0
    blt @L_8003E580
    @L_8003E65C:
    clrlwi r0, r29, 16
    b @L_8003E688
    @L_8003E664:
    clrlwi r7, r4, 16
    lwz r5, lbl_8047A4E4(r13)
    slwi r3, r7, 2
    addi r4, r4, 0x1
    lhzx r6, r30, r3
    slwi r3, r7, 1
    slwi r6, r6, 1
    lhzx r6, r31, r6
    sthx r6, r5, r3
    @L_8003E688:
    clrlwi r3, r4, 16
    cmplw r3, r0
    blt @L_8003E664
    @L_8003E694:
    cmplwi r30, 0x0
    sth r29, lbl_8047A4E8(r13)
    beq @L_8003E6C4
    mr r3, r30
    bl fn_800E202C
    mr r27, r3
    clrlwi r0, r27, 16
    cmplwi r0, 0x0
    beq @L_8003E6C4
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
    @L_8003E6C4:
    cmplwi r31, 0x0
    beq @L_8003F02C
    mr r3, r31
    bl fn_800E202C
    mr r27, r3
    clrlwi r0, r27, 16
    cmplwi r0, 0x0
    beq @L_8003F02C
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
    b @L_8003F02C
    @L_8003E6F4:
    li r3, 0x640
    li r4, 0x20
    bl fn_800E2C04
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8003E714
    bl fn_800E27B0
    b @L_8003E718
    @L_8003E714:
    li r3, 0x0
    @L_8003E718:
    mr r29, r3
    li r3, 0x320
    li r4, 0x20
    bl fn_800E2C04
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8003E73C
    bl fn_800E27B0
    b @L_8003E740
    @L_8003E73C:
    li r3, 0x0
    @L_8003E740:
    li r30, 0x0
    mr r28, r3
    mr r31, r30
    b @L_8003E848
    @L_8003E750:
    clrlwi r0, r31, 16
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r27, r3, r0
    rlwinm r0, r27, 0, 16, 16
    cmpwi r0, 0x0
    bne @L_8003E844
    lwz r24, lbl_8047A4E0(r13)
    cmplwi r24, 0x0
    beq @L_8003E820
    mr r26, r27
    beq @L_8003E7EC
    cmplwi r27, 0x8000
    blt @L_8003E78C
    clrlwi r26, r27, 18
    @L_8003E78C:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r24
    clrlwi r4, r26, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r26
    li r3, 0x0
    bl fn_8025FDDC
    mr r4, r26
    mr r26, r3
    li r3, 0x0
    bl fn_8025FD34
    mr r25, r3
    mr r3, r24
    mr r4, r26
    bl fn_8011DFE0
    mr r3, r24
    mr r4, r25
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_8003E7F0
    @L_8003E7EC:
    li r3, 0x0
    @L_8003E7F0:
    cmplwi r3, 0x0
    beq @L_8003E818
    bl fn_8011F5C8
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_8003E810
    bl fn_8011E18C
    b @L_8003E824
    @L_8003E810:
    li r3, 0x0
    b @L_8003E824
    @L_8003E818:
    li r3, 0x0
    b @L_8003E824
    @L_8003E820:
    li r3, 0x0
    @L_8003E824:
    clrlwi r0, r30, 16
    addi r30, r30, 0x1
    slwi r4, r0, 1
    slwi r0, r0, 2
    sthx r27, r28, r4
    add r4, r29, r0
    sth r3, 0x2(r4)
    sth r31, 0x0(r4)
    @L_8003E844:
    addi r31, r31, 0x1
    @L_8003E848:
    lhz r0, lbl_8047A4E8(r13)
    clrlwi r3, r31, 16
    cmplw r3, r0
    blt @L_8003E750
    clrlwi r4, r30, 16
    cmplwi r4, 0x0
    beq @L_8003E9A8
    mr r3, r29
    li r5, 0x0
    bl fn_8003F2DC
    clrlwi r0, r30, 16
    li r4, 0x0
    cmplwi r0, 0x0
    ble @L_8003E9A8
    cmplwi r0, 0x8
    subi r0, r30, 0x8
    ble @L_8003E970
    clrlwi r0, r0, 16
    b @L_8003E964
    @L_8003E894:
    clrlwi r5, r4, 16
    lwz r12, lbl_8047A4E4(r13)
    slwi r3, r5, 2
    addi r4, r4, 0x8
    add r3, r29, r3
    slwi r24, r5, 1
    lhz r5, 0x0(r3)
    addi r11, r24, 0x2
    addi r10, r24, 0x4
    addi r9, r24, 0x6
    slwi r5, r5, 1
    addi r8, r24, 0x8
    lhzx r31, r28, r5
    addi r7, r24, 0xa
    addi r6, r24, 0xc
    addi r5, r24, 0xe
    sthx r31, r12, r24
    lhz r31, 0x4(r3)
    lwz r12, lbl_8047A4E4(r13)
    slwi r31, r31, 1
    lhzx r31, r28, r31
    sthx r31, r12, r11
    lhz r12, 0x8(r3)
    lwz r11, lbl_8047A4E4(r13)
    slwi r12, r12, 1
    lhzx r12, r28, r12
    sthx r12, r11, r10
    lhz r11, 0xc(r3)
    lwz r10, lbl_8047A4E4(r13)
    slwi r11, r11, 1
    lhzx r11, r28, r11
    sthx r11, r10, r9
    lhz r10, 0x10(r3)
    lwz r9, lbl_8047A4E4(r13)
    slwi r10, r10, 1
    lhzx r10, r28, r10
    sthx r10, r9, r8
    lhz r9, 0x14(r3)
    lwz r8, lbl_8047A4E4(r13)
    slwi r9, r9, 1
    lhzx r9, r28, r9
    sthx r9, r8, r7
    lhz r8, 0x18(r3)
    lwz r7, lbl_8047A4E4(r13)
    slwi r8, r8, 1
    lhzx r8, r28, r8
    sthx r8, r7, r6
    lhz r6, 0x1c(r3)
    lwz r3, lbl_8047A4E4(r13)
    slwi r6, r6, 1
    lhzx r6, r28, r6
    sthx r6, r3, r5
    @L_8003E964:
    clrlwi r3, r4, 16
    cmplw r3, r0
    blt @L_8003E894
    @L_8003E970:
    clrlwi r0, r30, 16
    b @L_8003E99C
    @L_8003E978:
    clrlwi r7, r4, 16
    lwz r5, lbl_8047A4E4(r13)
    slwi r3, r7, 2
    addi r4, r4, 0x1
    lhzx r6, r29, r3
    slwi r3, r7, 1
    slwi r6, r6, 1
    lhzx r6, r28, r6
    sthx r6, r5, r3
    @L_8003E99C:
    clrlwi r3, r4, 16
    cmplw r3, r0
    blt @L_8003E978
    @L_8003E9A8:
    cmplwi r29, 0x0
    sth r30, lbl_8047A4E8(r13)
    beq @L_8003E9D8
    mr r3, r29
    bl fn_800E202C
    mr r27, r3
    clrlwi r0, r27, 16
    cmplwi r0, 0x0
    beq @L_8003E9D8
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
    @L_8003E9D8:
    cmplwi r28, 0x0
    beq @L_8003F02C
    mr r3, r28
    bl fn_800E202C
    mr r27, r3
    clrlwi r0, r27, 16
    cmplwi r0, 0x0
    beq @L_8003F02C
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
    b @L_8003F02C
    @L_8003EA08:
    li r3, 0x640
    li r4, 0x20
    bl fn_800E2C04
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8003EA28
    bl fn_800E27B0
    b @L_8003EA2C
    @L_8003EA28:
    li r3, 0x0
    @L_8003EA2C:
    mr r29, r3
    li r3, 0x320
    li r4, 0x20
    bl fn_800E2C04
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8003EA50
    bl fn_800E27B0
    b @L_8003EA54
    @L_8003EA50:
    li r3, 0x0
    @L_8003EA54:
    li r30, 0x0
    mr r28, r3
    mr r31, r30
    b @L_8003EB5C
    @L_8003EA64:
    clrlwi r0, r31, 16
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r27, r3, r0
    rlwinm r0, r27, 0, 16, 16
    cmpwi r0, 0x0
    bne @L_8003EB58
    lwz r24, lbl_8047A4E0(r13)
    cmplwi r24, 0x0
    beq @L_8003EB34
    mr r26, r27
    beq @L_8003EB00
    cmplwi r27, 0x8000
    blt @L_8003EAA0
    clrlwi r26, r27, 18
    @L_8003EAA0:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r24
    clrlwi r4, r26, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r26
    li r3, 0x0
    bl fn_8025FDDC
    mr r4, r26
    mr r26, r3
    li r3, 0x0
    bl fn_8025FD34
    mr r25, r3
    mr r3, r24
    mr r4, r26
    bl fn_8011DFE0
    mr r3, r24
    mr r4, r25
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_8003EB04
    @L_8003EB00:
    li r3, 0x0
    @L_8003EB04:
    cmplwi r3, 0x0
    beq @L_8003EB2C
    bl fn_8011F5C8
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_8003EB24
    bl fn_8011E1A4
    b @L_8003EB38
    @L_8003EB24:
    li r3, 0x0
    b @L_8003EB38
    @L_8003EB2C:
    li r3, 0x0
    b @L_8003EB38
    @L_8003EB34:
    li r3, 0x0
    @L_8003EB38:
    clrlwi r0, r30, 16
    addi r30, r30, 0x1
    slwi r4, r0, 1
    slwi r0, r0, 2
    sthx r27, r28, r4
    add r4, r29, r0
    sth r3, 0x2(r4)
    sth r31, 0x0(r4)
    @L_8003EB58:
    addi r31, r31, 0x1
    @L_8003EB5C:
    lhz r0, lbl_8047A4E8(r13)
    clrlwi r3, r31, 16
    cmplw r3, r0
    blt @L_8003EA64
    clrlwi r4, r30, 16
    cmplwi r4, 0x0
    beq @L_8003ECBC
    mr r3, r29
    li r5, 0x1
    bl fn_8003F2DC
    clrlwi r0, r30, 16
    li r4, 0x0
    cmplwi r0, 0x0
    ble @L_8003ECBC
    cmplwi r0, 0x8
    subi r0, r30, 0x8
    ble @L_8003EC84
    clrlwi r0, r0, 16
    b @L_8003EC78
    @L_8003EBA8:
    clrlwi r5, r4, 16
    lwz r12, lbl_8047A4E4(r13)
    slwi r3, r5, 2
    addi r4, r4, 0x8
    add r3, r29, r3
    slwi r24, r5, 1
    lhz r5, 0x0(r3)
    addi r11, r24, 0x2
    addi r10, r24, 0x4
    addi r9, r24, 0x6
    slwi r5, r5, 1
    addi r8, r24, 0x8
    lhzx r31, r28, r5
    addi r7, r24, 0xa
    addi r6, r24, 0xc
    addi r5, r24, 0xe
    sthx r31, r12, r24
    lhz r31, 0x4(r3)
    lwz r12, lbl_8047A4E4(r13)
    slwi r31, r31, 1
    lhzx r31, r28, r31
    sthx r31, r12, r11
    lhz r12, 0x8(r3)
    lwz r11, lbl_8047A4E4(r13)
    slwi r12, r12, 1
    lhzx r12, r28, r12
    sthx r12, r11, r10
    lhz r11, 0xc(r3)
    lwz r10, lbl_8047A4E4(r13)
    slwi r11, r11, 1
    lhzx r11, r28, r11
    sthx r11, r10, r9
    lhz r10, 0x10(r3)
    lwz r9, lbl_8047A4E4(r13)
    slwi r10, r10, 1
    lhzx r10, r28, r10
    sthx r10, r9, r8
    lhz r9, 0x14(r3)
    lwz r8, lbl_8047A4E4(r13)
    slwi r9, r9, 1
    lhzx r9, r28, r9
    sthx r9, r8, r7
    lhz r8, 0x18(r3)
    lwz r7, lbl_8047A4E4(r13)
    slwi r8, r8, 1
    lhzx r8, r28, r8
    sthx r8, r7, r6
    lhz r6, 0x1c(r3)
    lwz r3, lbl_8047A4E4(r13)
    slwi r6, r6, 1
    lhzx r6, r28, r6
    sthx r6, r3, r5
    @L_8003EC78:
    clrlwi r3, r4, 16
    cmplw r3, r0
    blt @L_8003EBA8
    @L_8003EC84:
    clrlwi r0, r30, 16
    b @L_8003ECB0
    @L_8003EC8C:
    clrlwi r7, r4, 16
    lwz r5, lbl_8047A4E4(r13)
    slwi r3, r7, 2
    addi r4, r4, 0x1
    lhzx r6, r29, r3
    slwi r3, r7, 1
    slwi r6, r6, 1
    lhzx r6, r28, r6
    sthx r6, r5, r3
    @L_8003ECB0:
    clrlwi r3, r4, 16
    cmplw r3, r0
    blt @L_8003EC8C
    @L_8003ECBC:
    cmplwi r29, 0x0
    sth r30, lbl_8047A4E8(r13)
    beq @L_8003ECEC
    mr r3, r29
    bl fn_800E202C
    mr r27, r3
    clrlwi r0, r27, 16
    cmplwi r0, 0x0
    beq @L_8003ECEC
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
    @L_8003ECEC:
    cmplwi r28, 0x0
    beq @L_8003F02C
    mr r3, r28
    bl fn_800E202C
    mr r27, r3
    clrlwi r0, r27, 16
    cmplwi r0, 0x0
    beq @L_8003F02C
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
    b @L_8003F02C
    @L_8003ED1C:
    li r3, 0x640
    li r4, 0x20
    bl fn_800E2C04
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8003ED3C
    bl fn_800E27B0
    b @L_8003ED40
    @L_8003ED3C:
    li r3, 0x0
    @L_8003ED40:
    mr r29, r3
    li r3, 0x320
    li r4, 0x20
    bl fn_800E2C04
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_8003ED64
    bl fn_800E27B0
    b @L_8003ED68
    @L_8003ED64:
    li r3, 0x0
    @L_8003ED68:
    li r30, 0x0
    mr r28, r3
    mr r31, r30
    b @L_8003EE70
    @L_8003ED78:
    clrlwi r0, r31, 16
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r27, r3, r0
    rlwinm r0, r27, 0, 16, 16
    cmpwi r0, 0x0
    bne @L_8003EE6C
    lwz r24, lbl_8047A4E0(r13)
    cmplwi r24, 0x0
    beq @L_8003EE48
    mr r26, r27
    beq @L_8003EE14
    cmplwi r27, 0x8000
    blt @L_8003EDB4
    clrlwi r26, r27, 18
    @L_8003EDB4:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r24
    clrlwi r4, r26, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r26
    li r3, 0x0
    bl fn_8025FDDC
    mr r4, r26
    mr r26, r3
    li r3, 0x0
    bl fn_8025FD34
    mr r25, r3
    mr r3, r24
    mr r4, r26
    bl fn_8011DFE0
    mr r3, r24
    mr r4, r25
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_8003EE18
    @L_8003EE14:
    li r3, 0x0
    @L_8003EE18:
    cmplwi r3, 0x0
    beq @L_8003EE40
    bl fn_8011F5C8
    bl fn_8011E778
    cmplwi r3, 0x0
    beq @L_8003EE38
    bl fn_8011E1A4
    b @L_8003EE4C
    @L_8003EE38:
    li r3, 0x0
    b @L_8003EE4C
    @L_8003EE40:
    li r3, 0x0
    b @L_8003EE4C
    @L_8003EE48:
    li r3, 0x0
    @L_8003EE4C:
    clrlwi r0, r30, 16
    addi r30, r30, 0x1
    slwi r4, r0, 1
    slwi r0, r0, 2
    sthx r27, r28, r4
    add r4, r29, r0
    sth r3, 0x2(r4)
    sth r31, 0x0(r4)
    @L_8003EE6C:
    addi r31, r31, 0x1
    @L_8003EE70:
    lhz r0, lbl_8047A4E8(r13)
    clrlwi r3, r31, 16
    cmplw r3, r0
    blt @L_8003ED78
    clrlwi r4, r30, 16
    cmplwi r4, 0x0
    beq @L_8003EFD0
    mr r3, r29
    li r5, 0x0
    bl fn_8003F2DC
    clrlwi r0, r30, 16
    li r4, 0x0
    cmplwi r0, 0x0
    ble @L_8003EFD0
    cmplwi r0, 0x8
    subi r0, r30, 0x8
    ble @L_8003EF98
    clrlwi r0, r0, 16
    b @L_8003EF8C
    @L_8003EEBC:
    clrlwi r5, r4, 16
    lwz r12, lbl_8047A4E4(r13)
    slwi r3, r5, 2
    addi r4, r4, 0x8
    add r3, r29, r3
    slwi r24, r5, 1
    lhz r5, 0x0(r3)
    addi r11, r24, 0x2
    addi r10, r24, 0x4
    addi r9, r24, 0x6
    slwi r5, r5, 1
    addi r8, r24, 0x8
    lhzx r31, r28, r5
    addi r7, r24, 0xa
    addi r6, r24, 0xc
    addi r5, r24, 0xe
    sthx r31, r12, r24
    lhz r31, 0x4(r3)
    lwz r12, lbl_8047A4E4(r13)
    slwi r31, r31, 1
    lhzx r31, r28, r31
    sthx r31, r12, r11
    lhz r12, 0x8(r3)
    lwz r11, lbl_8047A4E4(r13)
    slwi r12, r12, 1
    lhzx r12, r28, r12
    sthx r12, r11, r10
    lhz r11, 0xc(r3)
    lwz r10, lbl_8047A4E4(r13)
    slwi r11, r11, 1
    lhzx r11, r28, r11
    sthx r11, r10, r9
    lhz r10, 0x10(r3)
    lwz r9, lbl_8047A4E4(r13)
    slwi r10, r10, 1
    lhzx r10, r28, r10
    sthx r10, r9, r8
    lhz r9, 0x14(r3)
    lwz r8, lbl_8047A4E4(r13)
    slwi r9, r9, 1
    lhzx r9, r28, r9
    sthx r9, r8, r7
    lhz r8, 0x18(r3)
    lwz r7, lbl_8047A4E4(r13)
    slwi r8, r8, 1
    lhzx r8, r28, r8
    sthx r8, r7, r6
    lhz r6, 0x1c(r3)
    lwz r3, lbl_8047A4E4(r13)
    slwi r6, r6, 1
    lhzx r6, r28, r6
    sthx r6, r3, r5
    @L_8003EF8C:
    clrlwi r3, r4, 16
    cmplw r3, r0
    blt @L_8003EEBC
    @L_8003EF98:
    clrlwi r0, r30, 16
    b @L_8003EFC4
    @L_8003EFA0:
    clrlwi r7, r4, 16
    lwz r5, lbl_8047A4E4(r13)
    slwi r3, r7, 2
    addi r4, r4, 0x1
    lhzx r6, r29, r3
    slwi r3, r7, 1
    slwi r6, r6, 1
    lhzx r6, r28, r6
    sthx r6, r5, r3
    @L_8003EFC4:
    clrlwi r3, r4, 16
    cmplw r3, r0
    blt @L_8003EFA0
    @L_8003EFD0:
    cmplwi r29, 0x0
    sth r30, lbl_8047A4E8(r13)
    beq @L_8003F000
    mr r3, r29
    bl fn_800E202C
    mr r27, r3
    clrlwi r0, r27, 16
    cmplwi r0, 0x0
    beq @L_8003F000
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
    @L_8003F000:
    cmplwi r28, 0x0
    beq @L_8003F02C
    mr r3, r28
    bl fn_800E202C
    mr r27, r3
    clrlwi r0, r27, 16
    cmplwi r0, 0x0
    beq @L_8003F02C
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
    @L_8003F02C:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
