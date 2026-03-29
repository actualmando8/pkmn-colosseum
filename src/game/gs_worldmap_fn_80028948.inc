    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r29, r3
    lbz r0, 0x1(r29)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_80028CCC
    bge @L_80028990
    cmpwi r0, 0x0
    beq @L_8002899C
    b @L_80028F90
    @L_80028990:
    cmpwi r0, 0x4
    bge @L_80028F90
    b @L_80028F78
    @L_8002899C:
    lbz r0, 0x2(r29)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80028F90
    lis r3, lbl_803A20DC@ha
    addi r31, r3, lbl_803A20DC@l
    li r30, 0x0
    stw r30, 0x0(r31)
    addi r3, r31, 0xd8
    stw r30, 0x24(r31)
    stw r30, 0x48(r31)
    stw r30, 0x6c(r31)
    stw r30, 0x90(r31)
    stw r30, 0xb4(r31)
    stw r30, 0x0(r3)
    stw r30, 0x24(r3)
    stw r30, 0x48(r3)
    stw r30, 0x6c(r3)
    stw r30, 0x90(r3)
    stw r30, 0xb4(r3)
    addi r3, r3, 0xd8
    stw r30, 0x0(r3)
    stw r30, 0x24(r3)
    stw r30, 0x48(r3)
    stw r30, 0x6c(r3)
    stw r30, 0x90(r3)
    stw r30, 0xb4(r3)
    addi r3, r3, 0xd8
    stw r30, 0x0(r3)
    stw r30, 0x24(r3)
    stw r30, 0x48(r3)
    stw r30, 0x6c(r3)
    stw r30, 0x90(r3)
    stw r30, 0xb4(r3)
    addi r3, r3, 0xd8
    stw r30, 0x0(r3)
    stw r30, 0x24(r3)
    stw r30, 0x48(r3)
    stw r30, 0x6c(r3)
    stw r30, 0x90(r3)
    stw r30, 0xb4(r3)
    lfs f31, lbl_8047B958(r2)
    @L_80028A44:
    bl fn_800E0BE4
    fcmpo cr0, f1, f31
    bgt @L_80028BB4
    mr r4, r31
    li r3, 0x0
    li r0, 0x3
    mtctr r0
    @L_80028A60:
    lwz r0, 0x0(r4)
    cmpwi r0, 0x0
    beq @L_80028B2C
    addi r4, r4, 0x24
    addi r3, r3, 0x1
    lwz r0, 0x0(r4)
    cmpwi r0, 0x0
    beq @L_80028B2C
    addi r4, r4, 0x24
    addi r3, r3, 0x1
    lwz r0, 0x0(r4)
    cmpwi r0, 0x0
    beq @L_80028B2C
    addi r4, r4, 0x24
    addi r3, r3, 0x1
    lwz r0, 0x0(r4)
    cmpwi r0, 0x0
    beq @L_80028B2C
    addi r4, r4, 0x24
    addi r3, r3, 0x1
    lwz r0, 0x0(r4)
    cmpwi r0, 0x0
    beq @L_80028B2C
    addi r4, r4, 0x24
    addi r3, r3, 0x1
    lwz r0, 0x0(r4)
    cmpwi r0, 0x0
    beq @L_80028B2C
    addi r4, r4, 0x24
    addi r3, r3, 0x1
    lwz r0, 0x0(r4)
    cmpwi r0, 0x0
    beq @L_80028B2C
    addi r4, r4, 0x24
    addi r3, r3, 0x1
    lwz r0, 0x0(r4)
    cmpwi r0, 0x0
    beq @L_80028B2C
    addi r4, r4, 0x24
    addi r3, r3, 0x1
    lwz r0, 0x0(r4)
    cmpwi r0, 0x0
    beq @L_80028B2C
    addi r4, r4, 0x24
    addi r3, r3, 0x1
    lwz r0, 0x0(r4)
    cmpwi r0, 0x0
    beq @L_80028B2C
    addi r4, r4, 0x24
    addi r3, r3, 0x1
    bdnz @L_80028A60
    @L_80028B2C:
    cmpwi r3, 0x1e
    bge @L_80028BB4
    mulli r4, r3, 0x24
    lis r3, lbl_803A20DC@ha
    li r5, 0x1
    addi r0, r3, lbl_803A20DC@l
    add r28, r0, r4
    stw r5, 0x0(r28)
    bl fn_800E0BE4
    lfs f0, lbl_8047B95C(r2)
    fmuls f0, f0, f1
    stfs f0, 0x4(r28)
    bl fn_800E0BE4
    lfs f2, lbl_8047B960(r2)
    lfs f0, lbl_8047B964(r2)
    fmuls f1, f2, f1
    stfs f1, 0x8(r28)
    stfs f0, 0xc(r28)
    bl fn_800E0BE4
    lfs f3, lbl_8047B968(r2)
    li r3, -0x100
    lfs f2, lbl_8047B964(r2)
    li r0, 0x80
    lfs f0, lbl_8047B930(r2)
    fmadds f1, f3, f1, f2
    stfs f1, 0x10(r28)
    stw r3, 0x14(r28)
    stb r0, 0x18(r28)
    stfs f0, 0x1c(r28)
    bl fn_800E0BE4
    lfs f2, lbl_8047B970(r2)
    lfs f0, lbl_8047B96C(r2)
    fmadds f0, f2, f1, f0
    stfs f0, 0x20(r28)
    @L_80028BB4:
    mr r4, r31
    li r3, 0x0
    li r0, 0xf
    mtctr r0
    @L_80028BC4:
    lwz r0, 0x0(r4)
    cmpwi r0, 0x0
    beq @L_80028C34
    beq @L_80028C34
    lfs f1, 0x1c(r4)
    lfs f0, lbl_8047B934(r2)
    fadds f1, f1, f0
    stfs f1, 0x1c(r4)
    lfs f0, 0x20(r4)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_80028BFC
    li r0, 0x0
    stw r0, 0x0(r4)
    @L_80028BFC:
    lfs f2, 0x1c(r4)
    lfs f1, 0x20(r4)
    lfs f0, lbl_8047B934(r2)
    fdivs f3, f2, f1
    lfs f2, 0x10(r4)
    lfs f1, lbl_8047B954(r2)
    fsubs f0, f0, f3
    fmuls f2, f2, f3
    fmuls f0, f1, f0
    stfs f2, 0xc(r4)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x18(r4)
    @L_80028C34:
    addi r4, r4, 0x24
    lwz r0, 0x0(r4)
    cmpwi r0, 0x0
    beq @L_80028CA8
    beq @L_80028CA8
    lfs f1, 0x1c(r4)
    lfs f0, lbl_8047B934(r2)
    fadds f1, f1, f0
    stfs f1, 0x1c(r4)
    lfs f0, 0x20(r4)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_80028C70
    li r0, 0x0
    stw r0, 0x0(r4)
    @L_80028C70:
    lfs f2, 0x1c(r4)
    lfs f1, 0x20(r4)
    lfs f0, lbl_8047B934(r2)
    fdivs f3, f2, f1
    lfs f2, 0x10(r4)
    lfs f1, lbl_8047B954(r2)
    fsubs f0, f0, f3
    fmuls f2, f2, f3
    fmuls f0, f1, f0
    stfs f2, 0xc(r4)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x18(r4)
    @L_80028CA8:
    addi r4, r4, 0x24
    addi r3, r3, 0x1
    bdnz @L_80028BC4
    addi r30, r30, 0x1
    cmpwi r30, 0x258
    blt @L_80028A44
    li r0, 0x1
    stb r0, 0x2(r29)
    b @L_80028F90
    @L_80028CCC:
    bl fn_800E0BE4
    lfs f0, lbl_8047B958(r2)
    fcmpo cr0, f1, f0
    bgt @L_80028E70
    lis r3, lbl_803A20DC@ha
    li r4, 0x0
    addi r3, r3, lbl_803A20DC@l
    li r0, 0x3
    mtctr r0
    @L_80028CF0:
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80028DBC
    addi r3, r3, 0x24
    addi r4, r4, 0x1
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80028DBC
    addi r3, r3, 0x24
    addi r4, r4, 0x1
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80028DBC
    addi r3, r3, 0x24
    addi r4, r4, 0x1
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80028DBC
    addi r3, r3, 0x24
    addi r4, r4, 0x1
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80028DBC
    addi r3, r3, 0x24
    addi r4, r4, 0x1
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80028DBC
    addi r3, r3, 0x24
    addi r4, r4, 0x1
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80028DBC
    addi r3, r3, 0x24
    addi r4, r4, 0x1
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80028DBC
    addi r3, r3, 0x24
    addi r4, r4, 0x1
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80028DBC
    addi r3, r3, 0x24
    addi r4, r4, 0x1
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80028DBC
    addi r3, r3, 0x24
    addi r4, r4, 0x1
    bdnz @L_80028CF0
    @L_80028DBC:
    cmpwi r4, 0x1e
    bge @L_80028E70
    mulli r28, r4, 0x24
    lis r3, lbl_803A20DC@ha
    li r0, 0x1
    addi r3, r3, lbl_803A20DC@l
    stwx r0, r3, r28
    bl fn_800E0BE4
    lfs f0, lbl_8047B95C(r2)
    lis r3, lbl_803A20DC@ha
    addi r0, r3, lbl_803A20DC@l
    fmuls f0, f0, f1
    add r3, r0, r28
    stfs f0, 0x4(r3)
    bl fn_800E0BE4
    lfs f2, lbl_8047B960(r2)
    lis r3, lbl_803A20DC@ha
    addi r0, r3, lbl_803A20DC@l
    lfs f0, lbl_8047B964(r2)
    fmuls f1, f2, f1
    add r3, r0, r28
    stfs f1, 0x8(r3)
    stfs f0, 0xc(r3)
    bl fn_800E0BE4
    lfs f2, lbl_8047B968(r2)
    lis r3, lbl_803A20DC@ha
    lfs f0, lbl_8047B964(r2)
    addi r0, r3, lbl_803A20DC@l
    add r4, r0, r28
    li r3, -0x100
    fmadds f1, f2, f1, f0
    li r0, 0x80
    lfs f0, lbl_8047B930(r2)
    stfs f1, 0x10(r4)
    stw r3, 0x14(r4)
    stb r0, 0x18(r4)
    stfs f0, 0x1c(r4)
    bl fn_800E0BE4
    lfs f2, lbl_8047B970(r2)
    lis r3, lbl_803A20DC@ha
    lfs f0, lbl_8047B96C(r2)
    addi r0, r3, lbl_803A20DC@l
    add r3, r0, r28
    fmadds f0, f2, f1, f0
    stfs f0, 0x20(r3)
    @L_80028E70:
    lis r3, lbl_803A20DC@ha
    li r4, 0x0
    addi r3, r3, lbl_803A20DC@l
    li r0, 0xf
    mtctr r0
    @L_80028E84:
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80028EF4
    beq @L_80028EF4
    lfs f1, 0x1c(r3)
    lfs f0, lbl_8047B934(r2)
    fadds f1, f1, f0
    stfs f1, 0x1c(r3)
    lfs f0, 0x20(r3)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_80028EBC
    li r0, 0x0
    stw r0, 0x0(r3)
    @L_80028EBC:
    lfs f2, 0x1c(r3)
    lfs f1, 0x20(r3)
    lfs f0, lbl_8047B934(r2)
    fdivs f3, f2, f1
    lfs f2, 0x10(r3)
    lfs f1, lbl_8047B954(r2)
    fsubs f0, f0, f3
    fmuls f2, f2, f3
    fmuls f0, f1, f0
    stfs f2, 0xc(r3)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x18(r3)
    @L_80028EF4:
    addi r3, r3, 0x24
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80028F68
    beq @L_80028F68
    lfs f1, 0x1c(r3)
    lfs f0, lbl_8047B934(r2)
    fadds f1, f1, f0
    stfs f1, 0x1c(r3)
    lfs f0, 0x20(r3)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_80028F30
    li r0, 0x0
    stw r0, 0x0(r3)
    @L_80028F30:
    lfs f2, 0x1c(r3)
    lfs f1, 0x20(r3)
    lfs f0, lbl_8047B934(r2)
    fdivs f3, f2, f1
    lfs f2, 0x10(r3)
    lfs f1, lbl_8047B954(r2)
    fsubs f0, f0, f3
    fmuls f2, f2, f3
    fmuls f0, f1, f0
    stfs f2, 0xc(r3)
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x18(r3)
    @L_80028F68:
    addi r3, r3, 0x24
    addi r4, r4, 0x1
    bdnz @L_80028E84
    b @L_80028F90
    @L_80028F78:
    lbz r0, 0x2(r29)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80028F90
    li r0, 0x1
    stb r0, 0x2(r29)
    @L_80028F90:
    li r3, 0x0
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
