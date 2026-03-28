    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    lwz r6, -21104(r13)
    stw r3, 0x6b0(r6)
    lwz r6, -21104(r13)
    stw r4, 0x6b4(r6)
    lwz r4, -21104(r13)
    stw r5, 0x6b8(r4)
    lwz r4, -21104(r13)
    lhz r31, 0x698(r4)
    lhz r30, 0x694(r4)
    mfspr r4, GQR5
    mfspr r0, GQR6
    stw r4, -21116(r13)
    stw r0, -21112(r13)
    li r3, 0x7
    oris r3, r3, 0x7
    mtspr GQR5, r3
    li r3, 0x3d04
    oris r3, r3, 0x3d04
    mtspr GQR6, r3
    bl fn_801E632C
    lwz r4, -21104(r13)
    lhz r0, 0x692(r4)
    cmplwi r0, 0x200
    bne @L_801E6610
    cmplwi r30, 0x1c0
    bne @L_801E6610
    b @L_801E6600
@L_801E65F8:
    bl fn_801E6684
    addi r31, r31, 0x10
@L_801E6600:
    clrlwi r0, r31, 16
    cmplw r0, r30
    blt @L_801E65F8
    b @L_801E665C
@L_801E6610:
    lwz r4, -21104(r13)
    lhz r0, 0x692(r4)
    cmplwi r0, 0x280
    bne @L_801E6650
    cmplwi r30, 0x1e0
    bne @L_801E6650
    b @L_801E6634
@L_801E662C:
    bl fn_801E810C
    addi r31, r31, 0x10
@L_801E6634:
    clrlwi r0, r31, 16
    cmplw r0, r30
    blt @L_801E662C
    b @L_801E665C
    b @L_801E6650
@L_801E6648:
    bl fn_801E9B98
    addi r31, r31, 0x10
@L_801E6650:
    clrlwi r0, r31, 16
    cmplw r0, r30
    blt @L_801E6648
@L_801E665C:
    lwz r4, -21116(r13)
    lwz r0, -21112(r13)
    mtspr GQR5, r4
    mtspr GQR6, r0
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
