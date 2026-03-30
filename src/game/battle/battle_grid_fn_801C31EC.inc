    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r3, lbl_80466DE8@ha
    li r4, 0x0
    stw r0, 0x34(r1)
    addi r3, r3, lbl_80466DE8@l
    li r5, 0x44
    stmw r27, 0x1c(r1)
    bl memset
    lis r3, lbl_80466DE8@ha
    li r4, -0x1
    addi r29, r3, lbl_80466DE8@l
    stw r4, lbl_80478CAC(r13)
    lhz r0, 0x40(r29)
    li r3, 0xc8
    stw r3, lbl_80478CA8(r13)
    li r27, -0x2
    cmplwi r0, 0x0
    bne @L_801C3240
    li r27, 0x0
    b @L_801C32A4
    @L_801C3240:
    li r31, 0x0
    b @L_801C3298
    @L_801C3248:
    lwz r0, 0x0(r29)
    cmplwi r0, 0x0
    beq @L_801C3290
    addi r28, r29, 0x4
    li r30, 0x0
    b @L_801C3284
    @L_801C3260:
    lwz r3, 0x0(r28)
    cmplwi r3, 0x0
    beq @L_801C327C
    bl fn_801DAC24
    cmpw r3, r27
    ble @L_801C327C
    mr r27, r3
    @L_801C327C:
    addi r30, r30, 0x1
    addi r28, r28, 0x4
    @L_801C3284:
    clrlwi r0, r30, 16
    cmplwi r0, 0x2
    blt @L_801C3260
    @L_801C3290:
    addi r31, r31, 0x1
    addi r29, r29, 0x10
    @L_801C3298:
    clrlwi r0, r31, 16
    cmplwi r0, 0x4
    blt @L_801C3248
    @L_801C32A4:
    bl fn_80115BD8
    mr r31, r3
    bl fn_80113F48
    cmpwi r27, 0x2
    mr r28, r3
    li r29, 0x0
    beq @L_801C32E4
    bge @L_801C32D0
    cmpwi r27, 0x1
    bge @L_801C32DC
    b @L_801C32F4
    @L_801C32D0:
    cmpwi r27, 0x4
    bge @L_801C32F4
    b @L_801C32EC
    @L_801C32DC:
    lfs f1, lbl_8047DF5C(r2)
    b @L_801C32F8
    @L_801C32E4:
    lfs f1, lbl_8047DF60(r2)
    b @L_801C32F8
    @L_801C32EC:
    lfs f1, lbl_8047DF64(r2)
    b @L_801C32F8
    @L_801C32F4:
    lfs f1, lbl_8047DF68(r2)
    @L_801C32F8:
    fmr f2, f1
    addi r3, r1, 0x8
    fmr f3, f1
    bl fn_800E01F4
    bl fn_80113F48
    lwz r4, 0x8(r31)
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801C3384
    lis r4, lbl_802757FC@ha
    addi r4, r4, lbl_802757FC@l
    bl fn_80191ECC
    mr. r30, r3
    beq @L_801C3384
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801C3384
    lwz r3, 0x8(r31)
    bl fn_80115274
    mr r31, r3
    li r27, 0x0
    b @L_801C3374
    @L_801C3350:
    mr r3, r28
    or r4, r31, r29
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801C336C
    addi r4, r1, 0x8
    bl fn_800E407C
    @L_801C336C:
    addi r27, r27, 0x4
    addi r29, r29, 0x1
    @L_801C3374:
    lwz r3, 0x0(r30)
    lwzx r0, r3, r27
    cmplwi r0, 0x0
    bne @L_801C3350
    @L_801C3384:
    addi r3, r1, 0x8
    bl fn_80176600
    lbz r0, lbl_8047B39A(r13)
    li r28, 0x0
    lis r3, lbl_80466DE8@ha
    stb r28, lbl_8047B399(r13)
    cmplwi r0, 0x0
    addi r27, r3, lbl_80466DE8@l
    beq @L_801C341C
    mr r30, r28
    b @L_801C3408
    @L_801C33B0:
    clrlwi r0, r28, 16
    la r4, lbl_8047B39C(r13)
    lwz r3, 0x0(r27)
    addi r28, r28, 0x1
    lbzx r4, r4, r0
    bl fn_801DA4E8
    li r29, 0x0
    la r31, lbl_8047B39C(r13)
    b @L_801C33F4
    @L_801C33D4:
    clrlslwi r3, r29, 16, 2
    clrlwi r0, r28, 16
    addi r3, r3, 0x4
    lbzx r4, r31, r0
    lwzx r3, r27, r3
    addi r28, r28, 0x1
    bl fn_801DA4E8
    addi r29, r29, 0x1
    @L_801C33F4:
    clrlwi r0, r29, 16
    cmplwi r0, 0x2
    blt @L_801C33D4
    addi r30, r30, 0x1
    addi r27, r27, 0x10
    @L_801C3408:
    clrlwi r0, r30, 16
    cmplwi r0, 0x4
    blt @L_801C33B0
    li r0, 0x0
    stb r0, lbl_8047B39A(r13)
    @L_801C341C:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
