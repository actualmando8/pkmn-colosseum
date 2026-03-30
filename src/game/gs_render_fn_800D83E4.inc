    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    mulli r3, r31, 0x30
    bl fn_800E3534
    clrlwi. r0, r3, 16
    sth r3, lbl_8047AAB8(r13)
    beq @L_800D8478
    mr r3, r0
    bl fn_800E27B0
    subi r0, r31, 0x1
    stw r3, lbl_8047AABC(r13)
    mulli r0, r0, 0x30
    add r3, r3, r0
    stw r3, lbl_8047AAC4(r13)
    stw r3, lbl_8047AAC0(r13)
    bl fn_800E064C
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
    @L_800D8478:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
