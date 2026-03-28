    nofralloc
    lis r3, -32697
    addi r3, r3, -21408
    lwz r0, 0xa0(r3)
    cmpwi r0, 0x0
    beq @L_801E471C
    lwz r0, 0xb0(r3)
    cmpwi r0, 0x0
    beq @L_801E4680
    lwz r3, 0x58(r3)
    addi r0, r3, 0x1f
    clrrwi r7, r0, 5
    b @L_801E4690
@L_801E4680:
    lwz r3, 0x44(r3)
    addi r0, r3, 0x1f
    clrrwi r0, r0, 5
    mulli r7, r0, 0xa
@L_801E4690:
    lis r3, -32697
    addi r6, r3, -21408
    lwz r4, 0x80(r6)
    lwz r3, 0x84(r6)
    lbz r0, 0xa7(r6)
    mullw r3, r4, r3
    cmplwi r0, 0x0
    addi r0, r3, 0x1f
    srwi r3, r3, 2
    clrrwi r4, r0, 5
    addi r0, r3, 0x1f
    mulli r3, r4, 0x3
    clrrwi r0, r0, 5
    mulli r0, r0, 0x3
    add r7, r7, r3
    add r7, r7, r0
    add r7, r7, r0
    beq @L_801E4714
    lwz r3, 0x90(r6)
    li r0, 0x1f4
    lwz r5, 0x48(r6)
    mulli r3, r3, 0x28
    lwz r4, 0x8c(r6)
    slwi r5, r5, 2
    divwu r3, r3, r0
    addi r0, r5, 0x1f
    clrrwi r5, r0, 5
    addi r0, r3, 0x1f
    clrrwi r0, r0, 5
    mullw r0, r4, r0
    mulli r3, r5, 0x3
    add r7, r7, r3
    add r7, r7, r0
@L_801E4714:
    addi r3, r7, 0x1000
    blr
@L_801E471C:
    li r3, 0x0
    blr
