    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stw r31, 0x5c(r1)
    stw r30, 0x58(r1)
    stw r29, 0x54(r1)
    stw r28, 0x50(r1)
    mr. r28, r3
    beq @L_80140114
    lwz r31, 0x10(r28)
    bl fn_801779EC
    lis r5, lbl_80273060@ha
    lis r4, lbl_8027306C@ha
    addi r6, r5, lbl_80273060@l
    mr. r30, r3
    addi r5, r4, lbl_8027306C@l
    lwz r8, 0x0(r6)
    lwz r7, 0x4(r6)
    li r29, 0x0
    lwz r6, 0x8(r6)
    lwz r4, 0x0(r5)
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r8, 0x3c(r1)
    stw r7, 0x40(r1)
    stw r6, 0x44(r1)
    stw r4, 0x30(r1)
    stw r3, 0x34(r1)
    stw r0, 0x38(r1)
    beq @L_80140054
    bl fn_800D2248
    mr r3, r30
    addi r4, r1, 0x3c
    bl fn_800D1F84
    mr r3, r30
    addi r4, r1, 0x18
    addi r5, r1, 0x24
    bl fn_800D1EB8
    addi r3, r1, 0x18
    addi r4, r1, 0x24
    addi r5, r1, 0x3c
    bl fn_800E0168
    addi r3, r1, 0x18
    mr r4, r3
    bl fn_800E0060
    mr r3, r30
    addi r4, r1, 0x14
    addi r5, r1, 0xc
    addi r6, r1, 0x10
    addi r7, r1, 0x8
    bl fn_800D1FDC
    lfs f2, lbl_8047D334(r2)
    lfs f1, 0x14(r1)
    lfs f0, lbl_8047D328(r2)
    fmuls f2, f2, f1
    lfs f4, 0x10(r1)
    lfs f3, lbl_8047D330(r2)
    fmuls f1, f2, f0
    stfs f2, 0x14(r1)
    fmuls f0, f4, f3
    stfs f1, 0x14(r1)
    stfs f0, 0x10(r1)
    bl fn_800CE220
    lfs f2, lbl_8047D338(r2)
    addi r3, r1, 0x18
    lfs f5, 0x10(r1)
    frsp f4, f1
    lfs f0, lbl_8047D32C(r2)
    mr r4, r3
    fmuls f3, f2, f5
    lfs f2, 0xc(r1)
    fmr f1, f5
    stfs f0, 0x38(r1)
    fmuls f3, f3, f4
    fmuls f0, f3, f2
    stfs f3, 0x34(r1)
    stfs f0, 0x30(r1)
    bl fn_800E013C
    addi r3, r1, 0x3c
    addi r5, r1, 0x18
    mr r4, r3
    bl fn_800E019C
    @L_80140054:
    lwz r3, 0x20(r28)
    cmplwi r3, 0x0
    beq @L_8014007C
    bl fn_80118DA8
    cmplwi r3, 0x0
    beq @L_8014007C
    lwz r3, 0x20(r28)
    addi r4, r1, 0x3c
    bl fn_80118F04
    li r29, 0x1
    @L_8014007C:
    lwz r0, 0x10(r28)
    cmplwi r0, 0x0
    beq @L_8014010C
    mr r3, r31
    bl fn_800EC1BC
    clrlwi. r0, r3, 24
    beq @L_801400A8
    mr r3, r31
    bl fn_800EC954
    clrlwi. r0, r3, 24
    beq @L_801400C8
    @L_801400A8:
    mr r3, r31
    bl fn_800EC1B0
    clrlwi. r0, r3, 24
    beq @L_8014010C
    mr r3, r31
    bl fn_800EC1C8
    clrlwi. r0, r3, 24
    bne @L_8014010C
    @L_801400C8:
    mr r3, r31
    bl fn_800E3C5C
    lfs f2, 0x30(r1)
    addi r4, r1, 0x3c
    lfs f0, 0x28(r3)
    lfs f1, 0x34(r1)
    fdivs f0, f2, f0
    stfs f0, 0x30(r1)
    lfs f0, 0x2c(r3)
    mr r3, r31
    fdivs f0, f1, f0
    stfs f0, 0x34(r1)
    bl fn_800E43A4
    mr r3, r31
    addi r4, r1, 0x30
    bl fn_800E407C
    li r29, 0x1
    @L_8014010C:
    mr r3, r29
    b @L_80140118
    @L_80140114:
    li r3, 0x0
    @L_80140118:
    lwz r0, 0x64(r1)
    lwz r31, 0x5c(r1)
    lwz r30, 0x58(r1)
    lwz r29, 0x54(r1)
    lwz r28, 0x50(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
