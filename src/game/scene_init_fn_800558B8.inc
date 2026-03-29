    nofralloc
    stwu r1, -0x160(r1)
    mflr r0
    stw r0, 0x164(r1)
    stmw r27, 0x14c(r1)
    li r0, 0x0
    addi r30, r1, 0x8
    stw r0, 0x8(r1)
    li r3, 0x27
    li r4, 0x0
    li r5, 0xff
    stw r0, lbl_8047A560(r13)
    li r6, 0x0
    bl fn_80166A50
    @L_800558EC:
    mr r5, r30
    li r3, 0x93
    li r4, 0x10e
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    li r30, 0x0
    bne @L_8005595C
    bl fn_800576B4
    cmpwi r3, 0x3
    bne @L_80055934
    li r3, 0x1b7e
    li r4, 0x1
    bl fn_80058804
    b @L_800558EC
    @L_80055934:
    li r3, 0x1b8b
    li r4, 0x0
    bl fn_80058804
    li r3, 0x0
    bl fn_8005D3D0
    mr r29, r3
    bl fn_800587D8
    cmpwi r29, 0x0
    beq @L_800558EC
    b @L_80055B54
    @L_8005595C:
    lwz r0, lbl_8047A560(r13)
    cmpwi r0, 0x0
    beq @L_800559B4
    li r3, 0x0
    bl fn_800546F0
    b @L_80055978
    @L_80055974:
    bl fn_800F0308
    @L_80055978:
    bl fn_80054680
    cmpwi r3, 0x2
    beq @L_80055974
    li r3, 0x1
    bl fn_800599AC
    li r3, 0x0
    bl fn_800546C0
    b @L_8005599C
    @L_80055998:
    bl fn_800F0308
    @L_8005599C:
    bl fn_80054680
    cmpwi r3, 0x3
    beq @L_80055998
    li r0, 0x0
    stw r0, lbl_8047A560(r13)
    b @L_800558EC
    @L_800559B4:
    cmpwi r3, 0x0
    blt @L_800559C4
    cmpwi r3, 0x20
    blt @L_800559CC
    @L_800559C4:
    li r0, 0x3
    b @L_800559E4
    @L_800559CC:
    mulli r4, r3, 0xc
    lis r3, lbl_80267398@ha
    addi r0, r3, lbl_80267398@l
    add r3, r0, r4
    lwz r31, 0x8(r3)
    lwz r0, 0x4(r3)
    @L_800559E4:
    cmpwi r0, 0x1
    beq @L_80055A6C
    bge @L_800559FC
    cmpwi r0, 0x0
    bge @L_80055A54
    b @L_800558EC
    @L_800559FC:
    cmpwi r0, 0x3
    bge @L_800558EC
    li r3, 0x0
    bl fn_800546F0
    b @L_80055A14
    @L_80055A10:
    bl fn_800F0308
    @L_80055A14:
    bl fn_80054680
    cmpwi r3, 0x2
    beq @L_80055A10
    li r3, 0x0
    bl fn_800599AC
    li r3, 0x0
    bl fn_800546C0
    b @L_80055A38
    @L_80055A34:
    bl fn_800F0308
    @L_80055A38:
    bl fn_80054680
    cmpwi r3, 0x3
    beq @L_80055A34
    li r0, 0x0
    addi r30, r1, 0x8
    stw r0, 0x8(r1)
    b @L_800558EC
    @L_80055A54:
    bl fn_80056A78
    mr r4, r31
    bl fn_800552D4
    stw r3, 0x8(r1)
    addi r30, r1, 0x8
    b @L_800558EC
    @L_80055A6C:
    li r28, 0x0
    li r3, 0x1b87
    li r4, 0x0
    bl fn_80058804
    bl fn_8005D26C
    mr r29, r3
    bl fn_800587D8
    cmpwi r29, 0x0
    beq @L_80055A94
    b @L_800558EC
    @L_80055A94:
    lfs f1, lbl_8047BE98(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    bl fn_8005471C
    bl fn_80056A80
    bl fn_800576B4
    cmpwi r3, 0x3
    bne @L_80055AE8
    li r28, 0x1
    bl fn_800574E0
    li r0, 0x27
    addi r5, r1, 0x8
    subi r4, r3, 0x4
    mtctr r0
    @L_80055AD4:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80055AD4
    @L_80055AE8:
    bl fn_80057694
    mr r29, r3
    bl fn_80057A38
    bl fn_80056A78
    mr r27, r3
    li r3, 0x3
    mr r4, r27
    bl fn_80057B34
    mr r3, r27
    li r4, 0x0
    bl fn_80056B74
    li r3, 0x0
    li r4, 0x0
    bl fn_80054760
    cmpwi r28, 0x0
    beq @L_80055B30
    addi r3, r1, 0xc
    b @L_80055B34
    @L_80055B30:
    li r3, 0x0
    @L_80055B34:
    mr r4, r29
    bl fn_80057A64
    lfs f1, lbl_8047BE98(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    b @L_800558EC
    @L_80055B54:
    li r3, 0x28
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_80166A50
    li r3, 0x93
    li r4, 0x2
    li r5, 0x0
    bl fn_80102568
    li r3, 0x93
    li r4, 0x1
    bl fn_80102428
    lmw r27, 0x14c(r1)
    lwz r0, 0x164(r1)
    mtlr r0
    addi r1, r1, 0x160
    blr
