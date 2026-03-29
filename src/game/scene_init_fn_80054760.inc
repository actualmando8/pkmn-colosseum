    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, lbl_804788F8(r13)
    stw r4, lbl_8047A544(r13)
    cmpwi r0, 0x0
    beq @L_80054890
    lis r5, lbl_802E61E8@ha
    li r4, 0x0
    addi r5, r5, lbl_802E61E8@l
    lis r7, lbl_802EF0A8@ha
    li r6, 0x2
    addi r0, r7, lbl_802EF0A8@l
    mtctr r6
    @L_80054798:
    lwz r6, 0x0(r5)
    addi r4, r4, 0x8
    mulli r6, r6, 0x1c
    add r6, r0, r6
    lha r6, 0x4(r6)
    sth r6, 0x4(r5)
    lwz r6, 0x8(r5)
    mulli r6, r6, 0x1c
    add r6, r0, r6
    lha r6, 0x4(r6)
    sth r6, 0xc(r5)
    lwz r6, 0x10(r5)
    mulli r6, r6, 0x1c
    add r6, r0, r6
    lha r6, 0x4(r6)
    sth r6, 0x14(r5)
    lwz r6, 0x18(r5)
    mulli r6, r6, 0x1c
    add r6, r0, r6
    lha r6, 0x4(r6)
    sth r6, 0x1c(r5)
    lwz r6, 0x20(r5)
    mulli r6, r6, 0x1c
    add r6, r0, r6
    lha r6, 0x4(r6)
    sth r6, 0x24(r5)
    lwz r6, 0x28(r5)
    mulli r6, r6, 0x1c
    add r6, r0, r6
    lha r6, 0x4(r6)
    sth r6, 0x2c(r5)
    lwz r6, 0x30(r5)
    mulli r6, r6, 0x1c
    add r6, r0, r6
    lha r6, 0x4(r6)
    sth r6, 0x34(r5)
    lwz r6, 0x38(r5)
    mulli r6, r6, 0x1c
    add r6, r0, r6
    lha r6, 0x4(r6)
    sth r6, 0x3c(r5)
    addi r5, r5, 0x40
    bdnz @L_80054798
    lis r5, lbl_802E61E8@ha
    slwi r6, r4, 3
    addi r0, r5, lbl_802E61E8@l
    add r6, r0, r6
    lis r5, lbl_802EF0A8@ha
    subfic r0, r4, 0x11
    addi r5, r5, lbl_802EF0A8@l
    mtctr r0
    cmpwi r4, 0x11
    bge @L_80054888
    @L_8005486C:
    lwz r0, 0x0(r6)
    mulli r0, r0, 0x1c
    add r4, r5, r0
    lha r0, 0x4(r4)
    sth r0, 0x4(r6)
    addi r6, r6, 0x8
    bdnz @L_8005486C
    @L_80054888:
    li r0, 0x0
    stw r0, lbl_804788F8(r13)
    @L_80054890:
    cmpwi r3, 0x0
    bne @L_800548AC
    lfs f1, lbl_8047BE60(r2)
    lfs f0, lbl_8047BE68(r2)
    stfs f1, lbl_8047A558(r13)
    stfs f0, lbl_8047A554(r13)
    b @L_800548B8
    @L_800548AC:
    lfs f0, lbl_8047BE68(r2)
    stfs f0, lbl_8047A558(r13)
    stfs f0, lbl_8047A554(r13)
    @L_800548B8:
    li r3, 0xd34
    bl fn_8005D934
    lis r4, lbl_803A9720@ha
    lha r5, 0x8(r3)
    addi r0, r4, lbl_803A9720@l
    lha r4, 0x6(r3)
    mr r3, r0
    bl fn_8010A5BC
    lis r3, lbl_803A95E8@ha
    addi r3, r3, lbl_803A95E8@l
    bl fn_80124A60
    li r3, 0x8f
    li r4, 0x1f
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
