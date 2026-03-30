    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0xd9
    stw r0, 0x14(r1)
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80205200
    li r3, 0x0
    b @L_80205214
@L_80205200:
    li r4, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r3, r3, 0, 16, 31
@L_80205214:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
