    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    cmplwi r3, 0x0
    li r31, 0x0
    beq @L_80077A90
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_80077A94
@L_80077A90:
    li r31, 0x1
@L_80077A94:
    rlwinm r3, r31, 0, 24, 31
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
