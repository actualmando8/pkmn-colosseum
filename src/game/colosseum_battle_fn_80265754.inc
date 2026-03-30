    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r29, 0x24(r1)
    mr r29, r3
    mr r30, r4
    li r3, 0x2
    mr r4, r29
    mr r5, r30
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_8026578C
    li r3, 0x0
    b @L_802657E8
@L_8026578C:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    clrlwi r31, r3, 16
    mr r3, r29
    mr r4, r30
    bl fn_801F0134
    clrlwi. r0, r3, 16
    bne @L_802657BC
    li r3, 0x0
    b @L_802657E8
@L_802657BC:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_802657D4
    li r3, 0x0
    b @L_802657E8
@L_802657D4:
    mr r4, r31
    clrlwi r6, r3, 16
    li r3, 0x0
    li r5, 0x2
    bl fn_801F76B8
@L_802657E8:
    bl fn_80102620
    clrlwi. r0, r3, 24
    beq @L_802658B4
    mr r4, r29
    mr r5, r30
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_80265814
    li r31, 0x0
    b @L_80265874
@L_80265814:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    clrlwi r31, r3, 16
    mr r3, r29
    mr r4, r30
    bl fn_801F0134
    clrlwi. r0, r3, 16
    bne @L_80265844
    li r31, 0x0
    b @L_80265874
@L_80265844:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_8026585C
    li r31, 0x0
    b @L_80265874
@L_8026585C:
    mr r4, r31
    clrlwi r6, r3, 16
    li r3, 0x0
    li r5, 0x2
    bl fn_801F76B8
    mr r31, r3
@L_80265874:
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_801F7954
    lwz r4, 0x8(r1)
    mr r3, r31
    lhz r0, 0xc(r1)
    addi r9, r1, 0x10
    stw r4, 0x10(r1)
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    sth r0, 0x14(r1)
    li r7, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
@L_802658B4:
    lmw r29, 0x24(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
