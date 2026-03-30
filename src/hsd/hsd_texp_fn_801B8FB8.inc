    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    stw r30, 0x8(r1)
    li r30, 0x0
    bl fn_801B7C60
    cmpwi r3, 0x1
    beq @L_801B8FE8
    li r3, 0x0
    b @L_801B9030
@L_801B8FE8:
    mr r3, r31
    bl fn_801BB4C4
    cmpwi r3, 0x0
    beq @L_801B8FFC
    li r30, 0x1
@L_801B8FFC:
    mr r3, r31
    bl fn_801BAC8C
    cmpwi r3, 0x0
    beq @L_801B9010
    li r30, 0x1
@L_801B9010:
    mr r3, r31
    bl fn_801B9320
    cmpwi r3, 0x0
    beq @L_801B9024
    li r30, 0x1
@L_801B9024:
    mr r3, r31
    bl fn_801B9048
    mr r3, r30
@L_801B9030:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
