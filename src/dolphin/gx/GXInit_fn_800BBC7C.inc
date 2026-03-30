    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    rlwinm. r0, r6, 0, 24, 31
    stwu r1, -0x20(r1)
    beq @L_800BBC98
    li r6, 0x6
    b @L_800BBC9C
@L_800BBC98:
    li r6, 0x0
@L_800BBC9C:
    rlwinm. r0, r5, 0, 24, 31
    addi r8, r6, 0x0
    beq @L_800BBCB0
    li r6, 0x7
    b @L_800BBCB4
@L_800BBCB0:
    li r6, 0x0
@L_800BBCB4:
    li r0, 0x0
    stw r0, 0x8(r1)
    addi r9, r8, 0x0
    li r5, 0x0
    stw r0, 0xc(r1)
    li r10, 0x0
    bl fn_800BB780
    lwz r0, 0x24(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
