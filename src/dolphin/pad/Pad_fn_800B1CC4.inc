    nofralloc
    mflr r0
    mulli r7, r3, 0x110
    stw r0, 0x4(r1)
    lis r6, lbl_803FC620@ha
    stwu r1, -0x8(r1)
    addi r0, r6, lbl_803FC620@l
    add r9, r0, r7
    lwz r0, 0x0(r9)
    cmpwi r0, 0x0
    bne @L_800B1CF4
    li r3, -0x3
    b @L_800B1D50
    @L_800B1CF4:
    lwz r8, 0x88(r9)
    li r7, 0x0
    b @L_800B1D3C
    @L_800B1D00:
    clrlwi r4, r4, 16
    cmplwi r4, 0x5
    blt @L_800B1D18
    lhz r0, 0x10(r9)
    cmplw r4, r0
    blt @L_800B1D20
    @L_800B1D18:
    li r3, -0x6
    b @L_800B1D50
    @L_800B1D20:
    slwi r0, r4, 1
    add r6, r8, r0
    lhz r4, 0x0(r6)
    sth r7, 0x0(r6)
    lhz r6, 0x6(r8)
    addi r0, r6, 0x1
    sth r0, 0x6(r8)
    @L_800B1D3C:
    clrlwi r0, r4, 16
    cmplwi r0, 0xffff
    bne @L_800B1D00
    mr r4, r8
    bl fn_800B1D60
    @L_800B1D50:
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
