    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x9
    stw r0, 0x14(r1)
    bgt @L_801BF114
    lis r4, jumptable_8036D798@ha
    rlwinm r0, r3, 2, 0, 29
    addi r3, r4, jumptable_8036D798@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    li r3, 0x0
    b @L_801BF128
    li r3, 0x3
    b @L_801BF128
    li r3, 0x6
    b @L_801BF128
    li r3, 0x9
    b @L_801BF128
    li r3, 0xc
    b @L_801BF128
    li r3, 0xf
    b @L_801BF128
    li r3, 0x12
    b @L_801BF128
    li r3, 0x15
    b @L_801BF128
    li r3, 0x18
    b @L_801BF128
    li r3, 0x1b
    b @L_801BF128
@L_801BF114:
    addi r3, r2, -22400
    li r4, 0x84
    addi r5, r2, -22392
    bl fn_80196E10
    li r3, 0x0
@L_801BF128:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

