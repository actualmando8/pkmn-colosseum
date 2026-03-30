    nofralloc
    lwz r0, 0x0(r5)
    clrlwi r7, r3, 16
    cmpwi r4, 0x6
    add r0, r0, r7
    stw r0, 0x0(r5)
    bge @L_800D6AA8
    cmpwi r4, 0x3
    beq @L_800D6AB4
    bge @L_800D6AD8
    blr
    @L_800D6AA8:
    cmpwi r4, 0x8
    bgelr
    b @L_800D6AEC
    @L_800D6AB4:
    lis r3, 0x5555
    lwz r4, 0x0(r6)
    addi r0, r3, 0x5556
    mulhw r3, r0, r7
    srwi r0, r3, 31
    add r0, r3, r0
    add r0, r4, r0
    stw r0, 0x0(r6)
    blr
    @L_800D6AD8:
    lwz r0, 0x0(r6)
    add r3, r7, r0
    subi r0, r3, 0x2
    stw r0, 0x0(r6)
    blr
    @L_800D6AEC:
    lwz r4, 0x0(r6)
    extrwi r0, r3, 15, 16
    add r0, r4, r0
    stw r0, 0x0(r6)
    blr
