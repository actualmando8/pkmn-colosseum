    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_80109958
    li r3, 0x0
    b @L_80109B7C
@L_80109958:
    lwz r0, 0x34(r31)
    cmplwi r0, 0x0
    bne @L_8010996C
    li r3, 0x0
    b @L_80109B7C
@L_8010996C:
    lbz r0, 0x0(r31)
    cmplwi r0, 0x2
    beq @L_80109980
    li r3, 0x0
    b @L_80109B7C
@L_80109980:
    lbz r0, 0x14(r31)
    cmplwi r0, 0x0
    beq @L_8010999C
    lwz r3, 0x24(r31)
    bl fn_801DAC3C
    mr r30, r3
    b @L_801099A0
@L_8010999C:
    lwz r30, 0x24(r31)
@L_801099A0:
    cmplwi r30, 0x0
    mr r28, r30
    bne @L_801099B4
    li r3, 0x0
    b @L_80109B7C
@L_801099B4:
    mr r3, r30
    bl fn_800EC134
    lis r4, lbl_8035B468@ha
    mr r3, r30
    addi r4, r4, lbl_8035B468@l
    bl fn_800E6478
    mr r30, r31
    li r27, 0x0
@L_801099D4:
    lwz r3, 0x3c(r30)
    cmplwi r3, 0x0
    beq @L_801099E8
    li r4, 0x1
    bl fn_800DCC34
@L_801099E8:
    addi r30, r30, 0x4
    addi r27, r27, 0x1
    cmpwi r27, 0x3
    blt @L_801099D4
    bl fn_800D2584
    mr r0, r3
    li r3, 0x2
    mr r29, r0
    bl fn_800D4604
    li r3, 0x2
    bl fn_800D377C
    addi r3, r1, 0x16
    addi r4, r1, 0x14
    addi r5, r1, 0x12
    addi r6, r1, 0x10
    bl fn_800D9B24
    addi r3, r1, 0xe
    addi r4, r1, 0xc
    addi r5, r1, 0xa
    addi r6, r1, 0x8
    bl fn_800D9AF0
    lwz r3, 0x38(r31)
    bl fn_800D258C
    lwz r3, 0x38(r31)
    bl fn_800D258C
    lwz r3, 0x34(r31)
    bl fn_800EF4FC
    subi r0, r3, 0x1
    lwz r3, 0x34(r31)
    rlwinm r30, r0, 0, 16, 31
    bl fn_800EF4F4
    subi r0, r3, 0x1
    mr r5, r30
    rlwinm r27, r0, 0, 16, 31
    li r3, 0x0
    mr r6, r27
    li r4, 0x0
    bl fn_800D9D68
    mr r5, r30
    mr r6, r27
    li r3, 0x0
    li r4, 0x0
    bl fn_800D9C24
    bl fn_800D2248
    lwz r3, 0x34(r31)
    li r4, 0x0
    bl fn_800D3410
    lbz r0, 0x14(r31)
    cmplwi r0, 0x0
    beq @L_80109AF0
    lwz r3, 0x24(r31)
    li r4, 0x1
    bl fn_801DA4E8
    lwz r3, 0x24(r31)
    li r4, 0x1
    bl fn_801DA448
    bl fn_801DB088
    lwz r3, 0x24(r31)
    bl fn_801DAAAC
    lwz r3, 0x24(r31)
    li r4, 0x0
    bl fn_801DA4E8
    lwz r3, 0x24(r31)
    li r4, 0x0
    bl fn_801DA448
    b @L_80109AFC
@L_80109AF0:
    lwz r3, 0x24(r31)
    li r4, 0x3010
    bl fn_800E3760
@L_80109AFC:
    bl fn_800D3190
    li r3, 0x1
    bl fn_800D377C
    mr r3, r29
    bl fn_800D258C
    lhz r3, 0x16(r1)
    lhz r4, 0x14(r1)
    lhz r5, 0x12(r1)
    lhz r6, 0x10(r1)
    bl fn_800D9D68
    lhz r3, 0xe(r1)
    lhz r4, 0xc(r1)
    lhz r5, 0xa(r1)
    lhz r6, 0x8(r1)
    bl fn_800D9C24
    bl fn_800D2248
    li r3, 0x1
    bl fn_800D4604
    mr r30, r31
    li r29, 0x0
@L_80109B4C:
    lwz r3, 0x3c(r30)
    cmplwi r3, 0x0
    beq @L_80109B60
    li r4, 0x0
    bl fn_800DCC34
@L_80109B60:
    addi r30, r30, 0x4
    addi r29, r29, 0x1
    cmpwi r29, 0x3
    blt @L_80109B4C
    mr r3, r28
    bl fn_800E638C
    lwz r3, 0x34(r31)
@L_80109B7C:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
