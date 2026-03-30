    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x1
    bne @L_800D7CA8
    li r3, 0x45
    li r4, 0x0
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D7CFC
    @L_800D7CA8:
    lbz r0, lbl_8047AAC8(r13)
    cmplwi r0, 0x0
    beq @L_800D7CFC
    lis r4, lbl_80314610@ha
    lwz r3, lbl_8047AAC0(r13)
    addi r4, r4, lbl_80314610@l
    lwz r31, 0x24(r4)
    mr r4, r31
    bl fn_800BD4B4
    lwz r3, lbl_8047AAC0(r13)
    mr r4, r31
    bl fn_800BD504
    mr r3, r31
    bl fn_800BD554
    lis r3, lbl_80400948@ha
    lwz r4, lbl_8047AAC0(r13)
    addi r3, r3, lbl_80400948@l
    addi r3, r3, 0x1b0
    bl fn_800E0628
    li r0, 0x0
    stb r0, lbl_8047AAC8(r13)
    @L_800D7CFC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
