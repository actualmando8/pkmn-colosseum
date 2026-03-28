    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r31, r3
    mr r28, r4
    mr r29, r5
    li r30, 0x1
    bl fn_8025DA88
    cmpwi r3, 0x2
    beq @L_80068E10
    bge @L_80068E10
    cmpwi r3, 0x0
    bge @L_80068E04
    b @L_80068E10
@L_80068E04:
    cmpwi r29, 0x2
    blt @L_80068E10
    li r30, 0x0
@L_80068E10:
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    beq @L_80068F64
    lha r0, 0x6(r28)
    lis r3, lbl_802EDA20@ha
    addi r3, r3, lbl_802EDA20@l
    li r4, 0x0
    clrlwi r5, r0, 16
    li r0, 0x8
    mtctr r0
@L_80068E38:
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068E4C
    lhz r0, 0x2(r3)
    b @L_80068F1C
@L_80068E4C:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068E64
    lhz r0, 0x2(r3)
    b @L_80068F1C
@L_80068E64:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068E7C
    lhz r0, 0x2(r3)
    b @L_80068F1C
@L_80068E7C:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068E94
    lhz r0, 0x2(r3)
    b @L_80068F1C
@L_80068E94:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068EAC
    lhz r0, 0x2(r3)
    b @L_80068F1C
@L_80068EAC:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068EC4
    lhz r0, 0x2(r3)
    b @L_80068F1C
@L_80068EC4:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068EDC
    lhz r0, 0x2(r3)
    b @L_80068F1C
@L_80068EDC:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068EF4
    lhz r0, 0x2(r3)
    b @L_80068F1C
@L_80068EF4:
    addi r3, r3, 0x4
    lhz r0, 0x0(r3)
    cmplw r0, r5
    bne @L_80068F0C
    lhz r0, 0x2(r3)
    b @L_80068F1C
@L_80068F0C:
    addi r3, r3, 0x4
    addi r4, r4, 0x8
    bdnz @L_80068E38
    li r0, 0x0
@L_80068F1C:
    clrlwi r4, r0, 16
    mr r3, r29
    bl fn_8025D970
    bl fn_8011F4F0
    cmplwi r3, 0x0
    bne @L_80068F3C
    li r3, 0x1
    bl fn_800FA280
@L_80068F3C:
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lbz r5, 0x8b(r31)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0xe9
    bl fn_800FB680
@L_80068F64:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
