    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r5, 0x10
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r4
    li r4, 0x0
    bl memset
    cmplwi r28, 0x0
    beq @L_801ADF34
    lwz r0, 0x0(r28)
    cmplwi r0, 0x0
    beq @L_801ADEA0
    lwz r0, 0x0(r28)
    stw r0, 0x0(r31)
    b @L_801ADEAC
@L_801ADEA0:
    lis r3, -32741
    addi r0, r3, -8192
    stw r0, 0x0(r31)
@L_801ADEAC:
    lwz r29, 0x4(r28)
    li r0, 0x0
    addi r30, r1, 0x8
    cmplwi r29, 0x0
    stw r0, 0x8(r1)
    bne @L_801ADF18
    li r0, 0x0
    b @L_801ADF28
@L_801ADECC:
    lis r3, lbl_80465688@ha
    addi r3, r3, lbl_80465688@l
    bl fn_801AA4CC
    mr. r28, r3
    bne @L_801ADEF0
    addi r3, r2, -22864
    li r4, 0x485
    addi r5, r2, -22856
    bl fn_80196E10
@L_801ADEF0:
    mr r3, r28
    li r4, 0x0
    li r5, 0xc
    bl memset
    stw r28, 0x0(r30)
    lwz r0, 0x0(r29)
    addi r29, r29, 0x8
    lwz r3, 0x0(r30)
    stw r0, 0x4(r3)
    lwz r30, 0x0(r30)
@L_801ADF18:
    lwz r0, 0x4(r29)
    cmplwi r0, 0x0
    bne @L_801ADECC
    lwz r0, 0x8(r1)
@L_801ADF28:
    stw r0, 0x4(r31)
    li r0, -0x1
    stw r0, 0x8(r31)
@L_801ADF34:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
