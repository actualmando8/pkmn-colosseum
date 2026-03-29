    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    li r30, 0x0
    lis r3, lbl_803A6AB0@ha
    addi r31, r3, lbl_803A6AB0@l
    b @L_8004ECB8
    @L_8004EC78:
    mr r3, r30
    bl fn_801FBFBC
    clrlwi r0, r30, 16
    lwz r4, 0x0(r3)
    mulli r5, r0, 0x14
    lwz r0, 0x4(r3)
    addi r30, r30, 0x1
    add r5, r31, r5
    stw r4, 0x0(r5)
    stw r0, 0x4(r5)
    lwz r4, 0x8(r3)
    lwz r0, 0xc(r3)
    stw r4, 0x8(r5)
    stw r0, 0xc(r5)
    lwz r0, 0x10(r3)
    stw r0, 0x10(r5)
    @L_8004ECB8:
    clrlwi r0, r30, 16
    cmplwi r0, 0x229
    blt @L_8004EC78
    @L_8004ECC4:
    li r3, 0x88
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    bne @L_8004ED60
    li r3, 0x88
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r30, 0x0
    lis r3, lbl_803A6AB0@ha
    addi r31, r3, lbl_803A6AB0@l
    b @L_8004ED4C
    @L_8004ED0C:
    mr r3, r30
    bl fn_801FBFBC
    clrlwi r0, r30, 16
    addi r30, r30, 0x1
    mulli r0, r0, 0x14
    add r5, r31, r0
    lwz r4, 0x0(r5)
    lwz r0, 0x4(r5)
    stw r4, 0x0(r3)
    stw r0, 0x4(r3)
    lwz r4, 0x8(r5)
    lwz r0, 0xc(r5)
    stw r4, 0x8(r3)
    stw r0, 0xc(r3)
    lwz r0, 0x10(r5)
    stw r0, 0x10(r3)
    @L_8004ED4C:
    clrlwi r0, r30, 16
    cmplwi r0, 0x229
    blt @L_8004ED0C
    li r3, -0x1
    b @L_8004EDB4
    @L_8004ED60:
    cmpwi r3, -0x2
    bne @L_8004ECC4
    li r3, 0x44
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, 0x0
    beq @L_8004ED90
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    b @L_8004ECC4
    @L_8004ED90:
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x88
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x1
    @L_8004EDB4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
