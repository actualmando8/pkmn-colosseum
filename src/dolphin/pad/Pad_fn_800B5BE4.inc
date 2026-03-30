    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x68(r1)
    stw r31, 0x64(r1)
    addi r31, r6, 0x0
    stw r30, 0x60(r1)
    addi r30, r5, 0x0
    addi r5, r1, 0x18
    stw r29, 0x5c(r1)
    addi r29, r4, 0x0
    stw r28, 0x58(r1)
    addi r28, r3, 0x0
    bl fn_800B57D0
    cmpwi r3, 0x0
    bge @L_800B5C24
    b @L_800B5C3C
    @L_800B5C24:
    stb r30, 0x4c(r1)
    addi r3, r28, 0x0
    addi r4, r29, 0x0
    addi r6, r31, 0x0
    addi r5, r1, 0x18
    bl fn_800B588C
    @L_800B5C3C:
    lwz r0, 0x6c(r1)
    lwz r31, 0x64(r1)
    lwz r30, 0x60(r1)
    lwz r29, 0x5c(r1)
    lwz r28, 0x58(r1)
    addi r1, r1, 0x68
    mtlr r0
    blr
