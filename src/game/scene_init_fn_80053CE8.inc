    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    li r31, 0x0
    bl fn_80057270
    cmplwi r3, 0x0
    beq @L_80053D44
    bl fn_8001DA60
    clrlwi r0, r3, 24
    cmpwi r0, 0x1
    beq @L_80053D40
    bge @L_80053D30
    cmpwi r0, 0x0
    bge @L_80053D38
    b @L_80053D44
    @L_80053D30:
    cmpwi r0, 0x3
    b @L_80053D44
    @L_80053D38:
    li r31, 0xd67
    b @L_80053D44
    @L_80053D40:
    li r31, 0xd68
    @L_80053D44:
    stw r31, 0x4c(r30)
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
