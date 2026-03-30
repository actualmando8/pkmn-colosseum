    nofralloc
    li r5, 0x0
    b @L_801B3198
@L_801B317C:
    mulli r0, r5, 0xc
    lis r3, 0x8037
    li r4, 0x0
    addi r3, r3, -12312
    add r3, r3, r0
    addi r5, r5, 0x1
    stw r4, 0x8(r3)
@L_801B3198:
    cmpwi r5, 0x4
    blt @L_801B317C
    blr
