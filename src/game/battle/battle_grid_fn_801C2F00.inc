    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    lis r5, lbl_802757F0@ha
    stw r0, 0x44(r1)
    stmw r27, 0x2c(r1)
    mr r27, r4
    mr r28, r3
    lwzu r4, lbl_802757F0@l(r5)
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r4, 0x14(r1)
    stw r3, 0x18(r1)
    stw r0, 0x1c(r1)
    bl fn_80176F68
    mr r3, r28
    mr r4, r27
    bl fn_80176C04
    mr r0, r3
    mr r3, r28
    mr r5, r0
    mr r4, r27
    li r6, 0x1
    bl fn_80176C78
    addi r3, r1, 0x14
    bl fn_80176658
    addi r3, r1, 0x14
    bl fn_8017662C
    lis r3, lbl_80466DE8@ha
    li r30, -0x2
    addi r29, r3, lbl_80466DE8@l
    lhz r0, 0x40(r29)
    cmplwi r0, 0x0
    bne @L_801C2F8C
    li r30, 0x0
    b @L_801C2FF0
    @L_801C2F8C:
    li r27, 0x0
    b @L_801C2FE4
    @L_801C2F94:
    lwz r0, 0x0(r29)
    cmplwi r0, 0x0
    beq @L_801C2FDC
    addi r28, r29, 0x4
    li r31, 0x0
    b @L_801C2FD0
    @L_801C2FAC:
    lwz r3, 0x0(r28)
    cmplwi r3, 0x0
    beq @L_801C2FC8
    bl fn_801DAC24
    cmpw r3, r30
    ble @L_801C2FC8
    mr r30, r3
    @L_801C2FC8:
    addi r31, r31, 0x1
    addi r28, r28, 0x4
    @L_801C2FD0:
    clrlwi r0, r31, 16
    cmplwi r0, 0x2
    blt @L_801C2FAC
    @L_801C2FDC:
    addi r27, r27, 0x1
    addi r29, r29, 0x10
    @L_801C2FE4:
    clrlwi r0, r27, 16
    cmplwi r0, 0x4
    blt @L_801C2F94
    @L_801C2FF0:
    bl fn_80115BD8
    mr r31, r3
    bl fn_80113F48
    cmpwi r30, 0x2
    mr r28, r3
    li r29, 0x0
    beq @L_801C3030
    bge @L_801C301C
    cmpwi r30, 0x1
    bge @L_801C3028
    b @L_801C3040
    @L_801C301C:
    cmpwi r30, 0x4
    bge @L_801C3040
    b @L_801C3038
    @L_801C3028:
    lfs f1, lbl_8047DF5C(r2)
    b @L_801C3044
    @L_801C3030:
    lfs f1, lbl_8047DF60(r2)
    b @L_801C3044
    @L_801C3038:
    lfs f1, lbl_8047DF64(r2)
    b @L_801C3044
    @L_801C3040:
    lfs f1, lbl_8047DF68(r2)
    @L_801C3044:
    fmr f2, f1
    addi r3, r1, 0x8
    fmr f3, f1
    bl fn_800E01F4
    bl fn_80113F48
    lwz r4, 0x8(r31)
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801C30D0
    lis r4, lbl_802757FC@ha
    addi r4, r4, lbl_802757FC@l
    bl fn_80191ECC
    mr. r30, r3
    beq @L_801C30D0
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801C30D0
    lwz r3, 0x8(r31)
    bl fn_80115274
    mr r31, r3
    li r27, 0x0
    b @L_801C30C0
    @L_801C309C:
    mr r3, r28
    or r4, r31, r29
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801C30B8
    addi r4, r1, 0x8
    bl fn_800E407C
    @L_801C30B8:
    addi r27, r27, 0x4
    addi r29, r29, 0x1
    @L_801C30C0:
    lwz r3, 0x0(r30)
    lwzx r0, r3, r27
    cmplwi r0, 0x0
    bne @L_801C309C
    @L_801C30D0:
    addi r3, r1, 0x8
    bl fn_80176600
    li r3, 0x0
    bl fn_800D1070
    bl fn_80177A64
    li r3, 0xc8
    li r0, 0x0
    stw r3, lbl_80478CA8(r13)
    stb r0, lbl_8047B398(r13)
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
