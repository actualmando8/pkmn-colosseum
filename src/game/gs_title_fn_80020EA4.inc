    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    la r3, lbl_8047A360(r13)
    lwz r0, 0x4(r3)
    cmpwi r0, 0x4
    bne @L_80020F04
    bl fn_8002091C
    cmpwi r3, 0x0
    blt @L_80020F40
    bl fn_8002091C
    lis r4, lbl_802E4EF0@ha
    slwi r5, r3, 2
    addi r4, r4, lbl_802E4EF0@l
    lbz r3, 0x8b(r31)
    li r0, -0x100
    lwzx r6, r4, r5
    or r5, r3, r0
    li r3, 0x0
    li r4, -0x8
    bl fn_800FB680
    b @L_80020F40
    @L_80020F04:
    cmpwi r0, 0x5
    beq @L_80020F40
    lwz r0, lbl_8047A360(r13)
    cmpwi r0, 0x0
    blt @L_80020F40
    lis r3, lbl_802E4ED8@ha
    slwi r0, r0, 2
    addi r3, r3, lbl_802E4ED8@l
    lbz r5, 0x8b(r31)
    li r4, -0x100
    lwzx r6, r3, r0
    or r5, r5, r4
    li r3, 0x0
    li r4, -0x8
    bl fn_800FB680
    @L_80020F40:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
