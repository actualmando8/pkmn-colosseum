    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    rlwinm r6, r3, 0, 16, 31
    li r3, 0x0
    subi r0, r6, 0x200
    cmplwi r0, 0x11
    bgt @L_801C8704
    lis r6, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r6, r6, -8888
    lwzx r0, r6, r0
    mtctr r0
    bctr
    li r3, 0x99
    b @L_801C8704
    li r3, 0x9c
    b @L_801C8704
    li r3, 0x9f
    b @L_801C8704
    li r3, 0xb9
    b @L_801C8704
    li r3, 0xc8
    b @L_801C8704
    li r3, 0x11f
    b @L_801C8704
    li r3, 0xf3
    b @L_801C8704
    li r3, 0xf4
    b @L_801C8704
    li r3, 0xf5
@L_801C8704:
    rlwinm r6, r4, 0, 16, 31
    li r4, 0x0
    subi r0, r6, 0x200
    cmplwi r0, 0x11
    bgt @L_801C8774
    lis r6, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r6, r6, -8960
    lwzx r0, r6, r0
    mtctr r0
    bctr
    li r4, 0x99
    b @L_801C8774
    li r4, 0x9c
    b @L_801C8774
    li r4, 0x9f
    b @L_801C8774
    li r4, 0xb9
    b @L_801C8774
    li r4, 0xc8
    b @L_801C8774
    li r4, 0x11f
    b @L_801C8774
    li r4, 0xf3
    b @L_801C8774
    li r4, 0xf4
    b @L_801C8774
    li r4, 0xf5
@L_801C8774:
    rlwinm r6, r5, 0, 16, 31
    li r5, 0x0
    subi r0, r6, 0x200
    cmplwi r0, 0x11
    bgt @L_801C87E4
    lis r6, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r6, r6, -9032
    lwzx r0, r6, r0
    mtctr r0
    bctr
    li r5, 0x99
    b @L_801C87E4
    li r5, 0x9c
    b @L_801C87E4
    li r5, 0x9f
    b @L_801C87E4
    li r5, 0xb9
    b @L_801C87E4
    li r5, 0xc8
    b @L_801C87E4
    li r5, 0x11f
    b @L_801C87E4
    li r5, 0xf3
    b @L_801C87E4
    li r5, 0xf4
    b @L_801C87E4
    li r5, 0xf5
@L_801C87E4:
    bl fn_800884BC
    cntlzw r0, r3
    rlwinm r0, r0, 27, 5, 31
    rlwinm r3, r0, 0, 24, 31
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
