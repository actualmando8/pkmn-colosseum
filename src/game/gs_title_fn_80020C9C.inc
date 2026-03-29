    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    li r31, 0x0
    li r30, 0x1
    b @L_80020CC8
    @L_80020CC4:
    bl fn_800F0308
    @L_80020CC8:
    li r3, 0xaa
    bl fn_801070F4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80020CC4
    li r3, 0x0
    bl fn_800205C0
    lis r3, lbl_803A1FC8@ha
    lfs f2, lbl_8047B86C(r2)
    addi r3, r3, lbl_803A1FC8@l
    li r0, 0x0
    lfs f1, lbl_8047B890(r2)
    lfs f0, lbl_8047B870(r2)
    stw r0, 0x0(r3)
    stfs f2, 0x14(r3)
    stfs f2, 0x10(r3)
    stw r0, 0x4(r3)
    stfs f2, 0x20(r3)
    stfs f1, 0x28(r3)
    stfs f0, 0x18(r3)
    stw r0, 0x8(r3)
    stw r0, 0xc(r3)
    @L_80020D20:
    cmpwi r31, 0x64
    beq @L_80020DE0
    bge @L_80020E74
    cmpwi r31, 0x0
    beq @L_80020D38
    b @L_80020E74
    @L_80020D38:
    bl fn_80166C74
    mr r0, r3
    li r3, 0x0
    mr r29, r0
    li r4, 0x9
    bl fn_80135168
    mr r0, r3
    li r3, 0x7f
    mr r28, r0
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x80
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x7c
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x7d
    li r4, 0x0
    bl fn_8010264C
    li r3, 0x7e
    li r4, 0x0
    bl fn_8010264C
    lis r3, lbl_803A1FC8@ha
    addi r31, r3, lbl_803A1FC8@l
    @L_80020D9C:
    li r3, 0x7b
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, 0x0
    bge @L_80020DCC
    mr r3, r29
    bl fn_80166CC0
    mr r5, r28
    li r3, 0x0
    li r4, 0x9
    bl fn_80135030
    b @L_80020DD8
    @L_80020DCC:
    lwz r0, 0x0(r31)
    cmpwi r0, 0x2
    bne @L_80020D9C
    @L_80020DD8:
    li r31, 0x64
    b @L_80020E74
    @L_80020DE0:
    lis r3, lbl_803A1FC8@ha
    li r0, -0x1
    addi r4, r3, lbl_803A1FC8@l
    li r3, 0x0
    stw r0, 0x0(r4)
    li r4, 0x9
    bl fn_80135168
    cmpw r28, r3
    beq @L_80020E60
    bl fn_801D04E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80020E60
    li r3, 0x1
    li r4, 0x3d82
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x0
    li r4, 0x3c
    li r5, 0xaa
    li r6, 0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80020E58
    li r3, 0x7
    li r4, 0x2
    li r5, 0x0
    bl fn_801D0748
    @L_80020E58:
    li r3, 0x1
    bl fn_801069FC
    @L_80020E60:
    li r3, 0x80
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r30, 0x0
    @L_80020E74:
    cmpwi r30, 0x0
    bne @L_80020D20
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
