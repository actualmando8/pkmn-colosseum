    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r26, 0x28(r1)
    lbz r0, lbl_8047A409(r13)
    li r3, 0x1
    lis r4, lbl_803A2518@ha
    stb r3, lbl_804788B0(r13)
    cmplwi r0, 0x1
    addi r31, r4, lbl_803A2518@l
    beq @L_80032484
    bl fn_801D036C
    mr r30, r3
    b @L_800323E4
    @L_80031B84:
    cmplwi r0, 0x13
    bgt @L_800323E4
    lis r3, jumptable_802E4F90@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_802E4F90@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    lis r3, lbl_80266F90@ha
    addi r5, r1, 0x10
    addi r10, r3, lbl_80266F90@l
    li r3, 0x0
    lwz r9, 0x0(r10)
    li r4, 0x1
    lwz r8, 0x4(r10)
    li r6, 0x0
    lwz r7, 0x8(r10)
    lwz r0, 0xc(r10)
    stw r9, 0x10(r1)
    stw r8, 0x14(r1)
    stw r7, 0x18(r1)
    stw r0, 0x1c(r1)
    bl fn_800849B4
    cmpwi r3, 0x0
    bge @L_80031BF8
    li r0, 0x0
    stb r0, lbl_8047A40A(r13)
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    @L_80031BF8:
    li r0, 0x2
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    bl fn_8002FC58
    b @L_800323E4
    li r3, 0xd9
    li r4, 0x0
    bl fn_801021F8
    li r0, 0x1
    stw r0, 0xc(r1)
    bl fn_801046B8
    mr r4, r3
    addi r5, r1, 0xc
    li r3, 0xe3
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0xe3
    bl fn_80104704
    mr r29, r3
    li r4, 0x102a
    bl fn_801046C8
    cmplwi r29, 0x0
    mr r29, r3
    beq @L_80031C7C
    cmplwi r29, 0x0
    beq @L_80031C7C
    li r4, 0x1
    bl fn_80109220
    li r0, 0x43e4
    stw r0, 0x4c(r29)
    @L_80031C7C:
    li r3, 0xe3
    bl fn_80104704
    mr r29, r3
    li r4, 0x1029
    bl fn_801046C8
    cmplwi r29, 0x0
    mr r29, r3
    beq @L_80031CB4
    cmplwi r29, 0x0
    beq @L_80031CB4
    li r4, 0x1
    bl fn_80109220
    li r0, 0x43e5
    stw r0, 0x4c(r29)
    @L_80031CB4:
    li r3, 0xe3
    li r4, 0x1
    bl fn_801045A8
    li r3, 0xe3
    bl fn_801043A4
    mr r29, r3
    li r3, 0xe3
    bl fn_801023E4
    mr r26, r3
    li r3, 0xe3
    bl fn_80102510
    cmpwi r29, -0x1
    bne @L_80031CEC
    li r26, -0x1
    @L_80031CEC:
    cmpwi r26, 0x0
    beq @L_80031D18
    bge @L_80031D04
    cmpwi r26, -0x1
    bge @L_80031D24
    b @L_800323E4
    @L_80031D04:
    cmpwi r26, 0x2
    bge @L_800323E4
    li r0, 0x5
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    @L_80031D18:
    li r0, 0x6
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    @L_80031D24:
    li r0, 0x2
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    li r3, 0xd9
    li r4, 0x0
    bl fn_801021F8
    li r3, 0xd9
    bl fn_80104704
    mr r29, r3
    li r4, 0x10b2
    bl fn_801046C8
    cmplwi r29, 0x0
    mr r29, r3
    beq @L_80031D74
    cmplwi r29, 0x0
    beq @L_80031D74
    li r4, 0x1
    bl fn_80109220
    li r0, 0x43db
    stw r0, 0x4c(r29)
    @L_80031D74:
    li r0, 0x1
    stw r0, 0x8(r1)
    bl fn_801046B8
    mr r4, r3
    addi r5, r1, 0x8
    li r3, 0xe3
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0xe3
    bl fn_80104704
    mr r29, r3
    li r4, 0x102a
    bl fn_801046C8
    cmplwi r29, 0x0
    mr r29, r3
    beq @L_80031DD8
    cmplwi r29, 0x0
    beq @L_80031DD8
    li r4, 0x1
    bl fn_80109220
    li r0, 0x43d4
    stw r0, 0x4c(r29)
    @L_80031DD8:
    li r3, 0xe3
    bl fn_80104704
    mr r29, r3
    li r4, 0x1029
    bl fn_801046C8
    cmplwi r29, 0x0
    mr r29, r3
    beq @L_80031E10
    cmplwi r29, 0x0
    beq @L_80031E10
    li r4, 0x1
    bl fn_80109220
    li r0, 0x43d5
    stw r0, 0x4c(r29)
    @L_80031E10:
    li r3, 0xe3
    li r4, 0x1
    bl fn_801045A8
    li r3, 0xe3
    bl fn_801043A4
    mr r29, r3
    li r3, 0xe3
    bl fn_801023E4
    mr r26, r3
    li r3, 0xe3
    bl fn_80102510
    cmpwi r29, -0x1
    bne @L_80031E48
    li r26, -0x1
    @L_80031E48:
    li r3, 0xd9
    bl fn_80104704
    mr r29, r3
    li r4, 0x10b2
    bl fn_801046C8
    cmplwi r29, 0x0
    beq @L_80031E7C
    cmplwi r3, 0x0
    beq @L_80031E7C
    li r0, 0x0
    li r4, 0x0
    stw r0, 0x4c(r3)
    bl fn_80109220
    @L_80031E7C:
    cmpwi r26, 0x0
    beq @L_80031EA0
    bge @L_80031E94
    cmpwi r26, -0x1
    bge @L_80031EB8
    b @L_800323E4
    @L_80031E94:
    cmpwi r26, 0x2
    bge @L_800323E4
    b @L_80031EAC
    @L_80031EA0:
    li r0, 0x2
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    @L_80031EAC:
    li r0, 0x0
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    @L_80031EB8:
    li r0, 0x2
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    lfs f1, lbl_8047B9D0(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lwz r3, lbl_8047A414(r13)
    li r4, 0x0
    bl fn_800E4014
    li r3, 0xd9
    li r4, 0x0
    bl fn_80102254
    lfs f1, lbl_8047B9D4(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lwz r0, lbl_8047A428(r13)
    li r3, 0x0
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    li r4, 0x0
    li r5, 0x0
    bl fn_80097CD0
    lfs f1, lbl_8047B9D4(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lwz r3, lbl_8047A414(r13)
    li r4, 0x1
    bl fn_800E4014
    li r3, 0xd9
    li r4, 0x1
    bl fn_80102254
    lfs f1, lbl_8047B9D0(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r0, 0x2
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    bl fn_8002F79C
    b @L_800323E4
    bl fn_8002F284
    b @L_800323E4
    bl fn_8002EE74
    b @L_800323E4
    li r4, 0x1
    li r3, -0x1
    li r0, 0x2
    stb r4, lbl_8047A410(r13)
    stw r3, lbl_8047A424(r13)
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    lfs f1, lbl_8047B9D0(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lwz r3, lbl_8047A414(r13)
    li r4, 0x0
    bl fn_800E4014
    li r3, 0xdc
    li r4, 0x0
    bl fn_80102254
    li r3, 0xd9
    li r4, 0x0
    bl fn_80102254
    lfs f1, lbl_8047B9D4(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lwz r0, lbl_8047A428(r13)
    addi r3, r31, 0x170
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r29, r3
    li r3, 0x2
    bl fn_80103CC0
    mr r3, r29
    li r4, 0x0
    li r5, 0x0
    bl fn_80097CD0
    li r3, 0x1
    bl fn_80103CC0
    lfs f1, lbl_8047B9D4(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lwz r3, lbl_8047A414(r13)
    li r4, 0x1
    bl fn_800E4014
    li r3, 0xd9
    li r4, 0x1
    bl fn_80102254
    lfs f1, lbl_8047B9D0(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    bl fn_80102004
    cmpwi r3, 0x1
    bne @L_8003209C
    li r3, 0x2
    li r4, 0x4448
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0xd9
    bl fn_80102510
    li r0, 0x0
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    @L_8003209C:
    li r0, 0x7
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    bl fn_8002EA5C
    b @L_800323E4
    mr r3, r30
    bl fn_8002E460
    b @L_800323E4
    addi r3, r31, 0x170
    li r4, 0x1
    li r5, 0x0
    bl fn_8012A5B0
    lwz r0, lbl_8047A424(r13)
    mr r26, r3
    li r3, 0x0
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    lwz r0, lbl_8047A420(r13)
    mr r28, r3
    addi r3, r31, 0x170
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r29, r3
    li r3, 0xde
    li r4, 0x0
    bl fn_8010264C
    addi r3, r31, 0xd18
    li r4, 0xe8
    li r5, 0x11c
    bl fn_8010A5BC
    addi r3, r31, 0xcd0
    li r4, 0xe8
    li r5, 0x11c
    bl fn_8010A5BC
    mr r4, r28
    addi r3, r31, 0xd18
    bl fn_80109C88
    mr r4, r29
    addi r3, r31, 0xcd0
    bl fn_80109C88
    mr r3, r28
    bl fn_8011F4F0
    mr r27, r3
    mr r3, r29
    bl fn_8011F4F0
    mr r28, r3
    mr r4, r26
    li r3, 0x4d
    bl fn_80132A38
    mr r4, r27
    li r3, 0x32
    bl fn_80132A38
    mr r4, r28
    li r3, 0x33
    bl fn_80132A38
    lfs f1, lbl_8047B9D0(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0x2
    li r4, 0x43d6
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    mr r4, r27
    li r3, 0x32
    bl fn_80132A38
    mr r4, r28
    li r3, 0x33
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x43d8
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0xe
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    bl fn_8002E26C
    b @L_800323E4
    li r3, 0x2
    li r4, 0x43dc
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0xde
    bl fn_80102510
    addi r3, r31, 0xd18
    bl fn_8010A420
    addi r3, r31, 0xcd0
    bl fn_8010A420
    li r0, 0x0
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    addi r3, r31, 0x170
    li r4, 0x1
    li r5, 0x0
    bl fn_8012A5B0
    lwz r0, lbl_8047A424(r13)
    mr r28, r3
    li r3, 0x0
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    lwz r0, lbl_8047A420(r13)
    mr r27, r3
    addi r3, r31, 0x170
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r26, r3
    mr r3, r27
    li r4, 0x46
    bl fn_8011D904
    mr r3, r26
    li r4, 0x46
    bl fn_8011D904
    mr r4, r27
    li r3, 0x0
    bl fn_8025FF9C
    mr r3, r27
    bl fn_8011F4F0
    mr r27, r3
    mr r3, r26
    bl fn_8011F4F0
    mr r26, r3
    mr r4, r28
    li r3, 0x4d
    bl fn_80132A38
    mr r4, r27
    li r3, 0x32
    bl fn_80132A38
    mr r4, r26
    li r3, 0x33
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x43da
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x11
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    bl fn_8002DF10
    b @L_800323E4
    mr r3, r30
    bl fn_8002DD24
    b @L_800323E4
    li r3, 0x2
    li r4, 0x43de
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x0
    li r4, -0x1
    li r5, -0x1
    li r6, 0x0
    bl fn_8001E074
    mr r29, r3
    li r3, 0x1
    bl fn_801069FC
    extsb r0, r29
    cmpwi r0, 0x0
    beq @L_80032364
    bge @L_80032358
    cmpwi r0, -0x1
    bge @L_800323C4
    b @L_800323E4
    @L_80032358:
    cmpwi r0, 0x2
    bge @L_800323E4
    b @L_800323A0
    @L_80032364:
    li r3, 0xde
    bl fn_80102510
    li r0, -0x1
    addi r3, r31, 0xd18
    stw r0, lbl_8047A428(r13)
    stw r0, lbl_8047A424(r13)
    stw r0, lbl_8047A420(r13)
    bl fn_8010A420
    addi r3, r31, 0xcd0
    bl fn_8010A420
    li r3, 0x1
    li r0, 0x2
    stb r3, lbl_8047A410(r13)
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    @L_800323A0:
    li r3, 0xde
    bl fn_80102510
    addi r3, r31, 0xd18
    bl fn_8010A420
    addi r3, r31, 0xcd0
    bl fn_8010A420
    li r0, 0x0
    stw r0, lbl_8047A42C(r13)
    b @L_800323E4
    @L_800323C4:
    li r3, 0xde
    bl fn_80102510
    addi r3, r31, 0xd18
    bl fn_8010A420
    addi r3, r31, 0xcd0
    bl fn_8010A420
    li r0, 0x0
    stw r0, lbl_8047A42C(r13)
    @L_800323E4:
    lwz r0, lbl_8047A42C(r13)
    cmpwi r0, 0x0
    bgt @L_80031B84
    mr r3, r30
    bl fn_801D0314
    lbz r0, lbl_8047A40A(r13)
    cmplwi r0, 0x0
    beq @L_80032450
    lbz r0, lbl_804788B0(r13)
    cmplwi r0, 0x0
    beq @L_80032430
    li r3, 0x2
    li r4, 0x44d0
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_80032458
    @L_80032430:
    li r3, 0x2
    li r4, 0x44f1
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_80032458
    @L_80032450:
    li r0, 0x1
    stb r0, lbl_8047A40A(r13)
    @L_80032458:
    li r3, 0x1
    bl fn_801024E8
    bl fn_800FF52C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80032478
    bl fn_800FF660
    b @L_80032484
    @L_80032478:
    bl fn_8011394C
    li r4, 0x0
    bl fn_80113828
    @L_80032484:
    li r0, 0x1
    stb r0, lbl_8047A409(r13)
    lmw r26, 0x28(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
