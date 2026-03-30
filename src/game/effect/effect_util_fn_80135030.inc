    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    clrlwi. r0, r4, 16
    stw r31, 0x1c(r1)
    mr r31, r5
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    beq @L_8013514C
    cmplwi r0, 0xb
    blt @L_80135068
    b @L_8013514C
    @L_80135068:
    cmplwi r29, 0x0
    bne @L_80135094
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_8013514C
    li r4, 0x1
    bl fn_80129280
    mr. r29, r3
    beq @L_8013514C
    @L_80135094:
    mr r3, r29
    bl fn_80135CD0
    cmplwi r3, 0x0
    beq @L_8013514C
    clrlwi r0, r30, 16
    cmplwi r0, 0xa
    bgt @L_8013514C
    lis r4, jumptable_80363A70@ha
    slwi r0, r0, 2
    addi r4, r4, jumptable_80363A70@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    mr r3, r29
    mr r4, r31
    bl fn_80135C90
    b @L_8013514C
    mr r4, r31
    bl fn_80135BA0
    b @L_8013514C
    mr r4, r31
    bl fn_80135B7C
    b @L_8013514C
    mr r4, r31
    bl fn_80135B6C
    b @L_8013514C
    xoris r4, r31, 0x8000
    lis r0, 0x4330
    stw r4, 0xc(r1)
    lfd f1, lbl_8047D108(r2)
    stw r0, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f1, f0, f1
    bl fn_80135B5C
    b @L_8013514C
    mr r4, r31
    bl fn_80135B4C
    b @L_8013514C
    clrlwi r4, r31, 24
    bl fn_80135B3C
    b @L_8013514C
    clrlwi r4, r31, 24
    bl fn_80135B2C
    b @L_8013514C
    clrlwi r4, r31, 24
    bl fn_80135B1C
    @L_8013514C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
