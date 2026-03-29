    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, lbl_8047A490(r13)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_80037694
    bge @L_80037678
    cmpwi r0, 0x0
    beq @L_80037684
    bge @L_8003768C
    b @L_800376A4
    @L_80037678:
    cmpwi r0, 0x4
    bge @L_800376A4
    b @L_8003769C
    @L_80037684:
    li r6, 0x1b5b
    b @L_800376A8
    @L_8003768C:
    li r6, 0x1b5c
    b @L_800376A8
    @L_80037694:
    li r6, 0x1b5d
    b @L_800376A8
    @L_8003769C:
    li r6, 0x1b5e
    b @L_800376A8
    @L_800376A4:
    li r6, 0x1b59
    @L_800376A8:
    li r3, -0x4
    li r4, 0x0
    li r5, -0x1
    bl fn_800FB680
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
