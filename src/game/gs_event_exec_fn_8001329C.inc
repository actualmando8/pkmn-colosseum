    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r30, r3
    mr r31, r4
    li r4, 0x0
    bl fn_801040D0
    lha r4, 0x6(r31)
    subi r0, r4, 0x92
    cmplwi r0, 0x6
    bgt @L_80013648
    lis r4, jumptable_802E4D90@ha
    slwi r0, r0, 2
    addi r4, r4, jumptable_802E4D90@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    lwz r0, 0x4(r30)
    cmpwi r0, 0x107
    bne @L_80013314
    lha r5, 0x54(r31)
    li r3, 0x0
    lha r6, 0x56(r31)
    li r4, 0x0
    lbz r7, 0x8b(r30)
    bl fn_8001E644
    b @L_80013648
    @L_80013314:
    lha r5, 0x54(r31)
    li r3, 0x0
    lha r6, 0x56(r31)
    li r4, 0x0
    lbz r7, 0x8b(r30)
    li r8, 0x1
    bl fn_8001EC08
    b @L_80013648
    lbz r0, 0x0(r3)
    lha r29, 0x2(r3)
    cmplwi r0, 0x0
    beq @L_80013378
    li r3, 0x1a0
    bl fn_800FA444
    lha r5, 0x54(r31)
    srwi r3, r3, 16
    lbz r4, 0x8b(r30)
    li r0, -0x100
    extsh r6, r3
    subi r3, r5, 0x20
    subf r3, r6, r3
    or r5, r4, r0
    li r4, 0x0
    li r6, 0x1a0
    bl fn_800FB680
    @L_80013378:
    mr r4, r29
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xc9
    bl fn_800FA444
    srwi r0, r3, 16
    lha r3, 0x54(r31)
    extsh r6, r0
    lbz r5, 0x8b(r30)
    li r0, -0x100
    li r4, 0x0
    subf r3, r6, r3
    li r6, 0xc9
    or r5, r5, r0
    bl fn_800FB680
    b @L_80013648
    lbz r0, 0x0(r3)
    lha r29, 0x4(r3)
    cmplwi r0, 0x0
    beq @L_800133FC
    li r3, 0x1a0
    bl fn_800FA444
    lha r5, 0x54(r31)
    srwi r3, r3, 16
    lbz r4, 0x8b(r30)
    li r0, -0x100
    extsh r6, r3
    subi r3, r5, 0x20
    subf r3, r6, r3
    or r5, r4, r0
    li r4, 0x0
    li r6, 0x1a0
    bl fn_800FB680
    @L_800133FC:
    mr r4, r29
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xc9
    bl fn_800FA444
    srwi r0, r3, 16
    lha r3, 0x54(r31)
    extsh r6, r0
    lbz r5, 0x8b(r30)
    li r0, -0x100
    li r4, 0x0
    subf r3, r6, r3
    li r6, 0xc9
    or r5, r5, r0
    bl fn_800FB680
    b @L_80013648
    lbz r0, 0x0(r3)
    lha r29, 0x6(r3)
    cmplwi r0, 0x0
    beq @L_80013480
    li r3, 0x1a0
    bl fn_800FA444
    lha r5, 0x54(r31)
    srwi r3, r3, 16
    lbz r4, 0x8b(r30)
    li r0, -0x100
    extsh r6, r3
    subi r3, r5, 0x20
    subf r3, r6, r3
    or r5, r4, r0
    li r4, 0x0
    li r6, 0x1a0
    bl fn_800FB680
    @L_80013480:
    mr r4, r29
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xc9
    bl fn_800FA444
    srwi r0, r3, 16
    lha r3, 0x54(r31)
    extsh r6, r0
    lbz r5, 0x8b(r30)
    li r0, -0x100
    li r4, 0x0
    subf r3, r6, r3
    li r6, 0xc9
    or r5, r5, r0
    bl fn_800FB680
    b @L_80013648
    lbz r0, 0x0(r3)
    lha r29, 0x8(r3)
    cmplwi r0, 0x0
    beq @L_80013504
    li r3, 0x1a0
    bl fn_800FA444
    lha r5, 0x54(r31)
    srwi r3, r3, 16
    lbz r4, 0x8b(r30)
    li r0, -0x100
    extsh r6, r3
    subi r3, r5, 0x20
    subf r3, r6, r3
    or r5, r4, r0
    li r4, 0x0
    li r6, 0x1a0
    bl fn_800FB680
    @L_80013504:
    mr r4, r29
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xc9
    bl fn_800FA444
    srwi r0, r3, 16
    lha r3, 0x54(r31)
    extsh r6, r0
    lbz r5, 0x8b(r30)
    li r0, -0x100
    li r4, 0x0
    subf r3, r6, r3
    li r6, 0xc9
    or r5, r5, r0
    bl fn_800FB680
    b @L_80013648
    lbz r0, 0x0(r3)
    lha r29, 0xa(r3)
    cmplwi r0, 0x0
    beq @L_80013588
    li r3, 0x1a0
    bl fn_800FA444
    lha r5, 0x54(r31)
    srwi r3, r3, 16
    lbz r4, 0x8b(r30)
    li r0, -0x100
    extsh r6, r3
    subi r3, r5, 0x20
    subf r3, r6, r3
    or r5, r4, r0
    li r4, 0x0
    li r6, 0x1a0
    bl fn_800FB680
    @L_80013588:
    mr r4, r29
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xc9
    bl fn_800FA444
    srwi r0, r3, 16
    lha r3, 0x54(r31)
    extsh r6, r0
    lbz r5, 0x8b(r30)
    li r0, -0x100
    li r4, 0x0
    subf r3, r6, r3
    li r6, 0xc9
    or r5, r5, r0
    bl fn_800FB680
    b @L_80013648
    lbz r0, 0x0(r3)
    lha r29, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_8001360C
    li r3, 0x1a0
    bl fn_800FA444
    lha r5, 0x54(r31)
    srwi r3, r3, 16
    lbz r4, 0x8b(r30)
    li r0, -0x100
    extsh r6, r3
    subi r3, r5, 0x20
    subf r3, r6, r3
    or r5, r4, r0
    li r4, 0x0
    li r6, 0x1a0
    bl fn_800FB680
    @L_8001360C:
    mr r4, r29
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xc9
    bl fn_800FA444
    srwi r0, r3, 16
    lha r3, 0x54(r31)
    extsh r6, r0
    lbz r5, 0x8b(r30)
    li r0, -0x100
    li r4, 0x0
    subf r3, r6, r3
    li r6, 0xc9
    or r5, r5, r0
    bl fn_800FB680
    @L_80013648:
    li r3, 0x0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
