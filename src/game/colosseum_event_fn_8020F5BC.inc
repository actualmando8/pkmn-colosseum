    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r3, r4
    li r4, 0x2f
    stw r0, 0x14(r1)
    bl fn_80202360
    extsh r0, r3
    cmpwi r0, 0x2
    beq @L_8020F614
    bge @L_8020F5FC
    cmpwi r0, 0x0
    beq @L_8020F634
    bge @L_8020F60C
    cmpwi r0, -0x1
    bge @L_8020F62C
    b @L_8020F634
@L_8020F5FC:
    cmpwi r0, 0x4
    beq @L_8020F624
    bge @L_8020F634
    b @L_8020F61C
@L_8020F60C:
    li r3, 0x0
    b @L_8020F638
@L_8020F614:
    li r3, 0x1
    b @L_8020F638
@L_8020F61C:
    li r3, 0x2
    b @L_8020F638
@L_8020F624:
    li r3, 0x3
    b @L_8020F638
@L_8020F62C:
    li r3, 0x4
    b @L_8020F638
@L_8020F634:
    li r3, 0x0
@L_8020F638:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
