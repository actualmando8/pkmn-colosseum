    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r4, lbl_8047AA80(r13)
    lwz r0, 0x0(r4)
    cmpwi r0, 0x1
    bne @L_800DAD4C
    mr r5, r31
    li r3, 0x2a
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DADA0
    @L_800DAD4C:
    lbz r3, 0x1b(r4)
    lbz r0, 0x1a(r4)
    cmplw r3, r0
    bne @L_800DADA0
    lwz r3, 0x4(r4)
    lwz r0, 0x8(r4)
    and. r0, r3, r0
    beq @L_800DADA0
    lwz r0, 0x8(r31)
    cmplwi r0, 0x0
    beq @L_800DADA0
    lwz r3, 0xc(r31)
    bl fn_800D7A70
    lwz r3, 0xc(r31)
    bl fn_800D892C
    lwz r3, 0x4(r31)
    lwz r4, 0x8(r31)
    bl fn_800BD0FC
    lwz r3, 0x10(r31)
    lwz r4, 0x14(r31)
    bl fn_800D6A5C
    @L_800DADA0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
