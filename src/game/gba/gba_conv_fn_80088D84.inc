    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    bl fn_8006A76C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80088DB8
    li r3, 0x0
    b @L_80088E88
@L_80088DB8:
    lis r3, 0x2
    subi r3, r3, 0x2030
    bl fn_80071104
    mr r30, r3
    bl fn_80128E24
    lis r5, 0x2
    mr r4, r3
    mr r3, r30
    subi r5, r5, 0x2030
    bl memcpy
    li r29, 0x0
    mr r28, r29
@L_80088DE8:
    rlwinm r31, r29, 0, 16, 31
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    addi r0, r28, 0x59aa
    li r4, 0xe
    sthx r31, r3, r0
    li r3, 0x0
    bl fn_80129280
    addi r0, r28, 0x26
    addi r28, r28, 0x1660
    sthx r31, r3, r0
    addi r29, r29, 0x1
    cmplwi r29, 0x4
    blt @L_80088DE8
    li r31, 0x0
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    stb r31, 0x1c(r3)
    li r3, 0x8ae
    li r4, 0x0
    bl fn_8019075C
    li r3, 0x6
    li r4, 0x2
    li r5, 0x0
    bl fn_801D0748
    mr r31, r3
    bl fn_80128E24
    lis r5, 0x2
    mr r4, r30
    subi r5, r5, 0x2030
    bl memcpy
    mr r3, r30
    bl fn_8007109C
    cmpwi r31, 0x4
    bne @L_80088E84
    li r3, 0x0
    b @L_80088E88
@L_80088E84:
    li r3, -0x1
@L_80088E88:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
