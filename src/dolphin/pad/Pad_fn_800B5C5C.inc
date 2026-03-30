    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x8(r1)
    bl fn_800BAE5C
    cmpwi r3, 0x8
    beq @L_800B5CA8
    cmpwi r3, 0x9
    beq @L_800B5CA8
    cmpwi r3, 0xa
    beq @L_800B5CA8
    lwz r5, gx(r2)
    lwz r3, 0x2c8(r5)
    clrlwi r0, r3, 29
    addi r4, r3, 0x1
    slwi r3, r0, 4
    stw r4, 0x2c8(r5)
    addi r3, r3, 0x208
    add r3, r5, r3
    b @L_800B5CC8
    @L_800B5CA8:
    lwz r5, gx(r2)
    lwz r3, 0x2cc(r5)
    clrlwi r0, r3, 30
    addi r4, r3, 0x1
    slwi r3, r0, 4
    stw r4, 0x2cc(r5)
    addi r3, r3, 0x288
    add r3, r5, r3
    @L_800B5CC8:
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
