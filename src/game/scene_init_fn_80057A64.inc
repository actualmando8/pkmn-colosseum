    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    lis r3, lbl_803A9768@ha
    li r31, 0x0
    addi r30, r3, lbl_803A9768@l
    stw r31, 0x278(r30)
    stw r4, 0x4(r30)
    @L_80057A94:
    addi r3, r30, 0x8
    bl fn_80124A60
    addi r30, r30, 0x138
    addi r31, r31, 0x1
    cmpwi r31, 0x2
    blt @L_80057A94
    cmplwi r29, 0x0
    beq @L_80057AE8
    lis r3, lbl_803A9768@ha
    li r4, 0x3
    addi r3, r3, lbl_803A9768@l
    li r0, 0x27
    stw r4, 0x0(r3)
    addi r5, r3, 0x4
    subi r4, r29, 0x4
    mtctr r0
    @L_80057AD4:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80057AD4
    @L_80057AE8:
    lfs f1, lbl_8047BF00(r2)
    li r3, 0xa0
    lfs f0, lbl_8047BEF4(r2)
    li r4, 0x1f
    stfs f1, lbl_8047A58C(r13)
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    stfs f0, lbl_8047A588(r13)
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
