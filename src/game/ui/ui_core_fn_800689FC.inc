    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r31, r4
    mr r29, r5
    li r30, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80068A48
    bge @L_80068A48
    cmpwi r3, 0x0
    bge @L_80068A3C
    b @L_80068A48
@L_80068A3C:
    cmpwi r29, 0x2
    blt @L_80068A48
    li r30, 0x0
@L_80068A48:
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    beq @L_80068B94
    lha r0, 0x6(r31)
    lis r3, lbl_802EDA20@ha
    addi r3, r3, lbl_802EDA20@l
    li r4, 0x0
    clrlwi r5, r0, 16
    li r0, 0x8
    mtctr r0
@L_80068A70:
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068A84
    lhz r0, 0x2(r3)
    b @L_80068B54
@L_80068A84:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068A9C
    lhz r0, 0x2(r3)
    b @L_80068B54
@L_80068A9C:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068AB4
    lhz r0, 0x2(r3)
    b @L_80068B54
@L_80068AB4:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068ACC
    lhz r0, 0x2(r3)
    b @L_80068B54
@L_80068ACC:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068AE4
    lhz r0, 0x2(r3)
    b @L_80068B54
@L_80068AE4:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068AFC
    lhz r0, 0x2(r3)
    b @L_80068B54
@L_80068AFC:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068B14
    lhz r0, 0x2(r3)
    b @L_80068B54
@L_80068B14:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068B2C
    lhz r0, 0x2(r3)
    b @L_80068B54
@L_80068B2C:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068B44
    lhz r0, 0x2(r3)
    b @L_80068B54
@L_80068B44:
    addi r3, r3, 0x4
    addi r4, r4, 0x8
    bdnz @L_80068A70
    li r0, 0x0
@L_80068B54:
    clrlwi r4, r0, 16
    mr r3, r29
    bl fn_8025D970
    bl fn_801230E0
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_80068B84
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_80068B94
@L_80068B84:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
@L_80068B94:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
