    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_8013DE44
    lwz r3, 0x14(r31)
    li r0, 0x0
    stw r3, 0x18(r31)
    stw r0, 0x1c(r31)
    lwz r0, 0x10(r31)
    cmpwi r0, 0x0
    beq @L_8013DE24
    lwz r3, 0xc(r31)
    cmplwi r3, 0x0
    beq @L_8013DE44
    lis r4, fn_8013E258@ha
    mr r5, r31
    addi r4, r4, fn_8013E258@l
    bl fn_800DC390
    li r3, 0x1
    b @L_8013DE58
    @L_8013DE24:
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_8013DE44
    addi r4, r31, 0x8
    bl fn_800E584C
    stw r3, 0x4(r31)
    li r3, 0x1
    b @L_8013DE58
    @L_8013DE44:
    lis r3, lbl_80272F70@ha
    addi r3, r3, lbl_80272F70@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    @L_8013DE58:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
