    nofralloc
    lis r4, -32704
    rlwinm r5, r3, 2, 0, 29
    addi r0, r4, -19688
    li r4, 0x0
    add r3, r0, r5
    lwz r0, -0x4(r3)
    cmpwi r0, 0x0
    beq @L_8008ABDC
    lis r3, -32704
    addi r0, r3, -19704
    add r3, r0, r5
    lwz r0, -0x4(r3)
    cmpwi r0, 0x0
    bne @L_8008ABDC
    li r4, 0x1
@L_8008ABDC:
    rlwinm r3, r4, 0, 24, 31
    blr
