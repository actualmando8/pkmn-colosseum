    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    lwz r0, 0x0(r3)
    cmpwi r0, 0x2
    bne @L_80077DFC
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    lwz r0, 0x8(r3)
    cmpwi r0, 0x0
    bne @L_80077DFC
    li r3, 0x6
    b @L_80077E40
@L_80077DFC:
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    lwz r0, 0x4(r3)
    cmpwi r0, 0x1
    beq @L_80077E34
    bge @L_80077E24
    cmpwi r0, 0x0
    bge @L_80077E2C
    b @L_80077E3C
@L_80077E24:
    cmpwi r0, 0x3
    b @L_80077E3C
@L_80077E2C:
    li r3, 0x3
    b @L_80077E40
@L_80077E34:
    li r3, 0x4
    b @L_80077E40
@L_80077E3C:
    li r3, 0x2
@L_80077E40:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
