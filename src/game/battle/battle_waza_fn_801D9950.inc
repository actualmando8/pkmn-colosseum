    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmpwi r5, 0x1
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr r30, r3
    beq @L_801D99D0
    bge @L_801D9990
    cmpwi r5, -0x1
    beq @L_801D99B8
    bge @L_801D9A18
    cmpwi r5, -0x2
    bge @L_801D99A0
    b @L_801D9A18
@L_801D9990:
    cmpwi r5, 0x3
    beq @L_801D9A00
    bge @L_801D9A18
    b @L_801D99E8
@L_801D99A0:
    lfs f1, -21336(r2)
    mr r3, r31
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    b @L_801D9A2C
@L_801D99B8:
    lfs f1, -21324(r2)
    mr r3, r31
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    b @L_801D9A2C
@L_801D99D0:
    lfs f1, -21320(r2)
    mr r3, r31
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    b @L_801D9A2C
@L_801D99E8:
    lfs f1, -21316(r2)
    mr r3, r31
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    b @L_801D9A2C
@L_801D9A00:
    lfs f1, -21312(r2)
    mr r3, r31
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    b @L_801D9A2C
@L_801D9A18:
    lfs f1, -21332(r2)
    mr r3, r31
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
@L_801D9A2C:
    lhz r0, 0x2c(r30)
    cmpwi r0, 0x133
    beq @L_801D9B68
    bge @L_801D9A78
    cmpwi r0, 0xc4
    beq @L_801D9ADC
    bge @L_801D9A60
    cmpwi r0, 0xb1
    beq @L_801D9AC8
    bge @L_801D9C04
    cmpwi r0, 0x8f
    beq @L_801D9AA8
    b @L_801D9C04
@L_801D9A60:
    cmpwi r0, 0x119
    beq @L_801D9B54
    bge @L_801D9C04
    cmpwi r0, 0xfa
    beq @L_801D9B18
    b @L_801D9C04
@L_801D9A78:
    cmpwi r0, 0x149
    beq @L_801D9BC8
    bge @L_801D9A9C
    cmpwi r0, 0x143
    beq @L_801D9BA8
    bge @L_801D9C04
    cmpwi r0, 0x13b
    beq @L_801D9B88
    b @L_801D9C04
@L_801D9A9C:
    cmpwi r0, 0x162
    beq @L_801D9BE8
    b @L_801D9C04
@L_801D9AA8:
    lhz r0, 0x2e(r30)
    cmplwi r0, 0x2
    bne @L_801D9C04
    lfs f1, -21308(r2)
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    b @L_801D9C04
@L_801D9AC8:
    lfs f1, -21336(r2)
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    b @L_801D9C04
@L_801D9ADC:
    lhz r0, 0x2e(r30)
    cmplwi r0, 0x1
    bne @L_801D9AFC
    lfs f1, -21304(r2)
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    b @L_801D9C04
@L_801D9AFC:
    cmplwi r0, 0x2
    bne @L_801D9C04
    lfs f1, -21336(r2)
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    b @L_801D9C04
@L_801D9B18:
    lhz r0, 0x2e(r30)
    cmplwi r0, 0x1
    bne @L_801D9B38
    lfs f1, -21300(r2)
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    b @L_801D9C04
@L_801D9B38:
    cmplwi r0, 0x2
    bne @L_801D9C04
    lfs f1, -21296(r2)
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    b @L_801D9C04
@L_801D9B54:
    lfs f1, -21292(r2)
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    b @L_801D9C04
@L_801D9B68:
    lhz r0, 0x2e(r30)
    cmplwi r0, 0x2
    bne @L_801D9C04
    lfs f1, -21292(r2)
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    b @L_801D9C04
@L_801D9B88:
    lhz r0, 0x2e(r30)
    cmplwi r0, 0x2
    bne @L_801D9C04
    lfs f1, -21292(r2)
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    b @L_801D9C04
@L_801D9BA8:
    lhz r0, 0x2e(r30)
    cmplwi r0, 0x2
    bne @L_801D9C04
    lfs f1, -21288(r2)
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    b @L_801D9C04
@L_801D9BC8:
    lhz r0, 0x2e(r30)
    cmplwi r0, 0x2
    bne @L_801D9C04
    lfs f1, -21284(r2)
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
    b @L_801D9C04
@L_801D9BE8:
    lhz r0, 0x2e(r30)
    cmplwi r0, 0x2
    bne @L_801D9C04
    lfs f1, -21284(r2)
    mr r3, r31
    mr r4, r31
    bl fn_800E013C
@L_801D9C04:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
