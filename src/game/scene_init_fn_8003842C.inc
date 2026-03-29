    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    lis r4, lbl_803A6498@ha
    lis r3, lbl_802E51C8@ha
    addi r29, r4, lbl_803A6498@l
    li r25, 0x0
    addi r30, r3, lbl_802E51C8@l
    li r28, 0x1
    li r3, 0x55
    li r4, 0x1f4
    li r5, 0x1
    li r6, 0x1
    bl fn_801661D0
    lis r3, lbl_802EF0A8@ha
    addi r31, r3, lbl_802EF0A8@l
    @L_80038470:
    cmpwi r25, 0x14
    beq @L_800387F4
    bge @L_800384A0
    cmpwi r25, 0x1
    beq @L_80038594
    bge @L_80038494
    cmpwi r25, 0x0
    bge @L_800384C4
    b @L_80038894
    @L_80038494:
    cmpwi r25, 0xa
    beq @L_800387E8
    b @L_80038894
    @L_800384A0:
    cmpwi r25, 0x28
    beq @L_80038878
    bge @L_800384B8
    cmpwi r25, 0x1e
    beq @L_80038810
    b @L_80038894
    @L_800384B8:
    cmpwi r25, 0x3e8
    beq @L_80038880
    b @L_80038894
    @L_800384C4:
    li r0, 0x1b5a
    addi r27, r30, 0x0
    stw r0, lbl_8047A480(r13)
    addi r25, r29, 0xb4
    li r26, 0x0
    @L_800384D8:
    mr r3, r25
    mr r4, r27
    li r5, 0x18
    bl memcpy
    addi r27, r27, 0x18
    addi r25, r25, 0x18
    addi r26, r26, 0x1
    cmpwi r26, 0x4
    blt @L_800384D8
    addi r26, r30, 0x60
    addi r25, r29, 0x54
    li r27, 0x0
    @L_80038508:
    mr r3, r25
    mr r4, r26
    li r5, 0x18
    bl memcpy
    addi r26, r26, 0x18
    addi r25, r25, 0x18
    addi r27, r27, 0x1
    cmpwi r27, 0x4
    blt @L_80038508
    lfs f0, 0xe0(r30)
    li r0, 0x0
    stb r0, lbl_8047A47C(r13)
    li r3, 0x18
    li r4, 0x0
    stfs f0, lbl_8047A488(r13)
    stfs f0, lbl_8047A484(r13)
    stb r0, lbl_8047A47D(r13)
    bl fn_8010264C
    li r3, 0x1e
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x1f
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x1a
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x1b
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x19
    li r4, 0x0
    bl fn_8010264C
    li r25, 0x1
    b @L_80038894
    @L_80038594:
    li r0, -0x1
    li r3, 0x20
    stb r0, lbl_8047A490(r13)
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x21
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x22
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x23
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x1d
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, 0x0
    blt @L_800387E0
    extsb r0, r3
    cmpwi r3, 0x2
    stb r0, lbl_8047A490(r13)
    beq @L_80038740
    bge @L_80038604
    cmpwi r3, 0x0
    beq @L_80038610
    bge @L_800386A8
    b @L_80038894
    @L_80038604:
    cmpwi r3, 0x4
    bge @L_80038894
    b @L_800387D8
    @L_80038610:
    li r3, 0x20
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r0, 0x1b5a
    addi r26, r30, 0x0
    stw r0, lbl_8047A480(r13)
    addi r25, r29, 0xb4
    li r27, 0x0
    @L_80038634:
    mr r3, r25
    mr r4, r26
    li r5, 0x18
    bl memcpy
    addi r26, r26, 0x18
    addi r25, r25, 0x18
    addi r27, r27, 0x1
    cmpwi r27, 0x4
    blt @L_80038634
    addi r26, r30, 0x60
    addi r25, r29, 0x54
    li r27, 0x0
    @L_80038664:
    mr r3, r25
    mr r4, r26
    li r5, 0x18
    bl memcpy
    addi r26, r26, 0x18
    addi r25, r25, 0x18
    addi r27, r27, 0x1
    cmpwi r27, 0x4
    blt @L_80038664
    lfs f0, 0xe0(r30)
    li r0, 0x0
    stb r0, lbl_8047A47C(r13)
    li r25, 0xa
    stfs f0, lbl_8047A488(r13)
    stfs f0, lbl_8047A484(r13)
    stb r0, lbl_8047A47D(r13)
    b @L_80038894
    @L_800386A8:
    li r3, 0x20
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r0, 0x1b5a
    addi r26, r30, 0x0
    stw r0, lbl_8047A480(r13)
    addi r25, r29, 0xb4
    li r27, 0x0
    @L_800386CC:
    mr r3, r25
    mr r4, r26
    li r5, 0x18
    bl memcpy
    addi r26, r26, 0x18
    addi r25, r25, 0x18
    addi r27, r27, 0x1
    cmpwi r27, 0x4
    blt @L_800386CC
    addi r26, r30, 0x60
    addi r25, r29, 0x54
    li r27, 0x0
    @L_800386FC:
    mr r3, r25
    mr r4, r26
    li r5, 0x18
    bl memcpy
    addi r26, r26, 0x18
    addi r25, r25, 0x18
    addi r27, r27, 0x1
    cmpwi r27, 0x4
    blt @L_800386FC
    lfs f0, 0xe0(r30)
    li r0, 0x0
    stb r0, lbl_8047A47C(r13)
    li r25, 0x14
    stfs f0, lbl_8047A488(r13)
    stfs f0, lbl_8047A484(r13)
    stb r0, lbl_8047A47D(r13)
    b @L_80038894
    @L_80038740:
    li r3, 0x20
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r0, 0x1b5a
    addi r26, r30, 0x0
    stw r0, lbl_8047A480(r13)
    addi r25, r29, 0xb4
    li r27, 0x0
    @L_80038764:
    mr r3, r25
    mr r4, r26
    li r5, 0x18
    bl memcpy
    addi r26, r26, 0x18
    addi r25, r25, 0x18
    addi r27, r27, 0x1
    cmpwi r27, 0x4
    blt @L_80038764
    addi r26, r30, 0x60
    addi r25, r29, 0x54
    li r27, 0x0
    @L_80038794:
    mr r3, r25
    mr r4, r26
    li r5, 0x18
    bl memcpy
    addi r26, r26, 0x18
    addi r25, r25, 0x18
    addi r27, r27, 0x1
    cmpwi r27, 0x4
    blt @L_80038794
    lfs f0, 0xe0(r30)
    li r0, 0x0
    stb r0, lbl_8047A47C(r13)
    li r25, 0x1e
    stfs f0, lbl_8047A488(r13)
    stfs f0, lbl_8047A484(r13)
    stb r0, lbl_8047A47D(r13)
    b @L_80038894
    @L_800387D8:
    li r25, 0x3e8
    b @L_80038894
    @L_800387E0:
    li r25, 0x3e8
    b @L_80038894
    @L_800387E8:
    bl fn_80058150
    li r25, 0x1
    b @L_80038894
    @L_800387F4:
    li r0, 0x1b61
    stw r0, lbl_8047A480(r13)
    bl fn_8003A520
    li r0, 0x1b5a
    li r25, 0x1
    stw r0, lbl_8047A480(r13)
    b @L_80038894
    @L_80038810:
    lha r4, 0x5fd6(r31)
    addi r3, r29, 0x0
    lha r5, 0x5fd8(r31)
    addi r0, r4, 0xa
    extsh r4, r0
    bl fn_8010A5BC
    lis r4, 0xf7
    addi r3, r29, 0x0
    addi r4, r4, 0x400
    bl fn_8010A010
    lis r3, 0xf7
    addi r3, r3, 0x400
    bl fn_8018F6F4
    addi r5, r1, 0xc
    addi r6, r1, 0x8
    li r4, 0x1
    bl fn_8018F4C8
    lwz r4, 0xc(r1)
    addi r3, r29, 0x0
    bl fn_80109894
    bl fn_80038A0C
    mr r0, r3
    addi r3, r29, 0x0
    mr r25, r0
    bl fn_8010A420
    b @L_80038894
    @L_80038878:
    li r25, 0x3e8
    b @L_80038894
    @L_80038880:
    li r3, 0x1d
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r28, 0x0
    @L_80038894:
    cmpwi r28, 0x0
    bne @L_80038470
    li r3, 0x3e8
    li r4, 0x1
    li r5, 0x1
    bl fn_801660D8
    bl fn_800FF660
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
