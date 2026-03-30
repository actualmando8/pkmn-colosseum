    nofralloc
    mflr r0
    mulli r9, r3, 0x110
    stw r0, 0x4(r1)
    lis r8, lbl_803FC620@ha
    stwu r1, -0x8(r1)
    addi r0, r8, lbl_803FC620@l
    add r8, r0, r9
    lwz r0, 0x0(r8)
    cmpwi r0, 0x0
    bne @L_800B19D4
    li r3, -0x3
    b @L_800B19F8
    @L_800B19D4:
    stw r7, 0xd4(r8)
    srwi r0, r5, 7
    lis r5, fn_800B18C8@ha
    stw r0, 0xac(r8)
    addi r0, r5, fn_800B18C8@l
    stw r4, 0xb0(r8)
    mr r4, r0
    stw r6, 0xb4(r8)
    bl fn_800AFEC4
    @L_800B19F8:
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
