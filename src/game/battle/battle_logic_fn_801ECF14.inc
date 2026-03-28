    nofralloc
    lwz r0, 0x4(r3)
    clrlwi. r0, r0, 28
    bne @L_801ECF58
    lwz r4, 0x0(r3)
    lbz r0, 0x0(r4)
    extrwi r0, r0, 3, 25
    stb r0, 0x8(r3)
    lwz r4, 0x0(r3)
    lbz r0, 0x0(r4)
    clrlwi r0, r0, 28
    stb r0, 0x9(r3)
    lwz r4, 0x0(r3)
    addi r0, r4, 0x1
    stw r0, 0x0(r3)
    lwz r4, 0x4(r3)
    addi r0, r4, 0x2
    stw r0, 0x4(r3)
@L_801ECF58:
    lwz r0, 0x4(r3)
    clrlwi. r0, r0, 31
    beq @L_801ECF80
    lwz r5, 0x0(r3)
    lbz r4, 0x0(r5)
    addi r0, r5, 0x1
    stw r0, 0x0(r3)
    slwi r0, r4, 28
    srawi r5, r0, 28
    b @L_801ECF90
@L_801ECF80:
    lwz r4, 0x0(r3)
    lbz r0, 0x0(r4)
    extlwi r0, r0, 4, 24
    srawi r5, r0, 28
@L_801ECF90:
    lwz r4, 0x4(r3)
    addi r0, r4, 0x1
    stw r0, 0x4(r3)
    mr r3, r5
    blr
