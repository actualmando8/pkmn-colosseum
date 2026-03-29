    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    li r31, 0x0
    bl fn_801046B8
    li r0, 0x0
    mr r4, r3
    stw r0, 0x8(r1)
    mr r9, r29
    li r3, 0x2
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x3
    li r10, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0x2
    li r4, 0x32
    li r5, 0x3c
    bl fn_80102868
    li r3, 0x2
    li r4, 0x1
    bl fn_801045A8
    li r3, 0x2
    bl fn_801043A4
    li r3, 0x2
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_8001E494
    cmplwi r30, 0x0
    beq @L_8001E47C
    lwz r0, 0x80(r3)
    stw r0, 0x0(r30)
    @L_8001E47C:
    lbz r0, 0x99(r3)
    cmplwi r0, 0x0
    bne @L_8001E48C
    li r31, 0x1
    @L_8001E48C:
    li r3, 0x2
    bl fn_80102510
    @L_8001E494:
    mr r3, r31
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
