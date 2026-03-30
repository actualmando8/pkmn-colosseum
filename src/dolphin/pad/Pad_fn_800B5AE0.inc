    nofralloc
    mflr r0
    cmpwi r3, 0x0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    addi r31, r4, 0x0
    blt @L_800B5B04
    cmpwi r3, 0x2
    blt @L_800B5B0C
    @L_800B5B04:
    li r3, -0x80
    b @L_800B5B90
    @L_800B5B0C:
    addi r4, r1, 0x10
    bl fn_800B023C
    cmpwi r3, 0x0
    bge @L_800B5B20
    b @L_800B5B90
    @L_800B5B20:
    lwz r3, 0x10(r1)
    li r7, 0x0
    li r6, 0x0
    lwz r5, 0x80(r3)
    li r4, 0x0
    lwz r3, 0x4(r5)
    lwz r0, 0x0(r5)
    xor r7, r7, r3
    addi r3, r5, 0x8
    xor r6, r6, r0
    lwz r0, 0x8(r5)
    lwz r3, 0x4(r3)
    xor r6, r6, r0
    xor r7, r7, r3
    addi r3, r5, 0x10
    lwz r0, 0x10(r5)
    lwz r3, 0x4(r3)
    xor r6, r6, r0
    xor r7, r7, r3
    addi r3, r5, 0x18
    lwz r0, 0x18(r5)
    lwz r3, 0x4(r3)
    xor r6, r6, r0
    xor r7, r7, r3
    stw r7, 0x4(r31)
    stw r6, 0x0(r31)
    lwz r3, 0x10(r1)
    bl fn_800B02F4
    @L_800B5B90:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
