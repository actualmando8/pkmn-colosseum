    nofralloc
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x7
    bgt @L_8021C0EC
    lis r3, jumptable_8039A068@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8039A068@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, 0xe6
    blr
    li r3, 0xe7
    blr
    li r3, 0xea
    blr
    li r3, 0xe8
    blr
    li r3, 0xe9
    blr
    li r3, 0xeb
    blr
    li r3, 0xec
    blr
@L_8021C0EC:
    li r3, 0x0
    blr
