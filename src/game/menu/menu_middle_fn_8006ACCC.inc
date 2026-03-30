    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    lwz r0, 0x4(r3)
    cmpwi r0, 0x2
    beq @L_8006AD40
    bge @L_8006AD40
    cmpwi r0, 0x0
    bge @L_8006AD10
    b @L_8006AD40
    @L_8006AD10:
    cmpwi r29, 0x0
    blt @L_8006AD94
    cmpwi r29, 0x1
    bgt @L_8006AD94
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    mulli r4, r29, 0x1660
    mr r0, r3
    addi r3, r4, 0x24
    add r3, r0, r3
    b @L_8006AD98
    @L_8006AD40:
    li r30, 0x0
    li r31, 0x0
    @L_8006AD48:
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    addi r0, r31, 0x4c
    lwzx r0, r3, r0
    cmpw r29, r0
    bne @L_8006AD84
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    mulli r4, r30, 0x1660
    mr r0, r3
    addi r3, r4, 0x24
    add r3, r0, r3
    b @L_8006AD98
    @L_8006AD84:
    addi r31, r31, 0x1660
    addi r30, r30, 0x1
    cmpwi r30, 0x4
    blt @L_8006AD48
    @L_8006AD94:
    li r3, 0x0
    @L_8006AD98:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
