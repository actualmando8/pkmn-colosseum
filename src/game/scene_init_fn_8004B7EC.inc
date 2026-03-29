    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    lis r3, lbl_803A6A60@ha
    li r4, 0x0
    addi r30, r3, lbl_803A6A60@l
    lfs f3, lbl_8047BDAC(r2)
    lfs f2, lbl_8047BDA0(r2)
    addi r29, r30, 0x38
    stb r4, 0x4a(r30)
    li r0, 0x1
    lfs f0, lbl_8047BDF8(r2)
    addi r28, r30, 0x30
    lfs f1, lbl_8047BDF4(r2)
    li r27, 0x0
    stb r4, 0x0(r30)
    li r3, 0x0
    stb r4, 0x48(r30)
    stb r4, 0x1(r30)
    stfs f3, 0x14(r30)
    stfs f3, 0x10(r30)
    stfs f3, 0xc(r30)
    stfs f2, 0x2c(r30)
    stfs f2, 0x28(r30)
    stfs f1, 0x0(r28)
    stfs f0, 0x34(r30)
    stfs f3, 0x0(r29)
    stfs f3, 0x3c(r30)
    stfs f3, 0x40(r30)
    stw r4, 0x44(r30)
    stb r0, 0x49(r30)
    stb r4, 0x4a(r30)
    bl fn_801096E8
    lis r3, 0xc54
    addi r3, r3, 0x1000
    bl fn_800F92D4
    lis r4, 0xc54
    mr r31, r3
    addi r3, r4, 0x1601
    bl fn_800F92D4
    mr r0, r3
    mr r3, r31
    mr r26, r0
    li r4, 0x1
    bl fn_800E3CC8
    lis r4, 0xc54
    li r3, 0x17
    addi r4, r4, 0x1800
    li r5, 0x0
    li r6, 0x0
    bl fn_80176E0C
    cmplwi r31, 0x0
    beq @L_8004B8E8
    mr r3, r31
    li r4, 0x0
    bl fn_800ECCA8
    lfs f1, lbl_8047BDAC(r2)
    mr r3, r31
    bl fn_800ECA78
    lfs f1, lbl_8047BDA8(r2)
    mr r3, r31
    bl fn_800EC9DC
    @L_8004B8E8:
    mr r3, r31
    li r4, 0x0
    bl fn_800ECB74
    lfs f1, lbl_8047BDA8(r2)
    mr r3, r26
    bl fn_800DCAF0
    lfs f1, lbl_8047BDAC(r2)
    mr r3, r26
    bl fn_800DCADC
    mr r3, r26
    li r4, 0x0
    bl fn_800DCAD4
    mr r3, r26
    bl fn_800DCAB0
    mr r3, r31
    bl fn_800EC990
    li r3, 0x448
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    cmplwi r31, 0x0
    bne @L_8004B94C
    b @L_8004B960
    b @L_8004B94C
    @L_8004B948:
    bl fn_800F0308
    @L_8004B94C:
    mr r3, r31
    bl fn_800EC960
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8004B948
    @L_8004B960:
    mr r3, r31
    bl fn_800EC96C
    li r3, 0x1
    bl fn_80176B48
    mr r3, r26
    li r4, 0x0
    bl fn_800DCC34
    lis r3, 0xc54
    addi r3, r3, 0x1600
    bl fn_800F92D4
    li r4, 0x0
    bl fn_800DCC34
    lis r3, 0xc54
    addi r3, r3, 0x1602
    bl fn_800F92D4
    li r4, 0x0
    bl fn_800DCC34
    lis r3, 0xc54
    addi r3, r3, 0x200
    bl fn_800F92D4
    lis r3, 0xc54
    addi r3, r3, 0x1800
    bl fn_800F92D4
    bl fn_8004A47C
    li r7, 0x0
    la r8, lbl_8047A4FC(r13)
    li r6, 0xff
    la r5, lbl_8047A4F8(r13)
    stw r8, 0x10(r1)
    li r0, 0x36b2
    li r3, 0xf0
    li r4, 0x0
    stb r7, 0xc(r1)
    stb r6, 0xd(r1)
    stb r7, 0xe(r1)
    stw r5, 0x14(r1)
    stw r0, 0x0(r8)
    bl fn_8010264C
    li r3, 0x10d
    li r4, 0x0
    bl fn_8010264C
    bl fn_801046B8
    mr r4, r3
    addi r9, r1, 0xc
    li r3, 0x71
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    mr r3, r31
    li r4, 0x0
    bl fn_800E4014
    lis r3, lbl_803A6A60@ha
    addi r31, r3, lbl_803A6A60@l
    b @L_8004BC70
    @L_8004BA44:
    lwz r5, 0x10(r1)
    li r0, 0x36b2
    li r3, 0x97
    li r4, 0x0
    stw r0, 0x0(r5)
    stw r27, 0x8(r1)
    bl fn_8010264C
    li r3, 0x98
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x99
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x9a
    li r4, 0x0
    bl fn_8010264C
    bl fn_801046B8
    mr r4, r3
    addi r5, r1, 0x8
    li r3, 0x72
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    bne @L_8004BAB8
    li r27, -0x1
    b @L_8004BAC8
    @L_8004BAB8:
    lis r3, lbl_803A6A60@ha
    addi r3, r3, lbl_803A6A60@l
    lbz r27, 0x0(r3)
    extsb r27, r27
    @L_8004BAC8:
    extsb r0, r27
    li r26, 0x1
    cmpwi r27, 0x1
    stb r0, 0x0(r31)
    stb r26, 0x1(r30)
    beq @L_8004BB64
    bge @L_8004BAF0
    cmpwi r27, 0x0
    bge @L_8004BAFC
    b @L_8004BC4C
    @L_8004BAF0:
    cmpwi r27, 0x3
    bge @L_8004BC4C
    b @L_8004BBCC
    @L_8004BAFC:
    lfs f0, lbl_8047BDFC(r2)
    stfs f0, 0x0(r29)
    b @L_8004BB24
    @L_8004BB08:
    lfs f1, 0x0(r28)
    lfs f0, 0x0(r29)
    fcmpu cr0, f1, f0
    bne @L_8004BB20
    li r26, 0x0
    b @L_8004BB24
    @L_8004BB20:
    bl fn_800F0308
    @L_8004BB24:
    cmpwi r26, 0x0
    bne @L_8004BB08
    li r3, 0x97
    bl fn_80102510
    li r3, 0x97
    li r4, 0x1
    bl fn_80102428
    lwz r3, 0x10(r1)
    li r0, 0x36b4
    stw r0, 0x0(r3)
    bl fn_8003C7C0
    lfs f0, lbl_8047BDAC(r2)
    li r0, 0x0
    stb r0, 0x1(r30)
    stfs f0, 0x0(r29)
    b @L_8004BC70
    @L_8004BB64:
    lfs f0, lbl_8047BDFC(r2)
    stfs f0, 0x0(r29)
    b @L_8004BB8C
    @L_8004BB70:
    lfs f1, 0x0(r28)
    lfs f0, 0x0(r29)
    fcmpu cr0, f1, f0
    bne @L_8004BB88
    li r26, 0x0
    b @L_8004BB8C
    @L_8004BB88:
    bl fn_800F0308
    @L_8004BB8C:
    cmpwi r26, 0x0
    bne @L_8004BB70
    li r3, 0x97
    bl fn_80102510
    li r3, 0x97
    li r4, 0x1
    bl fn_80102428
    lwz r3, 0x10(r1)
    li r0, 0x36b5
    stw r0, 0x0(r3)
    bl fn_80044630
    lfs f0, lbl_8047BDAC(r2)
    li r0, 0x0
    stb r0, 0x1(r30)
    stfs f0, 0x0(r29)
    b @L_8004BC70
    @L_8004BBCC:
    lfs f0, lbl_8047BDFC(r2)
    stfs f0, 0x0(r29)
    b @L_8004BBF4
    @L_8004BBD8:
    lfs f1, 0x0(r28)
    lfs f0, 0x0(r29)
    fcmpu cr0, f1, f0
    bne @L_8004BBF0
    li r26, 0x0
    b @L_8004BBF4
    @L_8004BBF0:
    bl fn_800F0308
    @L_8004BBF4:
    cmpwi r26, 0x0
    bne @L_8004BBD8
    li r3, 0x97
    bl fn_80102510
    li r3, 0x97
    li r4, 0x1
    bl fn_80102428
    li r3, 0x3f0
    bl fn_801902E0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8004BC28
    li r27, 0x1
    @L_8004BC28:
    lwz r3, 0x10(r1)
    li r0, 0x36b3
    stw r0, 0x0(r3)
    bl fn_8004C120
    lfs f0, lbl_8047BDAC(r2)
    li r0, 0x0
    stb r0, 0x1(r30)
    stfs f0, 0x0(r29)
    b @L_8004BC70
    @L_8004BC4C:
    lfs f1, lbl_8047BE00(r2)
    li r3, 0x3
    bl fn_801C41C8
    lfs f1, lbl_8047BDFC(r2)
    li r0, 0x1
    lfs f0, lbl_8047BDF8(r2)
    stfs f1, 0x0(r29)
    stfs f0, 0x3c(r30)
    stb r0, 0x4a(r30)
    @L_8004BC70:
    lbz r0, 0x4a(r30)
    cmplwi r0, 0x0
    beq @L_8004BA44
    li r27, 0x0
    li r26, 0x1
    @L_8004BC84:
    cmpwi r27, 0x64
    beq @L_8004BCC0
    bge @L_8004BCC4
    cmpwi r27, 0x0
    beq @L_8004BC9C
    b @L_8004BCC4
    @L_8004BC9C:
    li r3, 0x0
    bl fn_801C40F0
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_8004BCB8
    li r27, 0x64
    b @L_8004BCC4
    @L_8004BCB8:
    bl fn_800F0308
    b @L_8004BCC4
    @L_8004BCC0:
    li r26, 0x0
    @L_8004BCC4:
    cmpwi r26, 0x0
    bne @L_8004BC84
    lbz r0, 0x49(r30)
    cmplwi r0, 0x1
    beq @L_8004BCE8
    li r3, 0x1
    bl fn_801096F8
    li r0, 0x1
    stb r0, 0x49(r30)
    @L_8004BCE8:
    li r3, 0x97
    bl fn_80102510
    li r3, 0x97
    li r4, 0x1
    bl fn_80102428
    li r3, 0xf0
    bl fn_80102510
    li r3, 0x71
    bl fn_80102510
    li r3, 0xf0
    li r4, 0x1
    bl fn_80102428
    li r3, 0x71
    li r4, 0x1
    bl fn_80102428
    li r27, 0x0
    li r26, 0x1
    @L_8004BD2C:
    cmpwi r27, 0x1
    beq @L_8004BD64
    bge @L_8004BD44
    cmpwi r27, 0x0
    bge @L_8004BD50
    b @L_8004BD8C
    @L_8004BD44:
    cmpwi r27, 0x64
    beq @L_8004BD88
    b @L_8004BD8C
    @L_8004BD50:
    lfs f1, lbl_8047BDA8(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r27, 0x1
    b @L_8004BD8C
    @L_8004BD64:
    li r3, 0x0
    bl fn_801C40F0
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_8004BD80
    li r27, 0x64
    b @L_8004BD8C
    @L_8004BD80:
    bl fn_800F0308
    b @L_8004BD8C
    @L_8004BD88:
    li r26, 0x0
    @L_8004BD8C:
    cmpwi r26, 0x0
    bne @L_8004BD2C
    bl fn_800FF660
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
