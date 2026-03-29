    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r30, r4
    lis r3, lbl_80266E90@ha
    li r29, 0x0
    addi r3, r3, lbl_80266E90@l
    li r31, 0x0
    li r6, 0x0
    li r7, 0x0
    li r5, 0x0
    li r0, 0x3
    mtctr r0
    @L_80030F4C:
    lha r4, 0x6(r30)
    lhz r0, 0x4(r3)
    cmpw r4, r0
    bne @L_80030F6C
    lbz r6, 0x0(r3)
    li r31, 0x1
    lbz r7, 0x1(r3)
    b @L_80030F84
    @L_80030F6C:
    lhz r0, 0x6(r3)
    cmpw r4, r0
    bne @L_80030F84
    lbz r6, 0x0(r3)
    li r31, 0x2
    lbz r7, 0x1(r3)
    @L_80030F84:
    addi r3, r3, 0x12
    lha r4, 0x6(r30)
    lhz r0, 0x4(r3)
    cmpw r4, r0
    bne @L_80030FA8
    lbz r6, 0x0(r3)
    li r31, 0x1
    lbz r7, 0x1(r3)
    b @L_80030FC0
    @L_80030FA8:
    lhz r0, 0x6(r3)
    cmpw r4, r0
    bne @L_80030FC0
    lbz r6, 0x0(r3)
    li r31, 0x2
    lbz r7, 0x1(r3)
    @L_80030FC0:
    addi r3, r3, 0x12
    lha r4, 0x6(r30)
    lhz r0, 0x4(r3)
    cmpw r4, r0
    bne @L_80030FE4
    lbz r6, 0x0(r3)
    li r31, 0x1
    lbz r7, 0x1(r3)
    b @L_80030FFC
    @L_80030FE4:
    lhz r0, 0x6(r3)
    cmpw r4, r0
    bne @L_80030FFC
    lbz r6, 0x0(r3)
    li r31, 0x2
    lbz r7, 0x1(r3)
    @L_80030FFC:
    addi r3, r3, 0x12
    lha r4, 0x6(r30)
    lhz r0, 0x4(r3)
    cmpw r4, r0
    bne @L_80031020
    lbz r6, 0x0(r3)
    li r31, 0x1
    lbz r7, 0x1(r3)
    b @L_80031038
    @L_80031020:
    lhz r0, 0x6(r3)
    cmpw r4, r0
    bne @L_80031038
    lbz r6, 0x0(r3)
    li r31, 0x2
    lbz r7, 0x1(r3)
    @L_80031038:
    addi r3, r3, 0x12
    addi r5, r5, 0x3
    bdnz @L_80030F4C
    cmpwi r6, 0x2
    beq @L_80031074
    bge @L_8003108C
    cmpwi r6, 0x1
    bge @L_8003105C
    b @L_8003108C
    @L_8003105C:
    clrlwi r5, r7, 16
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r29, r3
    b @L_8003108C
    @L_80031074:
    lis r3, lbl_803A2688@ha
    clrlwi r5, r7, 16
    addi r3, r3, lbl_803A2688@l
    li r4, 0x3
    bl fn_8012A5B0
    mr r29, r3
    @L_8003108C:
    mr r3, r29
    bl fn_8011E850
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_800310EC
    mr r3, r29
    bl fn_8011ED68
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_800310D0
    clrlwi r0, r31, 24
    cmplwi r0, 0x2
    bne @L_800310EC
    mr r3, r30
    li r4, 0x1
    bl fn_80109220
    b @L_8003116C
    @L_800310D0:
    clrlwi r0, r31, 24
    cmplwi r0, 0x1
    bne @L_800310EC
    mr r3, r30
    li r4, 0x1
    bl fn_80109220
    b @L_8003116C
    @L_800310EC:
    mr r3, r29
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80031160
    mr r3, r29
    bl fn_80075FEC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80031160
    mr r3, r29
    bl fn_8011ED68
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80031144
    clrlwi r0, r31, 24
    cmplwi r0, 0x2
    bne @L_80031160
    mr r3, r30
    li r4, 0x1
    bl fn_80109220
    b @L_8003116C
    @L_80031144:
    clrlwi r0, r31, 24
    cmplwi r0, 0x1
    bne @L_80031160
    mr r3, r30
    li r4, 0x1
    bl fn_80109220
    b @L_8003116C
    @L_80031160:
    mr r3, r30
    li r4, 0x0
    bl fn_80109220
    @L_8003116C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
