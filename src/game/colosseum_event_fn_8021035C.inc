    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r3, r4
    li r4, 0x2f
    stw r0, 0x14(r1)
    bl fn_80202360
    extsh r0, r3
    cmpwi r0, 0x2
    beq @L_802103B4
    bge @L_8021039C
    cmpwi r0, 0x0
    beq @L_802103D4
    bge @L_802103AC
    cmpwi r0, -0x1
    bge @L_802103CC
    b @L_802103D4
@L_8021039C:
    cmpwi r0, 0x4
    beq @L_802103C4
    bge @L_802103D4
    b @L_802103BC
@L_802103AC:
    li r3, 0x0
    b @L_802103D8
@L_802103B4:
    li r3, 0x1
    b @L_802103D8
@L_802103BC:
    li r3, 0x2
    b @L_802103D8
@L_802103C4:
    li r3, 0x3
    b @L_802103D8
@L_802103CC:
    li r3, 0x4
    b @L_802103D8
@L_802103D4:
    li r3, 0x0
@L_802103D8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
