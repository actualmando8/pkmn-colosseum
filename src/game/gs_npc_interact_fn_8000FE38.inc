    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_80105624
    lhz r3, 0x4(r3)
    li r4, -0x1
    rlwinm r0, r3, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_8000FE6C
    li r4, 0x0
    b @L_8000FEB4
    @L_8000FE6C:
    rlwinm r0, r3, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_8000FE80
    li r4, 0x2
    b @L_8000FEB4
    @L_8000FE80:
    rlwinm r0, r3, 0, 20, 20
    cmpwi r0, 0x0
    beq @L_8000FE94
    li r4, 0x3
    b @L_8000FEB4
    @L_8000FE94:
    rlwinm r0, r3, 0, 22, 22
    cmpwi r0, 0x0
    beq @L_8000FEB4
    li r3, 0x1
    li r0, -0x1
    stb r3, 0x98(r31)
    stb r3, 0x99(r31)
    stw r0, 0x80(r31)
    @L_8000FEB4:
    cmpwi r4, 0x0
    blt @L_8000FEC8
    li r0, 0x1
    stb r0, 0x98(r31)
    stw r4, 0x80(r31)
    @L_8000FEC8:
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000FF20
    li r3, 0x0
    bl fn_801F1700
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8000FF08
    bl fn_80265924
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8000FF08
    li r0, 0x1
    b @L_8000FF24
    @L_8000FF08:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_8000FF20
    li r0, 0x1
    b @L_8000FF24
    @L_8000FF20:
    li r0, 0x0
    @L_8000FF24:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_8000FF3C
    li r0, 0x1
    stb r0, 0x98(r31)
    stb r0, 0x99(r31)
    @L_8000FF3C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
