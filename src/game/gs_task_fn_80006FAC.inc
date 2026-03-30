    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    cmplwi r30, 0x0
    beq @L_80007070
    li r3, 0x1
    bl fn_800F7AF0
    mr r31, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r31
    rlwinm r0, r0, 0, 23, 23
    cmplwi r0, 0x0
    beq @L_80007004
    li r0, 0x1
    stb r0, 0x98(r30)
    lwz r3, 0x4(r30)
    bl fn_801022B8
    stw r3, 0x80(r30)
    @L_80007004:
    li r3, 0x1
    bl fn_800F7AF0
    mr r31, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r31
    rlwinm r0, r0, 0, 19, 19
    cmplwi r0, 0x0
    beq @L_80007038
    li r3, 0x1
    li r0, -0x2
    stb r3, 0x98(r30)
    stw r0, 0x80(r30)
    @L_80007038:
    li r3, 0x1
    bl fn_800F7AF0
    mr r31, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r31
    rlwinm r0, r0, 0, 22, 22
    cmplwi r0, 0x0
    beq @L_80007070
    li r3, 0x1
    li r0, -0x1
    stb r3, 0x98(r30)
    stb r3, 0x99(r30)
    stw r0, 0x80(r30)
    @L_80007070:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
