    nofralloc
    cmpwi r3, 0x0
    blt @L_801D187C
    lwz r5, -31112(r13)
    lwz r0, 0x0(r5)
    cmplw r3, r0
    blt @L_801D1884
@L_801D187C:
    li r5, 0x0
    b @L_801D1890
@L_801D1884:
    mulli r0, r3, 0x2c
    lwz r5, -31108(r13)
    add r5, r5, r0
@L_801D1890:
    cmplwi r5, 0x0
    bne @L_801D18A4
    lis r5, 0x1
    subi r7, r5, 0x1
    b @L_801D18A8
@L_801D18A4:
    lhz r7, 0x2(r5)
@L_801D18A8:
    rlwinm r0, r7, 0, 16, 31
    cmplwi r0, 0xffff
    bne @L_801D18C0
    lis r5, 0x1
    subi r6, r5, 0x1
    b @L_801D1900
@L_801D18C0:
    lwz r5, -31104(r13)
    li r6, 0x0
    lwz r8, -31100(r13)
    lwz r9, 0x0(r5)
    b @L_801D18F4
@L_801D18D4:
    rlwinm r0, r7, 0, 16, 31
    rlwinm r5, r0, 2, 0, 29
    addi r0, r5, 0x2
    lhzx r0, r8, r0
    cmplwi r0, 0x0
    beq @L_801D1900
    addi r6, r6, 0x1
    addi r7, r7, 0x1
@L_801D18F4:
    rlwinm r0, r7, 0, 16, 31
    cmplw r0, r9
    blt @L_801D18D4
@L_801D1900:
    rlwinm r0, r6, 0, 16, 31
    cmplwi r0, 0xffff
    bne @L_801D1918
    lis r3, 0x1
    subi r3, r3, 0x1
    blr
@L_801D1918:
    cmpw r4, r0
    blt @L_801D192C
    lis r3, 0x1
    subi r3, r3, 0x1
    blr
@L_801D192C:
    cmpwi r3, 0x0
    blt @L_801D1944
    lwz r5, -31112(r13)
    lwz r0, 0x0(r5)
    cmplw r3, r0
    blt @L_801D194C
@L_801D1944:
    li r3, 0x0
    b @L_801D1958
@L_801D194C:
    mulli r0, r3, 0x2c
    lwz r3, -31108(r13)
    add r3, r3, r0
@L_801D1958:
    cmplwi r3, 0x0
    bne @L_801D196C
    lis r3, 0x1
    subi r5, r3, 0x1
    b @L_801D1970
@L_801D196C:
    lhz r5, 0x2(r3)
@L_801D1970:
    rlwinm r0, r5, 0, 16, 31
    cmplwi r0, 0xffff
    bne @L_801D1988
    lis r3, 0x1
    subi r3, r3, 0x1
    blr
@L_801D1988:
    add r0, r5, r4
    lwz r3, -31100(r13)
    rlwinm r5, r0, 0, 16, 31
    rlwinm r0, r5, 2, 0, 29
    add r3, r3, r0
    lhz r3, 0x2(r3)
    blr
