    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    cmplwi r3, 0x0
    bne @L_80058834
    lis r3, lbl_803A9A18@ha
    li r0, 0x0
    addi r3, r3, lbl_803A9A18@l
    sth r0, 0x0(r3)
    b @L_80058848
    @L_80058834:
    lis r4, lbl_803A9A18@ha
    mr r5, r3
    addi r3, r4, lbl_803A9A18@l
    li r4, 0x21
    bl fn_800F96E4
    @L_80058848:
    clrlwi r4, r31, 24
    li r3, 0x9b
    bl fn_8010264C
    cmpwi r31, 0x0
    beq @L_8005886C
    li r3, 0x9b
    li r4, 0x2
    li r5, 0x1
    bl fn_80102568
    @L_8005886C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
