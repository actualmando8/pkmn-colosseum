    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_801FA654
    li r3, 0x0
    b @L_801FA6C4
    @L_801FA654:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_801FA66C
    li r3, 0x0
    b @L_801FA6C4
    @L_801FA66C:
    mr r3, r31
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    cmpwi r3, 0x0
    bne @L_801FA690
    li r3, 0x0
    b @L_801FA6C4
    @L_801FA690:
    mr r3, r31
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r3, 0x0
    bne @L_801FA6B4
    li r3, 0x0
    b @L_801FA6C4
    @L_801FA6B4:
    bl fn_8012A130
    clrlwi r3, r3, 24
    subic r0, r3, 0x1
    subfe r3, r0, r3
    @L_801FA6C4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
