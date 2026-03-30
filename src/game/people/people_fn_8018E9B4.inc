    nofralloc
    stwu r1, -0x1a0(r1)
    mflr r0
    stw r0, 0x1a4(r1)
    stfd f31, 0x190(r1)
    psq_st f31, 0x198(r1), 0, 0
    stw r31, 0x18c(r1)
    stw r30, 0x188(r1)
    stw r29, 0x184(r1)
    mr r30, r3
    mr r31, r4
    mr r29, r5
    li r3, 0x0
    li r4, 0x2
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_8018EA24
    lbz r0, 0x1(r3)
    cmplwi r0, 0x0
    beq @L_8018EA24
    li r3, 0x1
    bl fn_800F7BC4
    rlwinm. r0, r3, 0, 22, 22
    beq @L_8018EA24
    mr r3, r30
    mr r4, r31
    bl fn_8018FC74
    li r3, 0x1
    b @L_8018ECC8
    @L_8018EA24:
    li r3, 0xff
    bl fn_80101B90
    mr r3, r30
    li r4, 0x700
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_8018ECB0
    lwz r5, 0x0(r31)
    addi r3, r1, 0x2c
    lwz r0, 0x4(r31)
    addi r4, r1, 0x44
    stw r5, 0x38(r1)
    stw r0, 0x3c(r1)
    lwz r0, 0x8(r31)
    stw r0, 0x40(r1)
    lwz r5, 0x0(r29)
    lwz r0, 0x4(r29)
    stw r5, 0x44(r1)
    stw r0, 0x48(r1)
    lwz r0, 0x8(r29)
    stw r0, 0x4c(r1)
    bl fn_800E01D0
    addi r3, r1, 0x20
    addi r4, r1, 0x38
    bl fn_800E01D0
    lfs f1, 0x30(r1)
    lfs f2, lbl_8047D890(r2)
    lfs f0, 0x24(r1)
    fadds f1, f1, f2
    fadds f0, f0, f2
    stfs f1, 0x30(r1)
    stfs f0, 0x24(r1)
    lwz r3, 0x30(r30)
    bl fn_8018F6F4
    cmplwi r3, 0x0
    bne @L_8018EABC
    lfs f31, lbl_8047D7EC(r2)
    b @L_8018EAC4
    @L_8018EABC:
    bl fn_8018F5E4
    fmr f31, f1
    @L_8018EAC4:
    mr r3, r30
    li r4, 0x800
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_8018EAFC
    fmr f1, f31
    addi r3, r1, 0x2c
    addi r4, r1, 0x20
    addi r5, r1, 0xb0
    bl fn_8011163C
    mr r5, r3
    addi r4, r1, 0xb0
    li r3, 0x2
    bl fn_8012BAF0
    @L_8018EAFC:
    lwz r6, 0x20(r1)
    mr r3, r30
    lwz r5, 0x24(r1)
    li r4, 0x400
    lwz r0, 0x28(r1)
    stw r6, 0x38(r1)
    stw r5, 0x3c(r1)
    stw r0, 0x40(r1)
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_8018EB5C
    lwz r3, 0x50(r30)
    addi r4, r1, 0x44
    addi r5, r1, 0x38
    addi r6, r1, 0x14
    bl fn_8010FDF8
    cmpwi r3, 0x6
    bne @L_8018EB5C
    lwz r4, 0x14(r1)
    lwz r3, 0x18(r1)
    lwz r0, 0x1c(r1)
    stw r4, 0x38(r1)
    stw r3, 0x3c(r1)
    stw r0, 0x40(r1)
    @L_8018EB5C:
    mr r3, r30
    li r4, 0x100
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_8018EBAC
    fmr f1, f31
    addi r3, r1, 0x2c
    addi r4, r1, 0x38
    addi r5, r1, 0x14
    bl fn_8010F320
    cmpwi r3, 0x0
    beq @L_8018EBAC
    addi r3, r1, 0x14
    addi r4, r1, 0x38
    addi r5, r1, 0x8
    bl fn_800A3A9C
    addi r3, r1, 0x38
    addi r4, r1, 0x8
    mr r5, r3
    bl fn_800A3A78
    @L_8018EBAC:
    mr r3, r30
    li r4, 0x800
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_8018EBE0
    addi r3, r1, 0x2c
    addi r4, r1, 0x38
    addi r5, r1, 0xb0
    bl fn_801101B4
    mr r5, r3
    addi r4, r1, 0xb0
    li r3, 0x1
    bl fn_8012BAF0
    @L_8018EBE0:
    mr r3, r30
    li r4, 0x200
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_8018EC98
    addi r3, r1, 0x38
    addi r4, r1, 0x50
    bl fn_8010E138
    cmpwi r3, 0x2
    blt @L_8018EC7C
    lfs f5, lbl_8047D894(r2)
    addi r4, r1, 0x50
    lfs f2, 0x3c(r1)
    li r0, 0x0
    fmr f4, f5
    lfs f0, lbl_8047D800(r2)
    mtctr r3
    cmpwi r3, 0x0
    ble @L_8018EC64
    @L_8018EC2C:
    lfs f3, 0x0(r4)
    fcmpo cr0, f4, f3
    bge @L_8018EC3C
    fmr f4, f3
    @L_8018EC3C:
    fsubs f1, f3, f2
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    beq @L_8018EC5C
    fcmpo cr0, f5, f3
    bge @L_8018EC5C
    fmr f5, f3
    li r0, 0x1
    @L_8018EC5C:
    addi r4, r4, 0xc
    bdnz @L_8018EC2C
    @L_8018EC64:
    cmpwi r0, 0x0
    beq @L_8018EC74
    stfs f5, 0x3c(r1)
    b @L_8018EC98
    @L_8018EC74:
    stfs f4, 0x3c(r1)
    b @L_8018EC98
    @L_8018EC7C:
    cmpwi r3, 0x0
    ble @L_8018EC90
    lfs f0, 0x50(r1)
    stfs f0, 0x3c(r1)
    b @L_8018EC98
    @L_8018EC90:
    lfs f0, lbl_8047D7A0(r2)
    stfs f0, 0x3c(r1)
    @L_8018EC98:
    lwz r3, 0x38(r1)
    lwz r0, 0x3c(r1)
    stw r3, 0x0(r31)
    stw r0, 0x4(r31)
    lwz r0, 0x40(r1)
    stw r0, 0x8(r31)
    @L_8018ECB0:
    lis r3, 0xff
    bl fn_80101B90
    mr r3, r30
    mr r4, r31
    bl fn_8018FC74
    li r3, 0x1
    @L_8018ECC8:
    psq_l f31, 0x198(r1), 0, 0
    lwz r0, 0x1a4(r1)
    lfd f31, 0x190(r1)
    lwz r31, 0x18c(r1)
    lwz r30, 0x188(r1)
    lwz r29, 0x184(r1)
    mtlr r0
    addi r1, r1, 0x1a0
    blr
