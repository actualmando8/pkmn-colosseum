    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    li r30, 0x0
    li r31, 0x0
@L_80071180:
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    addi r0, r31, 0x59cc
    lwzx r29, r3, r0
    cmpwi r29, 0x0
    beq @L_800711D8
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    addi r0, r31, 0x59a8
    add r3, r3, r0
    bl fn_8006A7E8
    cmpwi r3, 0x0
    beq @L_800711D8
    mr r3, r29
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800711D8
    mr r3, r29
    b @L_800711EC
@L_800711D8:
    addi r31, r31, 0x1660
    addi r30, r30, 0x1
    cmplwi r30, 0x4
    blt @L_80071180
    li r3, 0x0
@L_800711EC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
