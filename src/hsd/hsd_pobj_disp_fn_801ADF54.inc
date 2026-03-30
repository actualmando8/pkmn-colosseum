    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    li r0, 0x0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    addi r30, r1, 0x8
    stw r29, 0x14(r1)
    mr. r29, r3
    stw r0, 0x8(r1)
    bne @L_801ADFD4
    li r3, 0x0
    b @L_801ADFE4
@L_801ADF88:
    lis r3, lbl_80465688@ha
    addi r3, r3, lbl_80465688@l
    bl fn_801AA4CC
    mr. r31, r3
    bne @L_801ADFAC
    addi r3, r2, -22864
    li r4, 0x485
    addi r5, r2, -22856
    bl fn_80196E10
@L_801ADFAC:
    mr r3, r31
    li r4, 0x0
    li r5, 0xc
    bl memset
    stw r31, 0x0(r30)
    lwz r0, 0x0(r29)
    addi r29, r29, 0x8
    lwz r3, 0x0(r30)
    stw r0, 0x4(r3)
    lwz r30, 0x0(r30)
@L_801ADFD4:
    lwz r0, 0x4(r29)
    cmplwi r0, 0x0
    bne @L_801ADF88
    lwz r3, 0x8(r1)
@L_801ADFE4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
