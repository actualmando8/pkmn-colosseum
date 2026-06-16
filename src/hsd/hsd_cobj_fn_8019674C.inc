    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr. r31, r3
    stw r30, 0x18(r1)
    mr r30, r5
    beq @L_80196AF8
    cmplwi r4, 0xc
    bgt @L_80196AF8
    lis r3, jumptable_8036C6E0@ha
    slwi r0, r4, 2
    addi r3, r3, jumptable_8036C6E0@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    cmplwi r31, 0x0
    bne @L_801967A4
    la r3, -23880(r2)
    li r4, 0x318
    la r5, -23872(r2)
    bl __assert
@L_801967A4:
    cmplwi r31, 0x0
    bne @L_801967BC
    la r3, -23880(r2)
    li r4, 0x2e8
    la r5, -23872(r2)
    bl __assert
@L_801967BC:
    lwz r3, 0x24(r31)
    addi r4, r1, 0x8
    bl fn_80191688
    lfs f0, 0x0(r30)
    cmplwi r31, 0x0
    stfs f0, 0x8(r1)
    bne @L_801967E8
    la r3, -23880(r2)
    li r4, 0x324
    la r5, -23872(r2)
    bl __assert
@L_801967E8:
    cmplwi r31, 0x0
    bne @L_80196800
    la r3, -23880(r2)
    li r4, 0x2e8
    la r5, -23872(r2)
    bl __assert
@L_80196800:
    lwz r3, 0x24(r31)
    addi r4, r1, 0x8
    bl HSD_WObjSetPosition
    b @L_80196AF8
    cmplwi r31, 0x0
    bne @L_80196828
    la r3, -23880(r2)
    li r4, 0x318
    la r5, -23872(r2)
    bl __assert
@L_80196828:
    cmplwi r31, 0x0
    bne @L_80196840
    la r3, -23880(r2)
    li r4, 0x2e8
    la r5, -23872(r2)
    bl __assert
@L_80196840:
    lwz r3, 0x24(r31)
    addi r4, r1, 0x8
    bl fn_80191688
    lfs f0, 0x0(r30)
    cmplwi r31, 0x0
    stfs f0, 0xc(r1)
    bne @L_8019686C
    la r3, -23880(r2)
    li r4, 0x324
    la r5, -23872(r2)
    bl __assert
@L_8019686C:
    cmplwi r31, 0x0
    bne @L_80196884
    la r3, -23880(r2)
    li r4, 0x2e8
    la r5, -23872(r2)
    bl __assert
@L_80196884:
    lwz r3, 0x24(r31)
    addi r4, r1, 0x8
    bl HSD_WObjSetPosition
    b @L_80196AF8
    cmplwi r31, 0x0
    bne @L_801968AC
    la r3, -23880(r2)
    li r4, 0x318
    la r5, -23872(r2)
    bl __assert
@L_801968AC:
    cmplwi r31, 0x0
    bne @L_801968C4
    la r3, -23880(r2)
    li r4, 0x2e8
    la r5, -23872(r2)
    bl __assert
@L_801968C4:
    lwz r3, 0x24(r31)
    addi r4, r1, 0x8
    bl fn_80191688
    lfs f0, 0x0(r30)
    cmplwi r31, 0x0
    stfs f0, 0x10(r1)
    bne @L_801968F0
    la r3, -23880(r2)
    li r4, 0x324
    la r5, -23872(r2)
    bl __assert
@L_801968F0:
    cmplwi r31, 0x0
    bne @L_80196908
    la r3, -23880(r2)
    li r4, 0x2e8
    la r5, -23872(r2)
    bl __assert
@L_80196908:
    lwz r3, 0x24(r31)
    addi r4, r1, 0x8
    bl HSD_WObjSetPosition
    b @L_80196AF8
    cmplwi r31, 0x0
    bne @L_80196930
    la r3, -23880(r2)
    li r4, 0x300
    la r5, -23872(r2)
    bl __assert
@L_80196930:
    cmplwi r31, 0x0
    bne @L_80196948
    la r3, -23880(r2)
    li r4, 0x2d0
    la r5, -23872(r2)
    bl __assert
@L_80196948:
    lwz r3, 0x28(r31)
    addi r4, r1, 0x8
    bl fn_80191688
    lfs f0, 0x0(r30)
    cmplwi r31, 0x0
    stfs f0, 0x8(r1)
    bne @L_80196974
    la r3, -23880(r2)
    li r4, 0x30c
    la r5, -23872(r2)
    bl __assert
@L_80196974:
    cmplwi r31, 0x0
    bne @L_8019698C
    la r3, -23880(r2)
    li r4, 0x2d0
    la r5, -23872(r2)
    bl __assert
@L_8019698C:
    lwz r3, 0x28(r31)
    addi r4, r1, 0x8
    bl HSD_WObjSetPosition
    b @L_80196AF8
    cmplwi r31, 0x0
    bne @L_801969B4
    la r3, -23880(r2)
    li r4, 0x300
    la r5, -23872(r2)
    bl __assert
@L_801969B4:
    cmplwi r31, 0x0
    bne @L_801969CC
    la r3, -23880(r2)
    li r4, 0x2d0
    la r5, -23872(r2)
    bl __assert
@L_801969CC:
    lwz r3, 0x28(r31)
    addi r4, r1, 0x8
    bl fn_80191688
    lfs f0, 0x0(r30)
    cmplwi r31, 0x0
    stfs f0, 0x8(r1)
    bne @L_801969F8
    la r3, -23880(r2)
    li r4, 0x30c
    la r5, -23872(r2)
    bl __assert
@L_801969F8:
    cmplwi r31, 0x0
    bne @L_80196A10
    la r3, -23880(r2)
    li r4, 0x2d0
    la r5, -23872(r2)
    bl __assert
@L_80196A10:
    lwz r3, 0x28(r31)
    addi r4, r1, 0x8
    bl HSD_WObjSetPosition
    b @L_80196AF8
    cmplwi r31, 0x0
    bne @L_80196A38
    la r3, -23880(r2)
    li r4, 0x300
    la r5, -23872(r2)
    bl __assert
@L_80196A38:
    cmplwi r31, 0x0
    bne @L_80196A50
    la r3, -23880(r2)
    li r4, 0x2d0
    la r5, -23872(r2)
    bl __assert
@L_80196A50:
    lwz r3, 0x28(r31)
    addi r4, r1, 0x8
    bl fn_80191688
    lfs f0, 0x0(r30)
    cmplwi r31, 0x0
    stfs f0, 0x8(r1)
    bne @L_80196A7C
    la r3, -23880(r2)
    li r4, 0x30c
    la r5, -23872(r2)
    bl __assert
@L_80196A7C:
    cmplwi r31, 0x0
    bne @L_80196A94
    la r3, -23880(r2)
    li r4, 0x2d0
    la r5, -23872(r2)
    bl __assert
@L_80196A94:
    lwz r3, 0x28(r31)
    addi r4, r1, 0x8
    bl HSD_WObjSetPosition
    b @L_80196AF8
    lfs f1, 0x0(r30)
    mr r3, r31
    bl fn_801947C8
    b @L_80196AF8
    cmplwi r31, 0x0
    lfs f0, 0x0(r30)
    beq @L_80196AF8
    lbz r0, 0x50(r31)
    cmplwi r0, 0x1
    bne @L_80196AF8
    stfs f0, 0x40(r31)
    b @L_80196AF8
    cmplwi r31, 0x0
    lfs f0, 0x0(r30)
    beq @L_80196AF8
    stfs f0, 0x38(r31)
    b @L_80196AF8
    cmplwi r31, 0x0
    lfs f0, 0x0(r30)
    beq @L_80196AF8
    stfs f0, 0x3c(r31)
@L_80196AF8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
