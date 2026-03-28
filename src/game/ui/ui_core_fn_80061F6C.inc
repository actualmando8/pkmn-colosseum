    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r27, 0x2c(r1)
    mr r30, r3
    mr r28, r4
    bl fn_8025DA88
    mr r31, r3
    li r29, 0x1
    bl fn_8025DA88
    cmpwi r31, 0x2
    bne @L_80061FAC
    cmpwi r3, 0x2
    beq @L_80061FB8
    li r29, 0x0
    b @L_80061FB8
@L_80061FAC:
    cmpwi r3, 0x2
    bne @L_80061FB8
    li r29, 0x0
@L_80061FB8:
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    beq @L_80062030
    li r3, 0x1
    bl fn_8025D914
    bl fn_8012AC54
    lis r4, lbl_803A9A60@ha
    addi r4, r4, lbl_803A9A60@l
    addi r27, r4, 0x3c4
    cmplwi r27, 0x0
    mr r4, r3
    beq @L_80061FF4
    mr r3, r27
    bl fn_800F9E70
    b @L_80062008
@L_80061FF4:
    li r3, 0x1
    bl fn_800FA280
    mr r4, r3
    mr r3, r27
    bl fn_800F9E70
@L_80062008:
    li r3, 0x1
    bl fn_8025D28C
    clrlwi r3, r3, 16
    bl fn_801FCCC4
    bl fn_801FCC64
    bl fn_801FBD58
    bl fn_801FBD28
    lis r4, lbl_803A9A60@ha
    addi r4, r4, lbl_803A9A60@l
    stw r3, 0x3dc(r4)
@L_80062030:
    bl fn_8025DBB0
    lis r4, lbl_803A9A60@ha
    li r0, 0x0
    addi r4, r4, lbl_803A9A60@l
    stw r3, 0x3c0(r4)
    stw r30, 0x0(r4)
    stw r28, 0x4(r4)
    stw r0, 0x38(r4)
    bl fn_8005FFE4
    bl fn_80062334
    lis r3, lbl_803A9A60@ha
    lfs f0, lbl_8047BF60(r2)
    addi r3, r3, lbl_803A9A60@l
    cmpwi r28, 0x1
    stfs f0, 0x3b4(r3)
    beq @L_80062170
    bge @L_80062270
    cmpwi r28, 0x0
    bge @L_80062080
    b @L_80062270
@L_80062080:
    bl fn_80068F84
    bl fn_800697C4
    bl fn_8025DBB0
    mr r31, r3
    bl fn_8025D9A8
    cmpwi r3, 0x2
    beq @L_8006211C
    bge @L_800620B0
    cmpwi r3, 0x0
    beq @L_800620BC
    bge @L_800620DC
    b @L_80062144
@L_800620B0:
    cmpwi r3, 0x4
    bge @L_80062144
    b @L_80062130
@L_800620BC:
    lis r4, lbl_802ED958@ha
    lis r3, lbl_803A9A60@ha
    slwi r0, r31, 2
    addi r4, r4, lbl_802ED958@l
    addi r3, r3, lbl_803A9A60@l
    lwzx r0, r4, r0
    stw r0, 0x3bc(r3)
    b @L_80062154
@L_800620DC:
    lis r3, 0x6666
    lis r4, lbl_802ED978@ha
    addi r0, r3, 0x6667
    lis r3, lbl_803A9A60@ha
    mulhw r0, r0, r31
    addi r4, r4, lbl_802ED978@l
    addi r3, r3, lbl_803A9A60@l
    srawi r0, r0, 2
    srwi r5, r0, 31
    add r0, r0, r5
    mulli r0, r0, 0xa
    subf r0, r0, r31
    slwi r0, r0, 2
    lwzx r0, r4, r0
    stw r0, 0x3bc(r3)
    b @L_80062154
@L_8006211C:
    lis r3, lbl_803A9A60@ha
    li r0, 0x3cd
    addi r3, r3, lbl_803A9A60@l
    stw r0, 0x3bc(r3)
    b @L_80062154
@L_80062130:
    lis r3, lbl_803A9A60@ha
    li r0, 0x3cd
    addi r3, r3, lbl_803A9A60@l
    stw r0, 0x3bc(r3)
    b @L_80062154
@L_80062144:
    lis r3, lbl_803A9A60@ha
    li r0, 0x3cd
    addi r3, r3, lbl_803A9A60@l
    stw r0, 0x3bc(r3)
@L_80062154:
    lis r3, lbl_803A9A60@ha
    li r4, 0x0
    addi r3, r3, lbl_803A9A60@l
    li r5, 0xff
    lwz r3, 0x3bc(r3)
    bl fn_80165A20
    b @L_80062270
@L_80062170:
    bl fn_80068F84
    bl fn_800697C4
    lis r3, lbl_803A9A60@ha
    li r29, 0x0
    addi r27, r3, lbl_803A9A60@l
@L_80062184:
    addi r28, r27, 0x58
    li r30, 0x0
@L_8006218C:
    mr r3, r29
    mr r4, r30
    bl fn_8025D938
    mr r31, r3
    cmplwi r31, 0x0
    beq @L_8006221C
    bl fn_8011F15C
    clrlwi r3, r3, 16
    lis r0, 0x4330
    stw r3, 0xc(r1)
    mr r3, r31
    lfd f1, lbl_8047BF88(r2)
    stw r0, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f1
    stfs f0, 0x6c(r28)
    bl fn_8011F188
    clrlwi r3, r3, 16
    lis r0, 0x4330
    stw r3, 0x14(r1)
    mr r3, r31
    lfd f1, lbl_8047BF88(r2)
    stw r0, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f1
    stfs f0, 0x84(r28)
    bl fn_8011F15C
    clrlwi r3, r3, 16
    lis r0, 0x4330
    stw r3, 0x1c(r1)
    lfd f1, lbl_8047BF88(r2)
    stw r0, 0x18(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f1
    stfs f0, 0x9c(r28)
    b @L_80062230
@L_8006221C:
    lfs f1, lbl_8047BF60(r2)
    lfs f0, lbl_8047BFAC(r2)
    stfs f1, 0x6c(r28)
    stfs f1, 0x84(r28)
    stfs f0, 0x9c(r28)
@L_80062230:
    addi r28, r28, 0x4
    addi r30, r30, 0x1
    cmpwi r30, 0x6
    blt @L_8006218C
    addi r27, r27, 0xb4
    addi r29, r29, 0x1
    cmpwi r29, 0x4
    blt @L_80062184
    lis r3, lbl_803A9A60@ha
    li r0, 0x1e
    addi r4, r3, lbl_803A9A60@l
    li r3, 0x1e
    stw r0, 0x3bc(r4)
    li r4, 0x0
    li r5, 0xff
    bl fn_80165A20
@L_80062270:
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
