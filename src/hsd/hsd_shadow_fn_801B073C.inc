    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_801B07C0
    cmplwi r4, 0x0
    beq @L_801B07B4
    b @L_801B078C
@L_801B0760:
    lwz r3, 0x0(r31)
    lwz r0, 0x4(r3)
    cmplw r0, r4
    bne @L_801B0788
    mr r3, r4
    bl fn_801A05EC
    lwz r3, 0x0(r31)
    bl fn_801A3E64
    stw r3, 0x0(r31)
    b @L_801B07C0
@L_801B0788:
    lwz r31, 0x0(r31)
@L_801B078C:
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_801B0760
    b @L_801B07C0
@L_801B079C:
    lwz r3, 0x0(r31)
    lwz r3, 0x4(r3)
    bl fn_801A05EC
    lwz r3, 0x0(r31)
    bl fn_801A3E64
    stw r3, 0x0(r31)
@L_801B07B4:
    lwz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_801B079C
@L_801B07C0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
